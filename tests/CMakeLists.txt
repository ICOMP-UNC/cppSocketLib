cmake_minimum_required(VERSION 3.10)
project(tests)

# Asegúrate de que estas rutas son correctas
include_directories(${PROJECT_SOURCE_DIR}/../src)
link_directories(${PROJECT_SOURCE_DIR}/../src)

#add_executable(tests test1.cpp test2.cpp) # Reemplaza test1.cpp y test2.cpp con tus archivos de prueba
#target_link_libraries(tests gtest gtest_main) # Enlaza con Google Test