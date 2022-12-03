
# isometric

Simple isometric application to explore the rendering problems associated to it.

# Installation

- Clone the repo: `git clone git@github.com:Knoblauchpilze/isometric.git`.
- Clone dependencies:
    * [core_utils](https://github.com/Knoblauchpilze/core_utils)
    * [maths_utils](https://github.com/Knoblauchpilze/maths_utils)
- Go to the project's directory `cd ~/path/to/the/repo`.
- Compile: `make run`.

Don't forget to add `/usr/local/lib` to your `LD_LIBRARY_PATH` to be able to load shared libraries at runtime. This is handled automatically when using the `make run` target (which internally uses the [run.sh](data/run.sh) script).

## Eigen

This application uses [Eigen](https://en.wikipedia.org/wiki/Eigen_(C%2B%2B_library)) to handle some matrix operations: this is used for the isometric representation of the app's data.

To install it, first go [here](https://eigen.tuxfamily.org/index.php?title=Main_Page) and download the latest revision (this app was designed with the [3.4](https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz) version).

Untar it and follow the instructions in the README (usually build it and install it in a common location).

The `CMakeLists.txt` is already configured to try to find the library from the common place:
```CMake
find_package (Eigen3 3.4 REQUIRED NO_MODULE)
```

In case another version of the library has been installed, update this to the relevant version and everything should work as expected.
