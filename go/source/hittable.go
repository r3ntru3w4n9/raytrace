package source

// Hittable represents something you can hit
type Hittable interface {
	Hit(source, towards Vector) (data HitData)
	Bounds() Box
}

// HitData stores data in one hit
type HitData struct {
	t             float64
	point, normal Vector
	matter        Material
}

// MakeHitData returns a new HitData
func MakeHitData(t float64, point, normal Vector, matter Material) HitData {
	return HitData{t, point, normal, matter}
}

// NewHitData returns a new HitData
func NewHitData(t float64, point, normal Vector, matter Material) *HitData {
	hd := MakeHitData(t, point, normal, matter)
	return &hd
}

// MakeHit creates a new data indicating that the object is hit
func MakeHit(t float64, point, normal Vector, matter Material) HitData {
	return MakeHitData(t, point, normal, matter)
}

// NewHit creates a new data indicating that the object is hit
func NewHit(t float64, point, normal Vector, matter Material) *HitData {
	hit := MakeHit(t, point, normal, matter)
	return &hit
}

// MakeMiss creates a new HitData indicating that the object has been missed
func MakeMiss() HitData {
	return HitData{t: PosInf}
}

// NewMiss creates a new HitData indicating that the object has been missed
func NewMiss() *HitData {
	miss := MakeMiss()
	return &miss
}

// T returns the distance at which the object is hit
func (hd HitData) T() float64 { return hd.t }

// Point shows where the object is hit
func (hd HitData) Point() Vector { return hd.point }

// Normal points out of the surface at the point the surface is hit
func (hd HitData) Normal() Vector { return hd.normal }

// Matter shows what kind of surface is hit
func (hd HitData) Matter() Material { return hd.matter }

// HasHit indicates that something is hit
func (hd HitData) HasHit() bool {
	return hd.t != PosInf
}
