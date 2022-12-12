chess: ChessMain.o ChessBoard.o Position.o ChessPiece.o
	g++ ChessMain.o ChessBoard.o ChessPiece.o Position.o -o chess

ChessMain.o: ChessBoard.o
	g++ -c ChessMain.cpp

ChessBoard.o: ChessPiece.o Position.o
	g++ -c ChessBoard.cpp

ChessPiece.o: Position.o ChessBoard.o
	g++ -c ChessPiece.cpp

Position.o:
	g++ -c Position.cpp

clean:
	rm -f *.o chess
