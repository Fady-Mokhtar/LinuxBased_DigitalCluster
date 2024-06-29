#include "Crypto_HMAC.hpp"

CryptoHMAC::CryptoHMAC() {
    // Constructor (if any initialization is needed)
}

void CryptoHMAC::padData(uint8_t* input, uint8_t input_len, uint8_t* output) {
    if (input_len > 64) {
        return;
    }
    std::memcpy(output, input, input_len);
    std::memset(output + input_len, input_len, 64 - input_len);
}

void CryptoHMAC::calculateHMAC(uint8_t* key, uint8_t key_len, uint8_t* input, uint8_t input_len, uint8_t* output) {
    unsigned int len = 20; // HMAC-SHA-1 produces a 20-byte hash
    HMAC_CTX* ctx = HMAC_CTX_new();
    HMAC_Init(ctx, key, key_len, EVP_sha1());
    HMAC_Update(ctx, input, input_len);
    HMAC_Final(ctx, output, &len);
    HMAC_CTX_free(ctx);
}

int CryptoHMAC::prepareBuffer(uint8_t* buffer, uint8_t buffer_len, uint8_t* key, uint8_t key_len,
                              uint8_t* data, uint8_t data_len, uint8_t truncated_hmac_len) {
    uint8_t padded_data[64];
    uint8_t hash_output[20]; // HMAC-SHA-1 produces a 20-byte hash
    uint8_t total_len = data_len + truncated_hmac_len;

    // Step 1: Pad the data to 64 bytes
    padData(data, data_len, padded_data);

    // Step 2: Calculate HMAC-SHA-1 of the padded data
    calculateHMAC(key, key_len, padded_data, 64, hash_output);

    // Step 3: Prepare the buffer with data followed by truncated HMAC
    if (buffer_len >= total_len) {
        std::memcpy(buffer, data, data_len); // Copy data to buffer
        std::memcpy(buffer + data_len, hash_output, truncated_hmac_len); // Copy truncated HMAC to buffer
        return 0; // Success
    } else {
        return -1; // Buffer not large enough
    }
}

int CryptoHMAC::verify(uint8_t* buffer, uint8_t buffer_len, uint8_t* key, uint8_t key_len,
                       uint8_t* data, uint8_t data_len, uint8_t truncated_hmac_len) {
    uint8_t padded_data[64];
    uint8_t calculated_hmac[20]; // HMAC-SHA-1 produces a 20-byte hash

    // Step 1: Pad the data to 64 bytes
    padData(data, data_len, padded_data);

    // Step 2: Calculate HMAC-SHA-1 of the padded data
    calculateHMAC(key, key_len, padded_data, 64, calculated_hmac);

    // Step 3: Compare the truncated HMAC in the buffer with the calculated HMAC
    uint8_t expected_hmac_len = truncated_hmac_len < sizeof(calculated_hmac) ? truncated_hmac_len : sizeof(calculated_hmac);
    if (std::memcmp(buffer + buffer_len - truncated_hmac_len, calculated_hmac, expected_hmac_len) == 0) {
        return 1; // Verified
    } else {
        return 0; // Not verified
    }
}

int CryptoHMAC::extractData(uint8_t* buffer, uint8_t buffer_len, uint8_t* data, uint8_t data_len) {
    if (buffer_len < data_len) {
        return -1; // Buffer too small to contain data
    }

    // Copy the data from buffer
    std::memcpy(data, buffer, data_len);

    return 0; // Success
}
