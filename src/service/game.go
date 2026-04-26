package service

import (
	"errors"
	"github.com/F4nk1/cocorium/src/game"
	"sync/atomic"
)

var instanceCounter int64

func CreateGame(gameType string) int64 {
	id := atomic.AddInt64(&instanceCounter, 1)
	game.Create(int(id), gameType)
	return id
}

func Action(instanceID int64, userID int, action string) error {
	g, ok := game.Get(int(instanceID))
	if !ok {
		return errors.New("instance not found")
	}

	return g.HandleAction(userID, action)
}

func GetState(instanceID int64) (interface{}, error) {
	g, ok := game.Get(int(instanceID))
	if !ok {
		return nil, errors.New("instance not found")
	}

	return g.GetState(), nil
}