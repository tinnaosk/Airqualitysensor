add_library(RFM95W INTERFACE)

target_sources(RFM95W INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/RFM95W.cpp
    ${CMAKE_CURRENT_LIST_DIR}/RFM95W_regs.h
)

target_include_directories(RFM95W INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(RFM95W INTERFACE hardware_spi)