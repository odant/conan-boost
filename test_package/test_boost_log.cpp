// Boost test for Conan package
// Dmitriy Vetutnev, ODANT, 2018

#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/core/null_deleter.hpp>

using Backend = boost::log::sinks::text_ostream_backend;
using Sink = boost::log::sinks::synchronous_sink<Backend>;

#include <iostream>
#include <cstdlib>

int main(int, char**) {
    boost::shared_ptr<std::ostream> stream{&std::cout, boost::null_deleter{}};

    auto backend = boost::make_shared<Backend>();
    backend->add_stream(stream);
    backend->auto_flush();

    auto sink = boost::make_shared<Sink>(backend);

    auto core = boost::log::core::get();
    core->add_sink(sink);

    boost::log::sources::logger lg;

    BOOST_LOG(lg) << "Probe Boost::log (char)";
    BOOST_LOG(lg) << u"Probe Boost::log (char16_t)";

    return EXIT_SUCCESS;
}


// Add facets id for MSVC MT runtime
#if defined(_MSC_VER) && !defined(_DLL)

#include <locale>

std::locale::id std::codecvt<char16_t, char, _Mbstatet>::id;
std::locale::id std::codecvt<char32_t, char, _Mbstatet>::id;

#endif
