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
  void printBoard();
  static void* startPrintBoard(void* object);
  void runProgram();
  void vScan();
  static void* startVScan(void* object);
  void hScan();
  static void* startHScan(void* object);
  void sScan();
  static void* startSScan(void* object);

private:
  string fileName;
  vector<vector<int>> board;
  vector<int> v; //returns duplicate values, column #, and missing values in the vertical scan
  vector<int> h; //returns duplicate values, row #, and missing values in the horizontal scan
  vector<int> s; //returns duplicate values, square #, and missing values in the square scan
};

#endif
