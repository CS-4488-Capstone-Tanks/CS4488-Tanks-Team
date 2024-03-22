cmake_minimum_required(VERSION 3.27)

project(Tanks)

# Qt6 requires C++17 at a minimum
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Use Catch2 for testing
include(FetchContent)
FetchContent_Declare(Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0)

FetchContent_MakeAvailable(Catch2)


# Try to find Qt6 if available, otherwise find Qt5
# Set some variables so the later linking/including commands can use the found info

find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets Network QUIET)
if(Qt6_FOUND)
    message(STATUS "Using Qt6")
    set(QT_VERSION_MAJOR 6)
    set(QT_LIBRARIES Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGLWidgets Qt6::Network)
    set(QT_INCLUDES ${Qt6_INCLUDE_DIRS})
else()
    # If Qt6 is not found, try to find Qt5
    find_package(Qt5 COMPONENTS Core Gui Widgets Network REQUIRED)
    message(STATUS "Using Qt5")
    set(QT_VERSION_MAJOR 5)
    set(QT_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)
    set(QT_INCLUDES ${Qt5_INCLUDE_DIRS})
endif()

# Enable the Qt MOC compiler
set(CMAKE_AUTOMOC ON)

enable_testing()

# Tell CMake to also process GLM's cmake file
# now we can link against the target defined in that file
add_subdirectory(external/glm)


# Configure Assimp's build settings. We want a static library, not shared, so that
# we have one executable out. And we can skip installing it to the system, or building
# tests for it, since we're not the ones testing it
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Assimp build tests" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "Assimp install" FORCE)
set(ASSIMP_INSTALL_PDB OFF CACHE BOOL "Assimp install PDB" FORCE)

function(find_or_build_assimp)
    set(ASSIMP_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/build/bin/libassimp.so")

    if(EXISTS "${ASSIMP_LIB_PATH}")
        message(STATUS "Using cached Assimp")
    else()
        set(ASSIMP_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/build")

        file(MAKE_DIRECTORY "${ASSIMP_BUILD_DIR}")

        execute_process(COMMAND
            cmake "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp" -DBUILD_SHARED_LIBS=ON -DASSIMP_BUILD_TESTS=OFF -DASSIMP_INSTALL=OFF
            WORKING_DIRECTORY "${ASSIMP_BUILD_DIR}"
        )

        execute_process(COMMAND
            cmake --build . --config Release
            WORKING_DIRECTORY "${ASSIMP_BUILD_DIR}"
        )

        if(NOT EXISTS "${ASSIMP_LIB_PATH}")
            message(FATAL_ERROR "Failed to build Assimp")
        endif()
    endif()

    add_library(assimp STATIC IMPORTED GLOBAL)
    set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${ASSIMP_LIB_PATH})
    target_include_directories(assimp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include")
endfunction()


# Now import assimp
add_subdirectory(external/assimp)

#find_or_build_assimp()

# Glob any .cpp files in the current directory, storing them as a list in TANK_SOURCE_FILES
file(GLOB TANK_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Create our executable
add_executable(tanks ${TANK_SOURCE_FILES})

# Declare the system libraries we need to link with
target_link_libraries(tanks PRIVATE ${QT_LIBRARIES} glm::glm assimp)

# Declare the locations of the system headers for those libraries we'll need
target_include_directories(tanks PRIVATE
    ${QT_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/build/include
)

# Add a post-build step to copy over the assets folder
add_custom_command(TARGET tanks POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:tanks>/assets"
)