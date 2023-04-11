#include "ChessBoard.h"

#include <cassert>
#include <cstring>
#include <iostream>

#include "ChessPiece.h"
#include "Position.h"

ChessBoard::ChessBoard() : activeColor(White) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) movePiece(NULL, Position(i, j));

    initialiseBoard();
}

void ChessBoard::printBoard() {
    for (int i = 0; i < 8; i++) {
        std::cout << 8 - i << "  ";
        for (int j = 0; j < 8; j++) {
            std::cout << getPiece(Position(i, j)) << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n   ";
    for (int i = 0; i < 8; i++) std::cout << static_cast<char>('A' + i) << "  ";
    std::cout << "\n\n" << std::endl;
}
void ChessBoard::movePiece(ChessPiece *piece, Position position) const {
    if (piece) {
        Position origin = piece->position();
        board[origin.rank()][origin.file()] = NULL;
        piece->setPosition(position);
    }

    board[position.rank()][position.file()] = piece;
}

ChessPiece *ChessBoard::getPiece(Position position) const {
    return board[position.rank()][position.file()];
}

bool ChessBoard::checkMove(Position origin, Position destination,
                           Color color) const {
    ChessPiece *originPiece = getPiece(origin);
    ChessPiece *capturedPiece = move(origin, destination);
    bool check = isInCheck(color);
    movePiece(originPiece, origin);
    originPiece->decrementMoveCount();
    if (capturedPiece) movePiece(capturedPiece, capturedPiece->position());
    return !check;
}

ChessPiece *ChessBoard::move(Position origin, Position destination) const {
    ChessPiece *originPiece = getPiece(origin);
    ChessPiece *destinationPiece = getPiece(destination);

    originPiece->incrementMoveCount();
    // must be en passant
    if ((originPiece->type() == tPawn) && !destinationPiece &&
        origin.file() != destination.file()) {
        const int priorRank = destination.rank() + (activeColor ? 1 : -1);
        ChessPiece *capturedPiece =
            getPiece(Position(priorRank, destination.file()));
        movePiece(originPiece, Position(destination));
        movePiece(NULL, capturedPiece->position());
        return capturedPiece;
    }

    // must be castling
    if (originPiece->type() == tKing &&
        abs(origin.file() - destination.file()) == 2) {
        int fileDiff = origin.file() - destination.file();

        // get corresdponding rook
        ChessPiece *rook = NULL;
        if (fileDiff > 0)
            rook = getPiece(Position(activeColor ? "A1" : "A8"));
        else
            rook = getPiece(Position(activeColor ? "H1" : "H8"));

        Move move = basicMoves[fileDiff > 0 ? R : L];
        movePiece(originPiece, destination);
        movePiece(rook, originPiece->position().go(move));
        return NULL;
    }

    ChessPiece *capturedPiece = getPiece(Position(destination));
    movePiece(originPiece, destination);
    return capturedPiece;
}

// for castling
void ChessBoard::submitMove(const char *castleCode) {
    assert(strlen(castleCode) <= 5);

    // right castle
    if (!strcmp(castleCode, "O-O"))
        submitMove(activeColor ? "E1" : "E8", activeColor ? "G1" : "G8");

    // left castle
    else if (!strcmp(castleCode, "O-O-O"))
        submitMove(activeColor ? "E1" : "E8", activeColor ? "C1" : "C8");

    else
        std::cout << "invalid singleton move" << std::endl;
}

void ChessBoard::submitMove(const char *origin, const char *destination) {
    std::cout << origin << " " << destination << std::endl;
    // verify that there is correctly colored piece at origin
    ChessPiece *activePiece = getPiece(Position(origin));

    if (!activePiece) {
        std::cout << "No Piece at requested position" << std::endl;
        return;
    }

    if (activePiece->color() != activeColor) {
        std::cout << "It is not their turn!" << std::endl;
        return;
    }

    // check piece can move to requested position
    bool canMove = activePiece->verifyMove(this, Position(destination));
    if (!canMove) return activePiece->reportInvalidMove(Position(destination));

    ChessPiece *capturedPiece = move(Position(origin), Position(destination));
    if (capturedPiece) delete capturedPiece;

    // end game if opponent is in checkmate
    if (isInCheckmate(activeColor ? Black : White)) {
        std::cout << (activeColor ? "White" : "Black") << " is in checkmate"
                  << std::endl;
        return;
    }

    // end game if opponent is in stalemate
    if (isInStalemate(activeColor ? Black : White)) {
        std::cout << (activeColor ? "White" : "Black") << " is in stalemate"
                  << std::endl;
        return;
    }

    // report check
    if (isInCheck(activeColor ? Black : White))
        std::cout << (activeColor ? "Black" : "White") << " is in check"
                  << std::endl;

    // swtich turns
    activeColor = activeColor ? Black : White;
    printBoard();
}

Color ChessBoard::opposite(Color color) { return color ? Black : White; }

bool ChessBoard::isInCheck(Color color) const {
    return isMarkedBy(king[color]->position(), opposite(color));
}

bool ChessBoard::isMarkedBy(Position position, Color color) const {
    for (Move move : basicMoves) {
        Position pos = position;
        while (pos.canGo(move)) {
            pos = pos.go(move);
            if (!getPiece(pos)) continue;
            if (getPiece(pos)->color() != color) break;
            if (getPiece(pos)->verifyMove(this, position)) return true;
            break;
        }
    }

    for (Move move : knightMoves) {
        if (!position.canGo(move)) continue;
        Position pos = position.go(move);
        if (!getPiece(pos) || getPiece(pos)->color() != color) continue;
        if (getPiece(pos)->verifyMove(this, position)) return true;
    }

    return false;
}

bool ChessBoard::isInStalemate(Color color) const {
    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++) {
            ChessPiece *piece = getPiece(Position(rank, file));
            if (!piece) continue;
            if (piece->color() != color) continue;
            if (piece->canMove(this)) return false;
        }

    return true;
}

bool ChessBoard::isInCheckmate(Color color) const {
    return isInStalemate(color) &&
           isMarkedBy(king[color]->position(), opposite(color));
}

void ChessBoard::place(ChessPiece *piece) {
    assert(piece);
    movePiece(piece, piece->position());
}

void ChessBoard::resetBoard() {
    deletePieces();
    initialiseBoard();
}

void ChessBoard::deletePieces() {
    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++) {
            ChessPiece *piece = getPiece(Position(rank, file));
            if (!piece) continue;
            delete piece;
            movePiece(NULL, Position(rank, file));
        }
}

void ChessBoard::initialiseBoard() {
    place(new Rook("A8", Black));
    place(new Rook("A1", White));
    place(new Rook("H8", Black));
    place(new Rook("H1", White));
    place(new Knight("B8", Black));
    place(new Knight("B1", White));
    place(new Knight("G8", Black));
    place(new Knight("G1", White));
    place(new Bishop("C8", Black));
    place(new Bishop("C1", White));
    place(new Bishop("F8", Black));
    place(new Bishop("F1", White));
    place(new Queen("D8", Black));
    place(new Queen("D1", White));
    place(new King("E8", Black));
    place(new King("E1", White));

    for (int i = 0; i < 8; i++) {
        char pos[2][3] = {{static_cast<char>('A' + i), '2', '\0'},
                          {static_cast<char>('A' + i), '7', '\0'}};

        place(new Pawn(pos[0], White));
        place(new Pawn(pos[1], Black));
    }

    king[Black] = getPiece(Position("E8"));
    king[White] = getPiece(Position("E1"));

    activeColor = White;
}
