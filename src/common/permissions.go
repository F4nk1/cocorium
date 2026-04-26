package common

import (
	"github.com/gorilla/mux"
	"net/http"
)

const (
	InvalidPermission      = "invalid"
)

var RoutePermissions = map[string]map[string]string{

}

func GetRequiredPermission(r *http.Request) string {
	route := mux.CurrentRoute(r)
	if route == nil {
		return InvalidPermission
	}

	template, err := route.GetPathTemplate()
	if err != nil {
		return InvalidPermission
	}

	if methods, exists := RoutePermissions[template]; exists {
		return methods[r.Method]
	}

	return InvalidPermission
}