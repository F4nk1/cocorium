package middleware

import (
	"fmt"
	"net/http"

	"github.com/F4nk1/cocorium/src/cache"
	"github.com/F4nk1/cocorium/src/common"
	"github.com/F4nk1/cocorium/src/model"
	"github.com/F4nk1/cocorium/src/tracer"
)

func Cache(cacheStore cache.Cache) func(http.Handler) http.Handler {
	return func(next http.Handler) http.Handler {
		return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
			ctx := r.Context()

			if r.Method != http.MethodGet {
				next.ServeHTTP(w, r)
				return
			}

			key := r.URL.Path
			if claims, ok := ctx.Value(common.UserContextKey).(*model.Claims); ok {
				key = fmt.Sprintf("%s:%s", claims.UserId, key)
			}

			if cached, found := cacheStore.Get(key); found {
				tracer.Debugf(ctx, "Cached response served for %s", key)
				w.Header().Set("Content-Type", "application/json")
				w.Write(cached.([]byte))
				return
			}

			recorder := cache.NewRecorder(w)
			next.ServeHTTP(recorder, r)

			cacheStore.Set(key, recorder.Body, cache.DefaultExpiration)
		})
	}
}