#include "CommandLineChecker.h"
#include "SudokuChecker.h"

int main(int argc, char* argv[]) {
  CommandLineChecker checkIt(argc);
  SudokuChecker a1(argv[1]);
  return 0;
}
