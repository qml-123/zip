cmake_minimum_required(VERSION 3.15)
project(zip)

set(CMAKE_CXX_STANDARD 14)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)
add_executable(server server_test.cpp src/Client.cpp src/Client.h src/Socket.cpp src/Socket.h src/com.h src/Server.cpp src/Server.h src/LZ77.h src/Base_Text_Process.h src/Factory.cpp src/Factory.h src/com.cpp src/Algorithm.cpp src/Algorithm.h src/Hahuman.cpp src/Hahuman.h src/Arith.cpp src/Arith.h src/Rle.cpp src/Rle.h)
add_executable(client client_test.cpp src/Client.cpp src/Client.h src/Socket.cpp src/Socket.h src/com.h src/Server.cpp src/Server.h src/LZ77.h src/Base_Text_Process.h src/Factory.cpp src/Factory.h src/com.cpp src/Algorithm.cpp src/Algorithm.h src/Hahuman.cpp src/Hahuman.h src/Arith.cpp src/Arith.h src/Rle.cpp src/Rle.h)
add_executable(test_hafuman test_hafuman.cpp src/Client.cpp src/Client.h src/Socket.cpp src/Socket.h src/com.h src/Server.cpp src/Server.h src/LZ77.h src/Base_Text_Process.h src/Factory.cpp src/Factory.h src/com.cpp src/Algorithm.cpp src/Algorithm.h src/Hahuman.cpp src/Hahuman.h src/Arith.cpp src/Arith.h src/Rle.cpp src/Rle.h)
add_executable(test_Arith test_Arith.cpp src/Client.cpp src/Client.h src/Socket.cpp src/Socket.h src/com.h src/Server.cpp src/Server.h src/LZ77.h src/Base_Text_Process.h src/Factory.cpp src/Factory.h src/com.cpp src/Algorithm.cpp src/Algorithm.h src/Hahuman.cpp src/Hahuman.h src/Arith.cpp src/Arith.h src/Rle.cpp src/Rle.h)
add_executable(test_rle test_rle.cpp src/Client.cpp src/Client.h src/Socket.cpp src/Socket.h src/com.h src/Server.cpp src/Server.h src/LZ77.h src/Base_Text_Process.h src/Factory.cpp src/Factory.h src/com.cpp src/Algorithm.cpp src/Algorithm.h src/Hahuman.cpp src/Hahuman.h src/Arith.cpp src/Arith.h src/Rle.cpp src/Rle.h)




