#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>





#include "SiteGrammar.hpp"
#include "DateGrammar.hpp"

#include "MoveValidation.hpp"

#include <iostream>
#include <numeric>

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
    Move move{{Square::c,1},{Square::e, 3}};
    BOOST_CHECK(!isBishopLegal(position, move));
}



BOOST_AUTO_TEST_CASE(RookMove)
{
    Position position;
    setInitial(position);
    Move move{{Square::a,1},{Square::a,6}};
    BOOST_CHECK(!isRookLegal(position, move));
}



BOOST_AUTO_TEST_CASE(PawnMove)
{
    Position position;
    setInitial(position);
    Move move{{Square::a,2},{Square::a,3}};
    BOOST_CHECK(isPawnLegal(position, move));
}

BOOST_AUTO_TEST_CASE(ParseAmbiguousMove)
{
    Position position;
    //set up position where a knight on f6 is pinned and Nd7 is played
    //it will be necessary to deduce b8-d7 from Nd7
}

BOOST_AUTO_TEST_CASE(FEN)
{
    FENGrammar parser;

    std::string given("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Position expected, parsed;
    setInitial(expected);

    bool success = boost::spirit::qi::parse(given.begin(), given.end(), parser, parsed);

    BOOST_CHECK(success);
    BOOST_CHECK_EQUAL(expected, parsed);

}


