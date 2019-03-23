#include <iostream>
#include "CommandLineChecker.h"

using namespace std;

CommandLineChecker::CommandLineChecker(int argc)
{
  //argc logic used from Assignment1
  if(argc == 1)
  {
    cout << "File name not included in command line. Please re-run the program with the file name." << endl;
    exit(0);
  }
  if(argc > 2)
  {
    cout << "Too many arguments entered in command line. Please re-run the program with only the file name as an argument." << endl;
    exit(0);
  }

}

CommandLineChecker::~CommandLineChecker()
{

}
