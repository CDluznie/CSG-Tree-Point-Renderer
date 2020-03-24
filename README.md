
# CSG Tree Point Renderer

3D point rendering system of constructive solid geometry (CSG) scenes based on OpenGL 3/GLUT.

## Presentation

The goal of the project is to realize a point cloud render to a scene obtained by using the [solid construction geometry](https://en.wikipedia.org/wiki/Constructive_solid_geometry) operators technique.
A scene is obtained by composing canonical shapes (spheres, cubes, cylinders, cones and torus) with the CSG operators (union, intersection and difference) represented as a tree.


![result-cheese](./doc/results/cheese.png)


The implemented strategies allow to obtain the best rendering (a homogeneous distribution of points and an equivalent point density between each form) but at the expense of a longer execution time.
Then, the level of resolution is configurable in order to be able to display scenes on every architectures.

## Point Cloud

A point cloud is point array, where each point have a position, a normal and a color.
The normals are used for the lighting computation.
A point cloud does can do nothing other than to model itself in 3D space.

## Canonical shapes

A canonical shape is a description of the basic geometric object we want to model.
The description includes the color of the shape, the scaling factors and some real arguments if required (like the internal radius for the torus).
Because a canonical shape is just an abstract representation of an object it is necessary to being able to convert it to a point cloud.
The difficulty of canonical shapes lies on their good conversion to their point cloud representations : keep a a homogeneous distribution of points across the surface of each shape and a constancy in the point density between the different shapes.

### Homogeneous distribution

For a smooth rendering, it is necessary to ensure that in figure, each part will be equally covered with points.
To achieved this, we distribute randomly the points over the shape with a particular probability distribution law for each surface of each shape.
The probability distribution law that is choosen when it fit the best as possible a surface.
For example we choose an uniform distribution for a cube face or a gaussian distribution for a sphere.

### Point density

The point density is defined as the number of point per area unit.
So the total number of point required to model a canonical shape is the product of the point density (selected by the user) by the area of the shape.
Hoping the point are evenly distribute over the surfaces, the problem is to compute the area of each canonical shape.

We can use the know formulas to compute the area of the canonical shapes but it work correcty only if no scaling operation is performed on the object.
To tackle this issue, we evaluate the new area of a canonical shape when a homothety is performed.
For some shape the calculation is still trivial (as in the cube), but in the in most cases the calculation is extremely complicated, for example the sphere which becomes an ellipsoid, and we can not retrieve the exact value of the area.
So we try to use in these cases approximation and just hoping the approximation is good enough to guarantee the consistency of density between canonical shape.

Here an example when the resolution is low.

![result-shapes-low](./doc/results/shapes_low.png)

Here an example when the resolution is low.

![result-shapes-medium](./doc/results/shapes_medium.png)

Here an example when the resolution is low.

![result-shapes-high](./doc/results/shapes_high.png)

## CSG Tree

TODO

## Example :

TODO constructive solid geometry [Wikipedia page](https://en.wikipedia.org/wiki/Constructive_solid_geometry).

![result-constructive](./doc/results/constructive.png)

## Usage

* Compilation : `make`

* Run : `./csg scene density`
	* *scene* : path to the file scene to display
	* *density* : resolution of the scene to display, can take the value `low`, `medium` and `high`

Some scenes examples are available in directory **scenes/**

* Delete binaries : `make mrproper`

