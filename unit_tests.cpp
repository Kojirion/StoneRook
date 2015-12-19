#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>

#include "boost/date_time/gregorian/gregorian.hpp"

#include <boost/math/special_functions/sign.hpp>

#include "SiteGrammar.hpp"
#include "DateGrammar.hpp"

#include "MoveValidation.hpp"

using boost::spirit::qi::grammar;
using boost::spirit::qi::rule;

using Iterator = std::string::iterator;



BOOST_AUTO_TEST_CASE(EventName)
{
    SiteGrammar parser;

    std::string given("[Site \"Budapest HUN\"]");

    std::string expected("Budapest HUN"), parsed;

    bool success = boost::spirit::qi::parse(given.begin(), given.end(), parser, parsed);

    BOOST_CHECK(success);
    BOOST_CHECK_EQUAL(expected, parsed);
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





BOOST_AUTO_TEST_CASE(BishopMove)
{
    Position position;
    setInitial(position);
    Move move{{0,2},{2,4}};
    BOOST_CHECK(!isBishopLegal(position, move));
}



BOOST_AUTO_TEST_CASE(RookMove)
{
    Position position;
    setInitial(position);
    Move move{{0,0},{5,0}};
    BOOST_CHECK(!isRookLegal(position, move));
}



BOOST_AUTO_TEST_CASE(PawnMove)
{
    Position position;
    setInitial(position);
    Move move{{1,0},{2,0}};
    BOOST_CHECK(isPawnLegal(position, move));
}


