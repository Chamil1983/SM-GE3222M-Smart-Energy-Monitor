/**
 * @file AccessControl.h
 * @brief Role-based access control system
 * 
 * SM-GE3222M Smart Energy Monitor V2.0
 * Implements three-tier role system with permission checking
 */

#pragma once

#include <Arduino.h>
#include <map>
#include <vector>
#include <ESPAsyncWebServer.h>

/**
 * @brief User roles enum
 */
enum class UserRole {
    VIEWER,     // Read-only access
    USER,       // Read + limited write
    ADMIN       // Full access
};

/**
 * @brief Permission types enum
 */
enum class Permission {
    // Read permissions
    READ_STATUS,
    READ_MEASUREMENTS,
    READ_ENERGY,
    READ_CONFIG,
    READ_LOGS,
    READ_NETWORK,
    
    // Write permissions
    WRITE_CONFIG,
    WRITE_CALIBRATION,
    RESET_ENERGY,
    CONTROL_RELAYS,
    
    // System permissions
    SYSTEM_REBOOT,
    SYSTEM_UPDATE,
    SYSTEM_FACTORY_RESET,
    
    // User management
    MANAGE_USERS,
    MANAGE_ROLES
};

/**
 * @brief User account structure
 */
struct UserAccount {
    String username;
    String passwordHash;
    UserRole role;
    bool enabled;
    unsigned long createdAt;
    unsigned long lastLogin;
    uint32_t loginCount;
    
    UserAccount() 
        : role(UserRole::VIEWER)
        , enabled(true)
        , createdAt(0)
        , lastLogin(0)
        , loginCount(0) {}
};

/**
 * @class AccessControl
 * @brief Role-based access control manager
 * 
 * Features:
 * - Three roles: Admin, User, Viewer
 * - Permission checking for API endpoints
 * - User account management
 * - Role persistence in NVS
 * - Session integration with AuthManager
 */
class AccessControl {
public:
    /**
     * @brief Get singleton instance
     * @return Reference to AccessControl instance
     */
    static AccessControl& getInstance();

    /**
     * @brief Initialize access control
     * @return true if successful, false on failure
     */
    bool init();

    /**
     * @brief Check if user has permission
     * @param username Username
     * @param permission Permission to check
     * @return true if permitted
     */
    bool hasPermission(const String& username, Permission permission);

    /**
     * @brief Check if role has permission
     * @param role User role
     * @param permission Permission to check
     * @return true if permitted
     */
    bool roleHasPermission(UserRole role, Permission permission);

    /**
     * @brief Check permission via HTTP request
     * @param request HTTP request
     * @param permission Permission to check
     * @return true if permitted
     */
    bool checkRequestPermission(AsyncWebServerRequest* request, Permission permission);

    /**
     * @brief Get user role
     * @param username Username
     * @return User role (VIEWER if not found)
     */
    UserRole getUserRole(const String& username);

    /**
     * @brief Set user role
     * @param username Username
     * @param role New role
     * @return true if successful
     */
    bool setUserRole(const String& username, UserRole role);

    /**
     * @brief Create new user account
     * @param username Username
     * @param password Password (will be hashed)
     * @param role Initial role
     * @return true if successful
     */
    bool createUser(const String& username, const String& password, UserRole role);

    /**
     * @brief Delete user account
     * @param username Username
     * @return true if successful
     */
    bool deleteUser(const String& username);

    /**
     * @brief Enable/disable user account
     * @param username Username
     * @param enabled Enable status
     * @return true if successful
     */
    bool setUserEnabled(const String& username, bool enabled);

    /**
     * @brief Get user account
     * @param username Username
     * @return Pointer to UserAccount or nullptr
     */
    const UserAccount* getUser(const String& username);

    /**
     * @brief Get all users
     * @return Vector of usernames
     */
    std::vector<String> getUserList();

    /**
     * @brief Record user login
     * @param username Username
     */
    void recordLogin(const String& username);

    /**
     * @brief Load users from NVS
     * @return true if successful
     */
    bool loadUsers();

    /**
     * @brief Save users to NVS
     * @return true if successful
     */
    bool saveUsers();

    /**
     * @brief Get role name as string
     * @param role User role
     * @return Role name
     */
    static const char* getRoleName(UserRole role);

    /**
     * @brief Get permission name as string
     * @param permission Permission
     * @return Permission name
     */
    static const char* getPermissionName(Permission permission);

private:
    AccessControl();
    ~AccessControl() = default;
    AccessControl(const AccessControl&) = delete;
    AccessControl& operator=(const AccessControl&) = delete;

    /**
     * @brief Initialize default permissions
     */
    void initializePermissions();

    /**
     * @brief Get username from request session
     * @param request HTTP request
     * @return Username or empty string
     */
    String getUsernameFromRequest(AsyncWebServerRequest* request);

    /**
     * @brief Hash password
     * @param password Plain text password
     * @return Password hash
     */
    String hashPassword(const String& password);

    bool _initialized;
    std::map<String, UserAccount> _users;
    std::map<UserRole, std::vector<Permission>> _rolePermissions;

    static const char* NVS_NAMESPACE;
    static const char* NVS_USERS_KEY;
};
