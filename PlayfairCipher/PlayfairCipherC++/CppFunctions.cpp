#define CppFunctions _declspec(dllexport)
#include <iostream>
#include <string>
#include <cstring>
#include "Engine.h"

extern "C" {

    // Function to encode a string with a pre-allocated buffer
    CppFunctions void encode(const char* text, const char* keyword, char separator, char* buffer, size_t buffer_size) {
        try {

            if (!text || strlen(text) == 0) {
                throw std::invalid_argument("Input text cannot be empty.");
            }
            if (!keyword || strlen(keyword) == 0) {
                throw std::invalid_argument("Keyword cannot be empty.");
            }

            if (separator == '\0') {
                throw std::invalid_argument("Separator cannot be null.");
            }

            Engine engine;
            std::string encoded = engine.encode(std::string(text), std::string(keyword), separator);

            if (encoded.size() + 1 > buffer_size) {
                // Indicate buffer size error
                if (buffer_size > 0) {
                    buffer[0] = '\0';
                }
                return;
            }

            // Copy the encoded string into the buffer
            strncpy_s(buffer, buffer_size, encoded.c_str(), buffer_size - 1); // Safe copy
        }
        catch (...) {
            if (buffer_size > 0) {
                buffer[0] = '\0'; // Indicate error with empty string
            }
        }
    }

    // Function to decode a string with a pre-allocated buffer
    CppFunctions void decode(const char* text, const char* keyword, char separator, char* buffer, size_t buffer_size) {
        try {

            if (!text || strlen(text) == 0) {
                throw std::invalid_argument("Input text cannot be empty.");
            }
            if (!keyword || strlen(keyword) == 0) {
                throw std::invalid_argument("Keyword cannot be empty.");
            }

            if (separator == '\0') {
                throw std::invalid_argument("Separator cannot be null.");
            }

            Engine engine;
            std::string decoded = engine.decode(std::string(text), std::string(keyword), separator);

            if (decoded.size() + 1 > buffer_size) {
                // Indicate buffer size error
                if (buffer_size > 0) {
                    buffer[0] = '\0';
                }
                return;
            }

            // Copy the decoded string into the buffer
            strncpy_s(buffer, buffer_size, decoded.c_str(), buffer_size - 1); // Safe copy
        }
        catch (...) {
            if (buffer_size > 0) {
                buffer[0] = '\0'; // Indicate error with empty string
            }
        }
    }
}

