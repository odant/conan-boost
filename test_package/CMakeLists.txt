# Boost test for Conan package
# Dmitriy Vetutnev, Odant, 2018


project(PackageTest CXX)
cmake_minimum_required(VERSION 3.0)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Threads REQUIRED)
find_package(Boost REQUIRED
    atomic
    chrono
    date_time
    iostreams
    locale
    regex
    system
    thread
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


add_executable(test_boost_date_time test_boost_date_time.cpp)
target_link_libraries(test_boost_date_time Boost::date_time)
set_target_properties(test_boost_date_time
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_date_time
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_date_time
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


add_executable(test_boost_locale_case_conv test_boost_locale_case_conv.cpp)
target_link_libraries(test_boost_locale_case_conv
    Boost::locale
    Boost::system
    Boost::thread
)
set_target_properties(test_boost_locale_case_conv
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_locale_case_conv
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_locale_case_conv
)


add_executable(test_boost_locale_utf_to_utf test_boost_locale_utf_to_utf.cpp)
target_link_libraries(test_boost_locale_utf_to_utf
    Boost::locale
    Boost::system
    Boost::thread
)
set_target_properties(test_boost_locale_utf_to_utf
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_locale_utf_to_utf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_locale_utf_to_utf
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
