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
          
          1. Download and unzip the libzmq build via Cmake. Set -DENABLE_DRAFTS=OFF option to disable DRAFT socket API when build libzmq and cppzmq.
          2. Install cppzmq:
                  - cd third_party/cppzmq
                  - mkdir build && cd build && cmake .. && sudo make -j4 install
                  
          3. Build RemoteRunnerd via Cmake
