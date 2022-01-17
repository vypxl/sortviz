cmake_minimum_required(VERSION 3.22)

project(sortviz)

set (CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SOURCE_FILES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(sortviz ${SOURCE_FILES})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/out)

if(NOT EMSCRIPTEN)
  find_package(GLEW REQUIRED)
  find_package(SDL REQUIRED)
  find_package(Freetype REQUIRED)
  find_package(OpenGL REQUIRED)


  include_directories(
    "./include/"
    ${GLEW_INCLUDE_DIR}
    ${SDL_INCLUDE_DIR}
    ${FREETYPE_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
  )

  target_link_libraries(sortviz
    ${GLEW_LIBRARIES}
    ${SDL_LIBRARY}
    ${FREETYPE_LIBRARY}
    ${OPENGL_LIBRARIES}
  )
else()
  set(CMAKE_EXECUTABLE_SUFFIX .html)
  set(LIBS "-s USE_SDL=1 -s USE_FREETYPE=1")
  set(OPTS "-s FULL_ES3=1 -s USE_WEBGL2=1 -s WASM=1 -s ASYNCIFY -g")
  set(OUT_OPTS "-o out/web/sortviz.html --preload-file ./res@/res --preload-file ./shaders@/shaders")

  if("${PROJECT_BINARY_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}")
  else()
    file(COPY res shaders DESTINATION ${PROJECT_BINARY_DIR})
  endif()

  # Disabled until browsers get good multithreading support.
  # set(THREADS "-s USE_PTHREADS -s PTHREAD_POOL_SIZE=4")
  set(THREADS "")

  include_directories(
    "./include/"
    "/usr/include/glm"
  )
  add_definitions(${LIBS})
  set_target_properties(sortviz PROPERTIES LINK_FLAGS "${LIBS} ${OPTS} ${THREADS} ${OUT_OPTS}")
endif()
