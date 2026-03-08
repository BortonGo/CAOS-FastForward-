FROM --platform=linux/amd64 ubuntu:22.04

RUN apt update && \
    apt install -y \
        build-essential \
        python3 \
        python3-pip && \
    pip3 install PyYAML

WORKDIR /app