FROM ubuntu:22.04
LABEL maintainer="robert.loesch@informatik.tu-freiberg.de"

WORKDIR /
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update \
 && apt-get dist-upgrade -y \
 && apt-get install -y \
    graphviz \
    wget \
 && wget https://www.doxygen.nl/files/doxygen-1.9.7.linux.bin.tar.gz \
 && tar xf doxygen-1.9.7.linux.bin.tar.gz \
 && cd doxygen-1.9.7/bin \
 && mv * /usr/local/bin \
 && rm -rf doxygen-1.9.7/
