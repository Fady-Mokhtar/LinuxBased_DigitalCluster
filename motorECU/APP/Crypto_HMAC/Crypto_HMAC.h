/*
 * Crypto_HMAC.h
 *
 *  Created on: Jun 24, 2024
 *      Author: abdel
 */

#ifndef CRYPTO_HMAC_CRYPTO_HMAC_H_
#define CRYPTO_HMAC_CRYPTO_HMAC_H_

#include <stdint.h>
#include <stddef.h>

#define USE_FRESHNESS_VALUE 1  // Set to 1 to include the freshness value, 0 to exclude

// API for preparing the send buffer
int prepare_send_buffer(uint8_t *buffer, size_t buffer_size, const uint8_t *data, size_t data_length, size_t truncated_hash_length);

// API for verifying a buffer
int verify_buffer(const uint8_t *buffer, size_t buffer_size, size_t data_length, size_t truncated_hash_length);

// API for extracting the data from a verified buffer
int extract_data_from_buffer(const uint8_t *buffer, size_t buffer_size, uint8_t *data, size_t data_length, size_t truncated_hash_length);

#endif // HMAC_H

