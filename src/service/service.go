package service

import (
	"context"
	"github.com/F4nk1/cocorium/src/connection"
	"github.com/F4nk1/cocorium/src/model"
)

type Service interface {
	Login(ctx context.Context, username, password string) (*model.User, error)
	Register(ctx context.Context, user *model.User) error

	GetUser(ctx context.Context, id string) (*model.User, error)
	ListUsers(ctx context.Context) ([]model.User, error)
	UpdateUser(ctx context.Context, user *model.User) error
	SoftDeleteUser(ctx context.Context, id int) error
	UnDeleteUser(ctx context.Context, id int) error

	HasPermission(ctx context.Context, id string, permission string) (bool, error)
}

type service struct {
	conn *connection.Connection
}

func NewService(conn *connection.Connection) Service {
	return &service{
		conn: conn,
	}
}
