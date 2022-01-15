# Software renderer from scratch

## Goals

The goal of this project was to implement a simple software raytracer from scratch in C++. The value of the project is purely educational.

Two versions are presented, yielding identical outputs. The first does not rely on any graphics API, and stores the raytraced result as a PPM image file. The second makes use of OpenGL to display the result on screen as a texture in a dedicated window. To switch from one to the other, toggle the RENDERER_TYPE flag in [utility_toolkit/src/graphics/renderer/renderer_defines.h](utility_toolkit/src/graphics/renderer/renderer_defines.h).

The project was build using Visual Studio 2019. Solution and project files are included in the repository.

## Inspiration

The series of articles by Gabriel Gambetta titled [Computer Graphics from Scratch](https://www.gabrielgambetta.com/computer-graphics-from-scratch/) were the main inspiration for this project.

## Future versions

If time allows, the project will be expanded to include a software rasterizer from scratch.

## Licenses

* Original source code is licensed under the MIT license.
* Third-party code is included in the [utility_toolkit/third_party](utility_toolkit/third_party) folder. The contained files may have their own license: see the corresponding [README](utility_toolkit/third_party/README.md) for details.