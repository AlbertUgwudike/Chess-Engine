#ifndef PIECE_H
#define PIECE_H

#include <iostream>

#include "Position.h"

class ChessBoard;

enum Color : int { Black, White };
enum Type { tPawn, tRook, tKnight, tBishop, tKing, tQueen };

class ChessPiece {
    friend std::ostream& operator<<(std::ostream& os, ChessPiece* p);
    friend class ChessBoard;

   private:
    Color col;
    Position pos;
    Type typ;
    int mvCnt;

   public:
    // initialise a piece by position
    ChessPiece(const char*, Color);
    virtual ~ChessPiece() = default;
    Color color() const;
    Position position() const;
    Type type() const;
    void setType(Type);
    const char* str() const;
    int moveCount() const;
    void setPosition(Position);
    void incrementMoveCount();
    void decrementMoveCount();
    void reportInvalidMove(Position);

   protected:
    virtual bool verifyMove(const ChessBoard*, Position) const = 0;
    virtual bool canMove(const ChessBoard*) const = 0;
};

class Rook : public ChessPiece {
   public:
    Rook(const char*, Color);
    bool verifyMove(const ChessBoard*, Position) const override;
    bool canMove(const ChessBoard*) const override;
};

class Knight : public ChessPiece {
   public:
    Knight(const char*, Color);
    bool verifyMove(const ChessBoard*, Position) const override;
    bool canMove(const ChessBoard*) const override;
};

class Bishop : public ChessPiece {
   public:
    Bishop(const char*, Color);
    bool verifyMove(const ChessBoard*, Position) const override;
    bool canMove(const ChessBoard*) const override;
};

class Queen : public ChessPiece {
   public:
    Queen(const char*, Color);
    bool verifyMove(const ChessBoard*, Position) const override;
    bool canMove(const ChessBoard*) const override;
};

class King : public ChessPiece {
   public:
    King(const char*, Color);
    bool verifyMove(const ChessBoard*, Position) const override;
    bool canMove(const ChessBoard*) const override;
};

class Pawn : public ChessPiece {
   public:
    Pawn(const char*, Color);
    bool verifyMove(const ChessBoard*, Position) const override;
    bool canMove(const ChessBoard*) const override;
};

#endif
