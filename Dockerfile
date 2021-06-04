FROM ubuntu:20.04

RUN apt-get update && apt-get install -y build-essential \
    sudo \
    cmake \
    g++ \
    git \
    && rm -rf /tmp/* /var/tmp/*

WORKDIR /runnerd
RUN git clone https://github.com/einerfreiheit/RemoteRunnerd.git
RUN cp /runnerd/RemoteRunnerd/remote-runnerd.conf /etc/remote-runnerd.conf
RUN cd /runnerd/RemoteRunnerd/ && ./build_ci.sh 
