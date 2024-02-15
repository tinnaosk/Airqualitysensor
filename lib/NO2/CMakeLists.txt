add_library(NO2 INTERFACE)

target_sources(NO2 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/NO2.cpp
)

target_include_directories(NO2 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(NO2 INTERFACE hardware_i2c)