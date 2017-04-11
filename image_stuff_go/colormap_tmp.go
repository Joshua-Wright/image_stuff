package main

import (
	"image/color"
	"fmt"
)
import "./image_stuff"

func gray(x float64) (out color.RGBA) {
	out.R = uint8(x*256.0)
	out.G = uint8(x*256.0)
	out.B = uint8(x*256.0)
	out.A = 0
	return
}

func main() {
	//fmt.Println(gray(0))
	//fmt.Println(gray(265))
	c := image_stuff.ColormapFromFunc(gray, 0,1,2048)
	fmt.Println(c.Get(0))
	fmt.Println(c.Get(1))
	fmt.Println(c.Get(0.5))
}