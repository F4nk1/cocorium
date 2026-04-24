package config

type Database struct {
	Driver  	string 	`json:"driver"`
	Name		string 	`json:"name"`
	User		string 	`json:"user"`
	Password 	string 	`json:"password"`
	Protocol 	string 	`json:"protocol"`
	Host		string 	`json:"host"`
	Port		string 	`json:"port"`
}