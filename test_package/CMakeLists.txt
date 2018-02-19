project(PackageTest CXX)
cmake_minimum_required(VERSION 3.0)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Boost REQUIRED COMPONENTS
    iostreams
    regex
    system
)
find_package(ZLIB REQUIRED)

enable_testing()

add_executable(test_boost_iostreams test_boost_iostreams.cpp)
target_link_libraries(test_boost_iostreams
    Boost::iostreams
    ZLIB::ZLIB
)
set_target_properties(test_boost_iostreams
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_iostreams
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_iostreams
)

add_executable(test_boost_regex test_boost_regex.cpp)
target_link_libraries(test_boost_regex
    Boost::regex
)
set_target_properties(test_boost_regex
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_regex
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_regex
)

add_executable(test_boost_system test_boost_system.cpp)
target_link_libraries(test_boost_system
    Boost::system
)
set_target_properties(test_boost_system
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_system
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_system
)
