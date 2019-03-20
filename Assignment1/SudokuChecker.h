#ifndef SudokuChecker_H
#define SudokuChecker_H
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class SudokuChecker {

public:
  SudokuChecker();
  SudokuChecker(string);
  ~SudokuChecker();

  void loadFile(string);
  void printBoard(vector<vector<int>> board);
  void runProgram();
  vector<int> vScan(vector<vector<int>> board);
  vector<int> hScan(vector<vector<int>> board);
  vector<int> sScan(vector<vector<int>> board);

private:
  string fileName;
  vector<vector<int>> board;
};

#endif
