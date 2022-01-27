# syntax=docker/dockerfile:1
FROM conanio/gcc10 AS sys
RUN sudo -A apt-get update > /dev/null # && sudo -A apt-get install -y --no-install-recommends libegl-dev libsdl-dev libx11-dev > /dev/null

FROM sys AS deps

WORKDIR /app
COPY ./conanfile.txt ./
RUN sudo chown -R conan /app && mkdir -p /app/build
WORKDIR /app/build

ENV CONAN_SYSREQUIRES_MODE=enabled
RUN conan install ..

FROM deps AS builder

WORKDIR /app
COPY . .
RUN sudo chown -R conan /app

WORKDIR /app/build
# RUN conan build .
RUN cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_MODULE_PATH=$PWD
RUN cmake --build .

CMD [ "/app/build/out/sortviz" ]

