cmake_minimum_required(VERSION 3.10.2)
project(restApp)

set(CMAKE_CXX_STANDARD 17)


## SOURCE
set(SOURCES Authorization/Authorization.cpp 
            Controllers/Controller.cpp 
            Database/FakeDB.cpp 
            Services/HelloWorldService.cpp 
            Services/ServiceManager.cpp
            RestEndpoint.cpp 
            Protocols/generated/sc.pb.cc app.cpp 
            )
add_executable(${PROJECT_NAME} ${SOURCES})

## includes 
include_directories(Authorization Controllers Database Services Protocols/generated /repo/cpp-jwt/include usr/local/openssl/include)
set(EXTRA_LIBRARY "-lpistache -lpthread -lcrypto -lssl -lprotobuf")

target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBRARY})