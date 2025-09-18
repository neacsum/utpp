add_executable(sample sample.cpp sample2.cpp)
set_property(TARGET sample PROPERTY CXX_STANDARD 20)
target_include_directories(sample PUBLIC ${PROJECT_SOURCE_DIR}/include)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-char8_t")
endif ()

