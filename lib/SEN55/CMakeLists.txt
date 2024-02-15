add_library(SEN55 INTERFACE)

target_sources(SEN55 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/SEN55.cpp
)

target_include_directories(SEN55 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(SEN55 INTERFACE hardware_i2c)