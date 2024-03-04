# Install script for directory: F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Tanks")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "libassimp5.3.0-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/external/assimp/lib/Debug/assimp-vc143-mtd.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/external/assimp/lib/Release/assimp-vc143-mt.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/external/assimp/lib/MinSizeRel/assimp-vc143-mt.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/external/assimp/lib/RelWithDebInfo/assimp-vc143-mt.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/anim.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/aabb.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/ai_assert.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/camera.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/color4.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/color4.inl"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/build/external/assimp/code/../include/assimp/config.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/ColladaMetaData.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/commonMetaData.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/defs.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/cfileio.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/light.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/material.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/material.inl"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/matrix3x3.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/matrix3x3.inl"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/matrix4x4.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/matrix4x4.inl"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/mesh.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/ObjMaterial.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/pbrmaterial.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/GltfMaterial.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/postprocess.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/quaternion.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/quaternion.inl"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/scene.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/metadata.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/texture.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/types.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/vector2.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/vector2.inl"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/vector3.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/vector3.inl"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/version.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/cimport.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/AssertHandler.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/importerdesc.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Importer.hpp"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/DefaultLogger.hpp"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/ProgressHandler.hpp"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/IOStream.hpp"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/IOSystem.hpp"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Logger.hpp"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/LogStream.hpp"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/NullLogger.hpp"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/cexport.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Exporter.hpp"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/DefaultIOStream.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/DefaultIOSystem.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/SceneCombiner.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/fast_atof.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/qnan.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/BaseImporter.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Hash.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/MemoryIOWrapper.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/ParsingUtils.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/StreamReader.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/StreamWriter.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/StringComparison.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/StringUtils.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/SGSpatialSort.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/GenericProperty.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/SpatialSort.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/SmallVector.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/SmoothingGroups.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/SmoothingGroups.inl"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/StandardShapes.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/RemoveComments.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Subdivision.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Vertex.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/LineSplitter.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/TinyFormatter.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Profiler.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/LogAux.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Bitmap.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/XMLTools.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/IOStreamBuffer.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/CreateAnimMesh.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/XmlParser.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/BlobIOSystem.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/MathFunctions.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Exceptional.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/ByteSwapper.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Base64.hpp"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Compiler/pushpack1.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Compiler/poppack1.h"
    "F:/GitHub/CS3321-high-rollers/CS4488-Tanks-Team/external/assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

