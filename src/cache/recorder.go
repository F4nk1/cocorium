package cache

import "net/http"

type Recorder struct {
	http.ResponseWriter
	Body []byte
}

func NewRecorder(w http.ResponseWriter) *Recorder {
	return &Recorder{ResponseWriter: w}
}

func (r *Recorder) Write(data []byte) (int, error) {
	r.Body = append(r.Body, data...)
	return r.ResponseWriter.Write(data)
}