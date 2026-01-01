add_executable(writer
  writer.cpp
)

target_include_directories(writer
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(writer
  PRIVATE
    Boost::boost
    pthread
    rt
)
