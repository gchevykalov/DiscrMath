#pragma once

#ifndef __COMBCOEFS_H_INCLUDED
#define __COMBCOEFS_H_INCLUDED

#include <iostream>
#include <limits>

namespace cc {
  //calculation the number of placements with repetitions from m to n
  unsigned U(unsigned m, unsigned n, bool *err) noexcept;
  //calculation the number of placements without repetitions from m to n
  unsigned A(unsigned m, unsigned n, bool *err) noexcept;
  //calculation the number of permutations for n
  unsigned P(unsigned n, bool *err) noexcept;
  //calculation of the number of combinations from m to n
  unsigned C(unsigned m, unsigned n, bool *err) noexcept;
  //calculation of the Stirling number of the second kind
  unsigned S(unsigned m, unsigned n, bool *err) noexcept;
  //Bell's number calculation
  unsigned B(unsigned m, bool *err) noexcept;

}
#endif /* __COMBCOEFS_H_INCLUDED */