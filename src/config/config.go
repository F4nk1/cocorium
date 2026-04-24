package config

import (
	"fmt"
	"os"
)

const (
	ModeDev = "dev"
	ModeProd = "prod"
)

type Config struct {
	Mode string 		`json:"mode"`
	Server Server		`json:"server"`
	Database Database	`json:"database"`
}

func NewConfiguration() (*Config, error) {
	port := os.Getenv("PORT")
	if port == "" {
		port = "8080"
	}

	dbHost := os.Getenv("DB_HOST")
	dbUser := os.Getenv("DB_USER")
	dbName := os.Getenv("DB_NAME")

	if dbHost == "" || dbUser == "" || dbName == "" {
		return nil, fmt.Errorf("missing required database environment variables")
	}

	return &Config{
		Mode: os.Getenv("MODE"),
		Server: Server{
			Port: port,
		},
		Database: Database{
			Driver:   "postgres",
			Name:     dbName,
			User:     dbUser,
			Password: os.Getenv("DB_PASSWORD"),
			Host:     dbHost,
			Port:     os.Getenv("DB_PORT"),
		},
	}, nil
}

func (c *Config) GetConnectionString() string {
	return fmt.Sprintf("%s:%s@tcp(%s:%s)/%s",
		c.Database.User,
		c.Database.Password,
		c.Database.Host,
		c.Database.Port,
		c.Database.Name,
	)
}