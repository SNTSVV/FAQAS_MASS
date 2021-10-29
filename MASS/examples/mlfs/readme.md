# Mathematical Library for Flight Software (MLFS)                     {#mainpage}

## Introduction

The goal of this software project is to develop a mathematical library reusing
the libm present in Newlib and pre-qualifying it as a category B software as
per ECSS standards.

The primary target hardware for this software are the SPARC V8 (e.g. LEON2,
LEON3, and LEON4 processors) and x86-64 processor architectures. Hardware
related restrictions for both the SPARC V8 and x86-64 processor architectures
are detailed in the Software User Manual (E1356-GTD-SUM-01).

## Compiling the library

The first step to include the mathematical library into another software project
is to build the static library from the source code. For this procedure, a
makefile is prepared.

The makefile provides the following targets:

* `all`: Compiles and builds the library
* `clean`: Removes the object files and the static library of the chosen build
* `cleanall`: Removes the object files and static libraries from all builds
* `SQRT=hw`: Tells the makefile to use the architecture specific hardware square root
functions instead of the software implementation. Currently this is only supported for
ARCH=sparc_v8, to add support for other architectures, place the respective files in the
directory mlfs/libm/machine/ARCH.
* `COVERAGE=true`: Tells the makefile to use flags specific to the generation of coverage
reports.

The targets `all` and `clean` can be modified using the following
constants:

* `CROSS_COMPILE`: Prefix to tell the makefile which compiler to use
* `ARCH`: Tells the makefile for which architecture the build is intended

The resulting libraries can be found in the `build-ARCH/bin` directory, with
`ARCH` being the provided constant, or if not provided defaults to the target
defined by GCC (this can be inspected with the shell command `gcc -v`). The
immediate object files can be found in the `build-ARCH/obj` directory.

Another noticeable aspect of compiling the library is that the library contains an
assert in the modf files. The library enforces the standard behaviour with regards to
assert. Therefore when using a toolchain that uses said behaviour, the assert can be
disabled by setting the NDEBUG flag. This can be done by adding `EXTRA_CFLAGS="-DNDEBUG"`
to the command line call of make.

## Linking with the library

Compiling and linking a test application consisting of a single C file can be
done with the following GCC commandline, for example with an RTEMS toolchain.

```
sparc-rtems-gcc -o test test_rtems.c mlfs/build-leon4/bin/libmlfs.a -Imlfs/include -frounding-math -fsignaling-nans
```

## Functions provided by the library

The library provides the functions listed in the following table for the user to call.

| Function | Description                                                                                   |
|----------|-----------------------------------------------------------------------------------------------|
| sin      | Procedure returning the trigonometric sine                                                    |
| asin     | Procedure returning the trigonometric arcsine                                                 |
| cos      | Procedure returning the trigonometric cosine                                                  |
| acos     | Procedure returning the trigonometric arcosine                                                |
| tan      | Procedure returning the trigonometric tangent                                                 |
| atan     | Procedure returning the trigonometric arctangent                                              |
| sqrt     | Procedure returning the square root                                                           |
| atan2    | Procedure returning the trigonometric arctangent of (y/x)                                     |
| exp      | Procedure returning the base e exponential of x                                               |
| pow      | Procedure returning x raised to the power of y                                                |
| log      | Procedure returning the natural logarithm                                                     |
| log10    | Procedure returning the base 10 logarithm                                                     |
| fabs     | Procedure returning the absolute value                                                        |
| round    | Procedure for rounding to the nearest integer (Halfway values rounded away from 0)            |
| floor    | Procedure for rounding downwards to the nearest integer                                       |
| trunc    | Procedure for rounding towards 0 to the nearest integer                                       |
| ceil     | Procedure for rounding upwards to the nearest integer                                         |
| fmod     | Procedure returning the floating-point remainder of (x/y)                                     |
| modf     | Procedure breaking x in its integral and fractional part                                      |
| fmin     | Procedure returning the smaller of two values                                                 |
| fmax     | Procedure returning the larger of two values                                                  |
| hypot    | Procedure returning the square root of (x^2+y^2)                                              |
| isfinite | Procedure returning whether the value of the argument is finite or not (not ±Inf and not NaN) |
| isinf    | Procedure returning whether the value of the argument is ±Inf or not                          |
| isnan    | Procedure returning whether the argument is NaN floating-point value or not                   |
| signbit  | Procedure returning whether the argument is negative or not                                   |
| copysign | Procedure returning a floating-point number with the magnitude of x and the sign of y         |

Additionally the helper functions listed in the following table are available.

| Function | Description                                                                                   |
|----------|-----------------------------------------------------------------------------------------------|
| deg2rad  | Procedure returning radians value of an angle provided in degrees                             |
| rem2pi   | Procedure returning argument reduced to an equal angle in the range [0, 2 * Pi)               |

Each of the mathematical functions has a variant to calculate with float values
instead of double. The float variants can be called by appending `f` to the
function name.

## Further Information

For a detailed description of each function's arguments, return value and
behaviour, as well as a detailed example how to use the library, please consult
the Software User Manual (E1356-GTD-SUM-01).

Copyright European Space Agency, 2016-18
