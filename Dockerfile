# syntax=docker/dockerfile:1
FROM conanio/gcc10 AS deps
RUN sudo -A apt-get update > /dev/null # && sudo -A apt-get install -y --no-install-recommends libegl-dev libsdl-dev libx11-dev > /dev/null

WORKDIR /app
COPY ./conanfile.txt ./
COPY ./Makefile ./
RUN sudo chown -R conan /app
ENV CONAN_SYSREQUIRES_MODE=enabled
RUN make deps

FROM deps AS builder

WORKDIR /app
COPY . .
RUN sudo chown -R conan /app

RUN make build

