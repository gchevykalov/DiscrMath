#include "labyrinth.h"
#include <fstream>
#include <sstream>
#include <iterator>
#include <iostream>
#include <stack>
#include <queue>

using std::exception;
using std::cout;

constexpr int NOTVISITED = -100;

static void SplitLine(const string& line, vector<string>* words) {
  std::istringstream str(line);
  std::istream_iterator<string> begin(str), end;

  std::copy(begin, end, std::back_inserter(*words));
}

// based on bfs
void labyrinth_t::GetShortWayBack(int index1, int index2, vector<int>* movements, const vector<int>& distance) {
  vector<int> bfsDistance;
  std::queue<int> q;

  if (distance[index2] != NOTVISITED)
    return;

  bfsDistance.resize(h * w);
  for (auto& b : bfsDistance)
    b = NOTVISITED;

  bfsDistance[index1] = 0;
  q.push(index1);
  int delta[4] = { w, 1, -1, -w };
  while (!q.empty()) {
    int cur = q.front();
    q.pop();

    if (cur == index2)
      break;

    for (int i = 0; i < 4; i++) {
      if ((distance[cur + delta[i]] != NOTVISITED || cur + delta[i] == index2) && bfsDistance[cur + delta[i]] == NOTVISITED) {
        q.push(cur + delta[i]);
        bfsDistance[cur + delta[i]] = bfsDistance[cur] + 1;
      }
    }
  }

  int cur = index2;
  vector<int> move;
  while (cur != index1) {
    for (int i = 0; i < 4; i++) {
      if (cur + delta[i] >= 0 && cur + delta[i] < h * w && bfsDistance[cur + delta[i]] == bfsDistance[cur] - 1) {
        cur += delta[i];
        break;
      }
    }
    move.push_back(cur);
  }
  movements->insert(movements->end(), move.rbegin() + 1, move.rend());
}

bool labyrinth_t::isAvailable(int index, const vector<int>& distance) {
  if (index < 0 || index >= h * w)
    return false;
  if (labyrinth[index] == '#')
    return false;
  if (distance[index] != NOTVISITED)
    return false;

  return true;
}

bool labyrinth_t::isExit(int index) {
  if (index < w || index % w == 0 || index % w == w - 1 || index / w == h -1)
    return true;

  return false;
}

void labyrinth_t::LoadFromFile(const string& fileName) {
  std::ifstream iStream(fileName);

  if (iStream.is_open() == 0)
    throw exception("Incorrect file name");
  if (iStream.eof() == true)
    throw exception("Empty file");

  string line;
  std::getline(iStream, line);
  vector<string> words;
  SplitLine(line, &words);
  if (words.size() != 2)
    throw exception("The first line should contain the height and width of the labyrinth");
  try {
    h = std::stoi(words[0]);
    w = std::stoi(words[1]);
    if (h <= 0 || w <= 0)
      throw exception("Size must be positive");
  }
  catch (exception&) {
    throw exception("The first line should contain the height and width of the labyrinth");
  }

  int size = h * w;
  labyrinth.resize(size);
  start = -1;
  int index = 0;
  int realW = 0;
  int realH = 1;
  while (!iStream.eof() && index < size) {
    char ch = iStream.get();
    switch (ch) {
      case 's':
        if (start == -1) {
          start = index;
          labyrinth[index] = ch;
        }
        else
          throw exception("Only 1 start point must be");
        break;
      case '\n':
        realH++;
        index--;
        if(realW != w)
          throw exception("Wrong line length");
        realW = -1;
        break;
      case ' ':
        labyrinth[index] = ch;
        break;
      default:
        labyrinth[index] = '#';
        break;
    }
    index++;
    realW++;
  }
  if(realH != h)
    throw exception("Wrong number of line");
  if (start == -1)
    throw exception("No start position");
}

// based on dfs
int labyrinth_t::GetWay(vector<int>* movements) {
  vector<int> distance;
  std::stack<int> s;
  distance.resize(h * w);
  movements->clear();
  for (auto& d : distance)
    d = NOTVISITED;

  distance[start] = 0;
  s.push(start);
  movements->push_back(start);
  int delta[4] = { w, 1, -1, -w };
  while (!s.empty()) {
    int cur = s.top();
    s.pop();

    if (isExit(cur)) {
      if (movements->size() == 1)
        return 0;
      movements->push_back(cur);
      return 1;
    }

    int notAvailable = 0;
    for (int i = 0; i < 4; i++) {
      if (isAvailable(cur + delta[i], distance))
        s.push(cur + delta[i]);
      else
        notAvailable++;
    }

    if (distance[cur] == NOTVISITED)
      movements->push_back(cur);
    int size = movements->size();
    if(size > 1)
      distance[cur] = distance[(*movements)[size - 2]] + 1;

    // simulates the return of a person to a fork
    if (notAvailable == 4) {
      while (!s.empty() && distance[s.top()] != NOTVISITED)
        s.pop();
      if (!s.empty())
        GetShortWayBack(cur, s.top(), movements, distance);
    }
  }

  return -1;
}

void labyrinth_t::DisplayWay(const vector<int>& movements) {
  for (int i = 1; i < movements.size(); i++) {
    int delta = movements[i] - movements[i - 1];
    if (delta == -w)
      cout << "up, ";
    else if (delta == -1)
      cout << "left, ";
    else if (delta == 1)
      cout << "right, ";
    else if (delta == w)
      cout << "down, ";
    else
      cout << "???, ";
  }
}

void labyrinth_t::Print(void) {
  int index = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      std::cout << labyrinth[index];
      index++;
    }
    std::cout << std::endl;
  }
}