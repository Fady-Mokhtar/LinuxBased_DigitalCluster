#ifndef CRYPTO_HMAC_SHA256_H
#define CRYPTO_HMAC_SHA256_H

#include <stdint.h>
#include <stddef.h>

// Configuration to use freshness value
#define USE_FRESHNESS 1

void Crypto_HMAC_init(const uint8_t *key, size_t key_len);
void Crypto_HMAC_create_send_buffer(const uint8_t *data, size_t data_len, uint8_t *buffer, size_t buffer_len, size_t truncated_hmac_len);
int Crypto_HMAC_verify(const uint8_t *buffer, size_t buffer_len, size_t data_len, size_t truncated_hmac_len);
void Crypto_HMAC_extract_data(const uint8_t *buffer, size_t buffer_len, uint8_t *data, size_t data_len);

#endif // CRYPTO_HMAC_H
