//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

// Test that header file is self-contained.
#include <boost/beast/core/ostream.hpp>

#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/beast/core/flat_buffer.hpp>
#include <boost/beast/core/flat_static_buffer.hpp>
#include <boost/beast/core/multi_buffer.hpp>
#include <boost/beast/core/string.hpp>
#include <boost/beast/_experimental/unit_test/suite.hpp>
#include <ostream>

namespace boost {
namespace beast {

class ostream_test : public beast::unit_test::suite
{
public:
    void
    testOstream()
    {
        string_view const s = "0123456789abcdef";
        BEAST_EXPECT(s.size() == 16);

        // overflow
        {
            flat_static_buffer<16> b;
            ostream(b) << s;
            BEAST_EXPECT(buffers_to_string(b.data()) == s);
        }

        // max_size
        {
            flat_static_buffer<16> b;
            auto os = ostream(b);
            os << s;
            os << '*';
            BEAST_EXPECT(os.bad());
        }

        // max_size (exception
        {
            flat_static_buffer<16> b;
            auto os = ostream(b);
            os.exceptions(os.badbit);
            os << s;
            try
            {
                os << '*';
                fail("missing exception", __FILE__, __LINE__);
            }
            // We used to catch std::ios_base::failure which is strictly correct
            // however there is a rare historic ABI bug when stdlibc++ and the
            // program are compiled with different ABIs.
            // see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66145
            catch(std::exception&)
            {
                pass();
            }
            catch(...)
            {
                fail("wrong exception", __FILE__, __LINE__);
            }
        }

        // flush
        {
            // Issue #1853
            flat_static_buffer<16> b;
            auto half_view = string_view(s.data(), 8);
            {
                auto os = ostream(b);
                os << half_view;
                os.flush();
            }
            BEAST_EXPECT(buffers_to_string(b.data()) == half_view);
        }

        {
            flat_static_buffer<16> b;
            {
                auto os = ostream(b);
                os << string_view(s.data(), 8);
                os.flush();
                os << string_view(s.data() + 8, 8);
                os.flush();
            }
            BEAST_EXPECT(buffers_to_string(b.data()) == s);
        }
    }

    void
    run() override
    {
        testOstream();
    }
};

BEAST_DEFINE_TESTSUITE(beast,core,ostream);

} // beast
} // boost
