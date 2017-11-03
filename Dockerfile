FROM alpine:3.6
RUN apk update
RUN apk add \
    automake \
    binutils-dev \
    build-base \
    linux-headers

WORKDIR /src
CMD [ "make", "clean", "test" ]

# recommended usage:
#
# docker build -t abe64 .
# docker run -v $(pwd):/src --rm -it abe64
