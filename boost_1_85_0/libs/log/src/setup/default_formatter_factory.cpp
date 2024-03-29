/*
 *          Copyright Andrey Semashev 2007 - 2015.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 */
/*!
 * \file   default_formatter_factory.cpp
 * \author Andrey Semashev
 * \date   14.07.2013
 *
 * \brief  This header is the Boost.Log library implementation, see the library documentation
 *         at http://www.boost.org/doc/libs/release/libs/log/doc/html/index.html.
 */

#if !defined(BOOST_LOG_WITHOUT_SETTINGS_PARSERS) && !defined(BOOST_LOG_WITHOUT_DEFAULT_FACTORIES)

#include <boost/log/detail/setup_config.hpp>
#include <cstddef>
#include <ctime>
#include <boost/core/snprintf.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/vector/vector40.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/date_time/special_defs.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>
#include <boost/date_time/local_time/local_time_types.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/attribute_name.hpp>
#include <boost/log/attributes/value_visitation.hpp>
#include <boost/log/utility/type_dispatch/standard_types.hpp>
#include <boost/log/utility/type_dispatch/date_time_types.hpp>
#include <boost/log/utility/string_literal.hpp>
#include <boost/log/utility/formatting_ostream.hpp>
#include <boost/log/detail/code_conversion.hpp>
#include <boost/log/detail/process_id.hpp>
#if !defined(BOOST_LOG_NO_THREADS)
#include <boost/log/detail/thread_id.hpp>
#endif
#include <boost/log/attributes/named_scope.hpp>
#include "default_formatter_factory.hpp"
#include <boost/log/detail/header.hpp>

namespace boost {

BOOST_LOG_OPEN_NAMESPACE

namespace aux {

#if !defined(BOOST_LOG_NO_THREADS)
#define BOOST_LOG_AUX_THREAD_ID_TYPE() (boost::log::aux::thread::id)
#else
#define BOOST_LOG_AUX_THREAD_ID_TYPE()
#endif

#define BOOST_LOG_AUX_LOG_ATTRIBUTE_VALUE_TYPES()\
    (boost::log::trivial::severity_level)\
    (boost::log::attributes::named_scope_list)\
    (boost::log::aux::process::id)\
    BOOST_LOG_AUX_THREAD_ID_TYPE()

// The list of the attribute value types supported by the default formatter. Note that we have to exclude std::time_t
// as it is an integral type, as well as double from the native time duration types - these are part of arithmetic types already.
#define BOOST_LOG_AUX_LOG_DEFAULT_VALUE_TYPES()\
    BOOST_LOG_DEFAULT_ATTRIBUTE_VALUE_TYPES()\
    (std::tm)\
    BOOST_LOG_BOOST_DATE_TYPES()\
    BOOST_LOG_BOOST_TIME_DURATION_TYPES()\
    BOOST_LOG_BOOST_TIME_PERIOD_TYPES()\
    BOOST_LOG_AUX_LOG_ATTRIBUTE_VALUE_TYPES()

BOOST_LOG_ANONYMOUS_NAMESPACE {

//! The default formatter generated by the default formatter factory
template< typename CharT >
class default_formatter
{
public:
    typedef void result_type;

private:
    //! Attribute value visitor
    struct visitor
    {
        typedef void result_type;

        explicit visitor(basic_formatting_ostream< CharT >& strm) : m_strm(strm)
        {
        }

        template< typename T >
        void operator() (T const& value) const
        {
            m_strm << value;
        }

        void operator() (std::tm const& value) const
        {
            char buf[32];
            std::size_t len = std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &value);
            m_strm.write(buf, len);
        }

        void operator() (boost::posix_time::ptime const& value) const
        {
            if (!value.is_special())
            {
                std::tm t = boost::posix_time::to_tm(value);
                char buf[32];
                std::size_t len = std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
                std::size_t size = sizeof(buf) - len;
                int res = boost::core::snprintf(buf + len, size, ".%.6u", static_cast< unsigned int >(value.time_of_day().total_microseconds() % 1000000));
                if (BOOST_UNLIKELY(res < 0))
                    buf[len] = '\0';
                else if (static_cast< std::size_t >(res) >= size)
                    len += size - 1;
                else
                    len += res;

                m_strm.write(buf, len);
            }
            else
            {
                format_special_date_time(value);
            }
        }

        void operator() (boost::local_time::local_date_time const& value) const
        {
            if (!value.is_special())
            {
                this->operator()(value.local_time());
                m_strm << ' ' << value.zone_as_posix_string();
            }
            else
            {
                format_special_date_time(value);
            }
        }

        void operator() (boost::gregorian::date const& value) const
        {
            if (!value.is_special())
            {
                std::tm t = boost::gregorian::to_tm(value);
                char buf[32];
                std::size_t len = std::strftime(buf, sizeof(buf), "%Y-%m-%d", &t);
                m_strm.write(buf, len);
            }
            else
            {
                format_special_date_time(value.as_special());
            }
        }

        void operator() (boost::posix_time::time_duration const& value) const
        {
            if (!value.is_special())
            {
                boost::posix_time::time_duration val = value;
                if (val.is_negative())
                {
                    m_strm << '-';
                    val = -val;
                }
                unsigned long long total_useconds = value.total_microseconds();
                unsigned long long hours = total_useconds / (3600ull * 1000000ull);
                unsigned int minutes = static_cast< unsigned int >(total_useconds / (60ull * 1000000ull) % 60ull);
                unsigned int seconds = static_cast< unsigned int >(total_useconds / 1000000ull % 60ull);
                unsigned int useconds = static_cast< unsigned int >(total_useconds % 1000000ull);
                char buf[64];
                int len = boost::core::snprintf(buf, sizeof(buf), "%.2llu:%.2u:%.2u.%.6u", hours, minutes, seconds, useconds);
                if (BOOST_LIKELY(len > 0))
                {
                    unsigned int size = static_cast< unsigned int >(len) >= sizeof(buf) ? static_cast< unsigned int >(sizeof(buf)) : static_cast< unsigned int >(len);
                    m_strm.write(buf, size);
                }
            }
            else
            {
                format_special_date_time(value);
            }
        }

        void operator() (boost::gregorian::date_duration const& value) const
        {
            if (!value.is_special())
            {
                m_strm << value.get_rep().as_number();
            }
            else
            {
                format_special_date_time(value.get_rep().as_special());
            }
        }

        template< typename PointRepT, typename DurationRepT >
        void operator() (boost::date_time::period< PointRepT, DurationRepT > const& value) const
        {
            m_strm << '[';
            this->operator()(value.begin());
            m_strm << '/';
            this->operator()(value.last());
            m_strm << ']';
        }

    private:
        template< typename T >
        void format_special_date_time(T const& value) const
        {
            if (value.is_not_a_date_time())
                m_strm << "not-a-date-time";
            else if (value.is_pos_infinity())
                m_strm << "+infinity";
            else if (value.is_neg_infinity())
                m_strm << "-infinity";
        }

        void format_special_date_time(boost::date_time::special_values value) const
        {
            switch (value)
            {
            case boost::date_time::not_a_date_time:
                m_strm << "not-a-date-time";
                break;
            case boost::date_time::pos_infin:
                m_strm << "+infinity";
                break;
            case boost::date_time::neg_infin:
                m_strm << "-infinity";
                break;
            default:
                break;
            }
        }

    private:
        basic_formatting_ostream< CharT >& m_strm;
    };

public:
    explicit default_formatter(attribute_name name) : m_attribute_name(name)
    {
    }

    result_type operator() (record_view const& rec, basic_formatting_ostream< CharT >& strm) const
    {
        typedef BOOST_PP_CAT(mpl::vector, BOOST_PP_SEQ_SIZE(BOOST_LOG_AUX_LOG_DEFAULT_VALUE_TYPES()))<
            BOOST_PP_SEQ_ENUM(BOOST_LOG_AUX_LOG_DEFAULT_VALUE_TYPES())
        > value_types;

        boost::log::visit< value_types >(m_attribute_name, rec, visitor(strm));
    }

private:
    const attribute_name m_attribute_name;
};

} // namespace

//! The callback for equality relation filter
template< typename CharT >
typename default_formatter_factory< CharT >::formatter_type
default_formatter_factory< CharT >::create_formatter(attribute_name const& name, args_map const& args)
{
    // No user-defined factory, shall use the most generic formatter we can ever imagine at this point
    return formatter_type(default_formatter< CharT >(name));
}

//  Explicitly instantiate factory implementation
#ifdef BOOST_LOG_USE_CHAR
template class default_formatter_factory< char >;
#endif
#ifdef BOOST_LOG_USE_WCHAR_T
template class default_formatter_factory< wchar_t >;
#endif

} // namespace aux

BOOST_LOG_CLOSE_NAMESPACE // namespace log

} // namespace boost

#include <boost/log/detail/footer.hpp>

#endif // !defined(BOOST_LOG_WITHOUT_SETTINGS_PARSERS) && !defined(BOOST_LOG_WITHOUT_DEFAULT_FACTORIES)
