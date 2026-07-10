#pragma once
#include <cstdint>

namespace Cocorium {
    enum class ErrorCode : uint32_t {
        SUCCESS = 0,
        
        // Autenticación (1000+)
        AUTH_INVALID_CREDENTIALS = 1001,
        AUTH_USER_ALREADY_LOGGED_IN = 1002,
        AUTH_USER_ALREADY_EXISTS = 1003,
        
        // Matchmaking y Juego (2000+)
        MATCH_NOT_FOUND = 2001,
        MATCH_FULL = 2002,
        
        // Cuenta (3000+)
        ACCOUNT_NAME_CHANGE_FAILED = 3001,
        ACCOUNT_PASS_CHANGE_FAILED = 3002,
        
        // Servidor (5000+)
        SERVER_INTERNAL_ERROR = 5000,
        SERVER_DATABASE_TIMEOUT = 5001,
        SERVER_MALFORMED_REQUEST = 5002
    };
}
