#ifndef CRYPTO_HMAC_H
#define CRYPTO_HMAC_H

#include <cstdint>
#include <cstring>
#include <openssl/hmac.h>

class CryptoHMAC {
public:
    /**
     * @brief Construct a new CryptoHMAC object
     */
    CryptoHMAC();

    /**
     * @brief Pad input data to 64 bytes using a specified padding scheme.
     *
     * @param input Pointer to input data.
     * @param input_len Length of the input data.
     * @param output Pointer to the output buffer for padded data.
     */
    void padData(uint8_t* input, uint8_t input_len, uint8_t* output);

    /**
     * @brief Calculate HMAC-SHA-1 of the input data using the provided key.
     *
     * @param key Pointer to the HMAC key.
     * @param key_len Length of the HMAC key.
     * @param input Pointer to the input data.
     * @param input_len Length of the input data.
     * @param output Pointer to the output buffer for the HMAC result (20 bytes for SHA-1).
     */
    void calculateHMAC(uint8_t* key, uint8_t key_len, uint8_t* input, uint8_t input_len, uint8_t* output);

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
    int prepareBuffer(uint8_t* buffer, uint8_t buffer_len, uint8_t* key, uint8_t key_len,
                      uint8_t* data, uint8_t data_len, uint8_t truncated_hmac_len);

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
    int verify(uint8_t* buffer, uint8_t buffer_len, uint8_t* key, uint8_t key_len,
               uint8_t* data, uint8_t data_len, uint8_t truncated_hmac_len);

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
    int extractData(uint8_t* buffer, uint8_t buffer_len, uint8_t* data, uint8_t data_len);
};

#endif // CRYPTO_HMAC_H
