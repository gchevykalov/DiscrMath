#include <iostream>

#include "labyrinth.h"

using std::cout;

int main(void) {
  cout << "Input a file name: ";
  string fn;
  std::cin >> fn;

  try {
    labyrinth_t labyrinth;
    labyrinth.LoadFromFile(fn);
    labyrinth.Print();
    std::vector<int> way;
    int res = labyrinth.GetWay(&way);
    if (res < 0) {
      cout << "No way to exit" << std::endl;
      labyrinth.DisplayWay(way);
    }
    else if (res == 0)
      cout << "Start point coincides with exit" << std::endl;
    else
      labyrinth.DisplayWay(way);
  }
  catch (std::exception& e) {
    cout << "ERROR: " << e.what() << std::endl;
  }

  return 0;
}
