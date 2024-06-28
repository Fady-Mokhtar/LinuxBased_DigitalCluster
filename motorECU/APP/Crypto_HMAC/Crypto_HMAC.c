
#include "Crypto_HMAC.h"


int32_t STM32_SHA1_HMAC_Compute(uint8_t* InputMessage,
                                uint32_t InputMessageLength,
                                uint8_t *HMAC_key,
                                uint32_t HMAC_keyLength,
                                uint8_t *MessageDigest,
                                int32_t* MessageDigestLength)
{
  HMAC_SHA1ctx_stt HMAC_SHA1ctx;
  uint32_t error_status = HASH_SUCCESS;

  /* Set the size of the desired MAC*/
  HMAC_SHA1ctx.mTagSize = CRL_SHA1_SIZE;

  /* Set flag field to default value */
  HMAC_SHA1ctx.mFlags = E_HASH_DEFAULT;

  /* Set the key pointer in the context*/
  HMAC_SHA1ctx.pmKey = HMAC_key;

  /* Set the size of the key */
  HMAC_SHA1ctx.mKeySize = HMAC_keyLength;

  /* Initialize the context */
  error_status = HMAC_SHA1_Init(&HMAC_SHA1ctx);

  /* check for initialization errors */
  if (error_status == HASH_SUCCESS)
  {
    /* Add data to be hashed */
    error_status = HMAC_SHA1_Append(&HMAC_SHA1ctx,
                                    InputMessage,
                                    InputMessageLength);

    if (error_status == HASH_SUCCESS)
    {
      /* retrieve */
      error_status = HMAC_SHA1_Finish(&HMAC_SHA1ctx, MessageDigest, MessageDigestLength);
    }
  }

  return error_status;
}


/**
 * @file Crypto_HMAC.c
 * @brief Implementation file for HMAC-SHA-1 cryptography functions.
 */

/**
 * @brief Pad input data to 64 bytes using a specified padding scheme.
 *
 * @param input Pointer to input data.
 * @param input_len Length of the input data.
 * @param output Pointer to the output buffer for padded data.
 */
void Crypto_HMAC_pad_data(uint8_t *input, uint8_t input_len, uint8_t *output) {
    if (input_len > 64) {
        return;
    }
    memcpy(output, input, input_len);
    memset(output + input_len, input_len, 64 - input_len);
}

/**
 * @brief Calculate HMAC-SHA-1 of the input data using the provided key.
 *
 * @param key Pointer to the HMAC key.
 * @param key_len Length of the HMAC key.
 * @param input Pointer to the input data.
 * @param input_len Length of the input data.
 * @param output Pointer to the output buffer for the HMAC result (20 bytes for SHA-1).
 */
void Crypto_HMAC_calculate(uint8_t *key, uint8_t key_len, uint8_t *input, uint8_t input_len, uint8_t *output) {
    int32_t output_len = 20; // HMAC-SHA-1 produces a 20-byte hash
    STM32_SHA1_HMAC_Compute(input, input_len, key, key_len, output, &output_len);
}

/**
 * @brief Prepare a buffer with data followed by a truncated HMAC.
 *
 * This function pads the input data to 64 bytes, calculates HMAC-SHA-1,
 * and prepares the output buffer with the original data followed by the
 * specified number of bytes of the HMAC.
 *
 * @param buffer Pointer to the output buffer.
 * @param buffer_len Length of the output buffer.
 * @param key Pointer to the HMAC key.
 * @param key_len Length of the HMAC key.
 * @param data Pointer to the input data.
 * @param data_len Length of the input data.
 * @param truncated_hmac_len Length of the truncated HMAC to include in the buffer.
 * @return 0 if successful, -1 if the buffer is not large enough to hold the data and HMAC.
 */
int Crypto_HMAC_prepare_buffer(uint8_t *buffer, uint8_t buffer_len, uint8_t *key, uint8_t key_len,
                               uint8_t *data, uint8_t data_len, uint8_t truncated_hmac_len) {
    uint8_t padded_data[64];
    uint8_t hash_output[20]; // HMAC-SHA-1 produces a 20-byte hash
    uint8_t total_len = data_len + truncated_hmac_len;

    // Step 1: Pad the data to 64 bytes
    Crypto_HMAC_pad_data(data, data_len, padded_data);

    // Step 2: Calculate HMAC-SHA-1 of the padded data
    Crypto_HMAC_calculate(key, key_len, padded_data, 64, hash_output);

    // Step 3: Prepare the buffer with data followed by truncated HMAC
    if (buffer_len >= total_len) {
        memcpy(buffer, data, data_len); // Copy data to buffer
        memcpy(buffer + data_len, hash_output, truncated_hmac_len); // Copy truncated HMAC to buffer
        return 0; // Success
    } else {
        return -1; // Buffer not large enough
    }
}

/**
 * @brief Verify the integrity of data using HMAC-SHA-1.
 *
 * This function extracts the original data and the truncated HMAC from the buffer,
 * recalculates the HMAC of the data, and verifies it against the stored HMAC.
 *
 * @param buffer Pointer to the buffer containing data followed by truncated HMAC.
 * @param buffer_len Length of the buffer.
 * @param key Pointer to the HMAC key.
 * @param key_len Length of the HMAC key.
 * @param data Pointer to the expected input data.
 * @param data_len Length of the input data.
 * @param truncated_hmac_len Length of the truncated HMAC in bytes.
 * @return 1 if the HMAC is verified, 0 if not verified.
 */
int Crypto_HMAC_verify(uint8_t *buffer, uint8_t buffer_len, uint8_t *key, uint8_t key_len,
                       uint8_t *data, uint8_t data_len, uint8_t truncated_hmac_len) {
    uint8_t padded_data[64];
    uint8_t calculated_hmac[20]; // HMAC-SHA-1 produces a 20-byte hash

    // Step 1: Pad the data to 64 bytes
    Crypto_HMAC_pad_data(data, data_len, padded_data);

    // Step 2: Calculate HMAC-SHA-1 of the padded data
    Crypto_HMAC_calculate(key, key_len, padded_data, 64, calculated_hmac);

    // Step 3: Compare the truncated HMAC in the buffer with the calculated HMAC
    uint8_t expected_hmac_len = truncated_hmac_len < sizeof(calculated_hmac) ? truncated_hmac_len : sizeof(calculated_hmac);
    if (memcmp(buffer + buffer_len - truncated_hmac_len, calculated_hmac, expected_hmac_len) == 0) {
        return 1; // Verified
    } else {
        return 0; // Not verified
    }
}

/**
 * @brief Extract data from a buffer.
 *
 * Copies data from the buffer to the provided output data array.
 *
 * @param buffer Pointer to the buffer containing data.
 * @param buffer_len Length of the buffer.
 * @param data Pointer to the output data array.
 * @param data_len Length of the data to extract.
 * @return 0 if successful, -1 if the buffer is too small.
 */
int Crypto_HMAC_extract_data(uint8_t *buffer, uint8_t buffer_len, uint8_t *data, uint8_t data_len) {
    if (buffer_len < data_len) {
        return -1; // Buffer too small to contain data
    }

    // Copy the data from buffer
    memcpy(data, buffer, data_len);

    return 0; // Success
}
