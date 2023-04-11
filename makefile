chess: ChessMain.o ChessBoard.o Position.o ChessPiece.o
	g++ -std=c++11 ChessMain.o ChessBoard.o ChessPiece.o Position.o -o chess
	make tidy

ChessMain.o: ChessBoard.o
	g++ -std=c++11 -c ChessMain.cpp

ChessBoard.o: ChessPiece.o Position.o
	g++ -std=c++11 -c ChessBoard.cpp

ChessPiece.o: Position.o ChessBoard.o
	g++ -std=c++11 -c ChessPiece.cpp

Position.o:
	g++ -std=c++11 -c Position.cpp

tidy:
	rm -f *.o
	
clean:
	rm -f *.o chess
