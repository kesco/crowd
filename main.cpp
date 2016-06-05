#include "main.hpp"

#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
  if (argc == 0)
  {
    cerr << "Wrong input." << endl;
    exit(-1);
  }
  boost::filesystem::path path(argv[0]);
  if (boost::filesystem::is_regular_file(path)) 
  {
    std::cout << "yes" << std::endl;
  }

  while (1);
  return 0;
}