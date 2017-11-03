FROM alpine:3.6
RUN apk update
RUN apk add \
    automake \
    binutils-dev \
    build-base \
    linux-headers

COPY / /src
WORKDIR /src

RUN make clean test
