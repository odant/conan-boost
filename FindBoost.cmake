# Conan wrapper for FindBoost.cmake
# Dmitriy Vetutnev 2018
# ODANT 2018

cmake_minimum_required(VERSION 3.15)

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

