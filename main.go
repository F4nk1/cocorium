package main

import (
	"context"
	"fmt"
	"net/http"

	"github.com/F4nk1/cocorium/src/config"
	"github.com/F4nk1/cocorium/src/connection"
	"github.com/F4nk1/cocorium/src/server"
	"github.com/F4nk1/cocorium/src/service"
	"github.com/F4nk1/cocorium/src/tracer"
)

func main() {
	ctx := context.Background()
	cfg, err := config.NewConfiguration()
	if err != nil {
		fmt.Printf("failed to load configuration: %v\n", err)
		return
	}
	tracer.Infof(ctx, "Starting in %s mode, database host %s:%s", cfg.Mode, cfg.Database.Host, cfg.Database.Port)

	conn, err := connection.NewConnection(ctx, cfg)
	if err != nil {
		tracer.Fatal(ctx, err)
	}

	defer conn.Close()

	service := service.NewService(conn)
	srv := server.NewServer(service)

	serverHost := fmt.Sprintf("%s:%s", cfg.Server.Host, cfg.Server.Port)
	tracer.Infof(ctx, "Server is running on %s", serverHost)
	tracer.Fatal(ctx, http.ListenAndServe(serverHost, srv.Handler))
	
} 