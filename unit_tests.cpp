#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

using boost::spirit::qi::grammar;
using boost::spirit::qi::rule;

using boost::spirit::qi::lit;

using Iterator = std::string::iterator;

struct EventParser : grammar<Iterator, std::string()>
{
    EventParser(): base_type(start){
        start = lit("[Site \"") >> +(boost::spirit::qi::alnum | boost::spirit::qi::space)  >> lit("\"]");
    }

    rule<Iterator, std::string()> start;
};

BOOST_AUTO_TEST_CASE(EventName)
{
    EventParser parser;

    std::string given("[Site \"Budapest HUN\"]");

    std::string expected("Budapest HUN"), parsed;

    bool success = boost::spirit::qi::parse(given.begin(), given.end(), parser, parsed);

    BOOST_CHECK(success);
    BOOST_CHECK_EQUAL(expected, parsed);
}
