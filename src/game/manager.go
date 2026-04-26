package game

import "sync"

var (
	instances = make(map[int]Game)
	mu        sync.RWMutex
)

func Create(instanceID int, gameType string) {
	var g Game

	switch gameType {
	case "clicker":
		g = &Clicker{}
	default:
		return
	}

	g.Init(instanceID)

	mu.Lock()
	instances[instanceID] = g
	mu.Unlock()
}

func Get(instanceID int) (Game, bool) {
	mu.RLock()
	defer mu.RUnlock()

	g, ok := instances[instanceID]
	return g, ok
}