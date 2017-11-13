FROM ubuntu:16.04
MAINTAINER Alexander Sukhanov <sukhanov@rightchain.io>
RUN apt-get update -y
RUN apt-get install -y autoconf cmake git libboost-all-dev libssl-dev g++
ADD . /rightchain
WORKDIR /rightchain
RUN cmake -DCMAKE_BUILD_TYPE=Release .
RUN make -j4
