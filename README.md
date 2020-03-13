# SPH-Fluid-Simualtion

This project implements the basic SPH fluid simulation in C++ and uses OpenGL to render the scene.

## Build and Run the Code

### In MacOS:

  Clone the repo and open 'SPH-Fluid-Simulation.xcodeproj' in Xcode. 
  
  Then under 'Product', click 'Run'.
  
## User Control

'a': move the fluid container left

'd': move the fluid container right

'r': reset the scene

## Algorithms and Techniques

The basic SPH solver simulates fluid dynamics by approximating other quantities of each particle (such as fluid pressure) from some basic known quantity.

To compute an unknown quantity of a particle, SPH uses a kernel function to take weighted sums from some known quantity of each particle neighbors within a defined support radius.

### Accerlating Data Structure

Spatial hash table was implemented to accerlate neighbors searching in constant runtime.

## Ongoing Things

Things to be added in the future:

1. Add surface tension force in simulation
2. Provide different initial setups options
2. Use new OpenGL pipeline
3. Add marching cube surface extraction

## Reference

“SPH Fluids in Computer Graphics”, Ihmsen, Orthmann, Solenthaler, Kolb, Teschner
