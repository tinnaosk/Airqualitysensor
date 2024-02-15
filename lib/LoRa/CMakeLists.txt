add_library(LoRa INTERFACE)

target_sources(LoRa INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/LoRa.cpp
)

target_include_directories(LoRa INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(LoRa INTERFACE hardware_i2c)