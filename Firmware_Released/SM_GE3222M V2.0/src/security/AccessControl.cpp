/**
 * @file AccessControl.cpp
 * @brief Role-based access control implementation
 */

#include "AccessControl.h"
#include "AuthManager.h"
#include "CryptoHelper.h"
#include "../diagnostics/Logger.h"
#include "../storage/NVStorage.h"
#include <ArduinoJson.h>

const char* AccessControl::NVS_NAMESPACE = "access";
const char* AccessControl::NVS_USERS_KEY = "users";

AccessControl& AccessControl::getInstance() {
    static AccessControl instance;
    return instance;
}

AccessControl::AccessControl()
    : _initialized(false) {
}

bool AccessControl::init() {
    if (_initialized) {
        return true;
    }

    // Initialize permission mappings
    initializePermissions();

    // Load users from NVS
    if (!loadUsers()) {
        Logger::getInstance().warn("AccessControl", "No users loaded, creating default admin");
        
        // Create default admin user
        createUser("admin", "admin", UserRole::ADMIN);
        saveUsers();
    }

    _initialized = true;
    Logger::getInstance().info("AccessControl", "Initialized successfully");
    return true;
}

void AccessControl::initializePermissions() {
    // ADMIN - Full permissions
    _rolePermissions[UserRole::ADMIN] = {
        Permission::READ_STATUS,
        Permission::READ_MEASUREMENTS,
        Permission::READ_ENERGY,
        Permission::READ_CONFIG,
        Permission::READ_LOGS,
        Permission::READ_NETWORK,
        Permission::WRITE_CONFIG,
        Permission::WRITE_CALIBRATION,
        Permission::RESET_ENERGY,
        Permission::CONTROL_RELAYS,
        Permission::SYSTEM_REBOOT,
        Permission::SYSTEM_UPDATE,
        Permission::SYSTEM_FACTORY_RESET,
        Permission::MANAGE_USERS,
        Permission::MANAGE_ROLES
    };

    // USER - Read + limited write
    _rolePermissions[UserRole::USER] = {
        Permission::READ_STATUS,
        Permission::READ_MEASUREMENTS,
        Permission::READ_ENERGY,
        Permission::READ_CONFIG,
        Permission::READ_LOGS,
        Permission::READ_NETWORK,
        Permission::WRITE_CONFIG,
        Permission::RESET_ENERGY,
        Permission::CONTROL_RELAYS
    };

    // VIEWER - Read only
    _rolePermissions[UserRole::VIEWER] = {
        Permission::READ_STATUS,
        Permission::READ_MEASUREMENTS,
        Permission::READ_ENERGY,
        Permission::READ_LOGS,
        Permission::READ_NETWORK
    };
}

bool AccessControl::hasPermission(const String& username, Permission permission) {
    auto it = _users.find(username);
    if (it == _users.end()) {
        return false;
    }

    if (!it->second.enabled) {
        return false;
    }

    return roleHasPermission(it->second.role, permission);
}

bool AccessControl::roleHasPermission(UserRole role, Permission permission) {
    auto it = _rolePermissions.find(role);
    if (it == _rolePermissions.end()) {
        return false;
    }

    const std::vector<Permission>& permissions = it->second;
    return std::find(permissions.begin(), permissions.end(), permission) != permissions.end();
}

bool AccessControl::checkRequestPermission(AsyncWebServerRequest* request, Permission permission) {
    if (request == nullptr) {
        return false;
    }

    String username = getUsernameFromRequest(request);
    if (username.length() == 0) {
        return false;
    }

    return hasPermission(username, permission);
}

UserRole AccessControl::getUserRole(const String& username) {
    auto it = _users.find(username);
    if (it != _users.end()) {
        return it->second.role;
    }
    return UserRole::VIEWER;  // Default to most restrictive
}

bool AccessControl::setUserRole(const String& username, UserRole role) {
    auto it = _users.find(username);
    if (it == _users.end()) {
        Logger::getInstance().warn("AccessControl", ("User not found: " + username).c_str());
        return false;
    }

    it->second.role = role;
    
    Logger::getInstance().info("AccessControl", 
        ("Role changed for " + username + " to " + String(getRoleName(role))).c_str());
    
    return saveUsers();
}

bool AccessControl::createUser(const String& username, const String& password, UserRole role) {
    if (username.length() == 0) {
        Logger::getInstance().error("AccessControl", "Empty username");
        return false;
    }

    if (_users.find(username) != _users.end()) {
        Logger::getInstance().warn("AccessControl", ("User already exists: " + username).c_str());
        return false;
    }

    UserAccount account;
    account.username = username;
    account.passwordHash = hashPassword(password);
    account.role = role;
    account.enabled = true;
    account.createdAt = millis() / 1000;
    account.lastLogin = 0;
    account.loginCount = 0;

    _users[username] = account;

    Logger::getInstance().info("AccessControl", ("User created: " + username).c_str());
    return saveUsers();
}

bool AccessControl::deleteUser(const String& username) {
    auto it = _users.find(username);
    if (it == _users.end()) {
        return false;
    }

    // Don't allow deleting the last admin
    if (it->second.role == UserRole::ADMIN) {
        int adminCount = 0;
        for (const auto& pair : _users) {
            if (pair.second.role == UserRole::ADMIN && pair.second.enabled) {
                adminCount++;
            }
        }
        
        if (adminCount <= 1) {
            Logger::getInstance().error("AccessControl", "Cannot delete last admin user");
            return false;
        }
    }

    _users.erase(it);
    Logger::getInstance().info("AccessControl", ("User deleted: " + username).c_str());
    return saveUsers();
}

bool AccessControl::setUserEnabled(const String& username, bool enabled) {
    auto it = _users.find(username);
    if (it == _users.end()) {
        return false;
    }

    it->second.enabled = enabled;
    
    Logger::getInstance().info("AccessControl", 
        ("User " + username + (enabled ? " enabled" : " disabled")).c_str());
    
    return saveUsers();
}

const UserAccount* AccessControl::getUser(const String& username) {
    auto it = _users.find(username);
    if (it != _users.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<String> AccessControl::getUserList() {
    std::vector<String> usernames;
    for (const auto& pair : _users) {
        usernames.push_back(pair.first);
    }
    return usernames;
}

void AccessControl::recordLogin(const String& username) {
    auto it = _users.find(username);
    if (it != _users.end()) {
        it->second.lastLogin = millis() / 1000;
        it->second.loginCount++;
        saveUsers();
    }
}

bool AccessControl::loadUsers() {
    NVStorage& nvs = NVStorage::getInstance();
    
    String jsonData = nvs.getString(NVS_NAMESPACE, NVS_USERS_KEY, "");
    if (jsonData.length() == 0) {
        Logger::getInstance().warn("AccessControl", "No user data in NVS");
        return false;
    }

    DynamicJsonDocument doc(4096);
    DeserializationError error = deserializeJson(doc, jsonData);
    
    if (error) {
        Logger::getInstance().error("AccessControl", "Failed to parse user data");
        return false;
    }

    _users.clear();
    
    JsonArray usersArray = doc["users"].as<JsonArray>();
    for (JsonObject userObj : usersArray) {
        UserAccount account;
        account.username = userObj["username"].as<String>();
        account.passwordHash = userObj["passwordHash"].as<String>();
        account.role = static_cast<UserRole>(userObj["role"].as<int>());
        account.enabled = userObj["enabled"].as<bool>();
        account.createdAt = userObj["createdAt"].as<unsigned long>();
        account.lastLogin = userObj["lastLogin"].as<unsigned long>();
        account.loginCount = userObj["loginCount"].as<uint32_t>();
        
        _users[account.username] = account;
    }

    Logger::getInstance().info("AccessControl", ("Loaded " + String(_users.size()) + " users").c_str());
    return true;
}

bool AccessControl::saveUsers() {
    DynamicJsonDocument doc(4096);
    JsonArray usersArray = doc.createNestedArray("users");

    for (const auto& pair : _users) {
        JsonObject userObj = usersArray.createNestedObject();
        userObj["username"] = pair.second.username;
        userObj["passwordHash"] = pair.second.passwordHash;
        userObj["role"] = static_cast<int>(pair.second.role);
        userObj["enabled"] = pair.second.enabled;
        userObj["createdAt"] = pair.second.createdAt;
        userObj["lastLogin"] = pair.second.lastLogin;
        userObj["loginCount"] = pair.second.loginCount;
    }

    String jsonData;
    serializeJson(doc, jsonData);

    NVStorage& nvs = NVStorage::getInstance();
    if (!nvs.putString(NVS_NAMESPACE, NVS_USERS_KEY, jsonData.c_str())) {
        Logger::getInstance().error("AccessControl", "Failed to save users to NVS");
        return false;
    }

    Logger::getInstance().info("AccessControl", "Users saved to NVS");
    return true;
}

String AccessControl::getUsernameFromRequest(AsyncWebServerRequest* request) {
    // Try to get username from session token
    if (request->hasHeader("Authorization")) {
        String authHeader = request->header("Authorization");
        
        // Extract token from "Bearer <token>"
        if (authHeader.startsWith("Bearer ")) {
            String token = authHeader.substring(7);
            
            // Get username from AuthManager session
            // This is a simplified approach - AuthManager would need to provide this
            // For now, check if token is valid
            if (AuthManager::getInstance().validateSessionToken(token)) {
                // In a full implementation, AuthManager should return username
                // For now, return "admin" if valid
                return "admin";
            }
        }
    }

    // Try to get from cookie
    if (request->hasHeader("Cookie")) {
        // Parse cookie for session token
        // Implementation depends on cookie format
    }

    return "";
}

String AccessControl::hashPassword(const String& password) {
    // Use CryptoHelper for password hashing (static method)
    return CryptoHelper::hashPassword(password);
}

const char* AccessControl::getRoleName(UserRole role) {
    switch (role) {
        case UserRole::ADMIN:
            return "Admin";
        case UserRole::USER:
            return "User";
        case UserRole::VIEWER:
            return "Viewer";
        default:
            return "Unknown";
    }
}

const char* AccessControl::getPermissionName(Permission permission) {
    switch (permission) {
        case Permission::READ_STATUS:
            return "ReadStatus";
        case Permission::READ_MEASUREMENTS:
            return "ReadMeasurements";
        case Permission::READ_ENERGY:
            return "ReadEnergy";
        case Permission::READ_CONFIG:
            return "ReadConfig";
        case Permission::READ_LOGS:
            return "ReadLogs";
        case Permission::READ_NETWORK:
            return "ReadNetwork";
        case Permission::WRITE_CONFIG:
            return "WriteConfig";
        case Permission::WRITE_CALIBRATION:
            return "WriteCalibration";
        case Permission::RESET_ENERGY:
            return "ResetEnergy";
        case Permission::CONTROL_RELAYS:
            return "ControlRelays";
        case Permission::SYSTEM_REBOOT:
            return "SystemReboot";
        case Permission::SYSTEM_UPDATE:
            return "SystemUpdate";
        case Permission::SYSTEM_FACTORY_RESET:
            return "SystemFactoryReset";
        case Permission::MANAGE_USERS:
            return "ManageUsers";
        case Permission::MANAGE_ROLES:
            return "ManageRoles";
        default:
            return "Unknown";
    }
}
