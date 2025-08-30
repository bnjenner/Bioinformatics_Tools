cmake_minimum_required(VERSION "3.18.0")
project(bioinformtics_tools)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-w)

include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_SOURCE_DIR}/ext/bamtools/src")
add_subdirectory(${PROJECT_SOURCE_DIR}/ext/bamtools/src/api)

add_executable(bamtofastq src/bamtofastq.cpp)
add_executable(fastqtofasta src/fastqtofasta.cpp)
add_executable(flatfasta src/flatfasta.cpp)
add_executable(gtfget src/gtfget.cpp)
add_executable(revcomp src/revcomp.cpp)
add_executable(transcribe src/transcribe.cpp)
add_executable(translate src/translate.cpp)
add_executable(qdist src/qdist.cpp)
add_executable(qbycycle src/qbycycle.cpp)

target_link_libraries(bamtofastq BamTools)


find_package(CUDAToolkit QUIET)
if(CUDAToolkit_FOUND)
  message(STATUS "CUDA Toolkit found.")
  # Additional CUDA configurations can be done here
else()
  message(STATUS "CUDA Toolkit not found.")
endif()
