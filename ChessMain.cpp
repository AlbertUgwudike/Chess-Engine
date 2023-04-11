#include <iostream>

#include "ChessBoard.h"

using std::cout;

int main() {
    cout << "========================\n";
    cout << "Testing the Chess Engine\n";
    cout << "========================\n\n";

    ChessBoard cb;
    cout << '\n';

    cb.submitMove("D7", "D6");
    cout << '\n';

    cb.submitMove("D4", "H6");
    cout << '\n';

    cb.submitMove("D2", "D4");
    cout << '\n';

    cb.submitMove("F8", "B4");
    cout << '\n';

    cout << "=========================\n";
    cout << "Alekhine vs. Vasic (1931)\n";
    cout << "=========================\n\n";

    cb.resetBoard();
    cout << '\n';

    cb.submitMove("E2", "E4");
    cb.submitMove("E7", "E6");
    cout << '\n';

    cb.submitMove("D2", "D4");
    cb.submitMove("D7", "D5");
    cout << '\n';

    cb.submitMove("B1", "C3");
    cb.submitMove("F8", "B4");
    cout << '\n';

    cb.submitMove("F1", "D3");
    cb.submitMove("B4", "C3");
    cout << '\n';

    cb.submitMove("B2", "C3");
    cb.submitMove("H7", "H6");
    cout << '\n';

    cb.submitMove("C1", "A3");
    cb.submitMove("B8", "D7");
    cout << '\n';

    cb.submitMove("D1", "E2");
    cb.submitMove("D5", "E4");
    cout << '\n';

    cb.submitMove("D3", "E4");
    cb.submitMove("G8", "F6");
    cout << '\n';

    cb.submitMove("E4", "D3");
    cb.submitMove("B7", "B6");
    cout << '\n';

    cb.submitMove("E2", "E6");
    cb.submitMove("F7", "E6");
    cout << '\n';

    cb.submitMove("D3", "G6");
    cout << '\n';

    cout << "=========================\n";
    cout << "Tests\n";
    cout << "=========================\n\n";

    cb.resetBoard();
    cout << '\n';

    cb.submitMove("E2", "E4");
    cb.submitMove("B7", "B5");

    cb.submitMove("D2", "D4");
    cb.submitMove("B5", "B4");

    cb.submitMove("C2", "C4");
    cb.submitMove("B4", "C3");

    cb.submitMove("G1", "F3");
    cb.submitMove("D7", "D6");

    cb.submitMove("F1", "B5");
    cb.submitMove("C8", "D7");

    cb.submitMove("O-O");
    cb.submitMove("C7", "C5");

    cb.submitMove("B1", "C3");
    cb.submitMove("D8", "A5");

    cb.submitMove("C1", "G5");
    cb.submitMove("B8", "C6");

    cb.submitMove("B2", "B4");
    cb.submitMove("O-O-O");
    cout << '\n';

    return 0;
}
