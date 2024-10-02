//------------------------------------------------------------------------------
//  Copyright 2012 Chris Scott (fbscds@gmail.com)
//  Copyright 2017-2018 WavX inc. (www.wavx.ca)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with This program.  If not, see <https://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <cmath>
#include <sstream>
#include <stddef.h>
#include <vector>
#include "third_party/eigen_archive/Eigen/Core"

inline Eigen::ArrayXd adjacent_diff_eigen(Eigen::ArrayXd const& A);
inline std::string s2dhmsms(const float &x);

inline
Eigen::ArrayXd adjacent_diff_eigen(Eigen::ArrayXd const& A)
{
    Eigen::Index n = A.size();
    Eigen::ArrayXd B(n - 1);
    B = A.tail(n-1) - A.head(n-1);
    return(B);
}

inline
Eigen::MatrixXf power2db (Eigen::MatrixXf mat)
{
    return(20. * mat.array().max(float(1e-10)).log10());
}

inline
std::string s2dhmsms(const float &x)
{
  size_t ms = size_t((x - std::trunc(x)) * 1000);
  size_t sR = size_t(x) % 60;
  size_t m = size_t(x) / 60;
  size_t mR = m % 60;
  size_t h = m / 60;

  std::stringstream ss;
  if(h < 10) ss << 0;
  ss << h << ":";
  if(mR < 10) ss << 0;
  ss << mR << ":";
  if(sR < 10) ss << 0;
  ss << sR << ".";
  if(ms < 100) ss << 0;
  if(ms < 10) ss << 0;
  ss << ms;

  return ss.str();
}

#endif // UTILS_H

