package middleware

import (
	"net/http"

	"github.com/F4nk1/cocorium/src/common"
	"github.com/F4nk1/cocorium/src/model"
	"github.com/F4nk1/cocorium/src/service"
	"github.com/F4nk1/cocorium/src/tracer"
)

func Permission(service service.Service) func(http.Handler) http.Handler {
	return func(next http.Handler) http.Handler {
		return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
			ctx := r.Context()

			requiredPermission := common.GetRequiredPermission(r)
			if requiredPermission == common.InvalidPermission {
				next.ServeHTTP(w, r)
				return
			}

			claims, ok := ctx.Value(common.UserContextKey).(*model.Claims)
			if !ok {
				tracer.Warnf(ctx, "Missing claims in context")
				common.WriteErrorResponse(w, common.ACCESS_DENIED_ERROR)
				return
			}

			hasPermission, err := service.HasPermission(ctx, claims.UserId, requiredPermission)
			if err != nil || !hasPermission {
				tracer.Warnf(ctx, "Permission '%s' denied", requiredPermission)
				common.WriteErrorResponse(w, common.MISSING_PERMISSION_ERROR)
				return
			}

			next.ServeHTTP(w, r)
		})
	}
}