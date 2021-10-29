# FAQAS srciror
This is an extension for SRCIROR (pronounced sorcerer) developed for the FAQAS project.
SRCIROR is a SRC and IR mutatOR. It performs mutations on source code written in C/C++ and on the LLVM IR.
The original version of SRCIROR is available from https://github.com/TestingResearchIllinois/srciror

BUILDING:

  0. Need dependencies:
        python
        python3
        clang
        lib32z1-dev

  1. Build LLVM locally (if you do not already have it built). You can execute the provided script:  
        `./llvm-build.sh`  
    This creates llvm/ and llvm-build/ directories.

  2. Build the SRC mutator. To build SRC mutator, see SRCMutation/README.

  5. Set up the environment variables needed by SRCIROR.
  
        `export SRCIROR_LLVMMutate_LIB=${PATH_TO_LLVM_BUILD}/Release+Asserts/lib/LLVMMutate.so`

        `export SRCIROR_SRC_MUTATOR=${PATH_TO_SRCIROR_DIR}/SRCMutation/build/mutator`

        `export SRCIROR_LLVM_BIN=${PATH_TO_LLVM_BUILD}/Release+Asserts/bin/`

        `export SRCIROR_LLVM_INCLUDES=${PATH_TO_LLVM_BUILD}/Release+Asserts/lib/clang/3.8.0/include/`

  Either set environment variables `PATH_TO_LLVM_BUILD` and `PATH_TO_SRCIROR_DIR`,
  or replace them with the corresponding values in the statements above.

