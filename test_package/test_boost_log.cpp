// Boost test for Conan package
// Dmitriy Vetutnev, ODANT, 2018

#ifdef BOOST_LOG_CXX11_CODECVT_FACETS_FORCE_ENABLE
#    ifndef BOOST_LOG_TEST_DISABLE
#        define BOOST_LOG_TEST_DISABLE
#    endif
#endif

#ifndef BOOST_LOG_TEST_DISABLE
#    include <boost/log/common.hpp>
#    include <boost/log/sinks.hpp>
#    include <boost/log/sources/logger.hpp>
#    include <boost/shared_ptr.hpp>
#    include <boost/core/null_deleter.hpp>

using Backend = boost::log::sinks::text_ostream_backend;
using Sink = boost::log::sinks::synchronous_sink<Backend>;
#endif

#include <iostream>
#include <cstdlib>

int main(int, char**) {
#ifndef BOOST_LOG_TEST_DISABLE
    boost::shared_ptr<std::ostream> stream{&std::cout, boost::null_deleter{}};

    auto backend = boost::make_shared<Backend>();
    backend->add_stream(stream);
    backend->auto_flush();

    auto sink = boost::make_shared<Sink>(backend);

    auto core = boost::log::core::get();
    core->add_sink(sink);

    boost::log::sources::logger lg;
    BOOST_LOG(lg) << "Probe Boost::log";
#else
    std::cout << "Boost::log test disable (BOOST_LOG_TEST_DISABLE or BOOST_LOG_CXX11_CODECVT_FACETS_FORCE_ENABLE defined)" << std::endl;
#endif
    return EXIT_SUCCESS;

}