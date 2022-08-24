#pragma once

#include <string>
#include <vector>

using std::vector;
using std::string;

class labyrinth_t {
private:
  int w;
  int h;
  vector<char> labyrinth;
  int start;

  bool isAvailable(int index, const vector<int>& distance);
  bool isExit(int index);
  void GetShortWayBack(int index1, int index2, vector<int>* movements, const vector<int>& distance);

public:
  void Print(void);
  void LoadFromFile(const string& fileName);
  int GetWay(vector<int>* movements);
  void DisplayWay(const vector<int>& movements);
};
