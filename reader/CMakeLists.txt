add_executable(reader
  reader.cpp
)

target_include_directories(reader
  PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(reader
  PRIVATE
    Boost::boost
    pthread
    rt
)
