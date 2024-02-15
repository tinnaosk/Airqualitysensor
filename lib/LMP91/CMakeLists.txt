add_library(LMP91 INTERFACE)

target_sources(SEN55 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/LMP91.cpp
)

target_include_directories(LMP91 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(LMP91 INTERFACE hardware_i2c)