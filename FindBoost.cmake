# Conan wrapper for FindBoost.cmake
# Dmitriy Vetutnev 2018
# ODANT 2018


set(BOOST_ROOT ${CONAN_BOOST_ROOT})
set(BOOST_INCLUDEDIR ${CONAN_INCLUDE_DIRS_BOOST})
set(BOOST_LIBRARYDIR ${CONAN_LIB_DIRS_BOOST})
set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_SYSTEM_PATHS ON)
set(Boost_NO_BOOST_CMAKE ON)
if(CONAN_USER_BOOST_USE_STATIC_RUNTIME)
    set(Boost_USE_STATIC_RUNTIME ON)
endif()
#set(Boost_DEBUG ON)
set(Boost_DETAILED_FAILURE_MSG ON)

if(WIN32)

    if(NOT MSVC_VERSION VERSION_LESS 1400 AND MSVC_VERSION VERSION_LESS 1500)
        set(_TMP_MSVC_TOOLSET_VERSION 80)
    elseif(NOT MSVC_VERSION VERSION_LESS 1500 AND MSVC_VERSION VERSION_LESS 1600)
        set(_TMP_MSVC_TOOLSET_VERSION 90)
    elseif(NOT MSVC_VERSION VERSION_LESS 1600 AND MSVC_VERSION VERSION_LESS 1700)
        set(_TMP_MSVC_TOOLSET_VERSION 100)
    elseif(NOT MSVC_VERSION VERSION_LESS 1700 AND MSVC_VERSION VERSION_LESS 1800)
        set(_TMP_MSVC_TOOLSET_VERSION 110)
    elseif(NOT MSVC_VERSION VERSION_LESS 1800 AND MSVC_VERSION VERSION_LESS 1900)
        set(_TMP_MSVC_TOOLSET_VERSION 120)
    elseif(NOT MSVC_VERSION VERSION_LESS 1900 AND MSVC_VERSION VERSION_LESS 1910)
        set(_TMP_MSVC_TOOLSET_VERSION 140)
    elseif(NOT MSVC_VERSION VERSION_LESS 1910 AND MSVC_VERSION VERSION_LESS 1920)
        set(_TMP_MSVC_TOOLSET_VERSION 141)
    elseif(NOT MSVC_VERSION VERSION_LESS 1920 AND MSVC_VERSION VERSION_LESS 1930)
        set(_TMP_MSVC_TOOLSET_VERSION 142)
    elseif(NOT MSVC_VERSION VERSION_LESS 1930 AND MSVC_VERSION VERSION_LESS 1940)
        set(_TMP_MSVC_TOOLSET_VERSION 143)
    else()
        message(FATAL_ERROR "Conan: Can`t set MSVC_TOOLSET_VERSION, unknown MSVC compiler version [${MSVC_VERSION}]")
    endif()

    # For CMake version < 3.12   - MSVC_TOOLSET_VERSION missing value
    #      3.12 < version < 3.15 - missing toolset 142
    if(NOT MSVC_TOOLSET_VERSION OR NOT MSVC_TOOLSET_VERSION VERSION_EQUAL _TMP_MSVC_TOOLSET_VERSION)
        set(MSVC_TOOLSET_VERSION ${_TMP_MSVC_TOOLSET_VERSION})
    endif()

endif()


include(${CMAKE_CURRENT_LIST_DIR}/_FindBoost.cmake)


# Add defines from package_info
set(Boost_FIND_COMPONENTS boost ${Boost_FIND_COMPONENTS})
foreach(COMPONENT ${Boost_FIND_COMPONENTS})

    if(TARGET Boost::${COMPONENT})
        set_property(TARGET Boost::${COMPONENT} APPEND PROPERTY
            INTERFACE_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_BOOST}
        )
    endif()

endforeach()


include(CMakeFindDependencyMacro)
find_dependency(Threads)

# Add zlib depends
if(TARGET Boost::iostreams)

    find_dependency(ZLIB)

    set_property(TARGET Boost::iostreams APPEND PROPERTY
        INTERFACE_LINK_LIBRARIES ZLIB::ZLIB
    )

endif()

# Add ICU depends
if(CONAN_USER_BOOST_WITH_ICU AND TARGET Boost::locale)

    find_dependency(ICU)

    set_property(TARGET Boost::locale APPEND PROPERTY
        INTERFACE_LINK_LIBRARIES ICU::in
    )

endif()

# Add smart-pointer debug hooks
if(CONAN_USER_BOOST_SP_DEBUG_HOOKS)

    message(STATUS "FindBoost.cmake (wrapper): Enable smart-pointer debug hooks")

    add_library(boost_sp_debug_hooks STATIC ${CONAN_INCLUDE_DIRS_BOOST}/boost/smart_ptr/extras/src/sp_debug_hooks.cpp)
    target_include_directories(boost_sp_debug_hooks PRIVATE ${CONAN_INCLUDE_DIRS_BOOST})
    target_compile_definitions(boost_sp_debug_hooks PRIVATE "BOOST_SP_ENABLE_DEBUG_HOOKS")

    set_property(TARGET Boost::boost APPEND PROPERTY
        INTERFACE_LINK_LIBRARIES boost_sp_debug_hooks
    )
    set_property(TARGET Boost::system APPEND PROPERTY
        INTERFACE_LINK_LIBRARIES boost_sp_debug_hooks
    )

endif()

