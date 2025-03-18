include(../../include/pico-sdk/pico_sdk_init.cmake)

project(display)

add_library(display)

pico_sdk_init()

# Link dependencies
target_include_directories(display PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(display PUBLIC pico_stdlib hardware_spi)

target_sources(display PRIVATE 
                            epd7in5_V2.cpp
                            epd7in5_V2.h
                            epdif.cpp
                            epdif.h
                            imagedata.cpp
                            imagedata.h
                            )
