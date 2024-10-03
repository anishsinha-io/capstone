cmake_minimum_required(VERSION 3.11) # FetchContent is available in 3.11+
project(roguelike)

set(CMAKE_CXX_STANDARD 17)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RESOURCES_DIR "${CMAKE_SOURCE_DIR}/resources")
set(RESOURCES_OUTPUT_DIR "${CMAKE_BINARY_DIR}/resources")

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${RESOURCES_DIR}" "${RESOURCES_OUTPUT_DIR}"
    COMMENT "Copying resources to the build directory..."
)


# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    # DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
  endif()
endif()

FetchContent_Declare(
  raylib-cpp
  GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
  GIT_TAG        master # or specify a specific tag/commit if needed
)
FetchContent_MakeAvailable(raylib-cpp)

FetchContent_Declare(
  googletest
  DOWNLOAD_EXTRACT_TIMESTAMP OFF
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  # URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Our Project

set(
    SRCS

    src/game.cc
    src/player.cc
    src/gameobject.cc
    src/events.cc
)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${GTEST_INCLUDE_DIRS}/include)

add_executable(${PROJECT_NAME} main.cc ${SRCS})
#set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${raylib-cpp_SOURCE_DIR}/include)

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework CoreVideo")
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
    target_link_libraries(${PROJECT_NAME} "-framework GLUT")
endif()

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running app"
)

enable_testing()

add_executable(
  main_test
  ${CMAKE_SOURCE_DIR}/test/main_test.cc
)
target_link_libraries(
  main_test
  GTest::gtest_main
  gmock_main
)

add_dependencies(${PROJECT_NAME} copy_resources)

include(GoogleTest)
gtest_discover_tests(main_test)
