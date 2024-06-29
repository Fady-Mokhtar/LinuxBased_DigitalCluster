#include <iostream>
#include "Crypto_HMAC.hpp"

int main() {
    std::cout << "Hello Easy C++ project!" << std::endl;
    // Example data and key
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04}; // 4 bytes of data
    uint8_t key[] = {0x11, 0x22, 0x33, 0x44, 0x55,
                     0x66, 0x77, 0x88, 0x99, 0xaa,
                     0xbb, 0xcc, 0xdd, 0xee, 0xff}; // 20 bytes of key

    // Initialize CryptoHMAC instance
    CryptoHMAC hmac;

    // Variables for testing
    const int buffer_len = 8; // Buffer length for prepareBuffer and verify tests
    uint8_t buffer[buffer_len];
    uint8_t extracted_data[sizeof(data)]; // Assuming extracted data can fit original data size
    int result;

    // Prepare buffer with data followed by truncated HMAC
    int truncated_hmac_len = 4; // 4 bytes of truncated HMAC
    result = hmac.prepareBuffer(buffer, buffer_len, key, sizeof(key), data, sizeof(data), truncated_hmac_len);
    if (result == 0) {
        std::cout << "Buffer prepared successfully:" << std::endl;
        // Print the content of the buffer in hexadecimal format
        for (int i = 0; i < buffer_len; ++i) {
            printf("%02x ", buffer[i]);
        }
        std::cout << std::endl;
    } else {
        std::cerr << "Failed to prepare buffer. Buffer size too small." << std::endl;
        return 1; // Exit with error
    }

    // Verify integrity of data using HMAC
    result = hmac.verify(buffer, buffer_len, key, sizeof(key), data, sizeof(data), truncated_hmac_len);
    if (result == 1) {
        std::cout << "HMAC verification successful." << std::endl;
    } else {
        std::cerr << "HMAC verification failed." << std::endl;
        return 1; // Exit with error
    }

    // Extract data from buffer
    result = hmac.extractData(buffer, buffer_len, extracted_data, sizeof(data));
    if (result == 0) {
        std::cout << "Data extracted successfully:" << std::endl;
        // Print the extracted data
        for (int i = 0; i < sizeof(data); ++i) {
            printf("%02x ", extracted_data[i]);
        }
        std::cout << std::endl;
    } else {
        std::cerr << "Failed to extract data. Buffer size too small." << std::endl;
        return 1; // Exit with error
    }

    getchar();
    return 0; // Success

}