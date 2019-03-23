#include <pthread.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <pthread.h>

using namespace std;

vector<vector<int>> board;
vector<int> v; //returns duplicate values, column #, and missing values in the vertical scan
vector<int> h; //returns duplicate values, row #, and missing values in the horizontal scan
vector<int> s; //returns duplicate values, square #, and missing values in the square scan

void fillBoard(string);
void printBoard();
void *vScan(void *);
void *hScan(void *);
void *sScan(void *);
void checkResults();

int main(int argc, char* argv[]) {
  if(argc < 2) {
    cout << "No file provided. Exiting." << endl;
    exit(0);
  } else {
    string fileName = argv[1];
    fillBoard(fileName);
    //printBoard();
    pthread_t myThreads[3];
    for(int i = 0; i < 3; ++i) {
      switch(i) {
        case 0:
          pthread_create(&myThreads[0], NULL, vScan, NULL);
          break;
        case 1:
          pthread_create(&myThreads[1], NULL, hScan, NULL);
          break;
        case 2:
          pthread_create(&myThreads[2], NULL, sScan, NULL);
          break;
      }
    }

    for(int i = 0; i < 3; ++i) {
      pthread_join(myThreads[i], NULL);
    }

    checkResults();


  }
  return 0;
}

void fillBoard(string fileName) {
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

void printBoard() {
  for(int i = 0; i < board.size(); ++i) {
    for(int j = 0; j < board[0].size(); ++j) {
      cout << board[i][j];
    }
    cout << "\r\n";
  }
}

void *vScan(void *) {
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

void *hScan(void *) {
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

void *sScan(void *) {

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

void checkResults() {
  if(v.size() == 0 && h.size() == 0 && s.size() == 0) {
    cout << "Valid sudoku board. Exiting." << endl;
  } else {
    for(int i = 0; i < v.size(); i += 3) {
      cout << "Duplicate value of " << v[i] << " found at " << h[i+1] << "," << v[i+1] << " in square " << s[i+1] << ". This value should be " << v[i+2] << endl;
    }
  }
}
