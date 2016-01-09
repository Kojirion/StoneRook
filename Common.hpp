#pragma once
#include <ostream>
#include <boost/spirit/include/karma.hpp>

enum class Color{
    None,
    White,
    Black
};

Color operator !(Color rhs)
{
    if (rhs==Color::White)
        return Color::Black;
    else{
        assert(rhs==Color::Black);
        return Color::White;
    }
}


struct Piece{
    enum class Type{
        Rook = 0,
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

bool isPiece(const Piece& piece)
{
    return (piece.type != Piece::Type::None) && (piece.type != Piece::Type::Shadow);
}

bool operator ==(const Piece &lhs, const Piece &rhs)
{
    return (lhs.color == rhs.color) && (lhs.type == rhs.type);
}

bool operator !=(const Piece& lhs, const Piece &rhs)
{
    return !(lhs==rhs);
}



std::ostream& operator<<(std::ostream& s, const Piece& piece){
    if (piece.color == Color::Black)
        s << '*';
    else
        s << ' ';

    static const char typeMap[] = "RNBQKP  ";
    s << typeMap[static_cast<int>(piece.type)];
}

struct Square{
    enum Column{
        a = 0,
        b = 1,
        c = 2,
        d = 3,
        e = 4,
        f = 5,
        g = 6,
        h = 7
    };

    Column column;
    int row;
};

struct Move{
    Square square_1, square_2;
};

struct Position{
    std::array<Piece, 64> m_cells;

    const Piece& operator ()(const Square& square) const
    {
        assert((square.row>0)&&(square.row<=8)&&
                         (square.column>=0)&&(square.column<8));

        return m_cells[(square.row-1)*8+square.column];
    }
};

std::ostream& operator<<(std::ostream& s, const Position& position){
    //copy()
}

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

template <typename OutputIterator>
struct PositionGrammar : boost::spirit::karma::grammar<OutputIterator, std::array<Piece, 64>()>
{    
    PositionGrammar() : PositionGrammar::base_type(start, "PositionGrammar")
    {
        using namespace boost::spirit::karma;
        piece = "| " << stream;
        auto g = lit("|\n|---+---+---+---+---+---+---+---|\n");
        start = "---------------------------------\n" << columns(8,g)[*piece];
    }

    boost::spirit::karma::rule<OutputIterator, std::array<Piece, 64>()> start;
    boost::spirit::karma::rule<OutputIterator, Piece()> piece;
};

void print(const Position& position){
    using Iterator = std::ostream_iterator<char>;

    Iterator sink(std::cout, "");

    PositionGrammar<Iterator> grammar;

    boost::spirit::karma::generate(sink,
                                   grammar,
                                   position.m_cells);
}
