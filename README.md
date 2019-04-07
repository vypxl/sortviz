# Sortviz

> An extendable sorting algorithm visualization

## Features
 * Sorting algorithms (so far):
     - Selectionsort
     - Bubblesort
     - Quicksort

 * Visual Styles (so far):
     - Variety Loop
     - Variety Dots

 * Constant and per-operation delay

## How to build (Linux)
You will need `make`, `gcc/g++`, `sfml` and `opengl` installed.

Build: `make build` (you will find the executable in out/sortviz)
Run: `make run`
Delete build files: `make clean`

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
