cmake_minimum_required(VERSION 2.6)

project(CppCeid)
SET(CMAKE_BUILD_TYPE Release)
# SET(CMAKE_BUILD_TYPE Debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)


find_package(GLUT)
find_package(OpenGL)


IF (SOIL_INCLUDE_DIR)
  # Already in cache, be silent
  SET(SOIL_FIND_QUIETLY TRUE)
ENDIF (SOIL_INCLUDE_DIR)
FIND_PATH(SOIL_INCLUDE_DIR SOIL.h PATH_SUFFIXES include/SOIL include)
SET(SOIL_NAMES SOIL Soil soil)
FIND_LIBRARY(SOIL_LIBRARY NAMES ${SOIL_NAMES} )
MARK_AS_ADVANCED( SOIL_LIBRARY SOIL_INCLUDE_DIR )
SET(SOIL_INCLUDE_DIRS "${SOIL_INCLUDE_DIR}")
SET(SOIL_LIBRARIES    "${SOIL_LIBRARY}")
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SOIL DEFAULT_MSG SOIL_LIBRARIES SOIL_INCLUDE_DIRS)



# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/src/)

# Extra compiler flags
add_definitions(-Wall)
add_definitions(-Wextra)
if(CMAKE_BUILD_TYPE EQUAL "DEBUG")
	add_definitions(-g)
endif()

file(GLOB_RECURSE BACK_SOURCE ${PROJECT_SOURCE_DIR}/src/backend/*.cpp)
file(GLOB_RECURSE BACK_INCS ${PROJECT_SOURCE_DIR}/include/backend/*.h)
add_library(mechanism SHARED ${BACK_INCS} ${BACK_SOURCE})


file(GLOB_RECURSE CONSOLE_INCS ${PROJECT_SOURCE_DIR}/include/frontend/Console/*.h)
file(GLOB_RECURSE CONSOLE_SOURCE ${PROJECT_SOURCE_DIR}/src/frontend/Console/*.cpp)

add_executable(Robsim ${CONSOLE_INCS} ${CONSOLE_SOURCE} ${PROJECT_SOURCE_DIR}/src/console.cpp)
target_link_libraries(Robsim mechanism)


if(OPENGL_FOUND AND GLUT_FOUND AND SOIL_FOUND)
	include_directories(${GLUT_INCLUDE_DIRS})
	add_definitions(${GLUT_DEFINITIONS})

	include_directories(${OpenGL_INCLUDE_DIRS})
	add_definitions(${OpenGL_DEFINITIONS})

	include_directories(${SOIL_INCLUDE_DIRS})
	add_definitions(${SOIL_DEFINITIONS})

	file(GLOB_RECURSE GUI_INCS ${PROJECT_SOURCE_DIR}/include/frontend/Gui/*.h)
        file(GLOB_RECURSE GUI_SOURCE ${PROJECT_SOURCE_DIR}/src/frontend/Gui/*.cpp)

        add_executable(RobsimGui ${GUI_INCS} ${GUI_SOURCE} ${PROJECT_SOURCE_DIR}/src/gui.cpp)
        target_link_libraries(RobsimGui ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} ${SOIL_LIBRARIES} mechanism)
endif()
