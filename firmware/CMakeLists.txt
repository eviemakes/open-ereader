
set(PICO_BOARD adafruit_feather_rp2040)
cmake_minimum_required(VERSION 3.5)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(include/pico-sdk/pico_sdk_init.cmake)
include_directories(${CMAKE_SOURCE_DIR})

project(ereader C CXX ASM)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Specify all cpp files and headers
set(SOURCES src/ereader_test.cpp)

# Create executable with target sources
add_executable(ereader ${SOURCES})

# Add library subdirectories
add_subdirectory(lib/display)

# Add the libraries to the build
target_link_libraries(ereader PUBLIC pico_stdlib hardware_spi)

# Link dependencies
target_link_libraries(ereader PUBLIC display)

# Enable USB output, disable UART output
pico_enable_stdio_usb(ereader 1)
pico_enable_stdio_uart(ereader 0)

# Configure output files
pico_add_extra_outputs(ereader)