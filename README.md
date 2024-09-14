# Roguelike

This is our capstone project.

## Build Instructions

There are two options for building this project:

- Zig
- CMake

Try them in order if you can.

### Building with Zig

You should have the zig toolchain installed. If you dont, install it.

MacOS: `brew install zig`
Windows: `winget install -e --id zig.zig`

Check your zig version. It should be `0.13.0`.

Now, in this directory, you can run `zig build` to build the project and dump the headers in `/zig-out/include`. To run the application manually, run the file `/zig-out/bin/roguelike`.

To build and run in the same step, type `zig build run` in your terminal.
