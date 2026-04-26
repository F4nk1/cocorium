package service

import (
	"context"

	"github.com/F4nk1/cocorium/src/tracer"
)

func (s *service) HasPermission(ctx context.Context, id string, permission string) (bool, error) {
	tracer.Debugf(ctx, "Checking permission '%s' for user '%s'", permission, id)

	query := `
		SELECT COUNT(*)
		FROM permissions p
		JOIN role_permissions rp ON p.id = rp.permission_id
		JOIN users u ON u.role_id = rp.role_id
		WHERE u.id = ? AND p.id = ? AND p.active = true AND rp.active = true AND u.active = true`

	var count int
	err := s.conn.Db.QueryRow(query, id, permission).Scan(&count)
	if err != nil {
		tracer.Errorf(ctx, "Permission check failed for user '%s': %s", id, err)
		return false, err
	}

	tracer.Debugf(ctx, "Permission '%s' for user '%s': %t", permission, id, count > 0)
	return count > 0, nil
}