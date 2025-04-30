cmake_minimum_required(VERSION "3.18.0")
project(bioinformtics_tools)

set(CMAKE_CXX_STANDARD 17)

# Find BamTools
#find_package(BamTools REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_SOURCE_DIR}/ext/bamtools/src")
add_subdirectory(${PROJECT_SOURCE_DIR}/ext/bamtools/src/api)

add_executable(fastqtofasta src/fastqtofasta.cpp)
add_executable(revcomp src/revcomp.cpp)
add_executable(transcribe src/transcribe.cpp)
add_executable(translate src/translate.cpp)
add_executable(bamtofastq src/bamtofastq.cpp)
target_link_libraries(bamtofastq BamTools)
