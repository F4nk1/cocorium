package service

import (
	"context"
	"database/sql"

	"github.com/F4nk1/cocorium/src/model"
	"github.com/F4nk1/cocorium/src/tracer"
)

func (s *service) GetUser(ctx context.Context, id string) (*model.User, error) {
	tracer.Debugf(ctx, "Querying database for user '%s' data", id)

	query := `SELECT u.id, u.username, u.active,
              FROM users u
              JOIN roles r ON u.role_id = r.id
              WHERE u.username = $1 AND u.active = TRUE`

	var user model.User
	var role model.Role

	err := s.conn.Db.QueryRow(query, id).Scan(&user.Id, &user.Username, &user.Active, &role.Id)

	if err != nil {
		if err == sql.ErrNoRows {
			tracer.Warnf(ctx, "Database returned no results for user '%s'", id)
			return nil, err
		}
		tracer.Errorf(ctx, "Database query failed for user %s: %s", id, err)
		return nil, err
	}

	tracer.Debugf(ctx, "Successfully retrieved user '%s' data", id)
	user.Role = &role
	return &user, nil
}

func (s *service) ListUsers(ctx context.Context) ([]model.User, error) {
	tracer.Debugf(ctx, "Executing database query to fetch all active users")

	query := `SELECT u.id, u.username, u.active,
              r.id
              FROM users u
              JOIN roles r ON u.role_id = r.id
              WHERE u.active = TRUE`

	rows, err := s.conn.Db.Query(query)
	if err != nil {
		tracer.Errorf(ctx, "Database query execution failed for list members: %s", err)
		return nil, err
	}
	defer rows.Close()

	var users []model.User
	for rows.Next() {
		var user model.User
		var role model.Role

		err = rows.Scan(&user.Id, &user.Username, &user.Active, &role.Id)

		if err != nil {
			tracer.Errorf(ctx, "Database row scan failed during user listing: %s", err)
			return nil, err
		}

		user.Role = &role
		users = append(users, user)
	}

	tracer.Debugf(ctx, "Database query retrieved %d users from database", len(users))
	return users, nil
}

func (s *service) UpdateUser(ctx context.Context, user *model.User) error {
	tracer.Debugf(ctx, "Updating user '%s'", user.Id)

	query := `UPDATE users SET password = $1, role_id = $2, active = $3 WHERE id = $4`
	_, err := s.conn.Db.Exec(query, user.Password, user.RoleId, user.Active, user.Id)
	if err != nil {
		tracer.Errorf(ctx, "Database update failed for user %s: %s", user.Id, err)
		return err
	}

	tracer.Debugf(ctx, "Successfully updated user '%s'", user.Id)
	return nil
}

func (s *service) SoftDeleteUser(ctx context.Context, id int) error {
	tracer.Debugf(ctx, "Soft deleting user '%d'", id)

	query := `UPDATE users SET active = FALSE WHERE id = $1`
	_, err := s.conn.Db.Exec(query, id)
	if err != nil {
		tracer.Errorf(ctx, "Database soft delete failed for user %d: %s", id, err)
		return err
	}

	tracer.Debugf(ctx, "Successfully soft deleted user '%d'", id)
	return nil
}

func (s *service) UnDeleteUser(ctx context.Context, id int) error {
	tracer.Debugf(ctx, "Undeleting user '%d'", id)

	query := `UPDATE users SET active = TRUE WHERE id = $1`
	_, err := s.conn.Db.Exec(query, id)
	if err != nil {
		tracer.Errorf(ctx, "Database undelete failed for user %d: %s", id, err)
		return err
	}
	tracer.Debugf(ctx, "Successfully undeleted user '%d'", id)
	return nil
}