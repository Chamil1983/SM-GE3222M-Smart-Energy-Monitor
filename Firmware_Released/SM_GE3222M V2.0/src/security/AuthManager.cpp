#include "AuthManager.h"
#include "../storage/NVStorage.h"
#include <mbedtls/md.h>
#include <esp_random.h>

const char* AuthManager::NVS_NAMESPACE = "auth";
const char* AuthManager::NVS_USERNAME_KEY = "username";
const char* AuthManager::NVS_PASSWORD_KEY = "password";
const char* AuthManager::NVS_FIRST_LOGIN_KEY = "first_login";
const char* AuthManager::NVS_API_KEYS_KEY = "api_keys";

AuthManager::AuthManager() 
    : username_("admin")
    , passwordHash_("")
    , firstLogin_(true)
    , sessionTimeoutMs_(30 * 60 * 1000) {
}

AuthManager::~AuthManager() {
    sessions_.clear();
    apiKeys_.clear();
    rateLimits_.clear();
}

AuthManager& AuthManager::getInstance() {
    static AuthManager instance;
    return instance;
}

void AuthManager::init() {
    NVStorage& nvs = NVStorage::getInstance();
    nvs.begin(NVS_NAMESPACE);
    
    if (!loadCredentials()) {
        String defaultPassword = hashPassword("admin");
        username_ = "admin";
        passwordHash_ = defaultPassword;
        firstLogin_ = true;
        saveCredentials();
    }
    
    loadAPIKeys();
    
    Serial.println("[AuthManager] Initialized");
}

bool AuthManager::loadCredentials() {
    NVStorage& nvs = NVStorage::getInstance();
    
    if (nvs.hasKey(NVS_USERNAME_KEY)) {
        username_ = nvs.getString(NVS_USERNAME_KEY, "admin");
        passwordHash_ = nvs.getString(NVS_PASSWORD_KEY, "");
        firstLogin_ = nvs.getBool(NVS_FIRST_LOGIN_KEY, true);
        return !passwordHash_.isEmpty();
    }
    
    return false;
}

bool AuthManager::saveCredentials() {
    NVStorage& nvs = NVStorage::getInstance();
    
    nvs.putString(NVS_USERNAME_KEY, username_);
    nvs.putString(NVS_PASSWORD_KEY, passwordHash_);
    nvs.putBool(NVS_FIRST_LOGIN_KEY, firstLogin_);
    
    return true;
}

bool AuthManager::loadAPIKeys() {
    NVStorage& nvs = NVStorage::getInstance();
    
    if (nvs.hasKey(NVS_API_KEYS_KEY)) {
        String keysJson = nvs.getString(NVS_API_KEYS_KEY, "");
        // Parse JSON and populate apiKeys_ map
        // Simple implementation: store as key1:name1:timestamp1:active1|key2:name2:timestamp2:active2
        if (keysJson.length() > 0) {
            int start = 0;
            while (start < keysJson.length()) {
                int end = keysJson.indexOf('|', start);
                if (end == -1) end = keysJson.length();
                
                String entry = keysJson.substring(start, end);
                int colon1 = entry.indexOf(':');
                int colon2 = entry.indexOf(':', colon1 + 1);
                int colon3 = entry.indexOf(':', colon2 + 1);
                
                if (colon1 > 0 && colon2 > colon1 && colon3 > colon2) {
                    APIKey apiKey;
                    apiKey.key = entry.substring(0, colon1);
                    apiKey.name = entry.substring(colon1 + 1, colon2);
                    apiKey.createdAt = entry.substring(colon2 + 1, colon3).toInt();
                    apiKey.isActive = entry.substring(colon3 + 1).toInt() == 1;
                    
                    apiKeys_[apiKey.key] = apiKey;
                }
                
                start = end + 1;
            }
        }
        return true;
    }
    
    return false;
}

bool AuthManager::saveAPIKeys() {
    NVStorage& nvs = NVStorage::getInstance();
    
    String keysJson = "";
    for (auto& pair : apiKeys_) {
        if (keysJson.length() > 0) keysJson += "|";
        APIKey& key = pair.second;
        keysJson += key.key + ":" + key.name + ":" + String(key.createdAt) + ":" + (key.isActive ? "1" : "0");
    }
    
    nvs.putString(NVS_API_KEYS_KEY, keysJson);
    return true;
}

String AuthManager::hashPassword(const String& password) {
    unsigned char hash[32];
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;
    
    mbedtls_md_init(&ctx);
    mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
    mbedtls_md_starts(&ctx);
    mbedtls_md_update(&ctx, (const unsigned char*)password.c_str(), password.length());
    mbedtls_md_finish(&ctx, hash);
    mbedtls_md_free(&ctx);
    
    String hashStr = "";
    for (int i = 0; i < 32; i++) {
        char hex[3];
        sprintf(hex, "%02x", hash[i]);
        hashStr += hex;
    }
    
    return hashStr;
}

String AuthManager::generateRandomToken(size_t length) {
    String token = "";
    for (size_t i = 0; i < length; i++) {
        uint8_t randomByte = esp_random() & 0xFF;
        char hex[3];
        sprintf(hex, "%02x", randomByte);
        token += hex;
    }
    return token;
}

bool AuthManager::setCredentials(const String& username, const String& password) {
    if (username.length() < 3 || password.length() < 8) {
        return false;
    }
    
    username_ = username;
    passwordHash_ = hashPassword(password);
    firstLogin_ = false;
    
    return saveCredentials();
}

bool AuthManager::authenticate(const String& username, const String& password) {
    if (username != username_) {
        return false;
    }
    
    String hash = hashPassword(password);
    return hash == passwordHash_;
}

String AuthManager::generateSessionToken(const String& ipAddress) {
    cleanupExpiredSessions();
    
    if (sessions_.size() >= MAX_SESSIONS) {
        unsigned long oldestTime = ULONG_MAX;
        String oldestToken = "";
        
        for (auto& pair : sessions_) {
            if (pair.second.lastAccess < oldestTime) {
                oldestTime = pair.second.lastAccess;
                oldestToken = pair.first;
            }
        }
        
        if (!oldestToken.isEmpty()) {
            sessions_.erase(oldestToken);
        }
    }
    
    String token = generateRandomToken(32);
    
    SessionToken session;
    session.token = token;
    session.createdAt = millis();
    session.lastAccess = millis();
    session.ipAddress = ipAddress;
    
    sessions_[token] = session;
    
    csrfTokens_[token] = generateRandomToken(16);
    
    return token;
}

bool AuthManager::validateSessionToken(const String& token) {
    auto it = sessions_.find(token);
    if (it == sessions_.end()) {
        return false;
    }
    
    unsigned long now = millis();
    unsigned long elapsed = now - it->second.lastAccess;
    
    if (elapsed > sessionTimeoutMs_) {
        sessions_.erase(it);
        csrfTokens_.erase(token);
        return false;
    }
    
    it->second.lastAccess = now;
    return true;
}

void AuthManager::invalidateSessionToken(const String& token) {
    sessions_.erase(token);
    csrfTokens_.erase(token);
}

void AuthManager::cleanupExpiredSessions() {
    unsigned long now = millis();
    std::vector<String> toRemove;
    
    for (auto& pair : sessions_) {
        unsigned long elapsed = now - pair.second.lastAccess;
        if (elapsed > sessionTimeoutMs_) {
            toRemove.push_back(pair.first);
        }
    }
    
    for (const String& token : toRemove) {
        sessions_.erase(token);
        csrfTokens_.erase(token);
    }
}

String AuthManager::generateAPIKey(const String& name) {
    String key = generateRandomToken(16);
    
    APIKey apiKey;
    apiKey.key = key;
    apiKey.name = name;
    apiKey.createdAt = millis();
    apiKey.isActive = true;
    
    apiKeys_[key] = apiKey;
    saveAPIKeys();
    
    return key;
}

bool AuthManager::validateAPIKey(const String& key) {
    auto it = apiKeys_.find(key);
    if (it == apiKeys_.end()) {
        return false;
    }
    
    return it->second.isActive;
}

bool AuthManager::revokeAPIKey(const String& key) {
    auto it = apiKeys_.find(key);
    if (it == apiKeys_.end()) {
        return false;
    }
    
    it->second.isActive = false;
    saveAPIKeys();
    return true;
}

std::vector<APIKey> AuthManager::getAPIKeys() {
    std::vector<APIKey> keys;
    for (auto& pair : apiKeys_) {
        keys.push_back(pair.second);
    }
    return keys;
}

bool AuthManager::checkRateLimit(const String& ip) {
    cleanupRateLimits();
    
    auto it = rateLimits_.find(ip);
    if (it == rateLimits_.end()) {
        return true;
    }
    
    return it->second.failedAttempts < MAX_FAILED_ATTEMPTS;
}

void AuthManager::recordRequest(const String& ip, bool success) {
    cleanupRateLimits();
    
    auto it = rateLimits_.find(ip);
    if (it == rateLimits_.end()) {
        RateLimitEntry entry;
        entry.timestamp = millis();
        entry.failedAttempts = success ? 0 : 1;
        rateLimits_[ip] = entry;
    } else {
        if (success) {
            rateLimits_.erase(it);
        } else {
            it->second.failedAttempts++;
            it->second.timestamp = millis();
        }
    }
}

void AuthManager::cleanupRateLimits() {
    unsigned long now = millis();
    std::vector<String> toRemove;
    
    for (auto& pair : rateLimits_) {
        unsigned long elapsed = now - pair.second.timestamp;
        if (elapsed > RATE_LIMIT_WINDOW_MS) {
            toRemove.push_back(pair.first);
        }
    }
    
    for (const String& ip : toRemove) {
        rateLimits_.erase(ip);
    }
}

bool AuthManager::isAuthenticated(AsyncWebServerRequest* request) {
    if (!request) return false;
    
    if (request->hasHeader("Cookie")) {
        String cookie = request->header("Cookie");
        int tokenStart = cookie.indexOf("session=");
        if (tokenStart >= 0) {
            tokenStart += 8;
            int tokenEnd = cookie.indexOf(';', tokenStart);
            if (tokenEnd < 0) tokenEnd = cookie.length();
            
            String token = cookie.substring(tokenStart, tokenEnd);
            return validateSessionToken(token);
        }
    }
    
    return false;
}

bool AuthManager::hasValidAPIKey(AsyncWebServerRequest* request) {
    if (!request) return false;
    
    if (request->hasHeader("X-API-Key")) {
        String apiKey = request->header("X-API-Key");
        return validateAPIKey(apiKey);
    }
    
    return false;
}

String AuthManager::getCSRFToken(const String& sessionToken) {
    auto it = csrfTokens_.find(sessionToken);
    if (it != csrfTokens_.end()) {
        return it->second;
    }
    return "";
}

bool AuthManager::validateCSRFToken(const String& sessionToken, const String& csrfToken) {
    auto it = csrfTokens_.find(sessionToken);
    if (it == csrfTokens_.end()) {
        return false;
    }
    
    return it->second == csrfToken;
}
