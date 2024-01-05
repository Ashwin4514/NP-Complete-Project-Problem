cmake_minimum_required(VERSION 2.8...3.13)
project(ece650-cpp-examples)

## ECE650 Specific configuration start here
## Project specific configurations go here

# enable c++11 support
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall ${CMAKE_CXX_FLAGS}")

option(WITH_ASAN "Enabled AddressSanitizer to detect memory faults" OFF)
if(WITH_ASAN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address" )
endif()

# create the main executable
## add additional .cpp files if needed

set(CMAKE_CXX_STANDARD 11)  # Adjust the standard as needed
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

option(WITH_ASAN "Enabled AddressSanitizer to detect memory faults" OFF)
if(WITH_ASAN)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

add_executable(rgen rgen.cpp)
add_executable(ece650-a2 ece650-a2.cpp graphStructure.cpp)
add_executable(ece650-a3 ece650-a3.cpp)
add_executable(test test.cpp)

install(TARGETS ece650-a3 rgen ece650-a2 DESTINATION ${CMAKE_BINARY_DIR}/run/bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
install(FILES ece650-a1.py DESTINATION ${CMAKE_BINARY_DIR}/run/bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)


#add_custom_target(install

#    COMMAND python3 ${CMAKE_SOURCE_DIR}/a1ece650.py
#    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#)
