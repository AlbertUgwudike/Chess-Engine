#ifndef BOARD_H
#define BOARD_H

#include "Position.h"

class ChessPiece;
enum Color : int;

class ChessBoard {
   public:
    Color activeColor;

    ChessBoard();
    void submitMove(const char *, const char *);
    void submitMove(const char *);
    ChessPiece *getPiece(Position) const;
    bool checkMove(Position, Position, Color) const;
    bool isMarkedBy(Position, Color) const;
    bool isInCheck(Color) const;
    bool isInStalemate(Color) const;
    bool isInCheckmate(Color) const;
    void resetBoard();
    void printBoard();
    static Color opposite(Color);

   private:
    mutable ChessPiece *board[8][8];
    ChessPiece *king[2];

    ChessPiece *move(Position, Position) const;
    void movePiece(ChessPiece *, Position) const;
    void place(ChessPiece *);
    void initialiseBoard();
    void deletePieces();
};

#endif
