#include <cassert>
#include <cstring>
#include <iostream>
#include "Position.h"

int Position::rank() const { return r; }
int Position::file() const { return f; }
const char *Position::str() const { return s; }

bool Position::validPosition(int _rank, int _file) {
  return _rank >= 0 && _rank < 8 && _file >= 0 && _file < 8;
}

void Position:: init(int _rank, int _file) {
  //    std::cout << _rank << " " << _file << std::endl;
  
  assert(validPosition(_rank, _file));
  r = _rank;
  f = _file;
  s[0] = 'A' + _file;
  s[1] = '8' - _rank;
  s[2] = '\0';
}

Position::Position(const char* positionString) {
  assert(strlen(positionString) == 2);

  //  std::cout << positionString << std::endl;
  const int _rank = '8' - positionString[1];
  const int _file = positionString[0] - 'A';

  init(_rank, _file);
}

Position::Position(int _rank, int _file) {
  init(_rank, _file);
}

bool Position::canGo(Move move) {
  int rank = r + move[Rank];
  int file = f + move[File];
  return validPosition(rank, file);
}
    
Position Position::go(Move move) {
  assert(canGo(move));
  int rank = r + move[Rank];
  int file = f + move[File];
  return Position(rank, file);
}

bool Position::operator==(Position const &pos) {
  return rank() == pos.rank() && file() == pos.file();
}

bool Position::operator!=(Position const &pos) {
  return rank() != pos.rank() || file() != pos.file();
}
