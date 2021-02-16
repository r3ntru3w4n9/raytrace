package main

import (
	"math"
	"math/rand"
	src "raytrace/source"
	"time"
)

// PROGRESS shows the iteration after which current progress will be logged
const PROGRESS = 1000

// NX is the width of the image
const NX = 1200

// NY is the height of the image
const NY = 675

// NS is the number of samples per pixel
const NS = 100

// DEP is the depth of our recursive path finder
const DEP = 10

// MAXU8 is the maximum number a byte can hold
const MAXU8 = 256

// DEG is the width of the viewport in terms of degrees
const DEG = 30.

// RATIO is the height width ratio
const RATIO = NX / NY

// APERTURE is the radius of the lens
const APERTURE = 0.

// TREE is whether or not to use tree structure
const TREE = true

func randomMaterial(materialCode, blur, refractive float64, albedo src.Vector) src.Material {
	mat := uint(materialCode * 3)
	blur /= 2
	refractive++
	albedo.IAddS(1.)
	albedo.IDivS(2.)
	switch mat {
	case 0:
		return src.MakeMatte(albedo)
	case 1:
		return src.MakeMetal(albedo, blur)
	case 2:
		return src.MakeGlass(albedo, blur, refractive)
	default:
		panic("unreachable")
	}
}

// Scenes are set up here
func Scenes() src.Scene {
	eye := src.MakeVector(13., 2., 3.)
	lookat := src.VectorO()
	viewup := src.VectorJ()

	vision := lookat.Sub(eye)

	rad := math.Pi * DEG / 360.
	height := math.Tan(rad) * vision.Length()
	width := height * RATIO

	unit := vision.Unit()
	proj := unit.MulS(viewup.Dot(unit))
	viewup = (viewup.Sub(proj)).Unit()
	horizon := vision.Cross(viewup).Unit()

	viewup.IMulS(height)
	horizon.IMulS(width)

	list := src.MakeList()

	gen := rand.New(rand.NewSource(time.Now().Unix()))
	for i := -11; i < 11; i++ {
		for j := -11; j < 11; j++ {
			iF := float64(i)
			jF := float64(j)
			center := src.MakeVector(
				iF+.9*gen.Float64(), .2, jF+.9*gen.Float64(),
			)

			list.Register(
				src.MakeSphere(
					center, .2, randomMaterial(
						gen.Float64(), gen.Float64(), gen.Float64(), src.VectorRandom(gen))))
		}
	}

	list.Register(src.MakeSphere(
		src.MakeVector(0., -1000., 0.), 1000., src.MakeMatte(src.VectorUniform(.9)),
	))

	list.Register(src.MakeSphere(
		src.VectorJ(), 1., src.MakeGlass(
			src.VectorUniform(1.), 0., 1.5,
		)),
	)

	list.Register(src.MakeSphere(
		src.MakeVector(-4., 1., 0.), 1., src.MakeMatte(src.MakeVector(.4, .2, .1)),
	))

	list.Register(src.MakeSphere(
		src.MakeVector(4., 1., 0.), 1.,
		src.MakeMetal(src.MakeVector(.7, .6, .5), 0.),
	))

	scene := src.MakeScene(
		eye,
		lookat.Sub(viewup).Sub(horizon),
		horizon.MulS(2.),
		viewup.MulS(2.),
		APERTURE,
	)

	var hittable src.Hittable

	if TREE {
		hittable = src.MakeTree(list)
	} else {
		hittable = list
	}

	scene.Save(hittable)

	return scene
}
