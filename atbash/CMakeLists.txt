cmake_minimum_required(VERSION 3.14)

project(atbash VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../build/out/debug")
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/../build/out/release")
endif()

find_package(QT NAMES Qt6 Qt5 COMPONENTS
  Core
  Widgets
  LinguistTools
REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS
  Core
  Widgets
  LinguistTools
REQUIRED)

set(TS_FILES ${PROJECT_NAME}_en_US.ts)
add_library(${PROJECT_NAME} SHARED
  atbash.cpp
  ../src/cryptography/latinalphabet.cpp
  ../src/interfaces/idynlibrary.h
  ../src/interfaces/ioptionsable.h
  ../src/cryptography/latinalphabet.h
  ${TS_FILES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../src
)

target_compile_definitions(${PROJECT_NAME} PRIVATE LIBRARY)

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
