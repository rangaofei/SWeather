file(GLOB_RECURSE BUILD_FILES LIST_DIRECTORIES TRUE ./build/* )

file(REMOVE_RECURSE ${BUILD_FILES})
message(STATUS "delete build files success")