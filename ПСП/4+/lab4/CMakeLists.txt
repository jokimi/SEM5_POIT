cmake_minimum_required(VERSION 3.29)
project(lab4)


add_executable(clientB clientB.cpp)
target_link_libraries(clientB ws2_32)

add_executable(serverB serverB.cpp)
target_link_libraries(serverB ws2_32)

add_executable(serverU serverU.cpp
        ErrorMsgText.h)
target_link_libraries(serverU ws2_32)


add_executable(clientU clientU.cpp ErrorMsgText.h)
target_link_libraries(clientU ws2_32)




