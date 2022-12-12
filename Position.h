#ifndef POSITION_H
#define POSITION_H

typedef const int* Move;

enum { Rank, File };
enum { D, DR, R, UR, U, UL, L, DL };

const int basicMoves[8][2] = {
  { 1, 0 }, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
};

const int knightMoves[8][2] = {
  { 1, 2 }, {-1, 2}, {2, -1}, {2, 1}, {1, -2}, {-1, -2}, {2, 1}, {2, -1}
};

class Position {
private:
  int r; int f;
  char s[3];
  
  bool validPosition(int rank, int file);
  void init(int _rank, int _file);

public:
  
  Position(const char* positionString);
  Position(int _rank, int _file);

  int rank() const;
  int file() const;
  const char *str() const;

  bool canGo(Move move);
  Position go(Move move);

  bool operator==(Position const &pos);
  bool operator!=(Position const &pos);  
};

#endif
