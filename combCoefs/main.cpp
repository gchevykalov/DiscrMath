#include "comb—oefs.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>

using std::vector;
using std::string;
using std::cout;
using std::endl;

//function to split the string 'line' into 'words' by space character
void SplitLine(const string &line, vector<string> &words) {
  std::istringstream str(line);
  std::istream_iterator<string> begin(str), end;

  std::copy(begin, end, std::back_inserter(words));
}
//display a message about an invalid format
inline void wrongForm(void) noexcept {
  cout << "\tWrong format, use H to learn more" << endl;
}
//display a message about an invalid argument
inline void wrongArg(void) noexcept {
  cout << "\tWrong argument, use H to learn more" << endl;
}
//display help
void displayHelp(void) {
  cout << "  Help:" << endl;
  cout << "\tFunction arguments can only be positive integers" << endl;
  cout << "  Possible commands:" << endl;
  cout << "\tQ - end session" << endl;
  cout << "\tH - display help" << endl;
  cout << "\tU <m> <n> - calculation the number of placements with repetitions from m to n" << endl;
  cout << "\tA <m> <n> - calculation the number of placements without repetitions from m to n" << endl;
  cout << "\tP <n> - calculation the number of permutations for n" << endl;
  cout << "\tC <m> <n> - calculation of the number of combinations from m to n" << endl;
  cout << "\tS <m> <n> - calculation of the Stirling number of the second kind" << endl;
  cout << "\tB <m> - Bell's number calculation" << endl;
}
//display result
inline void displayRes(unsigned res, bool err) {
  if (!err)
    cout << '\t' << res << endl;
}

unsigned getNum(string str, bool *err) {
  unsigned res = 0;

  for (int i = 0; i < str.length(); i++) {
    unsigned num = str[i] - '0';
    if (res > (UINT32_MAX - num)/ 10) {
      std::cerr << "\tOut of range argument" << std::endl;
      *err = true;
      return 0;
    }
    res *= 10;
    res += str[i] - '0';
  }

  return res;
}

int main(void) {
  bool err;
  string line;

  while (true) {
    char ch;
    vector<string> words;

    err = false;
    std::getline(std::cin, line);
    SplitLine(line, words);
    if (words[0].size() != 1)
      cout << "\tUnknown command, use H to learn more" << endl;
    else {
      if (words.size() > 3)
        wrongForm();
      else {
        //check arguments
        for (auto w = words.begin() + 1; w != words.end(); w++)
          if ((*w).find_first_not_of("0123456789") != std::string::npos) {
            wrongArg();
            err = true;
            break;
          }
        if (!err) {
          ch = words[0][0];
          switch (ch) {
            //display help
            case 'H':
              if (words.size() != 1)
                wrongForm();
              else
                displayHelp();
              break;
            //end session
            case 'Q':
              if (words.size() != 1)
                wrongForm();
              else
                return 0;
              break;
            //number of placements with repetitions
            case 'U':
              if (words.size() != 3)
                wrongForm();
              else {
                unsigned m = getNum(words[1], &err),
                  n = getNum(words[2], &err), res;

                if (!err) {
                  res = cc::U(m, n, &err);
                  displayRes(res, err);
                }
              }
              break;
            //number of placements without repetitions
            case 'A':
              if (words.size() != 3)
                wrongForm();
              else {
                unsigned m = getNum(words[1], &err),
                  n = getNum(words[2], &err), res;

                if (!err) {
                  res = cc::A(m, n, &err);
                  displayRes(res, err);
                }
              }
              break;
            //number of permutations
            case 'P':
              if (words.size() != 2)
                wrongForm();
              else {
                unsigned n = getNum(words[1], &err), res;

                if (!err) {
                  res = cc::P(n, &err);
                  displayRes(res, err);
                }
              }
              break;
            //number of combinations
            case 'C':
              if (words.size() != 3)
                wrongForm();
              else {
                unsigned m = getNum(words[1], &err),
                  n = getNum(words[2], &err), res;

                if (!err) {
                  res = cc::C(m, n, &err);
                  displayRes(res, err);
                }
              }
              break;
            //Stirling's number of the second kind
            case 'S':
              if (words.size() != 3)
                wrongForm();
              else {
                unsigned m = getNum(words[1], &err),
                  n = getNum(words[2], &err), res;

                if (!err) {
                  res = cc::S(m, n, &err);
                  displayRes(res, err);
                }
              }
              break;
            //Bell's number
            case 'B':
              if (words.size() != 2)
                wrongForm();
              else {
                unsigned m = getNum(words[1], &err), res;

                if (!err) {
                  res = cc::B(m, &err);
                  displayRes(res, err);
                }
              }
              break;
            default:
              cout << "\tUnknown command, use H to learn more" << endl;
              break;
          }
        }
      }
    }
  }

  return 0;
}