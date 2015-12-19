#pragma once
#include "Common.hpp"

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

bool isRookLegal(const Position& position, const Move& move)
{
    using boost::math::sign;

    if (move.square_1.row==move.square_2.row){
        const int signDiff = sign(move.square_2.col - move.square_1.col);
        const int maxIt = move.square_2.col - move.square_1.col;

        for (int i=signDiff; i!=maxIt; i+=signDiff){
            if (isPiece(position({move.square_1.row, move.square_1.col+i})))
                return false;
        }
        return true; //no obstruction
    }else if (move.square_1.col==move.square_2.col){
        const int signDiff = sign(move.square_2.row - move.square_1.row);
        const int maxIt = move.square_2.row - move.square_1.row;

        for (int i=signDiff; i!=maxIt; i+=signDiff){
            if (isPiece(position({move.square_1.row+i, move.square_1.col})))
                return false;
        }
        return true; //no obstruction
    }
    return false; //not in same row nor column
}

bool isPawnLegal(const Position& position, const Move& move)
{
    Piece noPiece{Color::None, Piece::Type::None};
    auto piece = position(move.square_1);

    if (position(move.square_1).color == Color::White){ //white pawn
        if ((move.square_1.row==1)&&(move.square_2.row==3)&&(move.square_1.col==move.square_2.col)){ //double advance
            if ((position(move.square_2) == noPiece) && (position({2, move.square_2.col}) == noPiece))
                return true;
        }else{
            if (move.square_1.row+1==move.square_2.row){
                if (move.square_1.col==move.square_2.col)
                    if (position(move.square_2) == noPiece)
                        return true;
                if ((move.square_1.col+1==move.square_2.col)||(move.square_1.col-1==move.square_2.col))
                    if (piece.color == !position(move.square_2).color)
                        return true;
            }
        }
    }else{
        if ((move.square_1.row==6)&&(move.square_2.row==4)&&(move.square_1.col==move.square_2.col)){ //double advance
            if ((position(move.square_2) == noPiece) && (position({5, move.square_2.col}) == noPiece))
                return true;
        }else{
            if (move.square_1.row-1==move.square_2.row){
                if (move.square_1.col==move.square_2.col)
                    if (position(move.square_2) == noPiece)
                        return true;
                if ((move.square_1.col+1==move.square_2.col)||(move.square_1.col-1==move.square_2.col))
                    if (piece.color == !position(move.square_2).color)
                        return true;
            }
        }
    }
    return false;
}
