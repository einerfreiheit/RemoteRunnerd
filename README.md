[![Actions Status](https://github.com/einerfreiheit/RemoteRunnerd/workflows/C/C++%20CI/badge.svg?branch=master)](https://github.com/einerfreiheit/RemoteRunnerd/actions)

# RemoteRunnerd

Simple remote task runner. Read list of permitted commands and accept incoming requests.

 - Configuration:

       /etc/remote-runnerd.conf - space separated list of permitted commands.
      
 - Options:

       -t: task timeout, sec; default = 0
    
       -a: protocol/end-point; default tpc://127.0.0.1:12345. Available protocols: tcp, ipc. 
        
  -  Usage example:
  
          ./RemoteRunnerd -t 10 -a ipc:///tmp/uds_test
 

 - Build:

       A: Manually
          
            1. Download and unzip the libzmq build via Cmake. 
            Set -DENABLE_DRAFTS=OFF option to disable DRAFT socket API when build libzmq and cppzmq.
            2. Install cppzmq:
                  2.1 cd third_party/cppzmq
                  2.2 mkdir build && cd build && cmake .. && sudo make -j4 install
                  
            3. Build RemoteRunnerd via Cmake  
            
        B: Via script (run from RemoteRunderd):
      
           1. cd RemoteRunderd && ./build_ci.sh
           
        C: Via Dockerfile:
      
           1. Build image: docker build --rm . -t runnerd_image:latest
           2. Run container: docker run --name runnerd_test --rm -ti -v /tmp/:/tmp/ --network="host"  runnerd_image:latest /bin/bash
           3. Change remote-runnerd.conf: cp remote-runnerd.conf runnerd_test:/etc/remote-runnerd.conf

