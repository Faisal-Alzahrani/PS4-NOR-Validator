cmake_minimum_required(VERSION 3.16)

project(ps4nor LANGUAGES C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
file(GLOB SOURCES "*.h" "*.c")
add_executable(ps4nor ${SOURCES})

include(GNUInstallDirs)
install(TARGETS ps4nor
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
