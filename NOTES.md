

# Simulation
* [x] Green cell: urban cell
* [x] Black cell: non urban cell
* [x] Basic visualization (black and white)
* [x] Load image and config from disk
* [ ] Pretty visualization

# Rule
### Diffusion
Run only once to create initial state (can be change easily)
* [x] Generate a random grid with x% of urban cells
* [ ] Use user-defined initial state (this should be straight forward)

### Breed
```
for each urban cell
    for each cell in its neighbours
        turn current cell to urban? with probabilty p[breed]
```
Parallel implementation
```
for each cell
    n = number of neighbour urban cell (use convolution with kernel [1 1 1; 1 0 1; 1 1 1])
    turn current cell to urban? with probabilty 1 - (1 - p[breed])^n
```

### Spread
```
for each connected component (blob)
    if size of component > threshold
        for each neighbour
            current cell have p[spread] to turn urban
```
Parallel implementation
```
find all regions in image
for each region
    assign cell value = region's size
run max filtering with kernel [0 1 0; 1 0 1; 0 1 0]
use threshold to decide if we should spread
for each cell (use above threshold to mask the result)
    turn cell to urban with p[spread]
```

### Road
Currently use a different matrix for road. 
Just do matrix_city + matrix_road to get all current state.
### Dynamic rules
TODO

# Installation
## Dependencies
### Arch Linux
Run `sudo pacman -Sy cuda cmake gcc arrayfire` to install everything

### Ubuntu/Debian based
`sudo apt-get install libarrayfire-unified-dev` should work
### Other OSes
* C++ compiler like Clang or GCC
* CMake to build the project
* [ArrayFire](http://arrayfire.org/docs/installing.htm) 
* Cuda/OpenCL is needed if you want to run the simulation on GPU

### Win64 Binary
* Install [ArrayFire](https://arrayfire.com/download/) and allow the installer to modify your computer's environment variables
## Building
* Grab the binary

Load it in CLion or  other IDE or build it manually with
```bash
mkdir build 
cd build && cmake ..
make
```

# Performance
Current implementation works on GPU (fast).