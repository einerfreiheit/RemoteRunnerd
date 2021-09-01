[![Actions Status](https://github.com/einerfreiheit/RemoteRunnerd/workflows/C/C++%20CI/badge.svg?branch=master)](https://github.com/einerfreiheit/RemoteRunnerd/actions)

# RemoteRunnerd

Simple remote task runner. Runner executes requests from clients (can be either local or remote), then all output data will be sent to the client. Data can be transmitted via TCP or UDS (Unix Domain Socket). 

 - Configuration:

       /etc/remote-runnerd.conf - space separated list of permitted commands.
      
 - Options:

       -t: task execution timeout, sec; default: 0.
    
       -a: address (port for TCP, path for UDS)
       
       -p: protocol (TCP or UDS)
        
  - Usage example:
  
         ./RemoteRunnerd -t 10 -p TCP -a 12345
 
 - Build:
 
       git clone https://github.com/einerfreiheit/RemoteRunnerd.git
       cd RemoteRunnerd
       mkdir build
       cd build
       cmake -DCMAKE_BUILD_TYPE=Release ..
       make
