cmake_minimum_required(VERSION 3.15)

project (LTC2943_api C CXX)

add_library(api STATIC 
    Source/Api/sensor_LTC2943.c
)
target_include_directories(api
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)

add_library(driver STATIC 
    Source/Drivers/i2c.c
)
target_include_directories(driver
    PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)



add_executable(${PROJECT_NAME} Source/main.c)

target_link_libraries(${PROJECT_NAME} PUBLIC
    api
    driver
)