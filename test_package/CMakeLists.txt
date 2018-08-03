# Boost test for Conan package
# Dmitriy Vetutnev, ODANT, 2018


project(PackageTest CXX)
cmake_minimum_required(VERSION 3.0)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Threads REQUIRED)
find_package(Boost REQUIRED
    atomic
    chrono
    date_time
    filesystem
    iostreams
    locale
    random
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
if(UNIX)
    set_property(TEST test_boost_atomic PROPERTY ENVIRONMENT LD_DEBUG=libs LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/bin)
endif()


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
if(UNIX)
    set_property(TEST test_boost_chrono PROPERTY ENVIRONMENT LD_DEBUG=libs LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/bin)
endif()


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
if(UNIX)
    set_property(TEST test_boost_date_time PROPERTY ENVIRONMENT LD_DEBUG=libs LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/bin)
endif()


add_executable(test_boost_filesystem test_boost_filesystem.cpp)
target_link_libraries(test_boost_filesystem Boost::filesystem)
set_target_properties(test_boost_filesystem
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_filesystem
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_filesystem
)
if(UNIX)
    set_property(TEST test_boost_filesystem PROPERTY ENVIRONMENT LD_DEBUG=libs LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/bin)
endif()


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
if(UNIX)
    set_property(TEST test_boost_iostreams PROPERTY ENVIRONMENT LD_DEBUG=libs LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/bin)
endif()


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
if(UNIX)
    set_property(TEST test_boost_locale_case_conv PROPERTY ENVIRONMENT LD_DEBUG=libs LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/bin)
endif()


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
if(UNIX)
    set_property(TEST test_boost_locale_utf_to_utf PROPERTY ENVIRONMENT LD_DEBUG=libs LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/bin)
endif()


add_executable(test_boost_random test_boost_random.cpp)
target_link_libraries(test_boost_random Boost::random)
set_target_properties(test_boost_random
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_random
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_random
)
if(UNIX)
    set_property(TEST test_boost_random PROPERTY ENVIRONMENT LD_DEBUG=libs LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/bin)
endif()


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
if(UNIX)
    set_property(TEST test_boost_regex PROPERTY ENVIRONMENT LD_DEBUG=libs LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/bin)
endif()


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
if(UNIX)
    set_property(TEST test_boost_system PROPERTY ENVIRONMENT LD_DEBUG=libs LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/bin)
endif()

