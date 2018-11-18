cmake_minimum_required(VERSION 3.12)
project(BreakingOut)

set(CMAKE_CXX_STANDARD 14)

add_executable(BreakingOut main.cpp Ball.cpp Ball.h Block.cpp Block.h UserName.cpp UserName.h)

set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc")
set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_ROOT "C:/Users/stalk/Desktop/SFML-2.5.1")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -IC:/Users/stalk/Desktop/SFML-2.5.1/include -LC:/Users/stalk/Desktop/SFML-2.5.1/lib")
set(SOURCE_FILES main.cpp)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake_modules)
find_package(SFML 2 REQUIRED audio network graphics window system)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

endif ()
target_link_libraries(BreakingOut sfml-audio sfml-network sfml-graphics sfml-window sfml-system)