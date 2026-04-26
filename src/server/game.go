package server

import (
	"encoding/json"
	"net/http"
	"github.com/F4nk1/cocorium/src/service"
)

func CreateGameHandler(w http.ResponseWriter, r *http.Request) {
	var body struct {
		Type string `json:"type"`
	}

	json.NewDecoder(r.Body).Decode(&body)

	id := service.CreateGame(body.Type)

	json.NewEncoder(w).Encode(map[string]int64{
		"instance_id": id,
	})
}

func ActionHandler(w http.ResponseWriter, r *http.Request) {
	var body struct {
		InstanceID int64  `json:"instance_id"`
		UserID     int    `json:"user_id"`
		Action     string `json:"action"`
	}

	json.NewDecoder(r.Body).Decode(&body)

	err := service.Action(body.InstanceID, body.UserID, body.Action)
	if err != nil {
		http.Error(w, err.Error(), 400)
		return
	}

	json.NewEncoder(w).Encode(map[string]string{
		"status": "ok",
	})
}

func StateHandler(w http.ResponseWriter, r *http.Request) {
	var body struct {
		InstanceID int64 `json:"instance_id"`
	}

	json.NewDecoder(r.Body).Decode(&body)

	state, err := service.GetState(body.InstanceID)
	if err != nil {
		http.Error(w, err.Error(), 400)
		return
	}

	json.NewEncoder(w).Encode(state)
}