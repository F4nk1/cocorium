package model

type RolePermission struct {
	Id		 		int    `json:"id,omitempty" db:"id"`
	RoleId   		string `json:"role_id,omitempty" db:"role_id"`
	PermissionId 	string `json:"permission_id,omitempty" db:"permission_id"`
	Active    		bool   `json:"active,omitempty" db:"active"`
}