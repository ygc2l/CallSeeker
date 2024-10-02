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

#ifndef FFT_H
#define FFT_H

#include <complex>
#include <stddef.h>
#include <vector>
#include <fftw3/fftw3.h>
#include "third_party/eigen_archive/Eigen/Core"

class fft
{
public:
    std::vector<float> after, before;
    Eigen::ArrayXf mag;
    fftwf_plan plan;

    inline void destroyPlan ();
    inline size_t getNBins();
    inline void process(long long sk, const std::vector<double> &samples);
    void setPlan(size_t size);

private:
    float norm;
    size_t n_bins = 0;
    size_t fft_size;
    std::vector<double> win;

    void setWin();
};

inline
size_t fft::getNBins()
{
    return(n_bins);
}

inline
void fft::process(long long sk, const std::vector<double> &samples)
{
  for (size_t i = 0; i < fft_size; ++i, ++sk)
  {
      before[i] = float(samples[sk] * win[i]);
  }

  fftwf_execute(plan);

  for (size_t i = 1, seek = n_bins * 2; i <= n_bins; ++i)
  {
      mag[i-1] = std::abs(std::complex<float>(after[i], after[--seek]));
  }
}

inline void fft::destroyPlan ()
{
    fftwf_destroy_plan( plan );
}

#endif // FFT_H
