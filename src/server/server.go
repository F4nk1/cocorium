package server

import (
	"net/http"
	"os"

	"github.com/gorilla/mux"
	"github.com/gorilla/sessions"

	"github.com/F4nk1/cocorium/src/cache"
	"github.com/F4nk1/cocorium/src/service"
	"github.com/F4nk1/cocorium/src/auth"
	"github.com/F4nk1/cocorium/src/middleware"
)

type Server struct {
	Handler 		http.Handler
	Service 		service.Service
	SessionStore 	*sessions.CookieStore
	Cache 			cache.Cache
	Auth 			*auth.Auth
}

func NewServer(service service.Service) *Server {
	s := &Server{
		Service: service,
		SessionStore: sessions.NewCookieStore([]byte(os.Getenv("SESSION_KEY"))),
		Cache: cache.NewSimpleCache(),
		Auth: auth.NewAuth(os.Getenv("AUTH_SECRET"), os.Getenv("REFRESH_SECRET")),
	}
	s.Handler = s.buildHandler()
	return s
}

func (s *Server) buildHandler() http.Handler {
	router := mux.NewRouter()

	router.Use(middleware.Correlation)
	router.Use(middleware.CORS)

	api := router.PathPrefix("/api").Subrouter()

	// Public routes
	api.HandleFunc("/login", s.login).Methods(http.MethodPost)
	api.HandleFunc("/refresh", s.refreshToken).Methods(http.MethodPost)
	
	// Protected routes
	protected := api.NewRoute().Subrouter()
	protected.Use(middleware.Auth(s.Auth))
	protected.Use(middleware.Cache(s.Cache))
	protected.Use(middleware.Permission(s.Service))

	protected.HandleFunc("/profile", s.checkSession).Methods(http.MethodOptions)

	protected.HandleFunc("/users", s.listUsers).Methods(http.MethodGet)
	protected.HandleFunc("/users/{id}", s.getUser).Methods(http.MethodGet)
	protected.HandleFunc("/users/{id}", s.updateUser).Methods(http.MethodPut)
	protected.HandleFunc("/users/{id}/delete", s.SoftDeleteUser).Methods(http.MethodDelete)
	protected.HandleFunc("/users/{id}/undelete", s.unDeleteUser).Methods(http.MethodPatch)

	protected.HandleFunc("/game/create", CreateGameHandler).Methods(http.MethodPost)
	protected.HandleFunc("/game/action", ActionHandler).Methods(http.MethodPost)
	protected.HandleFunc("/game/state", StateHandler).Methods(http.MethodPost)

	return router
}