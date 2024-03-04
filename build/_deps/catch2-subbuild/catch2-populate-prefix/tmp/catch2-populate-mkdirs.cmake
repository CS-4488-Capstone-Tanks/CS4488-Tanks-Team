# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/_deps/catch2-src"
  "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/_deps/catch2-build"
  "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/_deps/catch2-subbuild/catch2-populate-prefix"
  "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/_deps/catch2-subbuild/catch2-populate-prefix/tmp"
  "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp"
  "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/_deps/catch2-subbuild/catch2-populate-prefix/src"
  "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
