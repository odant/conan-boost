# Boost test for Conan package
# Dmitriy Vetutnev, Odant, 2018


project(PackageTest CXX)
cmake_minimum_required(VERSION 3.0)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Boost REQUIRED
    atomic
    chrono
    iostreams
    regex
    system
)


enable_testing()


add_executable(test_boost_atomic test_boost_atomic.cpp)
target_link_libraries(test_boost_atomic Boost::atomic Threads::Threads)
set_target_properties(test_boost_atomic
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_atomic
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_atomic
)


add_executable(test_boost_chrono test_boost_chrono.cpp)
target_link_libraries(test_boost_chrono Boost::chrono)
set_target_properties(test_boost_chrono
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_chrono
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_chrono
)


add_executable(test_boost_iostreams test_boost_iostreams.cpp)
target_link_libraries(test_boost_iostreams Boost::iostreams)
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
target_link_libraries(test_boost_regex Boost::regex)
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

