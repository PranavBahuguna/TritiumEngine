file(GLOB_RECURSE RENDERING_BENCHMARK_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_executable(RenderingBenchmark ${RENDERING_BENCHMARK_FILES})

target_include_directories(RenderingBenchmark PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(RenderingBenchmark
  PRIVATE TritiumEngine glm glfw libglew_static EnTT freetype)

link_resources(RenderingBenchmark)