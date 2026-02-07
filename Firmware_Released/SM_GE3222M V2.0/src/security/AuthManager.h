#pragma once

#include <Arduino.h>
#include <map>
#include <vector>
#include <ESPAsyncWebServer.h>

struct SessionToken {
    String token;
    unsigned long createdAt;
    unsigned long lastAccess;
    String ipAddress;
};

struct APIKey {
    String key;
    String name;
    unsigned long createdAt;
    bool isActive;
};

struct RateLimitEntry {
    unsigned long timestamp;
    int failedAttempts;
};

class AuthManager {
public:
    static AuthManager& getInstance();
    
    void init();
    
    bool setCredentials(const String& username, const String& password);
    bool authenticate(const String& username, const String& password);
    
    String generateSessionToken(const String& ipAddress);
    bool validateSessionToken(const String& token);
    void invalidateSessionToken(const String& token);
    void cleanupExpiredSessions();
    
    String generateAPIKey(const String& name = "default");
    bool validateAPIKey(const String& key);
    bool revokeAPIKey(const String& key);
    std::vector<APIKey> getAPIKeys();
    
    bool checkRateLimit(const String& ip);
    void recordRequest(const String& ip, bool success);
    void cleanupRateLimits();
    
    bool isAuthenticated(AsyncWebServerRequest* request);
    bool hasValidAPIKey(AsyncWebServerRequest* request);
    
    bool isFirstLogin() const { return firstLogin_; }
    void setFirstLoginComplete() { firstLogin_ = false; }
    
    void setSessionTimeout(unsigned long timeoutMs) { sessionTimeoutMs_ = timeoutMs; }
    unsigned long getSessionTimeout() const { return sessionTimeoutMs_; }
    
    String getCSRFToken(const String& sessionToken);
    bool validateCSRFToken(const String& sessionToken, const String& csrfToken);

private:
    AuthManager();
    ~AuthManager();
    AuthManager(const AuthManager&) = delete;
    AuthManager& operator=(const AuthManager&) = delete;
    
    String hashPassword(const String& password);
    String generateRandomToken(size_t length = 32);
    bool loadCredentials();
    bool saveCredentials();
    bool loadAPIKeys();
    bool saveAPIKeys();
    
    String username_;
    String passwordHash_;
    bool firstLogin_;
    
    std::map<String, SessionToken> sessions_;
    std::map<String, APIKey> apiKeys_;
    std::map<String, RateLimitEntry> rateLimits_;
    std::map<String, String> csrfTokens_;
    
    unsigned long sessionTimeoutMs_;
    static const int MAX_SESSIONS = 10;
    static const int MAX_FAILED_ATTEMPTS = 5;
    static const unsigned long RATE_LIMIT_WINDOW_MS = 60000;
    
    static const char* NVS_NAMESPACE;
    static const char* NVS_USERNAME_KEY;
    static const char* NVS_PASSWORD_KEY;
    static const char* NVS_FIRST_LOGIN_KEY;
    static const char* NVS_API_KEYS_KEY;
};
