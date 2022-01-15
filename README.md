# Software renderer from scratch

## Goals

The goal of this project was to implement a simple software raytracer from scratch in C++. The value of the project is purely educational.

Two versions are presented, yielding identical outputs. The first does not rely on any graphics API, and stores the raytraced result as a PPM image file. The second makes use of OpenGL to display the result on screen as a texture in a dedicated window. To switch from one to the other, toggle the RENDERER_TYPE flag in [utility_toolkit/src/graphics/renderer/renderer_defines.h](utility_toolkit/src/graphics/renderer/renderer_defines.h).

The project was build using Visual Studio 2019. Solution and project files are included in the repository.

Here is an example of what you should see as a result of launching the raytracer project:

<p align="center">
  <img width="512" height="512" src="https://github.com/DinechinGreg/software-renderer-from-scratch/blob/main/raytracer/example_output.png?raw=true" alt="Example output"/>
</p>

## Inspiration

The series of articles by Gabriel Gambetta titled [Computer Graphics from Scratch](https://www.gabrielgambetta.com/computer-graphics-from-scratch/) were the main inspiration for this project.

## Future versions

If time allows, the project will be expanded to include a software rasterizer from scratch.

## Licenses

* Original source code is licensed under the MIT license.
* Third-party code is included in the [utility_toolkit/third_party](utility_toolkit/third_party) folder. The contained files may have their own license: see the corresponding [README](utility_toolkit/third_party/README.md) for details.
