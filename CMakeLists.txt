# general settings
cmake_minimum_required(VERSION 3.22)
project(gitm C)

if (NOT DEFINED TOKEN)
  set(TOKEN "") # SET YOUR TOKEN HERE
endif ()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -fno-strict-aliasing -Wall -fdiagnostics-color=always")
if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif ()

# feel free to organize your own project, just assuring that it compiles with this file
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)
add_executable(${PROJECT_NAME} ${SOURCES})

# <openssl/ssl.h> support
find_package(OpenSSL)
if (OPENSSL_FOUND)
  message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")
  include_directories(${OPENSSL_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} crypto)
elseif (WIN32)
  message(STATUS "OpenSSL not found, linking the local one.")
  include_directories(${CMAKE_SOURCE_DIR}/external/include)
  target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/external/lib/libcrypto.a)
else ()
  message(FATAL_ERROR "OpenSSL not found. Please refer to handout on Feishu.")
endif ()

# <math.h> support (unnecessary if you don't use it)
target_link_libraries(${PROJECT_NAME} m)

# if you're able to manipulate git, feel free to comment this and commit by yourself
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND git gc
    COMMAND git add -A --ignore-errors
    COMMAND git commit --allow-empty -m "change the icon.txt"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Committing changes automatically."
)

# do not modify the following code
set(FILE_TO_SUBMIT ${CMAKE_BINARY_DIR}/submission.zip)

add_custom_command(
    OUTPUT ${FILE_TO_SUBMIT}
    COMMAND ${CMAKE_COMMAND} -E remove -f ${FILE_TO_SUBMIT}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/temp
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/.git ${CMAKE_BINARY_DIR}/temp/.git
    COMMAND ${CMAKE_COMMAND} -E tar cf "${FILE_TO_SUBMIT}" --format=zip -- ${CMAKE_BINARY_DIR}/temp/.git
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/temp
    COMMENT "Creating submission archive."
)

add_custom_target(
    submit
    COMMAND curl -m 5 -X POST -F "TOKEN=${TOKEN}" -F "FILE=@${FILE_TO_SUBMIT}" https://oj.cpl.icu/api/v2/submission/lab
    DEPENDS ${FILE_TO_SUBMIT}
    COMMENT "Submitting the project to DotOJ."
)

if (NOT DEFINED TOKEN)
  add_custom_command(
      TARGET submit
      PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --red "Error: TOKEN not found. Please complete it in this file or pass it with flag '-DTOKEN=...' when invoking cmake."
      COMMAND ${CMAKE_COMMAND} -E false
  )
endif ()
