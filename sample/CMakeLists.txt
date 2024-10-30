add_executable(sample sample.cpp)
set_property(TARGET sample PROPERTY CXX_STANDARD 17)
target_include_directories(sample PUBLIC ${PROJECT_SOURCE_DIR}/include)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-char8_t")
endif ()

