package image_stuff

import (
	"image/color"
	"fmt"
)


type Colormap struct {
	data []color.RGBA
}

func (c *Colormap) Get(x float64) color.RGBA {
	switch {
	case x <= 0:
		return c.data[0]
	case x >= 1:
		return c.data[len(c.data)-1]
	default:
		idx := int(x * float64(len(c.data)))
		return c.data[idx]
	}
}

func ColormapFromFunc(f func(float64 float64) color.RGBA, min, max float64, steps int) (out Colormap) {
	out.data = make([]color.RGBA, steps)

	stepSize := (max - min) / float64(steps)
	for i := 0; i < steps; i++ {
		out.data[i] = f(float64(i)*stepSize - min)
		fmt.Println(i, float64(i)*stepSize - min, f(float64(i)*stepSize - min))
	}
	return
}

