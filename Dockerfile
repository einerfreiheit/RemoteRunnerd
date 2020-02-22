FROM ubuntu:18.04

RUN apt-get update && apt-get install -y build-essential \
    sudo \
    cmake \
    g++ \
    curl \
    git \
    && rm -rf /tmp/* /var/tmp/*

WORKDIR /github
RUN git clone https://github.com/einerfreiheit/RemoteRunnerd.git
RUN cd /github/RemoteRunnerd/ && ./build_ci.sh
RUN cp remote-runnerd.conf /etc/remote-runnerd.conf
