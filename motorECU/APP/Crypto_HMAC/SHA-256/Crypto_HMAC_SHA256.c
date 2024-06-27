#include "Crypto_HMAC_SHA256.h"
#include <string.h>
#include "psa/crypto.h"

#define KEY_SIZE 32      // 256-bit key for HMAC-SHA-256
#define HMAC_SIZE 32     // SHA-256 output size is 32 bytes
#define MESSAGE_SIZE 64  // Fixed message size for HMAC computation

static uint8_t hmac_key[KEY_SIZE];
static uint32_t freshness_counter = 0;

// Function prototypes
static void pad_message(const uint8_t *data, size_t data_len, uint8_t *padded_message, size_t padded_len);
static void compute_hmac_sha256(const uint8_t *message, size_t message_len, uint8_t *output_hmac);
static int check_freshness(const uint8_t *buffer, size_t buffer_len);

static void pad_message(const uint8_t *data, size_t data_len, uint8_t *padded_message, size_t padded_len) {
    memset(padded_message, 0x00, padded_len);
    memcpy(padded_message, data, data_len);

#if USE_FRESHNESS
    padded_message[data_len] = (uint8_t)(freshness_counter & 0xFF);
#endif
}

static void compute_hmac_sha256(const uint8_t *message, size_t message_len, uint8_t *output_hmac) {
    psa_status_t status;

    status = psa_mac_compute(
        PSA_ALG_HMAC(PSA_ALG_SHA_256),
        hmac_key, KEY_SIZE,
        message, message_len,
        output_hmac, HMAC_SIZE
    );
    if (status != PSA_SUCCESS) {
        // Handle error (e.g., log the error, assert, or handle as appropriate for your application)
    }
}

static int check_freshness(const uint8_t *buffer, size_t buffer_len) {
#if USE_FRESHNESS
    uint8_t received_freshness = buffer[buffer_len - HMAC_SIZE - 1];
    if (received_freshness != (uint8_t)(freshness_counter & 0xFF)) {
        return 0; // Freshness value mismatch
    }
#endif
    return 1; // Freshness value matches or not used
}

void Crypto_HMAC_init(const uint8_t *key, size_t key_len) {
    memcpy(hmac_key, key, key_len > KEY_SIZE ? KEY_SIZE : key_len);
    psa_crypto_init();
}

void Crypto_HMAC_create_send_buffer(const uint8_t *data, size_t data_len, uint8_t *buffer, size_t buffer_len, size_t truncated_hmac_len) {
    uint8_t padded_message[MESSAGE_SIZE];
    uint8_t full_hmac[HMAC_SIZE];

    pad_message(data, data_len, padded_message, MESSAGE_SIZE);
    compute_hmac_sha256(padded_message, MESSAGE_SIZE, full_hmac);

    memcpy(buffer, data, data_len);

#if USE_FRESHNESS
    buffer[data_len] = (uint8_t)(freshness_counter & 0xFF);
#endif

    memcpy(buffer + data_len + 1, full_hmac, truncated_hmac_len);

#if USE_FRESHNESS
    freshness_counter++;
#endif
}

int Crypto_HMAC_verify(const uint8_t *buffer, size_t buffer_len, size_t data_len, size_t truncated_hmac_len) {
    uint8_t padded_message[MESSAGE_SIZE];
    uint8_t computed_hmac[HMAC_SIZE];
    uint8_t received_hmac[HMAC_SIZE];

    if (!check_freshness(buffer, buffer_len)) {
        return 0; // Freshness value mismatch
    }

    pad_message(buffer, data_len, padded_message, MESSAGE_SIZE);
    compute_hmac_sha256(padded_message, MESSAGE_SIZE, computed_hmac);
    memcpy(received_hmac, buffer + data_len + 1, truncated_hmac_len);

    if (memcmp(computed_hmac, received_hmac, truncated_hmac_len) == 0) {
        return 1; // HMAC matches
    }

    return 0; // HMAC mismatch
}

void Crypto_HMAC_extract_data(const uint8_t *buffer, size_t buffer_len, uint8_t *data, size_t data_len) {
    memcpy(data, buffer, data_len);
}
