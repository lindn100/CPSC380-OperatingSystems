#include "SudokuChecker.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <pthread.h>

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

static void* startPrintBoard(void* object) {
  reinterpret_cast<SudokuChecker*>(object)->printBoard();
  return 0;
}

void SudokuChecker::printBoard() {
  for(int i = 0; i < board.size(); ++i) {
    for(int j = 0; j < board[0].size(); ++j) {
      cout << board[i][j];
    }
    cout << "\r\n";
  }
}

void SudokuChecker::runProgram() {
  pthread_t thread1;
  /*pthread_t thread2;
  pthread_t thread3;*/

  pthread_create(&thread1, NULL, &startVScan, this);
  /*pthread_create(&thread2, NULL, &startHScan, this);
  pthread_create(&thread3, NULL, &startSScan, this);*/

  pthread_join(thread1, NULL);
  /*pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);*/

  /*pthread_create(&thread1, NULL, &startPrintBoard, this);
  pthread_join(thread1, NULL);
  vScan();
  hScan();
  sScan();*/

  if(v.size() == 0 && h.size() == 0 && s.size() == 0) {
    cout << "Valid sudoku board. Exiting." << endl;
  } else {
    for(int i = 0; i < v.size(); i += 3) {
      cout << "Duplicate value of " << v[i] << " found at " << h[i+1] << "," << v[i+1] << " in square " << s[i+1] << ". This value should be " << v[i+2] << endl;
    }
  }
}

static void* startVScan(void* object) {
  reinterpret_cast<SudokuChecker*>(object)->vScan();
  return 0;
}

static void* startHScan(void* object) {
  reinterpret_cast<SudokuChecker*>(object)->hScan();
  return 0;
}

static void* startSScan(void* object) {
  reinterpret_cast<SudokuChecker*>(object)->sScan();
  return 0;
}

void SudokuChecker::vScan() {
  vector<int> dups;
  vector<int> col;
  vector<int> missing;
  for(int i = 0; i < board.size(); ++i) {
    vector<int> seen(9);
    for(int j = 0; j < board[0].size(); ++j) {
      if(seen[board[j][i]-1] != 0) {
        //problem occurrs here
        //cout << "Duplicate vertical number " << board[j][i] << " found in column " << i+1 << endl;
        dups.push_back(board[j][i]);
        col.push_back(i+1);
      } else {
        seen[board[j][i]-1] = board[j][i];
      }

    }
    for(int k = 0; k < 9; ++k) {
      if(seen[k] != k+1) {
        //cout << "Missing number " << k+1 << " in column " << i+1 << endl;
        missing.push_back(k+1);
      }
    }
  }
   for(int i = 0; i < dups.size(); ++i) {
     v.push_back(dups[i]); //order of the returning array will be duplicated num followed by missing num
     v.push_back(col[i]);
     v.push_back(missing[i]);
   }
}

void SudokuChecker::hScan() {
  vector<int> dups;
  vector<int> row;
  vector<int> missing;
  for(int i = 0; i < board.size(); ++i) {
    vector<int> seen(9);
    for(int j = 0; j < board[0].size(); ++j) {
      if(seen[board[i][j]-1] != 0) {
        //problem occurrs here
        //cout << "Duplicate horizontal number " << board[i][j] << " found in row " << i+1 << endl;
        dups.push_back(board[i][j]);
        row.push_back(i+1);
      } else {
        seen[board[i][j]-1] = board[i][j];
      }

    }
    for(int k = 0; k < 9; ++k) {
      if(seen[k] != k+1) {
        //cout << "Missing number " << k+1 << " in row " << i+1 << endl;
        missing.push_back(k+1);
      }
    }
  }

  for(int i = 0; i < dups.size(); ++i) {
    h.push_back(dups[i]);
    h.push_back(row[i]);
    h.push_back(missing[i]);
  }
}

void SudokuChecker::sScan() {

  vector<int> dups;
  vector<int> square;
  vector<int> missing;

  for(int a = 0; a < 3; ++a) {
    for(int b = 0; b < 3; ++b) {
      vector<int> seen(9);
      for(int i = b*3; i < (b*3)+3; ++i){
        for(int j = a*3; j < (a*3)+3; ++j) {
          if(seen[board[i][j]-1] != 0) {
            //cout << "Duplicate number " << board[i][j] << " in square " << a+(b*3)+1 << endl;
            dups.push_back(board[i][j]);
            square.push_back(a+(b*3)+1);
          } else {
            seen[board[i][j]-1] = board[i][j];
          }
        }
      }
      for(int k = 0; k < 9; ++k) {
        if(seen[k] != k+1) {
          //cout << "Missing number " << k+1 << " in square " << a+b+1 << endl;
          missing.push_back(k+1);
        }
      }
    }
  }

  for(int i = 0; i < dups.size(); ++i) {
    s.push_back(dups[i]);
    s.push_back(square[i]);
    s.push_back(missing[i]);
  }
}
