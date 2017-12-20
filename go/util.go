package golang_raytracer

import (
	"math"
	"log"
	"path/filepath"
	"os"
	"fmt"
)

//type Float = float32
type Float = float64

func Sqrt(f Float) Float { return Float(math.Sqrt(float64(f))) }
func Sin(f Float) Float  { return Float(math.Sin(float64(f))) }
func Cos(f Float) Float  { return Float(math.Cos(float64(f))) }

type Ray struct {
	Origin    Vec3
	Direction Vec3
}

func (r *Ray) PointAt(t Float) Vec3 {
	v := r.Direction.MulS(t)
	return v.AddV(r.Origin)
}

func Die(err error) {
	if err != nil {
		log.Fatal(err)
	}
}

func ExecutableName() string { return filepath.Base(os.Args[0]) }
func ExecutableNamePng() string {
	return fmt.Sprintf("%s.png", ExecutableName())
}