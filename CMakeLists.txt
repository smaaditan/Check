cmake_minimum_required(VERSION 3.16)
project(sample_ecu_demo C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -------- XCPlite (fetched by build.sh) --------
set(XCPLITE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/XCPlite)
if(NOT EXISTS "${XCPLITE_DIR}/CMakeLists.txt")
  message(FATAL_ERROR "XCPlite not found. Run ./build.sh (it clones third_party/XCPlite).")
endif()

# Build XCPlite as a library; it exposes examples and a library target (xcplib)
add_subdirectory(${XCPLITE_DIR} ${CMAKE_BINARY_DIR}/_xcplite_build)

# -------- Our demo app --------
add_executable(sample_ecu_demo
  main.c
  a2l_generator.c
)

target_include_directories(sample_ecu_demo
  PRIVATE
    ${XCPLITE_DIR}/inc
    ${XCPLITE_DIR}/src  # for internal cfg headers used by examples
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link against XCPlite library
target_link_libraries(sample_ecu_demo PRIVATE xcplib)

# Put A2L next to the binary on first run (we generate at runtime)
set_target_properties(sample_ecu_demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Optional: more warnings
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(sample_ecu_demo PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()
