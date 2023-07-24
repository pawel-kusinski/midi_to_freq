cmake_minimum_required(VERSION 3.0)
project(midi_to_freq_example C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)

# Set the path to the Python script
set(PYTHON_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/../midi_to_freq_code_gen.py")

# Set default values for the optional parameters
set(PHASE_INCREMENT_WIDTH 32)
set(DDS_F_CLK_MHZ 100)

# Generate the source files using the Python script during the build process
add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/generated/midi_to_freq.c ${CMAKE_SOURCE_DIR}/generated/midi_to_freq.h
    COMMAND python ${PYTHON_SCRIPT} --phase_increment_width ${PHASE_INCREMENT_WIDTH} --dds_f_clk_mhz ${DDS_F_CLK_MHZ} --output_directory ${CMAKE_SOURCE_DIR}/generated
    DEPENDS ${PYTHON_SCRIPT}
)

# Add the generated source and header files to the project
include_directories(${CMAKE_SOURCE_DIR}/generated)

set(SOURCE_FILES main.c ${CMAKE_SOURCE_DIR}/generated/midi_to_freq.c)
add_executable(midi_to_freq_example ${SOURCE_FILES})
