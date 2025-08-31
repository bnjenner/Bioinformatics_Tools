cmake_minimum_required(VERSION "3.18.0")
project(bioinformtics_tools)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions(-w)

include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_SOURCE_DIR}/ext/bamtools/src")
add_subdirectory(${PROJECT_SOURCE_DIR}/ext/bamtools/src/api)

add_executable(bamtofastq src/bamtofastq.cpp)
set_target_properties(bamtofastq PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_executable(fastqtofasta src/fastqtofasta.cpp)
set_target_properties(fastqtofasta PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_executable(flatfasta src/flatfasta.cpp)
set_target_properties(flatfasta PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_executable(gtfget src/gtfget.cpp)
set_target_properties(gtfget PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_executable(revcomp src/revcomp.cpp)
set_target_properties(revcomp PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_executable(transcribe src/transcribe.cpp)
set_target_properties(transcribe PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_executable(translate src/translate.cpp)
set_target_properties(translate PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_executable(qdist src/qdist.cpp)
set_target_properties(qdist PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
add_executable(qbycycle src/qbycycle.cpp)
set_target_properties(qbycycle PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(bamtofastq BamTools)


find_package(CUDAToolkit QUIET)
if(CUDAToolkit_FOUND)
  message(STATUS "CUDA Toolkit found.")
  # Additional CUDA configurations can be done here
else()
  message(STATUS "CUDA Toolkit not found.")
endif()
