// Copyright Louis Dionne 2013-2022
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

<% if input_size > 10 %>
    #define FUSION_MAX_VECTOR_SIZE <%= ((input_size + 9) / 10) * 10 %>
<% end %>

#include <boost/fusion/include/as_vector.hpp>
#include <boost/fusion/include/make_vector.hpp>
#include <boost/fusion/include/transform.hpp>
namespace fusion = boost::fusion;


struct f {
    template <typename X>
    constexpr X operator()(X x) const { return x; }
};

template <int i>
struct x { };

int main() {
    auto xs = fusion::make_vector(
        <%= (1..input_size).map { |n| "x<#{n}>{}" }.join(', ') %>
    );

    auto result = fusion::as_vector(fusion::transform(xs, f{}));
    (void)result;
}
