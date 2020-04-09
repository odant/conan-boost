# Boost test for Conan package
# Dmitriy Vetutnev, ODANT, 2018-2020


project(PackageTest CXX)
cmake_minimum_required(VERSION 3.0)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(Threads REQUIRED)
find_package(Boost REQUIRED
    atomic
    chrono
    container
    date_time
    filesystem
    iostreams
    locale
    log
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


add_executable(test_boost_container test_boost_container.cpp)
target_link_libraries(test_boost_container Boost::container)
set_target_properties(test_boost_container
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_container
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_container
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


add_executable(test_boost_locale_numbers test_boost_locale_numbers.cpp)
target_link_libraries(test_boost_locale_numbers
    Boost::locale
    Boost::system
    Boost::thread
)
set_target_properties(test_boost_locale_numbers
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_locale_numbers
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_locale_numbers
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

if(NOT DEFINED ENV{APPVEYOR})
add_executable(test_boost_locale_to_utf_from_utf test_boost_locale_to_utf_from_utf.cpp)
target_link_libraries(test_boost_locale_to_utf_from_utf
    Boost::locale
    Boost::system
    Boost::thread
)
set_target_properties(test_boost_locale_to_utf_from_utf
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_boost_locale_to_utf_from_utf
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_locale_to_utf_from_utf
)
endif() # Not Appveyor


add_executable(test_boost_log test_boost_log.cpp)
target_link_libraries(test_boost_log Boost::log)
set_target_properties(test_boost_log
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
message(STATUS "CONAN_LINK_RUNTIME: ${CONAN_LINK_RUNTIME}")
if(MSVC AND ("${CONAN_LINK_RUNTIME}" STREQUAL "/MD" OR "${CONAN_LINK_RUNTIME}" STREQUAL "/MDd"))
    message(STATUS "Build facets Id")
    add_library(facets_id SHARED facets_id.cpp)
    target_link_libraries(test_boost_log facets_id)
endif()
add_test(
    NAME test_boost_log
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_boost_log
)
if(UNIX)
    set_property(TEST test_boost_log PROPERTY ENVIRONMENT LD_DEBUG=libs LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/bin)
endif()

if(UNIX)
    add_executable(test_boost_process_leak_child test_boost_process_leak_child.cpp)
    target_link_libraries(test_boost_process_leak_child
        Boost::filesystem
        Threads::Threads
    )
    set_target_properties(test_boost_process_leak_child
        PROPERTIES
        INSTALL_RPATH "$ORIGIN"
        BUILD_WITH_INSTALL_RPATH True
    )
    add_test(
        NAME test_boost_process_leak_child
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        COMMAND test_boost_process_leak_child
    )
    set_property(TEST test_boost_process_leak_child PROPERTY ENVIRONMENT LC_ALL=C)
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

