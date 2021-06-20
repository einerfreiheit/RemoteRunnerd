cmake_minimum_required (VERSION 3.2)

project (RemoteRunnerd)

find_package( Threads )

set(SOURCE
    "src/Communication/Server.cpp"
    "src/Communication/Session.cpp"

    "src/Endpoint/EndpointFactory.cpp"
    "src/Endpoint/TCPEndpoint.cpp"
    "src/Endpoint/UDSEndpoint.cpp"
    
    "src/Task/RemoteTask.cpp"
    "src/Task/TaskRunner.cpp"
    
    "src/Utils/TextProcessing.cpp"
    
    "src/Configuration.cpp"
    "src/Permissions.cpp"
)

include_directories(
    "src" 
    "src/Communication" 
    "src/Endpoint" 
    "src/Task"
    "src/Utils"
)

set (CMAKE_C_FLAGS "-O3 -g3")
add_executable(RemoteRunnerd "src/RemoteRunnerd.cpp" ${SOURCE}) 
target_link_libraries(RemoteRunnerd ${CMAKE_THREAD_LIBS_INIT} )
target_compile_features(RemoteRunnerd PRIVATE cxx_std_17)
