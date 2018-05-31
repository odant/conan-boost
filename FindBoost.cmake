# Conan wrapper for FindBoost.cmake
# Dmitriy Vetutnev 2018
# ODANT 2018


set(BOOST_ROOT ${CONAN_BOOST_ROOT})
set(BOOST_INCLUDEDIR ${CONAN_INCLUDE_DIRS_BOOST})
set(BOOST_LIBRARYDIR ${CONAN_LIB_DIRS_BOOST})
set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_SYSTEM_PATHS ON)

set(Boost_DEBUG ON)
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
if(TARGET Boost::iostreams)

    find_dependency(ICU)

    set_property(TARGET Boost::locale APPEND PROPERTY
        INTERFACE_LINK_LIBRARIES ICU::in
    )

endif()
