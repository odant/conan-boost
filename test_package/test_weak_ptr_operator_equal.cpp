// Boost test for Conan package
// Dmitriy Vetutnev, Odant, 2020


#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/unordered_set.hpp>

#include <cstdlib>
#include <cassert>
#include <unordered_set>

int main(int, char**) {

    const auto sp1 = boost::make_shared<int>();
    const auto sp2 = boost::make_shared<int>();
    boost::weak_ptr<int> wp1_1 = sp1;
    boost::weak_ptr<int> wp1_2 = sp1;
    boost::weak_ptr<int> wp2 = sp2;
    
    const auto res1 = wp1_1 == wp1_2;
    assert(res1);
    const auto res2 = wp1_2 == wp1_1;
    assert(res2);

    const auto res3 = wp2 != wp1_1;
    assert(res3);
    const auto res4 = wp1_1 != wp2;
    assert(res4);
    
    std::unordered_set<boost::weak_ptr<int>> set1;
    set1.emplace(wp1_1);
    set1.emplace(wp2);
    std::unordered_set<boost::weak_ptr<int>> set2;
    set2.emplace(wp2);
    set2.emplace(wp1_2);
    assert(set1 == set2);

    return EXIT_SUCCESS;
}
