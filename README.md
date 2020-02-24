[![Actions Status](https://github.com/einerfreiheit/RemoteRunnerd/workflows/C/C++%20CI/badge.svg?branch=master)](https://github.com/einerfreiheit/RemoteRunnerd/actions)

# RemoteRunnerd

Simple remote task runner. Recieve commands and run task if permitted.

 - Configuration:

       /etc/remote-runnerd.conf - space separated list of permitted commands.
      
 - Options:

       -t: task timeout, sec; default: 0.
    
       -a: protocol/end-point; default: tcp://127.0.0.1:12345. Available protocols: tcp, ipc.
        
  -  Usage example:
  
          ./RemoteRunnerd -t 10 -a ipc:///tmp/uds_test
 
 - Build:

       A: Manually
           1. Download and unzip the libzmq then build via Cmake
           Set -DENABLE_DRAFTS=OFF option to disable DRAFT socket API when build libzmq and cppzmq
           2. Build and install cppzmq from source (third_party/cppzmq)
           3. Build RemoteRunnerd
       B: Via script
           1.  Run ./build_ci.sh
       C: Via Docker
           1. Build image: 
               docker build --rm . -t runnerd_image:test
           2. Run container: 
               docker run --name runnerd_test --rm -ti -v /tmp/:/tmp/ --network="host"  runnerd_image:test /bin/bash  
           3. Start RemoteRunner: 
               RemoteRunnerd/build/RemoteRunnerd
           4. Change remote-runnerd.conf: 
               cp remote-runnerd.conf runnerd_test:/etc/remote-runnerd.conf
