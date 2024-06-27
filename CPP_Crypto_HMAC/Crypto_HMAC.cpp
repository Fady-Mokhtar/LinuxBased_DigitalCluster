#include <iostream>
#include <cstring>
#include "mbedtls/md.h"

// Define a macro to determine if the freshness value is used or not
#define USE_FRESHNESS_VALUE 1

class Crypto_HMAC {
private:
    uint8_t key[32]; // Shared key (32 bytes for HMAC-SHA256)
    uint8_t freshness_counter; // Freshness value counter

    // Function to pad the message with a specified byte up to a fixed length
    void pad_message(const uint8_t *input_data, size_t input_len, uint8_t *output_message, size_t output_len, uint8_t pad_byte = 0x00) {
        // Copy input_data into output_message
        memcpy(output_message, input_data, input_len);

        // Fill the remaining bytes with pad_byte
        memset(output_message + input_len, pad_byte, output_len - input_len);
    }

    // Function to prepare the data buffer for HMAC calculation
    void prepare_data(const uint8_t *data, size_t data_len, uint8_t *output_buffer, size_t buffer_len) {
        // Ensure the buffer_len is at least data_len + 1 if using freshness value
#if USE_FRESHNESS_VALUE
        if (buffer_len < data_len + 1) {
            std::cerr << "Buffer length is too small" << std::endl;
            return;
        }

        // Copy data into output_buffer
        memcpy(output_buffer, data, data_len);

        // Add freshness value (increment counter)
        output_buffer[data_len] = freshness_counter++;
#else
        // Ensure the buffer_len is at least data_len
        if (buffer_len < data_len) {
            std::cerr << "Buffer length is too small" << std::endl;
            return;
        }

        // Copy data into output_buffer
        memcpy(output_buffer, data, data_len);
#endif

        // Pad the rest of the buffer to 64 bytes
        pad_message(output_buffer, data_len + (USE_FRESHNESS_VALUE ? 1 : 0), output_buffer, buffer_len);
    }

    // Function to compute HMAC-SHA256 over a 64-byte message
    void compute_hmac_sha256(const uint8_t *message, size_t message_len, uint8_t *output_hmac) {
        mbedtls_md_context_t ctx;
        const mbedtls_md_info_t *info;

        mbedtls_md_init(&ctx);

        // Initialize the HMAC context with SHA-256
        info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
        mbedtls_md_setup(&ctx, info, 1);

        // Perform HMAC computation
        mbedtls_md_hmac_starts(&ctx, key, sizeof(key));
        mbedtls_md_hmac_update(&ctx, message, message_len); // HMAC calculated over 64 bytes
        mbedtls_md_hmac_finish(&ctx, output_hmac);

        // Clean up
        mbedtls_md_free(&ctx);
    }

    // Function to calculate HMAC-SHA256 for variable-sized data
    void calculate_hmac(const uint8_t *input_data, size_t input_len, uint8_t *output_hmac) {
        uint8_t padded_message[64];

        // Prepare the data to 64 bytes
        prepare_data(input_data, input_len, padded_message, 64);

        // Compute HMAC for the prepared message
        compute_hmac_sha256(padded_message, 64, output_hmac);
    }

    // Function to create a buffer to be sent with the truncated HMAC
    void create_send_buffer(uint8_t *buffer, size_t buffer_len, const uint8_t *data, size_t data_len, const uint8_t *full_hmac, size_t truncated_hmac_len) {
        if (buffer_len < data_len + truncated_hmac_len + (USE_FRESHNESS_VALUE ? 1 : 0)) {
            std::cerr << "Buffer length is too small" << std::endl;
            return;
        }

        // Copy data into buffer
        memcpy(buffer, data, data_len);

        // Copy freshness value after data
#if USE_FRESHNESS_VALUE
        buffer[data_len] = freshness_counter - 1; // use the last freshness value calculated
#endif

        // Copy truncated HMAC into buffer
        memcpy(buffer + data_len + (USE_FRESHNESS_VALUE ? 1 : 0), full_hmac, truncated_hmac_len);
    }

    // Function to extract data and truncated HMAC from the received buffer
    void extract_data_and_hmac(const uint8_t *buffer, size_t buffer_len, uint8_t *data, size_t data_len, uint8_t *truncated_hmac, size_t truncated_hmac_len) {
        if (buffer_len < data_len + truncated_hmac_len + (USE_FRESHNESS_VALUE ? 1 : 0)) {
            std::cerr << "Buffer length is too small" << std::endl;
            return;
        }

        // Copy data from buffer
        memcpy(data, buffer, data_len);

        // Check and compare freshness value if used
#if USE_FRESHNESS_VALUE
        if (buffer[data_len] != freshness_counter - 1) {
            std::cerr << "Freshness value mismatch" << std::endl;
            return;
        }
#endif

        // Skip freshness value if used
        size_t offset = data_len + (USE_FRESHNESS_VALUE ? 1 : 0);

        // Copy truncated HMAC from buffer
        memcpy(truncated_hmac, buffer + offset, truncated_hmac_len);
    }

    // Static function to check freshness value against the current counter
    static int check_freshness_value(const uint8_t *receive_buffer, size_t buffer_len, size_t data_len, uint8_t current_freshness_counter) {
        // Check if freshness value is used
#if USE_FRESHNESS_VALUE
        if (buffer_len < data_len + 1) {
            std::cerr << "Buffer length is too small for freshness value check" << std::endl;
            return 0; // Buffer too small
        }

        // Compare freshness value in buffer with current counter
        if (receive_buffer[data_len] != current_freshness_counter - 1) {
            std::cerr << "Freshness value mismatch" << std::endl;
            return 0; // Freshness value mismatch
        }
#endif

        // Freshness value check passed
        return 1;
    }

public:
    Crypto_HMAC(const uint8_t *shared_key) : freshness_counter(0) {
        // Copy the shared key into the class instance
        memcpy(key, shared_key, 32);
    }

    // High-level method to prepare the buffer with data, freshness value, and truncated HMAC
    void prepare_send_buffer(const uint8_t *data, size_t data_len, uint8_t *send_buffer, size_t buffer_len, size_t truncated_hmac_len) {
        uint8_t full_hmac[32]; // Buffer for full HMAC output

        // Calculate HMAC for the data
        calculate_hmac(data, data_len, full_hmac);

        // Create the buffer to be sent with the data, freshness value, and truncated HMAC
        create_send_buffer(send_buffer, buffer_len, data, data_len, full_hmac, truncated_hmac_len);
    }

    // High-level method to extract the actual data from the receive buffer without freshness value
    void extract_data_from_buffer(const uint8_t *receive_buffer, size_t buffer_len, uint8_t *extracted_data, size_t data_len) {
        // Extract data from buffer (excluding freshness value if used)
        memcpy(extracted_data, receive_buffer, data_len);
    }

    // High-level method to verify the receive buffer and return 1 (valid) or 0 (invalid)
    int verify_receive_buffer(const uint8_t *receive_buffer, size_t buffer_len, size_t data_len, size_t truncated_hmac_len) {
        uint8_t extracted_data[data_len];
        uint8_t extracted_truncated_hmac[truncated_hmac_len];
        uint8_t calculated_hmac[32]; // Buffer for full HMAC output

        // Check freshness value
        if (!check_freshness_value(receive_buffer, buffer_len, data_len, freshness_counter)) {
            return 0; // Freshness value mismatch
        }

        // Extract data and truncated HMAC from the buffer
        extract_data_and_hmac(receive_buffer, buffer_len, extracted_data, data_len, extracted_truncated_hmac, truncated_hmac_len);

        // Calculate HMAC for the extracted data
        calculate_hmac(extracted_data, data_len, calculated_hmac);

        // Verify the extracted truncated HMAC against the calculated HMAC
        if (memcmp(calculated_hmac, extracted_truncated_hmac, truncated_hmac_len) != 0) {
            return 0; // HMAC verification failed
        }

        // All checks passed
        return 1;
    }
};

// Example usage
int main() {
    // Example shared key (32 bytes for HMAC-SHA256)
    uint8_t shared_key[32] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
    };

    // Example data to be HMACed (variable size)
    uint8_t data[] = {0x01, 0x02, 0x03}; // Example data, size can vary

    // Create an instance of Crypto_HMAC
    Crypto_HMAC crypto(shared_key);

    // Buffer to be sent (size should be at least data_len + truncated_hmac_len)
    uint8_t send_buffer[64];
    size_t truncated_hmac_len = 4; // Example truncated HMAC length

    // Prepare the send buffer with the data, freshness value, and truncated HMAC
    crypto.prepare_send_buffer(data, sizeof(data), send_buffer, sizeof(send_buffer), truncated_hmac_len);

    // Print the send buffer (for demonstration purposes)
    std::cout << "Send Buffer: ";
    for (int i = 0; i < sizeof(data) + truncated_hmac_len + (USE_FRESHNESS_VALUE ? 1 : 0); i++) {
        printf("%02x", send_buffer[i]);
    }
    std::cout << std::endl;

    // Verify the receive buffer and print the result
    int verify_result = crypto.verify_receive_buffer(send_buffer, sizeof(data) + truncated_hmac_len + (USE_FRESHNESS_VALUE ? 1 : 0), sizeof(data), truncated_hmac_len);
    std::cout << "Verification result: " << verify_result << std::endl;

    // Extract data from the receive buffer (excluding freshness value)
    uint8_t extracted_data[sizeof(data)];
    crypto.extract_data_from_buffer(send_buffer, sizeof(data) + truncated_hmac_len + (USE_FRESHNESS_VALUE ? 1 : 0), extracted_data, sizeof(data));

    // Print the extracted data (for demonstration purposes)
    std::cout << "Extracted Data: ";
    for (int i = 0; i < sizeof(data); i++) {
        printf("%02x", extracted_data[i]);
    }
    std::cout << std::endl;

    return 0;
}
