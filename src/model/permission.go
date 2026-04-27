package model

type Permission struct {
	Id          string `json:"id,omitempty" db:"id"`
	Description string `json:"description,omitempty" db:"description"`
	Active      bool   `json:"active,omitempty" db:"active"`
}