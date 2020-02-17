cmake_minimum_required (VERSION 3.2)
project (RemoteRunnerd)
find_package(cppzmq)
set(SOURCE
    "src/Messaging/ZMQServer.cpp"
    "src/InstructionProcessor.cpp"
    "src/RemoteRunnerd.cpp"
    "src/TaskRunner.cpp"
)
include_directories("src/" "src/Messaging/")
set (CMAKE_C_FLAGS "-O3 -g3")
add_executable(RemoteRunnerd ${SOURCE}) 
target_link_libraries(RemoteRunnerd cppzmq)
target_compile_features(RemoteRunnerd PRIVATE cxx_std_17)
