add_library(utilities request.h request.cpp logger.h logger.cpp)


#target_include_directories(utilities PUBLIC .)
target_link_libraries(utilities libzmqm)