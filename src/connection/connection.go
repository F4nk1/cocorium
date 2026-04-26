package connection

import (
	"context"
	"database/sql"
	_ "github.com/lib/pq"
	"github.com/F4nk1/cocorium/src/tracer"
	"github.com/F4nk1/cocorium/src/config"
)

type Connection struct {
	Db *sql.DB
}

func NewConnection(ctx context.Context,config *config.Config) (*Connection, error)	{
	tracer.Debug(ctx,"Initializing database connection")

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
	return c.Db.Close()
}