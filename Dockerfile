# syntax=docker/dockerfile:1

# Native system dependencies
FROM conanio/gcc10 AS sys_deps
RUN sudo apt-get update && sudo apt-get install -y libsdl2-dev libegl-dev ninja-build

FROM sys_deps AS DEPS
WORKDIR /app
COPY ./conanfile.txt ./
RUN sudo chown -R conan /app
ENV CONAN_SYSREQUIRES_MODE=enabled
RUN mkdir -p build/native \
    && cd build/native \
    && conan install ../.. -s compiler.version=10

# Building native target
FROM deps AS native
WORKDIR /app
COPY . .
RUN sudo chown -R conan /app && make build

#####

# Web dependencies
FROM emscripten/emsdk:3.1.2 AS web_deps
WORKDIR /app
RUN sudo apt-get update && sudo apt-get install ninja-build
# Caching used ports
RUN echo "" | emcc -s USE_SDL=2 -s USE_FREETYPE -o /dev/null -x c++ -
# Predownloading glm
RUN git clone https://github.com/g-truc/glm.git /tmp/glm \
    && mkdir -p include \
    && cp -TR /tmp/glm/glm include/glm \
    && rm -rf /tmp/glm

# Building web target
FROM web_deps AS web
COPY . .
RUN make embuild

#####

# Copying compilation results
FROM scratch AS export
COPY --from=web /app/out /
COPY --from=native /app/out /
