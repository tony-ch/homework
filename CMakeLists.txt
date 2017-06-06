cmake_minimum_required(VERSION 2.7)
project(FTPS)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/headers/ftp_client.h
        src/headers/ftp_request.h
        src/headers/ftp_response.h
        src/headers/ftp_server.h
        src/libs/client_socket.cpp
        src/libs/client_socket.h
        src/libs/server_socket.cpp
        src/libs/server_socket.h
        src/libs/socket.cpp
        src/libs/socket.h
        src/libs/socket_exception.cpp
        src/libs/socket_exception.h
        src/libs/utility.cpp
        src/libs/utility.h
        src/sources/ftp_client.cpp
        src/sources/ftp_request.cpp
        src/sources/ftp_response.cpp
        src/sources/ftp_server.cpp)

add_executable(FTPS ${SOURCE_FILES} src/main.cpp)
add_executable(FTP_Server ${SOURCE_FILES} src/main_server.cpp)
add_executable(FTP_Client ${SOURCE_FILES} src/main_client.cpp)
target_link_libraries(FTP_Server ssl crypto)
target_link_libraries(FTP_Client ssl crypto)
target_link_libraries(FTPS ssl crypto)