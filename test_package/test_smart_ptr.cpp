/*
 * Boost Conan package
 * Test smart pointer hooks
 * Dmitriy Vetutnev, ODANT 2021
*/


#ifdef NDEBUG
#undef NDEBUG
#endif


#include <boost/shared_ptr.hpp>

#include <cassert>
#include <cstdlib>


int main(int, char**) {
    boost::shared_ptr<int> p{new int(42)};

    assert(p);
    assert(*p == 42);

    return EXIT_SUCCESS;
}


#ifdef BOOST_SP_ENABLE_DEBUG_HOOKS
#include <boost/smart_ptr/extras/src/sp_debug_hooks.cpp>
#endif
