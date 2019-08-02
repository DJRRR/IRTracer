set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-rtti")
include_directories(include)
aux_source_directory(src SOURCES)
add_library(tracer_pass SHARED ${SOURCES})
install(TARGETS tracer_pass LIBRARY DESTINATION lib)

# remove name prefix in order not to generate libxxxxxx name
set_target_properties(tracer_pass PROPERTIES PREFIX "")

