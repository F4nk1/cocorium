package common

import "net/http"

const (
	// Client errors (1xxx)
	INVALID_REQUEST_ERROR = 1001
	MISSING_FIELDS_ERROR  = 1002
	NOT_FOUND_ERROR       = 1003
	ALREADY_EXISTS_ERROR  = 1004

	// Auth errors (2xxx)
	INVALID_CREDENTIALS_ERROR = 2001
	ACCESS_DENIED_ERROR       = 2002
	MISSING_PERMISSION_ERROR  = 2003

	// Server errors (5xxx)
	DATABASE_ERROR = 5001
	INTERNAL_ERROR = 5002
)

var ErrorCodes = map[int]int{
	INVALID_REQUEST_ERROR:     http.StatusBadRequest,
	MISSING_FIELDS_ERROR:      http.StatusBadRequest,
	NOT_FOUND_ERROR:           http.StatusNotFound,
	ALREADY_EXISTS_ERROR:      http.StatusConflict,
	INVALID_CREDENTIALS_ERROR: http.StatusUnauthorized,
	ACCESS_DENIED_ERROR:       http.StatusForbidden,
	MISSING_PERMISSION_ERROR:  http.StatusForbidden,
	DATABASE_ERROR:            http.StatusInternalServerError,
	INTERNAL_ERROR:            http.StatusInternalServerError,
}

var ErrorMessages = map[int]string{
	INVALID_REQUEST_ERROR:     "Invalid request format",
	MISSING_FIELDS_ERROR:      "Required fields are missing",
	NOT_FOUND_ERROR:           "Resource not found",
	ALREADY_EXISTS_ERROR:      "Resource already exists",
	INVALID_CREDENTIALS_ERROR: "Invalid credentials provided",
	ACCESS_DENIED_ERROR:       "Authentication required",
	MISSING_PERMISSION_ERROR:  "Insufficient permissions",
	DATABASE_ERROR:            "Database operation failed",
	INTERNAL_ERROR:            "Internal server error",
}