#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include "boost/date_time/gregorian/gregorian.hpp"

using boost::spirit::qi::grammar;
using boost::spirit::qi::rule;

using boost::spirit::qi::lit;

using Iterator = std::string::iterator;

struct SiteGrammar : grammar<Iterator, std::string()>
{
    SiteGrammar(): base_type(start){
        start = lit("[Site \"") >> +(boost::spirit::qi::alnum | boost::spirit::qi::space)  >> lit("\"]");
    }

    rule<Iterator, std::string()> start;
};

BOOST_AUTO_TEST_CASE(EventName)
{
    SiteGrammar parser;

    std::string given("[Site \"Budapest HUN\"]");

    std::string expected("Budapest HUN"), parsed;

    bool success = boost::spirit::qi::parse(given.begin(), given.end(), parser, parsed);

    BOOST_CHECK(success);
    BOOST_CHECK_EQUAL(expected, parsed);
}

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

BOOST_AUTO_TEST_CASE(Date)
{
    DateGrammarNamespace::DateGrammar parser;

    using boost::gregorian::date;

    std::string given("2000.8.16");

    date expected(2000, 8, 16), parsed;

    bool success = boost::spirit::qi::parse(given.begin(), given.end(), parser, parsed);

    BOOST_CHECK(success);
    BOOST_CHECK_EQUAL(expected, parsed);
}


