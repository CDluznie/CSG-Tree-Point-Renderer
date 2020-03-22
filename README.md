
# CSG Tree Point Renderer

- CSG + point rendering
- using OPENGL(version ? 3?)/GLUT

#### Exemple :

![result-shapes-high](./doc/results/shapes_high.png)

![result-shapes-medium](./doc/results/shapes_medium.png)

![result-shapes-low](./doc/results/shapes_low.png)

TODO constructive solid geometry [Wikipedia page](https://en.wikipedia.org/wiki/Constructive_solid_geometry).

![result-constructive](./doc/results/constructive.png)

![result-cheese](./doc/results/cheese.png)

## Usage

* Compilation : `make`
* Run : `./csg scene density`
	* *filename* : path to the grammar file of the L-System
	* *n* : number of iterations
	* *angle* : optional, angle of rotation to apply to the figure (default value is 0)
* Delete binaries : `make mrproper`

