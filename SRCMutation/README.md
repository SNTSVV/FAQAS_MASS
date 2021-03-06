To build the mutator, one needs to have a source of LLVM and a build of it locally.
The top of the Makefile assumes that these exist under ../llvm and ../llvm-build.
We have included a script for convenience to download the LLVM sources for version 3.8.1 and
build it.

If one wants to use an LLVM in a different location, change the paths to LLVM at the beginning
of the Makefile

## Compiling the SRC Mutator

`make`

## Using the SRC Mutator

After the compilation of the SRCMutator it can be used through the following Python wrapper `${SRCIROR}/PythonWrappers/mutationClang`. The wrapper receives two parameters `compilation` and `operators`. The compilation parameter is mandatory and specifies the original compilation command of the source under test. 

Compilation command example:

```
"cc -c -g -Wall -Werror -Wextra -Wuninitialized -Wcast-qual -Wshadow -Wundef -fdiagnostics-show-option -D_DEBUG -I . -O0 -fprofile-arcs -ftest-coverage -o ./test.o ./test.c"
```

Instead, the operators parameter is optional and specifies the mutation operators to be applied, if the parameter is not used all the operators are applied.

Operators command example: 

`ABS,UOI,LVR`

The full list of operators is the following:

`AOR,ROR,ICR,LCR,ROR,SDL,UOI,ABS,AOD,LOD,ROD,BOD,SOD,LVR`


SRCMutation mutates a source file based on the statement covered by test cases. Consequently, we tell SRCMutation which statements to mutate by loading a file named `coverage` inside the folder $HOME/.srciror/ with the line numbers to be mutated for each source.

Example of $HOME/.srciror/coverage:

```
./test.c:1,2,3,4,5,6
```

