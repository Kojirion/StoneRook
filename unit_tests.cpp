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

enum class Color{
    None,
    White,
    Black
};



struct Piece{
    enum class Type{
        Rook,
        Knight,
        Bishop,
        Queen,
        King,
        Pawn,
        Shadow,
        None
    };

    Color color;
    Type type;
};

struct Square{
    int row, col;
};

struct Move{
    Square square_1, square_2;
};

struct Position{
    std::array<Piece, 64> m_cells;

    const Piece& operator ()(const Square& square) const
    {
        assert((square.row>=0)&&(square.row<8)&&
                         (square.col>=0)&&(square.col<8));

        return m_cells[square.row*8+square.col];
    }
};

void setInitial(Position& position){
    position.m_cells[0] = {Color::White, Piece::Type::Rook};
    position.m_cells[1] = {Color::White, Piece::Type::Knight};
    position.m_cells[2] = {Color::White, Piece::Type::Bishop};
    position.m_cells[3] = {Color::White, Piece::Type::Queen};
    position.m_cells[4] = {Color::White, Piece::Type::King};
    position.m_cells[5] = {Color::White, Piece::Type::Bishop};
    position.m_cells[6] = {Color::White, Piece::Type::Knight};
    position.m_cells[7] = {Color::White, Piece::Type::Rook};
    position.m_cells[7*8+0] = {Color::Black, Piece::Type::Rook};
    position.m_cells[7*8+1] = {Color::Black, Piece::Type::Knight};
    position.m_cells[7*8+2] = {Color::Black, Piece::Type::Bishop};
    position.m_cells[7*8+3] = {Color::Black, Piece::Type::Queen};
    position.m_cells[7*8+4] = {Color::Black, Piece::Type::King};
    position.m_cells[7*8+5] = {Color::Black, Piece::Type::Bishop};
    position.m_cells[7*8+6] = {Color::Black, Piece::Type::Knight};
    position.m_cells[7*8+7] = {Color::Black, Piece::Type::Rook};

    for (int i=0; i<8; ++i){
        position.m_cells[1*8+i] = {Color::White, Piece::Type::Pawn};
        position.m_cells[6*8+i] = {Color::Black, Piece::Type::Pawn};
        for (int j=2; j<6; ++j){
            position.m_cells[j*8+i] = {Color::None, Piece::Type::None};
        }
    }
}

bool isPiece(const Piece& piece)
{
    return (piece.type != Piece::Type::None) && (piece.type != Piece::Type::Shadow);
}



bool isBishopLegal(const Position& position, const Move& move){
    const int deltaRow = move.square_2.row - move.square_1.row;
    const int deltaCol = move.square_2.col - move.square_1.col;

    //if not in same diagonal, do not proceed
    if (std::abs(deltaRow) != std::abs(deltaCol))
        return false;

    using boost::math::sign;

    const int signRowDiff = sign(deltaRow);
    const int signColDiff = sign(deltaCol);

    for (int i=move.square_1.row+signRowDiff; i!=move.square_2.row; i+=signRowDiff){
        int j = move.square_1.col + (i-move.square_1.row)*signColDiff*signRowDiff;
        if (isPiece(position({i, j})))
            return false;
    }
    return true;
}

BOOST_AUTO_TEST_CASE(BishopMove)
{
    Position position;
    setInitial(position);
    Move move{{0,2},{2,4}};
    BOOST_CHECK(!isBishopLegal(position, move));
}


