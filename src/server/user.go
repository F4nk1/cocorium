package server

import (
	"database/sql"
	"encoding/json"
	"fmt"
	"net/http"
	"strconv"

	"github.com/gorilla/mux"

	"github.com/F4nk1/cocorium/src/common"
	"github.com/F4nk1/cocorium/src/model"
	"github.com/F4nk1/cocorium/src/tracer"
)

func (s *Server) listUsers(w http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	members, err := s.Service.ListUsers(ctx)
	if err != nil {
		tracer.Errorf(ctx, "Failed to list users: %s", err)
		common.WriteErrorResponse(w, common.DATABASE_ERROR)
		return
	}

	common.WriteObjectResponse(w, http.StatusOK, members)
}

func (s *Server) getUser(w http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	id := mux.Vars(r)["id"]

	user, err := s.Service.GetUser(ctx, id)
	if err != nil {
		if err == sql.ErrNoRows {
			tracer.Warnf(ctx, "User '%s' not found", id)
			common.WriteErrorMessage(w, common.NOT_FOUND_ERROR, "User not found")
		} else {
			tracer.Errorf(ctx, "Database error retrieving user '%s': %s", id, err)
			common.WriteErrorResponse(w, common.DATABASE_ERROR)
		}
		return
	}

	common.WriteObjectResponse(w, http.StatusOK, user)
}

func (s *Server) updateUser(w http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	id := mux.Vars(r)["id"]

	var user model.User
	if err := json.NewDecoder(r.Body).Decode(&user); err != nil {
		tracer.Warnf(ctx, "Failed to decode request: %s", err)
		common.WriteErrorResponse(w, common.INVALID_REQUEST_ERROR)
		return
	}
	user.Username = id

	if err := validateUser(&user); err != nil {
		tracer.Warnf(ctx, "User validation failed: %s", err)
		common.WriteErrorResponse(w, common.MISSING_FIELDS_ERROR)
		return
	}

	err := s.Service.UpdateUser(ctx, &user)
	if err != nil {
		tracer.Errorf(ctx, "Failed to update user: %s", err)
		common.WriteErrorResponse(w, common.DATABASE_ERROR)
		return
	}

	tracer.Infof(ctx, "User '%s' updated successfully", id)

	common.WriteSuccessResponse(w, http.StatusOK, fmt.Sprintf("User %s updated successfully", id))
}

func (s *Server) activateUser(w http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	id := mux.Vars(r)["id"]
	isActive, _ := strconv.ParseBool(r.URL.Query().Get("status"))

	err := s.Service.ActivateUser(ctx, id, isActive)
	if err != nil {
		tracer.Warnf(ctx, "Failed to activate user %s: %s", id, err)
		common.WriteErrorMessage(w, common.NOT_FOUND_ERROR, "User not found")
		return
	}

	tracer.Infof(ctx, "User '%s' activated successfully", id)

	common.WriteSuccessResponse(w, http.StatusOK, fmt.Sprintf("User %s activated successfully", id))
}