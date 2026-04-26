package model

import "github.com/golang-jwt/jwt/v5"

type Token struct {
	AccessToken  	string `json:"access_token,omitempty"`
	RefreshToken 	string `json:"refresh_token,omitempty"`
	TokenType 		string `json:"token_type,omitempty"`
	ExpiresInt 		int64  `json:"expires_in,omitempty"`
}

type Claims struct {
	UserId 			string `json:"user_id"`
	RoleId   		string `json:"role_id"`
	jwt.RegisteredClaims
}