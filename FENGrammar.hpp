#pragma once
#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include "Common.hpp"

struct PieceMap : boost::spirit::qi::symbols<char, Piece>
{
    PieceMap(){
        add
                ("r", Piece{Color::Black, Piece::Type::Rook})
                ("n", Piece{Color::Black, Piece::Type::Knight})
                ("b", Piece{Color::Black, Piece::Type::Bishop})
                ("q", Piece{Color::Black, Piece::Type::Queen})
                ("k", Piece{Color::Black, Piece::Type::King})
                ("p", Piece{Color::Black, Piece::Type::Pawn})
                ("R", Piece{Color::White, Piece::Type::Rook})
                ("N", Piece{Color::White, Piece::Type::Knight})
                ("B", Piece{Color::White, Piece::Type::Bishop})
                ("Q", Piece{Color::White, Piece::Type::Queen})
                ("K", Piece{Color::White, Piece::Type::King})
                ("P", Piece{Color::White, Piece::Type::Pawn})
                ("-", Piece{Color::None, Piece::Type::None})
                ;
    }

};

using Iterator = std::string::iterator;

using Row = std::vector<Piece>;

PieceMap pieceMap;

using namespace boost::spirit::qi;

struct FENGrammar : boost::spirit::qi::grammar<Iterator, Row()>
{
    FENGrammar(): base_type(start){
        start = repeat(8)[repeat(8)[pieceMap] >> '/'];
    }

    boost::spirit::qi::rule<Iterator, Row()> start;
};
