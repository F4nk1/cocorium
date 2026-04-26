package middleware

import (
	"context"
	"net/http"

	"github.com/google/uuid"

	"github.com/F4nk1/cocorium/src/tracer"
)

func Correlation(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		requestID := uuid.New().String()

		ctx := context.WithValue(r.Context(), tracer.RequestIDKey, requestID)

		w.Header().Set("X-Request-Id", requestID)

		next.ServeHTTP(w, r.WithContext(ctx))
	})
}