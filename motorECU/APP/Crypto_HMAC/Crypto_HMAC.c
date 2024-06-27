/*
 * Crypto_HMAC.c
 *
 *  Created on: Jun 24, 2024
 *      Author: abdel
 */

#include "Crypto_HMAC.h"
#include "mbedtls/sha256.h"
#include <string.h>
#include <stdio.h>

#define KEY "sharedsecretkey1234567890123456"  // Example key, must be 32 bytes for HMAC-SHA-256
#define KEY_LENGTH 32

// Static function declarations
static int compute_hmac(const uint8_t *data, size_t data_length, uint8_t *hmac);
static int truncate_hmac(const uint8_t *full_hmac, size_t full_hmac_length, uint8_t *truncated_hmac, size_t truncated_length);

int prepare_send_buffer(uint8_t *buffer, size_t buffer_size, const uint8_t *data, size_t data_length, size_t truncated_hash_length) {
    if (buffer_size < data_length + truncated_hash_length + (USE_FRESHNESS_VALUE ? 1 : 0)) {
        return -1;  // Buffer too small
    }

    uint8_t freshness_value = 0xAA;  // Example freshness value
    uint8_t full_hmac[32];

    // Copy data to buffer
    memcpy(buffer, data, data_length);

    // Add freshness value if enabled
    if (USE_FRESHNESS_VALUE) {
        buffer[data_length] = freshness_value;
    }

    // Compute HMAC
    if (compute_hmac(buffer, data_length + (USE_FRESHNESS_VALUE ? 1 : 0), full_hmac) != 0) {
        return -1;  // HMAC computation failed
    }

    // Truncate HMAC and append to buffer
    if (truncate_hmac(full_hmac, sizeof(full_hmac), buffer + data_length + (USE_FRESHNESS_VALUE ? 1 : 0), truncated_hash_length) != 0) {
        return -1;  // HMAC truncation failed
    }

    return 0;
}

int verify_buffer(const uint8_t *buffer, size_t buffer_size, size_t data_length, size_t truncated_hash_length) {
    if (buffer_size < data_length + truncated_hash_length + (USE_FRESHNESS_VALUE ? 1 : 0)) {
        return -1;  // Buffer too small
    }

    uint8_t full_hmac[32];
    uint8_t computed_truncated_hmac[truncated_hash_length];

    // Compute HMAC
    if (compute_hmac(buffer, data_length + (USE_FRESHNESS_VALUE ? 1 : 0), full_hmac) != 0) {
        return -1;  // HMAC computation failed
    }

    // Truncate HMAC
    if (truncate_hmac(full_hmac, sizeof(full_hmac), computed_truncated_hmac, truncated_hash_length) != 0) {
        return -1;  // HMAC truncation failed
    }

    // Compare truncated HMAC with the one in the buffer
    if (memcmp(buffer + data_length + (USE_FRESHNESS_VALUE ? 1 : 0), computed_truncated_hmac, truncated_hash_length) != 0) {
        return -1;  // HMAC verification failed
    }

    return 0;
}

int extract_data_from_buffer(const uint8_t *buffer, size_t buffer_size, uint8_t *data, size_t data_length, size_t truncated_hash_length) {
    if (verify_buffer(buffer, buffer_size, data_length, truncated_hash_length) != 0) {
        return -1;  // Buffer verification failed
    }

    // Copy data from buffer
    memcpy(data, buffer, data_length);

    return 0;
}

static int compute_hmac(const uint8_t *data, size_t data_length, uint8_t *hmac) {
    mbedtls_sha256_context ctx;
    uint8_t k_ipad[65];    /* inner padding - key XORd with ipad */
    uint8_t k_opad[65];    /* outer padding - key XORd with opad */
    uint8_t tk[32];
    uint8_t key[KEY_LENGTH];
    size_t i;

    /* If key is longer than 64 bytes reset it to key = SHA256(key) */
    if (KEY_LENGTH > 64) {
        mbedtls_sha256(KEY, KEY_LENGTH, tk, 0);
        memcpy(key, tk, 32);
    } else {
        memcpy(key, KEY, KEY_LENGTH);
    }

    /*
     * The HMAC_SHA256 transform looks like:
     *
     * SHA256(K XOR opad, SHA256(K XOR ipad, text))
     *
     * where K is an n byte key
     * ipad is the byte 0x36 repeated 64 times
     * opad is the byte 0x5c repeated 64 times
     * and text is the data being protected
     */

    /* start out by storing key in pads */
    memset(k_ipad, 0, sizeof(k_ipad));
    memset(k_opad, 0, sizeof(k_opad));
    memcpy(k_ipad, key, KEY_LENGTH);
    memcpy(k_opad, key, KEY_LENGTH);

    /* XOR key with ipad and opad values */
    for (i = 0; i < 64; i++) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }

    /* perform inner SHA256 */
    mbedtls_sha256_init(&ctx);
    mbedtls_sha256_starts(&ctx, 0);                /* init context for 1st pass */
    mbedtls_sha256_update(&ctx, k_ipad, 64);       /* start with inner pad */
    mbedtls_sha256_update(&ctx, data, data_length);/* then text of datagram */
    mbedtls_sha256_finish(&ctx, hmac);             /* finish up 1st pass */

    /* perform outer SHA256 */
    mbedtls_sha256_init(&ctx);                     /* init context for 2nd pass */
    mbedtls_sha256_starts(&ctx, 0);                /* setup SHA256 context */
    mbedtls_sha256_update(&ctx, k_opad, 64);       /* start with outer pad */
    mbedtls_sha256_update(&ctx, hmac, 32);         /* then results of 1st hash */
    mbedtls_sha256_finish(&ctx, hmac);             /* finish up 2nd pass */
    return 0;
}

static int truncate_hmac(const uint8_t *full_hmac, size_t full_hmac_length, uint8_t *truncated_hmac, size_t truncated_length) {
    if (full_hmac_length < truncated_length) {
        return -1;  // Cannot truncate to a length greater than the full HMAC
    }
    memcpy(truncated_hmac, full_hmac, truncated_length);
    return 0;
}

