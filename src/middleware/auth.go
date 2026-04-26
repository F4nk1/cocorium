package middleware

import (
	"context"
	"fmt"
	"net/http"
	"strings"

	"github.com/F4nk1/cocorium/src/auth"
	"github.com/F4nk1/cocorium/src/common"
	"github.com/F4nk1/cocorium/src/tracer"
)

func Auth(authService *auth.Auth) func(http.Handler) http.Handler {
	return func(next http.Handler) http.Handler {
		return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
			ctx := r.Context()

			authHeader := r.Header.Get("Authorization")
			if authHeader == "" {
				tracer.Warnf(ctx, "Missing authorization header")
				common.WriteErrorResponse(w, common.ACCESS_DENIED_ERROR)
				return
			}

			tokenString := strings.TrimPrefix(authHeader, fmt.Sprintf("%s ", auth.TokenType))
			if tokenString == authHeader {
				tracer.Warnf(ctx, "Invalid token format")
				common.WriteErrorResponse(w, common.INVALID_CREDENTIALS_ERROR)
				return
			}

			claims, err := authService.ValidateAccessToken(tokenString)
			if err != nil {
				tracer.Warnf(ctx, "Invalid access token: %s", err)
				common.WriteErrorResponse(w, common.INVALID_CREDENTIALS_ERROR)
				return
			}

			ctx = context.WithValue(ctx, common.UserContextKey, claims)
			ctx = context.WithValue(ctx, tracer.UserIdKey, claims.UserId)

			next.ServeHTTP(w, r.WithContext(ctx))
		})
	}
}