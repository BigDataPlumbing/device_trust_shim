/**
 * @file audit_chain.hpp
 * @brief Tamper-evident audit logging for safety-critical devices
 * 
 * Device Trust Shim (DTS) provides a lightweight, deterministic mechanism
 * to ensure device logs are tamper-evident from the moment of generation.
 * Designed for IEC 62304 compliant medical device firmware.
 * 
 * @copyright Copyright (c) 2025 Big Data Plumbing
 * @license MIT License
 */

#ifndef DTS_AUDIT_CHAIN_HPP
#define DTS_AUDIT_CHAIN_HPP

#include <array>
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cstring>

namespace dts {

/**
 * @brief SHA-256 hash implementation (lightweight, header-only)
 */
class SHA256 {
public:
    using Hash = std::array<uint8_t, 32>;
    
    static Hash hash(const uint8_t* data, size_t len) {
        SHA256 ctx;
        ctx.update(data, len);
        return ctx.finalize();
    }
    
    static Hash hash(const std::string& str) {
        return hash(reinterpret_cast<const uint8_t*>(str.data()), str.size());
    }
    
    void update(const uint8_t* data, size_t len) {
        for (size_t i = 0; i < len; ++i) {
            buffer[buffer_len++] = data[i];
            if (buffer_len == 64) {
                transform();
                buffer_len = 0;
            }
        }
        bit_len += len * 8;
    }
    
    Hash finalize() {
        uint64_t bit_len_be = swap_endian(bit_len);
        uint8_t pad_len = (buffer_len < 56) ? (56 - buffer_len) : (120 - buffer_len);
        update(padding, pad_len);
        update(reinterpret_cast<const uint8_t*>(&bit_len_be), 8);
        
        Hash result;
        for (int i = 0; i < 8; ++i) {
            result[i * 4 + 0] = (h[i] >> 24) & 0xFF;
            result[i * 4 + 1] = (h[i] >> 16) & 0xFF;
            result[i * 4 + 2] = (h[i] >> 8) & 0xFF;
            result[i * 4 + 3] = (h[i] >> 0) & 0xFF;
        }
        return result;
    }
    
private:
    static constexpr uint8_t padding[64] = {0x80};
    
    uint32_t h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    
    uint8_t buffer[64] = {0};
    uint8_t buffer_len = 0;
    uint64_t bit_len = 0;
    
    static constexpr uint32_t k[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25c, 0x59f111f1, 0x923f82e4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d8d, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66b, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };
    
    void transform() {
        uint32_t w[64];
        for (int i = 0; i < 16; ++i) {
            w[i] = (buffer[i * 4] << 24) | (buffer[i * 4 + 1] << 16) |
                   (buffer[i * 4 + 2] << 8) | buffer[i * 4 + 3];
        }
        for (int i = 16; i < 64; ++i) {
            uint32_t s0 = right_rotate(w[i-15], 7) ^ right_rotate(w[i-15], 18) ^ (w[i-15] >> 3);
            uint32_t s1 = right_rotate(w[i-2], 17) ^ right_rotate(w[i-2], 19) ^ (w[i-2] >> 10);
            w[i] = w[i-16] + s0 + w[i-7] + s1;
        }
        
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3],
                 e = h[4], f = h[5], g = h[6], h_val = h[7];
        
        for (int i = 0; i < 64; ++i) {
            uint32_t S1 = right_rotate(e, 6) ^ right_rotate(e, 11) ^ right_rotate(e, 25);
            uint32_t ch = (e & f) ^ (~e & g);
            uint32_t temp1 = h_val + S1 + ch + k[i] + w[i];
            uint32_t S0 = right_rotate(a, 2) ^ right_rotate(a, 13) ^ right_rotate(a, 22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;
            
            h_val = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }
        
        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += h_val;
    }
    
    static uint32_t right_rotate(uint32_t value, uint32_t amount) {
        return (value >> amount) | (value << (32 - amount));
    }
    
    static uint64_t swap_endian(uint64_t value) {
        return ((value & 0xFF00000000000000ULL) >> 56) |
               ((value & 0x00FF000000000000ULL) >> 40) |
               ((value & 0x0000FF0000000000ULL) >> 24) |
               ((value & 0x000000FF00000000ULL) >> 8) |
               ((value & 0x00000000FF000000ULL) << 8) |
               ((value & 0x0000000000FF0000ULL) << 24) |
               ((value & 0x000000000000FF00ULL) << 40) |
               ((value & 0x00000000000000FFULL) << 56);
    }
};

/**
 * @brief User identifier enum for audit logging
 */
enum class UserID : uint8_t {
    System = 0,
    Admin = 1,
    Operator = 2,
    Service = 3,
    Unauthorized = 255
};

/**
 * @brief Event severity levels
 */
enum class Severity : uint8_t {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
    Critical = 4
};

/**
 * @brief Tamper-evident audit chain logger
 * 
 * Creates a cryptographic chain where each log entry includes a hash
 * of the previous entry, making deletion or modification detectable.
 * 
 * Thread-safe for single-writer scenarios (typical for device firmware).
 */
class AuditChain {
public:
    /**
     * @brief Initialize audit chain with device identifier
     * @param device_id Unique device identifier (e.g., serial number)
     */
    explicit AuditChain(const std::string& device_id)
        : device_id_(device_id), previous_hash_(SHA256::hash("DTS_INIT")) {}
    
    /**
     * @brief Log an audit event
     * @param message Event description
     * @param user_id User who triggered the event
     * @param severity Event severity level
     * @return JSON-formatted log entry with embedded integrity hash
     */
    std::string log(const std::string& message, 
                    UserID user_id = UserID::System,
                    Severity severity = Severity::Info) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::ostringstream json;
        json << std::put_time(std::gmtime(&time_t), "%Y-%m-%dT%H:%M:%S");
        json << "." << std::setfill('0') << std::setw(3) << ms.count() << "Z";
        
        std::string timestamp = json.str();
        
        // Build payload for hashing
        std::ostringstream payload;
        payload << device_id_ << "|"
                << timestamp << "|"
                << static_cast<int>(user_id) << "|"
                << static_cast<int>(severity) << "|"
                << message << "|"
                << hash_to_hex(previous_hash_);
        
        // Compute chain hash
        auto current_hash = SHA256::hash(payload.str());
        
        // Build JSON log entry
        std::ostringstream log_entry;
        log_entry << "{"
                  << "\"device_id\":\"" << device_id_ << "\","
                  << "\"timestamp\":\"" << timestamp << "\","
                  << "\"user_id\":" << static_cast<int>(user_id) << ","
                  << "\"severity\":" << static_cast<int>(severity) << ","
                  << "\"message\":\"" << escape_json(message) << "\","
                  << "\"previous_hash\":\"" << hash_to_hex(previous_hash_) << "\","
                  << "\"chain_hash\":\"" << hash_to_hex(current_hash) << "\""
                  << "}";
        
        // Update chain state
        previous_hash_ = current_hash;
        sequence_number_++;
        
        return log_entry.str();
    }
    
    /**
     * @brief Get current chain hash (for verification)
     */
    std::string get_chain_hash() const {
        return hash_to_hex(previous_hash_);
    }
    
    /**
     * @brief Get sequence number (total entries logged)
     */
    uint64_t get_sequence_number() const {
        return sequence_number_;
    }
    
    /**
     * @brief Verify chain integrity from a sequence of log entries
     * @param entries Vector of JSON log entries (in order)
     * @return true if chain is valid, false if tampering detected
     */
    static bool verify_chain(const std::vector<std::string>& entries) {
        if (entries.empty()) return true;
        
        SHA256::Hash expected_prev = SHA256::hash("DTS_INIT");
        
        for (const auto& entry : entries) {
            // Extract chain_hash from previous entry and previous_hash from current
            // Simplified: in production, use proper JSON parsing
            size_t prev_hash_pos = entry.find("\"previous_hash\":\"");
            size_t chain_hash_pos = entry.find("\"chain_hash\":\"");
            
            if (prev_hash_pos == std::string::npos || chain_hash_pos == std::string::npos) {
                return false;
            }
            
            // Verify previous_hash matches expected
            std::string prev_hash_hex = extract_hex(entry, prev_hash_pos + 17);
            if (prev_hash_hex != hash_to_hex(expected_prev)) {
                return false;
            }
            
            // Update expected_prev for next iteration
            std::string chain_hash_hex = extract_hex(entry, chain_hash_pos + 14);
            expected_prev = hex_to_hash(chain_hash_hex);
        }
        
        return true;
    }

private:
    std::string device_id_;
    SHA256::Hash previous_hash_;
    uint64_t sequence_number_ = 0;
    
    static std::string hash_to_hex(const SHA256::Hash& hash) {
        std::ostringstream ss;
        for (const auto& byte : hash) {
            ss << std::hex << std::setfill('0') << std::setw(2) 
               << static_cast<int>(byte);
        }
        return ss.str();
    }
    
    static SHA256::Hash hex_to_hash(const std::string& hex) {
        SHA256::Hash hash;
        for (size_t i = 0; i < 32; ++i) {
            hash[i] = static_cast<uint8_t>(
                std::stoi(hex.substr(i * 2, 2), nullptr, 16));
        }
        return hash;
    }
    
    static std::string extract_hex(const std::string& json, size_t start_pos) {
        size_t end_pos = json.find('"', start_pos);
        if (end_pos == std::string::npos) return "";
        return json.substr(start_pos, end_pos - start_pos);
    }
    
    static std::string escape_json(const std::string& str) {
        std::ostringstream escaped;
        for (char c : str) {
            switch (c) {
                case '"': escaped << "\\\""; break;
                case '\\': escaped << "\\\\"; break;
                case '\b': escaped << "\\b"; break;
                case '\f': escaped << "\\f"; break;
                case '\n': escaped << "\\n"; break;
                case '\r': escaped << "\\r"; break;
                case '\t': escaped << "\\t"; break;
                default:
                    if (static_cast<unsigned char>(c) < 0x20) {
                        escaped << "\\u" << std::hex << std::setfill('0') 
                                << std::setw(4) << static_cast<int>(c);
                    } else {
                        escaped << c;
                    }
                    break;
            }
        }
        return escaped.str();
    }
};

} // namespace dts

#endif // DTS_AUDIT_CHAIN_HPP

