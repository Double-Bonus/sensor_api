cmake_minimum_required(VERSION 3.15)

project (LTC2943_api C CXX)

set(SOURCES
    Source/Drivers/i2c.c
    Source/Api/sensor_LTC2943.c
)

set(LIBRARY_NAME api)

add_library(${LIBRARY_NAME} OBJECT ${SOURCES})

target_include_directories(${LIBRARY_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME} Source/main.c)

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARY_NAME})