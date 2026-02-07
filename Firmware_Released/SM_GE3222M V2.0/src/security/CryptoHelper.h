/**
 * @file CryptoHelper.h
 * @brief Cryptographic and checksum utilities
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Provides CRC32, hashing, and basic cryptographic functions
 */

#pragma once

#include <Arduino.h>

/**
 * @class CryptoHelper
 * @brief Utility class for cryptographic operations
 * 
 * Static class providing:
 * - CRC32 checksum calculation
 * - SHA-256 hashing
 * - Password hashing and verification
 * - Random token generation
 */
class CryptoHelper {
public:
    /**
     * @brief Calculate CRC32 checksum
     * @param data Pointer to data buffer
     * @param length Data length in bytes
     * @return 32-bit CRC checksum
     */
    static uint32_t calculateCRC32(const uint8_t* data, size_t length);
    
    /**
     * @brief Calculate CRC32 checksum of string
     * @param str String to checksum
     * @return 32-bit CRC checksum
     */
    static uint32_t calculateCRC32(const String& str);
    
    /**
     * @brief Verify CRC32 checksum
     * @param data Pointer to data buffer
     * @param length Data length in bytes
     * @param expectedCRC Expected CRC value
     * @return true if CRC matches
     */
    static bool verifyCRC32(const uint8_t* data, size_t length, uint32_t expectedCRC);
    
    /**
     * @brief Calculate SHA-256 hash
     * @param data Pointer to data buffer
     * @param length Data length in bytes
     * @param hash Output buffer for 32-byte hash (must be 32 bytes)
     * @return true if successful
     */
    static bool calculateSHA256(const uint8_t* data, size_t length, uint8_t* hash);
    
    /**
     * @brief Calculate SHA-256 hash of string
     * @param str String to hash
     * @param hash Output buffer for 32-byte hash (must be 32 bytes)
     * @return true if successful
     */
    static bool calculateSHA256(const String& str, uint8_t* hash);
    
    /**
     * @brief Get SHA-256 hash as hex string
     * @param data Pointer to data buffer
     * @param length Data length in bytes
     * @return Hex string representation (64 characters)
     */
    static String getSHA256Hex(const uint8_t* data, size_t length);
    
    /**
     * @brief Get SHA-256 hash of string as hex string
     * @param str String to hash
     * @return Hex string representation (64 characters)
     */
    static String getSHA256Hex(const String& str);
    
    /**
     * @brief Hash password with salt
     * @param password Password string
     * @param salt Salt string (if empty, random salt is generated)
     * @return Hashed password as hex string with salt (format: "salt:hash")
     */
    static String hashPassword(const String& password, const String& salt = "");
    
    /**
     * @brief Verify password against hash
     * @param password Password to verify
     * @param hashedPassword Hashed password (format: "salt:hash")
     * @return true if password matches
     */
    static bool verifyPassword(const String& password, const String& hashedPassword);
    
    /**
     * @brief Generate random token
     * @param length Token length in bytes
     * @return Random token as hex string
     */
    static String generateToken(size_t length = 16);
    
    /**
     * @brief Generate random salt
     * @param length Salt length in bytes
     * @return Random salt as hex string
     */
    static String generateSalt(size_t length = 16);
    
    /**
     * @brief Convert byte array to hex string
     * @param data Byte array
     * @param length Array length
     * @return Hex string
     */
    static String toHex(const uint8_t* data, size_t length);
    
    /**
     * @brief Convert hex string to byte array
     * @param hex Hex string
     * @param data Output byte array
     * @param maxLength Maximum output length
     * @return Number of bytes written
     */
    static size_t fromHex(const String& hex, uint8_t* data, size_t maxLength);

private:
    CryptoHelper() = delete;  // Static class, no instantiation
    
    /**
     * @brief CRC32 lookup table
     */
    static const uint32_t crc32_table[256];
    
    /**
     * @brief Initialize CRC32 table (called automatically)
     */
    static void initCRC32Table();
};
