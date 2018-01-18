FROM ubuntu:18.04

RUN apt-get update && apt-get -y install \
        make g++ \
        libboost-dev \
        libboost-filesystem-dev

ADD . /code
WORKDIR /code

# Build dynamically and statically linked
RUN make PROGRAM=a_dyn && \
    make PROGRAM=a_static LINK=STATIC

# Test if program can be executed
CMD [ "/bin/bash", "-c", \
      "/code/bin/a_dyn; /code/bin/a_static" ]