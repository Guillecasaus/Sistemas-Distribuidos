cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)

project(fileManagerSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)	
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Importar la librería FileManager
add_library(FileManager STATIC IMPORTED) 
set_target_properties(FileManager PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/libFileManager.a"
  INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/"
)

# Ejecutable del servidor principal
add_executable(server utils.h utils.cpp server.cpp clientManager.cpp)
target_link_libraries(server pthread FileManager stdc++fs)

# Ejecutable del cliente
add_executable(client utils.h utils.cpp client.cpp)
target_link_libraries(client pthread stdc++fs)

# Ejecutable del fileManager local
add_executable(fileManager main_fm.cpp filemanager.h)
target_link_libraries(fileManager pthread FileManager stdc++fs)

# Ejecutable del broker
add_executable(broker utils.h utils.cpp broker.cpp)
target_link_libraries(broker pthread stdc++fs)

# Ejecutable del segundo servidor (server2)
add_executable(server2 utils.h utils.cpp server2.cpp clientManager.cpp)
target_link_libraries(server2 pthread FileManager stdc++fs)
