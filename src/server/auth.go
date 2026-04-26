package server

import (
	"database/sql"
	"encoding/json"
	"net/http"

	"github.com/F4nk1/cocorium/src/common"
	"github.com/F4nk1/cocorium/src/model"
	"github.com/F4nk1/cocorium/src/tracer"
)

type LoginRequest struct {
	User model.User `json:"user"`
}

func (s *Server) login(w http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	var request LoginRequest
	if err := json.NewDecoder(r.Body).Decode(&request); err != nil {
		tracer.Errorf(ctx, "Failed to decode JWT login request: %s", err)
		common.WriteErrorResponse(w, common.INVALID_REQUEST_ERROR)
		return
	}

	if request.User.Username == "" || request.User.Password == "" {
		tracer.Warnf(ctx, "Missing login credentials")
		common.WriteErrorResponse(w, common.MISSING_FIELDS_ERROR)
		return
	}

	user, err := s.Service.Login(ctx, request.User.Username, request.User.Password)
	if err != nil {
		if err == sql.ErrNoRows {
			tracer.Warnf(ctx, "Failed JWT login attempt for user with Username '%s' from IP '%s'", request.User.Username, r.RemoteAddr)
			common.WriteErrorMessage(w, common.NOT_FOUND_ERROR, "User not found")
			return
		}
		tracer.Errorf(ctx, "Database error during JWT login for user with Username '%s': %s", request.User.Username, err)
		common.WriteErrorResponse(w, common.DATABASE_ERROR)
		return
	}

	tracer.Infof(ctx, "Successful JWT login for user with Username '%s' from IP '%s'", request.User.Username, r.RemoteAddr)

	// Generate JWT tokens
	tokens, err := s.Auth.GenerateAuthToken(user.Username, user.RoleId)
	if err != nil {
		tracer.Errorf(ctx, "Failed to generate tokens for user '%s': %s", user.Id, err)
		common.WriteErrorResponse(w, common.INTERNAL_ERROR)
		return
	}

	common.WriteObjectResponse(w, http.StatusOK, tokens)
}

func (s *Server) refreshToken(w http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	var request struct {
		RefreshToken string `json:"refresh_token"`
	}

	if err := json.NewDecoder(r.Body).Decode(&request); err != nil {
		tracer.Errorf(ctx, "Failed to decode refresh request: %s", err)
		common.WriteErrorResponse(w, common.INVALID_REQUEST_ERROR)
		return
	}

	if request.RefreshToken == "" {
		tracer.Warnf(ctx, "Missing refresh token")
		common.WriteErrorResponse(w, common.MISSING_FIELDS_ERROR)
		return
	}

	claims, err := s.Auth.ValidateRefreshToken(request.RefreshToken)
	if err != nil {
		tracer.Warnf(ctx, "Invalid refresh token from IP '%s': %s", r.RemoteAddr, err)
		common.WriteErrorResponse(w, common.ACCESS_DENIED_ERROR)
		return
	}

	// Generate new token pair
	tokens, err := s.Auth.GenerateAuthToken(claims.UserId, claims.RoleId)
	if err != nil {
		tracer.Errorf(ctx, "Failed to generate new tokens for user '%s': %s", claims.UserId, err)
		common.WriteErrorResponse(w, common.INTERNAL_ERROR)
		return
	}

	common.WriteObjectResponse(w, http.StatusOK, tokens)
}

func (s *Server) checkSession(w http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	claims, ok := ctx.Value(common.UserContextKey).(*model.Claims)
	if !ok {
		tracer.Warnf(ctx, "Missing claims in session check")
		common.WriteErrorResponse(w, common.ACCESS_DENIED_ERROR)
		return
	}

	user, err := s.Service.GetUser(ctx, claims.UserId)
	if err != nil {
		if err == sql.ErrNoRows {
			tracer.Warnf(ctx, "Session user not found")
			common.WriteErrorMessage(w, common.NOT_FOUND_ERROR, "User not found")
			return
		}
		tracer.Errorf(ctx, "Session check failed: %s", err)
		common.WriteErrorResponse(w, common.DATABASE_ERROR)
		return
	}

	common.WriteObjectResponse(w, http.StatusOK, user)
}