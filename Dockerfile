FROM alpine:3.6
RUN apk update
RUN apk add \
    automake \
    binutils-dev \
    build-base \
    linux-headers

COPY / /

RUN make
RUN ./abe -t
