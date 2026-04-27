package service

import (
	"context"
	"database/sql"
	"fmt"
	"crypto/sha512"

	"github.com/F4nk1/cocorium/src/tracer"
	"github.com/F4nk1/cocorium/src/model"
)

func (s *service) Login(ctx context.Context, username string, password string) (*model.User, error) {
	tracer.Debugf(ctx, "Executing database authentication query for user '%s'", username)

	hashedPassword := hashPassword(password)

	query := `SELECT u.username, u.role_id, u.active
          FROM users u
          WHERE u.username = $1 AND u.password = $2 AND u.active = TRUE`

	var user model.User

	err := s.conn.Db.QueryRow(query, username, hashedPassword).Scan(
		&user.Username, &user.RoleId, &user.Active)

	if err != nil {
		if err == sql.ErrNoRows {
			tracer.Warnf(ctx, "Database authentication failed matching credentials for user '%s'", username)
			return nil, err
		}
		tracer.Errorf(ctx, "Database authentication query failed: %s", err)
		return nil, err
	}

	tracer.Debugf(ctx, "Database authentication successful for user '%s'", username)
	user.Role = &model.Role{Id: user.RoleId}
	return &user, nil
}

func (s *service) Register(ctx context.Context, user *model.User) error {
	tracer.Debugf(ctx, "Starting registration for user '%s'", user.Username)

	hashedPassword := hashPassword(user.Password)

	transaction, err := s.conn.Db.Begin()
	if err != nil {
		tracer.Errorf(ctx, "Failed to start transaction for user %s: %s", user.Username, err)
		return err
	}
	defer transaction.Rollback()

	userQuery := `INSERT INTO users (username, password, active) VALUES ($1, $2, $3)`
	
	_, err = transaction.Exec(userQuery, user.Username, hashedPassword, user.Active)

	if err != nil {
		tracer.Errorf(ctx, "Failed to insert user %s: %s", user.Username, err)
		return err
	}

	err = transaction.Commit()
	if err != nil {
		tracer.Errorf(ctx, "Failed to commit transaction for user %s: %s", user.Username, err)
		return err
	}

	tracer.Debugf(ctx, "Registration completed successfully for user '%s'", user.Username)
	return nil
}

func hashPassword(password string) string {
	return fmt.Sprintf("%x", sha512.Sum512([]byte(password)))
}