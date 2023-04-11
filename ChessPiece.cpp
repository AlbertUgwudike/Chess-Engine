#include "ChessPiece.h"

#include <initializer_list>
#include <iostream>

#include "ChessBoard.h"
#include "Position.h"

ChessPiece::ChessPiece(const char *postr, Color color)
    : pos(Position(postr)), col(color), mvCnt(0){};

Color ChessPiece::color() const { return col; }
Position ChessPiece::position() const { return pos; }
Type ChessPiece::type() const { return typ; }
void ChessPiece::setType(Type t) { typ = t; }
int ChessPiece::moveCount() const { return mvCnt; }
void ChessPiece::incrementMoveCount() { mvCnt++; }
void ChessPiece::decrementMoveCount() { mvCnt--; }
void ChessPiece::setPosition(Position position) { pos = position; }

void ChessPiece::reportInvalidMove(Position position) {
    std::cout << this << " cannot move to position " << position.str()
              << std::endl;
}

const char *ChessPiece::str() const {
    char *s = new char[3];
    s[0] = color() ? 'W' : 'B';
    s[2] = '\0';
    switch (type()) {
        case tPawn:
            s[1] = 'P';
            break;
        case tRook:
            s[1] = 'R';
            break;
        case tKnight:
            s[1] = 'N';
            break;
        case tBishop:
            s[1] = 'B';
            break;
        case tKing:
            s[1] = 'K';
            break;
        case tQueen:
            s[1] = 'Q';
            break;
    }
    return s;
}

std::ostream &operator<<(std::ostream &os, ChessPiece *p) {
    return os << (p ? p->str() : "__");
}

Rook::Rook(const char *postr, Color color) : ChessPiece(postr, color) {
    setType(tRook);
}

bool Rook::verifyMove(const ChessBoard *board, Position destination) const {
    if (destination == position()) return false;

    int rankDiff = position().rank() - destination.rank();
    int fileDiff = position().file() - destination.file();

    if (rankDiff == 0) {
        Move move = basicMoves[fileDiff > 0 ? L : R];
        Position next = position().go(move);
        for (Position pos = next; pos != destination; pos = pos.go(move))
            if (board->getPiece(pos)) return false;
    }

    else if (fileDiff == 0) {
        Move move = basicMoves[rankDiff > 0 ? U : D];
        Position next = position().go(move);
        for (Position pos = next; pos != destination; pos = pos.go(move))
            if (board->getPiece(pos)) return false;
    }

    ChessPiece *piece = board->getPiece(destination);
    if (piece && piece->color() == color()) return false;

    return board->checkMove(position(), destination, color());
}

bool Rook::canMove(const ChessBoard *board) const {
    // attempt four possible rook moves

    for (Move move :
         {basicMoves[U], basicMoves[D], basicMoves[L], basicMoves[R]}) {
        if (!position().canGo(move)) continue;
        if (verifyMove(board, position().go(move))) return true;
    }

    return false;
}

Knight::Knight(const char *postr, Color color) : ChessPiece(postr, color) {
    setType(tKnight);
}

bool Knight::verifyMove(const ChessBoard *board, Position destination) const {
    if (position() == destination) return false;

    const int rankDiff = abs(position().rank() - destination.rank());
    const int fileDiff = abs(position().file() - destination.file());

    if (rankDiff * fileDiff != 2) return false;

    ChessPiece *piece = board->getPiece(destination);
    if (piece && piece->color() == color()) return false;

    return board->checkMove(position(), destination, color());
}

bool Knight::canMove(const ChessBoard *board) const {
    for (Move move : knightMoves) {
        if (!position().canGo(move)) continue;
        if (verifyMove(board, position().go(move))) return true;
    }

    return false;
}

Bishop::Bishop(const char *postr, Color color) : ChessPiece(postr, color) {
    setType(tBishop);
}

bool Bishop::verifyMove(const ChessBoard *board, Position destination) const {
    if (position() == destination) return false;

    const int fileDiff = position().file() - destination.file();
    const int rankDiff = position().rank() - destination.rank();

    if (abs(fileDiff) != abs(rankDiff)) return false;

    int fileIdx = 0.5 + fileDiff / (2.0 * abs(fileDiff));
    int rankIdx = 0.5 + rankDiff / (2.0 * abs(rankDiff));

    const int directions[2][2] = {{DR, DL}, {UR, UL}};
    const int direction = directions[rankIdx][fileIdx];

    Position next = position().go(basicMoves[direction]);
    for (Position pos = next; pos != destination;
         pos = pos.go(basicMoves[direction]))
        if (board->getPiece(pos)) return false;

    ChessPiece *piece = board->getPiece(destination);
    if (piece && piece->color() == color()) return false;

    return board->checkMove(position(), destination, color());
}

bool Bishop::canMove(const ChessBoard *board) const {
    // attempt four possible rook moves
    for (Move move :
         {basicMoves[UL], basicMoves[DL], basicMoves[DL], basicMoves[DR]}) {
        if (!position().canGo(move)) continue;
        if (verifyMove(board, position().go(move))) return true;
    }

    return false;
}

Queen::Queen(const char *postr, Color color) : ChessPiece(postr, color) {
    setType(tQueen);
}

bool Queen::verifyMove(const ChessBoard *board, Position destination) const {
    return Rook(position().str(), color()).verifyMove(board, destination) ||
           Bishop(position().str(), color()).verifyMove(board, destination);
}

bool Queen::canMove(const ChessBoard *board) const {
    return Rook(position().str(), color()).canMove(board) ||
           Bishop(position().str(), color()).canMove(board);
}

King::King(const char *postr, Color color) : ChessPiece(postr, color) {
    setType(tKing);
}

bool King::verifyMove(const ChessBoard *board, Position destination) const {
    if (position() == destination) return false;

    int rankDiff = position().rank() - destination.rank();
    int fileDiff = position().file() - destination.file();

    // attempting castle
    if (moveCount() == 0 && rankDiff == 0 && abs(fileDiff) == 2) {
        // get corresponding rook
        ChessPiece *rook = NULL;
        if (fileDiff > 0)
            rook = board->getPiece(Position(board->activeColor ? "A1" : "A8"));
        else
            rook = board->getPiece(Position(board->activeColor ? "H1" : "H8"));

        if (!rook) return false;
        if (rook->moveCount() != 0) return false;

        // check king and spaces between are unmarked and empty
        const Move move = basicMoves[fileDiff > 0 ? L : R];
        for (Position pos = position(); pos != rook->position();
             pos = pos.go(move)) {
            ChessPiece *piece = board->getPiece(pos);

            if (piece && (piece->type() == tRook || piece->type() == tKing))
                continue;

            if (piece || board->isMarkedBy(pos, ChessBoard::opposite(color())))
                return false;
        }

        return true;
        // return board->checkMove(position(), destination, color());
    }

    if (abs(fileDiff) > 1 || abs(rankDiff) > 1) return false;

    ChessPiece *piece = board->getPiece(destination);
    if (piece && piece->color() == color()) return false;

    return board->checkMove(position(), destination, color());
}

bool King::canMove(const ChessBoard *board) const {
    for (Move move : basicMoves) {
        if (!position().canGo(move)) continue;
        if (verifyMove(board, position().go(move))) return true;
    }
    return false;
}

Pawn::Pawn(const char *postr, Color color) : ChessPiece(postr, color) {
    setType(tPawn);
}

bool Pawn::verifyMove(const ChessBoard *board, Position destination) const {
    if (position() == destination) return false;

    int rankDiff = position().rank() - destination.rank();
    int fileDiff = position().file() - destination.file();

    // never a backwards step!
    if (color() && rankDiff < 0 || !color() && rankDiff > 0) return false;

    // double move
    if (fileDiff == 0 && abs(rankDiff) == 2 && moveCount() == 0) {
        const Move move = basicMoves[rankDiff > 0 ? U : D];
        Position fst = position().go(move);
        Position snd = fst.go(move);
        if (board->getPiece(fst) || board->getPiece(snd)) return false;
    }

    // en passant
    else if (abs(rankDiff * fileDiff) == 1 && !board->getPiece(destination)) {
        Move move = basicMoves[fileDiff > 0 ? L : R];
        ChessPiece *adjacent = board->getPiece(position().go(move));
        if (!(adjacent && adjacent->color() != color() &&
              adjacent->type() == tPawn && adjacent->moveCount() == 1 &&
              adjacent->position().rank() == (color() ? 5 : 4))) {
            return false;
        }
    }

    // capture
    else if (abs(rankDiff * fileDiff) == 1) {
        ChessPiece *piece = board->getPiece(destination);
        if (piece && piece->color() == color()) return false;
    }

    // normal move
    else if (abs(rankDiff) > 1 || fileDiff != 0) {
        if (board->getPiece(destination)) return false;
    }

    return board->checkMove(position(), destination, color());
}

bool Pawn::canMove(const ChessBoard *board) const {
    Move move = basicMoves[board->activeColor ? U : D];
    if (!position().canGo(move)) return false;
    if (!position().go(move).canGo(move)) return false;

    return verifyMove(board, position().go(move)) ||
           verifyMove(board, position().go(move).go(move));
}
