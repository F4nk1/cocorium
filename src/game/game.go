package game

type Game interface {
	Init(instanceID int)
	HandleAction(userID int, action string) error
	GetState() interface{}
}