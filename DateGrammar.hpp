#pragma once
#include <boost/spirit/include/qi.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"

namespace DateGrammarNamespace { BOOST_SPIRIT_TERMINAL(date_) }

namespace boost { namespace spirit
    {
    template <>
    struct use_terminal<qi::domain, DateGrammarNamespace::tag::date_>
            : mpl::true_
    {};
    }}

namespace DateGrammarNamespace {
struct DateGrammar
      : boost::spirit::qi::primitive_parser<DateGrammar>
    {
        template <typename Context, typename Iterator>
        struct attribute
        {
            typedef boost::gregorian::date type;
        };

        template <typename Iterator, typename Context
          , typename Skipper, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context&, Skipper const& skipper, Attribute& attr) const
        {
            boost::spirit::qi::skip_over(first, last, skipper);
            std::string toParseFrom(first, last);
            auto d = boost::gregorian::from_simple_string(toParseFrom);
            boost::spirit::traits::assign_to(d, attr);
            return true;
        }

        template <typename Context>
        boost::spirit::info what(Context&) const
        {
            return boost::spirit::info("Date failed to parse");
        }
    };
}
