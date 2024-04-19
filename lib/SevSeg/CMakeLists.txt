add_library(SevSeg INTERFACE)

target_sources(SevSeg INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/SevSeg.cpp
)

target_include_directories(SevSeg INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(SevSeg INTERFACE hardware_i2c)