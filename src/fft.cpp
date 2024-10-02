//------------------------------------------------------------------------------
//  Copyright 2012 Chris Scott (fbscds@gmail.com)
//  Copyright 2016-2017 WavX inc. (www.wavx.ca)
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

#include <math.h>
#include <vector>
#include "fft.h"
#include <fftw3/fftw3.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

void fft::setPlan(size_t size)
{    
    fft_size = size;
    after.resize(size, 0);
    before.resize(size, 0);
    n_bins = fft_size / 2;
    mag.resize(n_bins);
    setWin();
    plan = fftwf_plan_r2r_1d(int(fft_size), before.begin().base(), after.begin().base(), FFTW_R2HC, FFTW_PATIENT);
}

void fft::setWin()
{
  win.resize(fft_size);
  double z = 2 * M_PI / double(fft_size - 1);

  for (size_t i = 0; i < fft_size; i++)
  {
      win[i] = 0.5 * (1 - std::cos(z*i));
  }
}

