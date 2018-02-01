project(PackageTest CXX)
cmake_minimum_required(VERSION 3.0)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

find_package(Boost REQUIRED COMPONENTS
    system
)

enable_testing()

add_executable(test_fake test_fake.cpp)
set_target_properties(test_fake
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_fake
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_fake
)

add_executable(test_boost_system test_boost_system.cpp)
target_link_libraries(test_boost_system Boost::system)
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
