// Boost test for Conan package
// Dmitriy Vetutnev, ODANT, 2018


#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/core/null_deleter.hpp>

#include <iostream>
#include <cstdlib>


using Backend = boost::log::sinks::text_ostream_backend;
using Sink = boost::log::sinks::synchronous_sink<Backend>;

int main(int, char**) {

    boost::shared_ptr<std::ostream> stream{&std::cout, boost::null_deleter{}};

    auto backend = boost::make_shared<Backend>();
    backend->add_stream(stream);
    backend->auto_flush();

    auto sink = boost::make_shared<Sink>(backend);

    auto core = boost::log::core::get();
    core->add_sink(sink);

    boost::log::sources::logger lg;
    BOOST_LOG(lg) << "Probe Boost::log";

    return EXIT_SUCCESS;
}