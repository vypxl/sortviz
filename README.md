# Sortviz

> An extendable sorting algorithm visualization

[Live Browser Version](https://vypxl.github.io/sortviz/sortviz)

## Features
 * Sorting algorithms (so far):
     - Selectionsort
     - Bubblesort
     - Quicksort

 * Visual Styles (so far):
     - Variety Loop
     - Variety Dots

## Building

This project is built using cmake and ninja, with make as an easy to use interface to them.

### How to build (Linux)

You will need `cmake`, `make`, `ninja`, `conan`, and `gcc/g++` installed.

To manage dependencies, this project uses [Conan](https://conan.io).
Before building, you may want to run `make deps` to pull the dependencies via conan.

It is also possible to build this project without conan, if you install all dependencies system-wide.

Build: `make build` (you will find the executable in `out/sortviz`)
Run: `make run`

### How to build (Web / Emscripten)

You will need `cmake`, `make`, `ninja`, `emscripten/emcc` and `glm` installed.

You might have to copy your headers for `glm` into `./include`. To find the system-wide `glm` headers, you can use `$ find / -type f -name glm.hpp 2> /dev/null`. Then do `$ cp -r <dir>/glm ./include/`

Build: `make embuild` will generate a html page with other files in `out/web/`.
Open in Browser: `make emrun`.

### How to build (Docker)

If you happen to have docker (and make) installed, you can build both the native and web version via `make dockerbuild`. This builds both versions and puts them into `out/`, without the need to install any libraries!

## How to use

The initial configuration is Selection Sort with Variety Loop and constant delay.

To change the parameters, use 
 - Left/Right to cycle through sorting algorithms
 - Up/Down to cycle through visual styles
 - Q/E to decrease/increase data size

Other keys:
 - Space: Shuffle
 - P: Pause
 - Escape: Quit program

### On the delays

Smaller data sizes a constant per-cycle delay is added, which roughly add up to 10 seconds per full sort.
If the number of required cycles exceeds the data size, no delay is added.
With added delay every cycle takes constant time so no exponential speeedup can be observed.
This is not the case without delay.

## How to extend and contribute

You are welcome to submit extensions like new sorting algorithms or display styles.

#### To add a new sorting algorithm

1. Define your sort as a `Sort` subclass in `include/sorts.hpp`
2. Register your sort in `sorts.hpp` > `Sorts::Sorts` and `Sorts::names`, and in `src/sorts.cpp` > `Sorts::create` (add a `CASE(<name>)`)
3. Implement `step` and `_reset` in your `src/sorts/<sortname>.cpp`

You should now be able to use the new sorting algorithm in the program!

#### To add a new visual style

1. Register your style in `src/viz.cpp`
    - register `enum Viz::Styles`
    - define it in `Viz::styles` (`Style{ "<vertexShaderName>, <fragmentShaderName>, <method for glDrawArrays>" }`)
    - set display name in `Viz::styleNames`
    - Do not forget to update the array sizes for `styles` and `styleNames`!
2. If you do not want to reuse shaders, create new shaders in `shaders/`

You should now be able to use the new style in the program!
