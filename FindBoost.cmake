# Conan wrapper for FindBoost.cmake
# Dmitriy Vetutnev 2018
# ODANT 2018

set(BOOST_ROOT ${CONAN_BOOST_ROOT})
set(BOOST_LIBRARYDIR ${CONAN_LIB_DIRS_BOOST})
set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_SYSTEM_PATHS ON)

include(${CMAKE_CURRENT_LIST_DIR}/_FindBoost.cmake)

