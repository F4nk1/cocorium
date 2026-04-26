package server

import (
	"errors"
	"github.com/F4nk1/cocorium/src/model"
)

func validateUser(user *model.User) error {
	if user.Username == "" || user.Password == "" || user.RoleId == "" {
		return errors.New("User Username, Password and RoleId are required")
	}
	return nil
}