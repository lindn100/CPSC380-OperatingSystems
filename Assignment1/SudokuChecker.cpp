#include "SudokuChecker.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
//#include <pthread.h>

using namespace std;

SudokuChecker::SudokuChecker() {
  loadFile("test.txt");
  runProgram();
}

SudokuChecker::SudokuChecker(string fileName) {
  loadFile(fileName);
  runProgram();
}

SudokuChecker::~SudokuChecker() {

}

void SudokuChecker::loadFile(string s) {
  this->fileName = s;

  if(!ifstream(fileName)) {
    cout << "File not found. Exiting program." << endl;
    exit(0);
  }

  ifstream readFile(fileName);
  string line;
  int r = 0;

  while(r < 9 && getline(readFile, line)) {
    vector<int> row;
    stringstream iss(line);
    int c = 0;
    string val;

    while(c < 9 && getline(iss, val, ',')) {
      row.push_back(stoi(val));
    }
    board.push_back(row);
  }
}

void SudokuChecker::printBoard(vector<vector<int>> board) {
  for(int i = 0; i < board.size(); ++i) {
    for(int j = 0; j < board[0].size(); ++j) {
      cout << board[i][j];
    }
    cout << "\r\n";
  }
}

void SudokuChecker::runProgram() {
  vScan(board);
  hScan(board);
  sScan(board);
}

void SudokuChecker::vScan(vector<vector<int>> board) {
  for(int i = 0; i < board.size(); ++i) {
    vector<int> seen(9);
    for(int j = 0; j < board[0].size(); ++j) {
      if(seen[board[j][i]-1] != 0) {
        //problem occurrs here
        cout << "Duplicate vertical number " << board[j][i] << " found in column " << i+1 << endl;
      } else {
        seen[board[j][i]-1] = board[j][i];
      }

    }
    for(int k = 0; k < 9; ++k) {
      if(seen[k] != k+1) {
        cout << "Missing number " << k+1 << " in column " << i+1 << endl;
      }
    }
  }
}

void SudokuChecker::hScan(vector<vector<int>> board) {
  for(int i = 0; i < board.size(); ++i) {
    vector<int> seen(9);
    for(int j = 0; j < board[0].size(); ++j) {
      if(seen[board[i][j]-1] != 0) {
        //problem occurrs here
        cout << "Duplicate horizontal number " << board[i][j] << " found in row " << i+1 << endl;
      } else {
        seen[board[i][j]-1] = board[i][j];
      }

    }
    for(int k = 0; k < 9; ++k) {
      if(seen[k] != k+1) {
        cout << "Missing number " << k+1 << " in row " << i+1 << endl;
      }
    }
  }
}

void SudokuChecker::sScan(vector<vector<int>> board) {

  for(int a = 0; a < 3; ++a) {
    for(int b = 0; b < 3; ++b) {
      vector<int> seen(9);
      for(int i = a*3; i < (a*3)+3; ++i){
        for(int j = b*3; j < (b*3)+3; ++j) {
          if(seen[board[i][j]-1] != 0) {
            cout << "Duplicate number " << board[i][j] << " in square " << a+b+1 << endl;
          } else {
            seen[board[i][j]-1] = board[i][j];
          }
        }
      }
      for(int k = 0; k < 9; ++k) {
        if(seen[k] != k+1) {
          cout << "Missing number " << k+1 << " in square " << a+b+1 << endl;
        }
      }
    }
  }
}
