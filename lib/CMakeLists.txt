# Find all source and header files
file(GLOB_RECURSE lib_sources *.cpp)
file(GLOB_RECURSE lib_headers *.hpp)

add_library(bempp SHARED ${lib_sources})

# Add a target to generate API documentation with Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in 
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../doc)
  add_custom_target(doc 
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../doc
    COMMENT "Generate API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

# Install library
install(TARGETS bempp
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

# Install header files
install(FILES ${lib_headers}
  DESTINATION include/bempp)