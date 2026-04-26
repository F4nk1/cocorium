package model

type User struct {
	Id       int    `json:"id" db:"id"`
	Username string `json:"username" db:"username"`
	Password string `json:"password" db:"password"`
	RoleId   string	`json:"role_id" db:"role_id"`
	Role	 *Role 	`json:"role,omitempty"`
	Active   bool   `json:"active" db:"active"`
}