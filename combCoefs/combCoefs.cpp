#include "comb—oefs.h"
//calculation the number of placements with repetitions from m to n
unsigned cc::U(unsigned m, unsigned n, bool *err) noexcept {
    unsigned res = 1;

    *err = false;
    //edge cases
    if (m == 1)
      return 1;

    for (unsigned i = 0; i < n; i++) {
      //check going out of range
      if (m != 0 && res > UINT32_MAX / m) {
        std::cerr << "\tOut of range" << std::endl;
        *err = true;
        return 0;
      }
      res *= m;
    }

    return res;
  }
//calculation the number of placements without repetitions from m to n
unsigned cc::A(unsigned m, unsigned n, bool *err) noexcept {
    unsigned res = 1;

    *err = false;
    //edge cases
    if (n > m)
      return 0;
    if (n == 0)
      return 1;

    for (unsigned i = 0; i < n; i++) {
      //check going out of range
      if (res > UINT32_MAX / (m - i)) {
        std::cerr << "\tOut of range" << std::endl;
        *err = true;
        return 0;
      }
      res *= m - i;
    }

    return res;
  }
//calculation the number of permutations for n
unsigned cc::P(unsigned n, bool *err) noexcept {
    unsigned res = 1;

    *err = false;
    //edge cases
    if (n == 0)
      return 1;

    for (unsigned i = 1; i <= n; i++) {
      //check going out of range
      if (res > UINT32_MAX / i) {
        std::cerr << "\tOut of range" << std::endl;
        *err = true;
        return 0;
      }
      res *= i;
    }

    return res;
  }
//calculation of the number of combinations from m to n
unsigned cc::C(unsigned m, unsigned n, bool *err) noexcept {
    *err = false;
    //edge cases
    if (n == 1 || m == n + 1)
      return m;
    if (n > m)
      return 0;
    if (n == m || n == 0)
      return 1;
    if (n > m - n)
      n = m - n;

    unsigned a = m - n + 1, k = m - n + 2;
    for (unsigned i = 2; i <= n; i++) {
      unsigned ad = a / i, am = a % i;
      //check going out of range
      if (ad > UINT32_MAX / k || am > UINT32_MAX / k * i || ad * k > UINT32_MAX - am * k / i) {
        std::cerr << "\tOut of range" << std::endl;
        *err = true;
        return 0;
      }
      //recursive formula
      a = ad * k + am * k / i;
      k++;
    }

    return a;
  }
//calculation of the Stirling number of the second kind
unsigned cc::S(unsigned m, unsigned n, bool *err) noexcept {
    *err = false;
    //edge cases
    if (n == m || n == 1)
      return 1;
    if (n == 0 || n > m)
      return 0;

    unsigned d, s;
    //working array length and number of repetitions
    if (n > m - n + 1) {
      s = n;
      d = m - n + 1;
    }
    else {
      d = n;
      s = m - n + 1;
    }
    if(d > 1000) {
      std::cerr << "\tOut of range" << std::endl;
      *err = true;

      return 0;
    }

    unsigned *arr = new unsigned[d];
    for (unsigned i = 0; i < d; i++)
      arr[i] = 1;

    //Stirling's triangle
    for (unsigned i = 2; i <= s; i++)
      for (unsigned j = 1; j < d; j++)
        if (d == n) {
          //check going out of range
          if (arr[j] > UINT32_MAX / (j + 1) || arr[j - 1] > UINT32_MAX - (j + 1) * arr[j]) {
            std::cerr << "\tOut of range" << std::endl;
            *err = true;
            delete[] arr;
            return 0;
          }
          arr[j] = arr[j - 1] + (j + 1) * arr[j];
        }
        else {
          //check going out of range
          if (arr[j - 1] > UINT32_MAX / i || arr[j] > UINT32_MAX - i * arr[j - 1]) {
            std::cerr << "\tOut of range" << std::endl;
            *err = true;
            delete[] arr;
            return 0;
          }
          arr[j] = arr[j] + i * arr[j - 1];
        }

    unsigned res = arr[d - 1];
    delete[] arr;
    return res;
  }
//Bell's number calculation
unsigned cc::B(unsigned m, bool *err) noexcept {
    *err = false;
    //edge cases
    if (m == 0)
      return 1;
    if(m > 20) {
      std::cerr << "\tOut of range" << std::endl;
      *err = true;

      return 0;
    }

    unsigned *arr = new unsigned[m];
    arr[0] = 1;
    //Bell's triangle
    for (unsigned i = 1; i < m; i++) {
      unsigned t = arr[0];
      arr[0] = arr[i - 1];
      for (unsigned k = 1; k <= i; k++) {
        unsigned s = arr[k];
        //check going out of range
        if (arr[k - 1] > UINT32_MAX - t) {
          std::cerr << "\tOut of range" << std::endl;
          *err = true;
          delete[] arr;
          return 0;
        }
        arr[k] = arr[k - 1] + t;
        t = s;
      }
    }

    unsigned res = arr[m - 1];
    delete[] arr;
    return res;
}