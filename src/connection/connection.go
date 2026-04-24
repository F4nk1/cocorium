package connection

import (
	"context"
	"database/sql"
	"fmt"
)

type Connection struct {
	Db *sql
}

func NewConnection(ctx context.Context,config *config.Config) (*Connection, error)	{
	tracer.Debug("Initializing database connection")

	ConnectionString := config.GetConnectionString()

	db, err := sql.Open(config.Database.Driver, ConnectionString)
	if err != nil {
		tracer.Errorf(ctx,"failed to open database connection: %v", err)
		return nil,err
	}

	if err := db.PingContext(ctx); err != nil {
		tracer.Errorf(ctx,"failed to ping database: %v", err)
		return nil,err
	}

	connection := &Connection{
		Db: db,
	}

	return connection, nil
}

func (c *Connection) Close() error {
	return &c.Db.Close()
}