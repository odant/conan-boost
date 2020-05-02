// Boost test for Conan package
// Dmitriy Vetutnev, Odant, 2020


#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/unordered_set.hpp>

#include <cstdlib>


int main(int, char**) {

    const auto sp = boost::make_shared<int>();
    boost::weak_ptr<int> wp = sp;
    boost::unordered_set<boost::weak_ptr<int>> set;
    set.insert(wp);

    return EXIT_SUCCESS;
}
