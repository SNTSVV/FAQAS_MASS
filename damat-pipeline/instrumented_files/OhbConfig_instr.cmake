# Copyright (C) OHB-System AG
#
# All Rights Reserved. No part of this software may
# be published, distributed, translated or otherwise
# reproduced by any means or for any purpose without
# the prior written consent of OHB-System AG.
#
# Proprietary and confidential information.

# CMake include guard
get_property(already_included GLOBAL PROPERTY "included_${CMAKE_CURRENT_LIST_FILE}")
if(already_included)
  return()
endif()
set_property(GLOBAL PROPERTY "included_${CMAKE_CURRENT_LIST_FILE}" TRUE)

get_filename_component(BASE_DIR "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

#Don't use -isystem when including /opt/Sim-Platform files in a simulator. When using -isystem,
#gcc does not emit dependency informations, thus the simulator is not rebuild when dependend headers
#of the platform change
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)

#Creates compile_commands.json in the build directory, which is needed by clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread -Wall -Wextra -Werror -Wnon-virtual-dtor -Woverloaded-virtual")
#To make debugging easier; code with those warnings still should not be checked in
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-parameter -Wno-error=unused-variable")

#Warn when doing '#if SOME_FLAG' and SOME_FLAG is not defined
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wundef")

#If marking a function deprecated would result in compile errors, then 
#one could as well remove the function.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=deprecated-declarations")

# corei7 is the lowest common denominator across our developers and users, see
# https://jira.ohb-system.de/browse/SIM-555
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=corei7")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=corei7")

#FAQAS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMUTATIONOPT=$ENV{MUTATIONOPT}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FAQAS_SINGLETON_FM=$ENV{_FAQAS_SINGLETON_FM}")

#This is caused by using value-initialization with struct without providing a value for each struct member seperatly,
#e.g. in
#  struct Data { int a; int b; };
#   Data data{}; // data.a and data.b will be initized to zero. Still, gcc warns about this
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-missing-field-initializers")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # In our tests, we sometimes have
    #   #define private public
    # to test private members.
    # Suppress the clang warning about this.
    set(CMAKE_CXX_FLAGS_CLANG "-Wno-keyword-macro")

    # Silence " error: must specify at least one argument for '...' parameter of variadic macro"
    # for googletest's INSTANTIATE_TEST_CASE_P macro.
    set(CMAKE_CXX_FLAGS_CLANG "${CMAKE_CXX_FLAGS_CLANG} -Wno-gnu-zero-variadic-macro-arguments")

    # warning: 'Function' overrides a member function but is not marked 'override'
    set(CMAKE_CXX_FLAGS_CLANG "${CMAKE_CXX_FLAGS_CLANG} -Wno-inconsistent-missing-override")

    # This is not a hard error (useful for debugging) just like unused parameters
    set(CMAKE_CXX_FLAGS_CLANG "${CMAKE_CXX_FLAGS_CLANG} -Wno-error=unused-lambda-capture")

    #ASAN_OPTIONS=check_initialization_order=1 ASAN_OPTIONS=detect_stack_use_after_return=1

    set(CMAKE_CXX_FLAGS_CLANGRELEASE "${CMAKE_CXX_FLAGS_CLANG} -ggdb3 -O3")
    set(CMAKE_C_FLAGS_CLANGRELEASE "-ggdb3 -O3")
    set(CMAKE_EXE_LINKER_FLAGS_CLANGRELEASE "-O3")
    set(CMAKE_SHARED_LINKER_FLAGS_CLANGRELEASE "${CMAKE_EXE_LINKER_FLAGS_CLANGRELEASE}")
   # Sanitizers:
   # -fsanitize=address detects out-of-bounds access, use-after-free, double-free, etc 
   # -fsanitize=leak checks for memory leaks (can be combined with asan, use clang++ for linking)
   # -fsanitize=memory detects accesss to uninitialized memory (not allowed together with asan) (https://clang.llvm.org/docs/MemorySanitizer.html)
   # -fsanitize=thread checks for race conditions (not allowed together with msan or asan, add -O1 or higher for reasonable performance) (https://clang.llvm.org/docs/ThreadSanitizer.html)
   # -fsanitize=undefined (make sure to use clang++ as a linker)
   # -fsanitize-coverage=func for function-level coverage (very fast, see https://clang.llvm.org/docs/SanitizerCoverage.html).
   # -fsanitize-coverage=bb for basic-block-level coverage (may add up to 30% extra slowdown).
   # -fsanitize-coverage=edge for edge-level coverage (up to 40% slowdown).
   # An alternative for coverage (operating on source files instead of binary) is https://clang.llvm.org/docs/SourceBasedCodeCoverage.html

   # Make debugging easier.
   set(CMAKE_CXX_FLAGS_CLANGDEBUG "${CMAKE_CXX_FLAGS_CLANG} -ggdb3 -fno-omit-frame-pointer -fno-optimize-sibling-calls")

   # TODO: add -fsanitize=leak to CXXFLAGS and LINKER
   set(CMAKE_CXX_FLAGS_CLANGASAN "${CMAKE_CXX_FLAGS_CLANGDEBUG} -fsanitize=address,undefined -fsanitize-address-use-after-scope")
   set(CMAKE_EXE_LINKER_FLAGS_CLANGASAN "-fsanitize=address,undefined -fsanitize-blacklist=${CMAKE_CURRENT_LIST_DIR}/sanitizer_blacklist.txt")
   set(CMAKE_SHARED_LINKER_FLAGS_CLANGASAN "${CMAKE_EXE_LINKER_FLAGS_CLANGASAN}")

   # -fsanitize=thread checks for race conditions (not allowed together with msan or asan, add -O1 or higher for reasonable performance) (https://clang.llvm.org/docs/ThreadSanitizer.html)
   # Tsan does not support C++ exceptions. Tsan does not work well with C++11 threading 
   # (source: https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual)
   set(CMAKE_CXX_FLAGS_CLANGTSAN "${CMAKE_CXX_FLAGS_CLANGDEBUG} -fsanitize=thread -O01")
   set(CMAKE_EXE_LINKER_FLAGS_CLANGTSAN "-fsanitize=thread -fsanitize-blacklist=${CMAKE_CURRENT_LIST_DIR}/sanitizer_blacklist.txt")
   set(CMAKE_SHARED_LINKER_FLAGS_CLANGTSAN "${CMAKE_EXE_LINKER_FLAGS_CLANGTSAN}")

   # WARNING: All libraries (including libstdc++/libc++!) must be re-build using memory sanitizer,
   # see https://github.com/google/sanitizers/wiki/MemorySanitizerLibcxxHowTo
   set(CMAKE_CXX_FLAGS_CLANGMSAN "${CMAKE_CXX_FLAGS_CLANGDEBUG} -fsanitize=memory -fsanitize-memory-track-origins")
   set(CMAKE_EXE_LINKER_FLAGS_CLANGMSAN "-fsanitize=memory")
   set(CMAKE_SHARED_LINKER_FLAGS_CLANGMSAN "${CMAKE_EXE_LINKER_FLAGS_CLANGMSAN}")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls"
	CACHE STRING "Flags used by the C++ compiler during ASan builds."
	FORCE)
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-fsanitize=address -Wl,--no-undefined"
	CACHE STRING "Flags used for linking binaries during ASan builds."
	FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "-fsanitize=address -Wl,--no-undefined"
	CACHE STRING "Flags used by the shared libraries linker during ASan builds."
	FORCE)

set(CMAKE_CXX_FLAGS_DEBUG_NOASAN "-ggdb3 -fno-omit-frame-pointer -fno-optimize-sibling-calls"
    CACHE STRING "Flags used by the C++ compiler during ASan builds."
    FORCE)
set(CMAKE_EXE_LINKER_FLAGS_DEBUG_NOASAN "-Wl,--no-undefined"
    CACHE STRING "Flags used for linking binaries."
    FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG_NOASAN "-Wl,--no-undefined"
    CACHE STRING "Flags used by the shared libraries linker."
    FORCE)

set(CMAKE_CXX_FLAGS_COVERAGE "-ggdb3 -fno-omit-frame-pointer -fno-optimize-sibling-calls --coverage"
    CACHE STRING "Flags used by the C++ compiler during builds."
    FORCE)
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "--coverage -Wl,--no-undefined"
    CACHE STRING "Flags used for linking binaries."
    FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "--coverage -Wl,--no-undefined"
    CACHE STRING "Flags used by the shared libraries linker."
    FORCE)

# We should use -flto when https://gcc.gnu.org/bugzilla/show_bug.cgi?id=58251 is fixed
# We removed -DNDEBUG on purpose to have asserts even in release builds. It is better
# to have the simulator crashing in an assert than to invoke undefined behavior
# and corrupt the simulation.
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-fno-omit-frame-pointer -O2 -ggdb3"
    CACHE STRING "Flags used by the C++ compiler."
    FORCE)
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "-Wl,--no-undefined"
    CACHE STRING "Flags used for linking binaries."
    FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO "-Wl,--no-undefined"
	CACHE STRING "Flags used for linking binaries."
	FORCE)

option(USE_LD_GOLD "Use the gold linker" ON)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(USE_LD_GOLD OFF)
endif()


if(USE_LD_GOLD)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version ERROR_QUIET OUTPUT_VARIABLE LD_VERSION)
  if("${LD_VERSION}" MATCHES "GNU gold")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
  else()
    message(WARNING "GNU gold linker isn't available, using the default system linker.")
    set(USE_LD_GOLD OFF)
  endif()
endif()

# Use "thin" archives, which do not contain the object files, but only an index.
# Make builds faster, because it trashes the disc cache less (saves about 600 MB).
# The downside is that you cannot copy the static libraries to another machine (or into an rpm),
# because they do not actually contain the contents of the object files.
# See https://sourceware.org/binutils/docs/binutils/ar.html
if ((NOT USE_FAT_ARCHIVES) AND (${CMAKE_SYSTEM_NAME} STREQUAL "Linux"))
    message(STATUS "Using thin archives - static libraries cannot be delivered (see OhbConfig.cmake)")
    set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> cruT <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> cruT <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_CXX_ARCHIVE_APPEND "<CMAKE_AR> ruT <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_C_ARCHIVE_APPEND "<CMAKE_AR> ruT <TARGET> <LINK_FLAGS> <OBJECTS>")
endif ()
