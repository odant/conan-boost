/*
 * Boost Conan package
 * Test smart pointer hooks
 * Dmitriy Vetutnev, ODANT 2021
*/


#ifdef NDEBUG
#undef NDEBUG
#endif


#include <boost/make_shared.hpp>
#include <boost/smart_ptr/local_shared_ptr.hpp>

#include <cassert>
#include <cstdlib>


int main(int, char**) {
    {
        auto p = boost::make_shared<int>(43);
        assert(p);
        assert(*p == 43);
    }
    {
        boost::shared_ptr<int> p{new int(42)};
        assert(p);
        assert(*p == 42);
    }
    {
        boost::local_shared_ptr<int> p{new int(41)};
        assert(p);
        assert(*p == 41);
    }

    return EXIT_SUCCESS;
}


#ifdef BOOST_SP_ENABLE_DEBUG_HOOKS
//#include <boost/smart_ptr/extras/src/sp_debug_hooks.cpp>
#endif
