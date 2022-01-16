cmake_minimum_required(VERSION 3.22)

project(glapp VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCE_FILES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(glapp ${SOURCE_FILES})

set(SDL_BUILDING_LIBRARY True)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(SDL REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/out)

include_directories(
  "./include/"
  ${GLEW_INCLUDE_DIR}
  ${GLM_INCLUDE_DIRS}
  ${SDL_INCLUDE_DIR}
  ${FREETYPE_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(glapp
  ${GLEW_LIBRARIES}
  ${SDL_LIBRARY}
  ${FREETYPE_LIBRARY}
  ${OPENGL_LIBRARIES}
)
