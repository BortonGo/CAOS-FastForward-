FROM ubuntu:24.04

ENV TZ=Europe/Moscow
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && apt-get install -y wget gnupg

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    python3 \
    python3-pip \
    python3-venv \
    firejail \
    git \
    curl

RUN apt-get update && apt-get install -y lsb-release software-properties-common

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    ccache \
    ninja-build \
    g++ \
    clang \
    clang-format \
    clang-tidy \
    valgrind \
    mysql-common

RUN apt-get update \
 && apt-get install -y --no-install-recommends sudo

RUN useradd -m -s /bin/bash ejexec

RUN echo "ejexec ALL=(ALL) NOPASSWD:SETENV:ALL" > /etc/sudoers.d/ejexec \
 && chmod 0440 /etc/sudoers.d/ejexec
