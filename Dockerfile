FROM ubuntu:latest
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    build-essential \
    ragel \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /advent-2024
COPY . .
RUN ["make", "-B"]
