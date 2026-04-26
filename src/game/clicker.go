package game

type Clicker struct {
	scores map[int]int
}

func (c *Clicker) Init(instanceID int) {
	c.scores = make(map[int]int)
}

func (c *Clicker) HandleAction(userID int, action string) error {
	if action == "click" {
		c.scores[userID]++
	}
	return nil
}

func (c *Clicker) GetState() interface{} {
	return c.scores
}