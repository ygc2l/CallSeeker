//------------------------------------------------------------------------------
//  Copyright 2020 WavX inc. (www.wavx.ca)
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

#include <algorithm>
#include <vector>
#include "fft.h"
#include "mel.h"
#include "utils.h"
#include "third_party/eigen_archive/Eigen/Core"
#include <QDebug>

double hz2mel(double x)
{
    return(2595. * std::log10(1. + x / 700.));
}

double mel2hz(double x)
{
    return(700. * (std::pow(10., x / 2595.) - 1));
}

Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> filterBanks(std::size_t &fft_size, std::size_t &samplerate)
{
    std::size_t fmin = 400;
    std::size_t fmax = 8000;

    constexpr size_t nmels = 224;
    size_t ncols = fft_size / 2;

    // Initialize the weight matrix
    Eigen::Matrix<double, nmels, Eigen::Dynamic, Eigen::ColMajor> weights (nmels, ncols);

    // Compute points evenly spaced in the mel space
    double fmin_mel = hz2mel(double(fmin));
    double fmax_mel = hz2mel(double(fmax));

    Eigen::ArrayXd mels_f = (Eigen::ArrayXd::LinSpaced(nmels + 2, fmin_mel, fmax_mel)).unaryExpr(std::ptr_fun(mel2hz));

    Eigen::ArrayXd fdiffs = adjacent_diff_eigen(mels_f);

    // Center freqs of each FFT bin
    Eigen::ArrayXd freqs = (Eigen::ArrayXd::LinSpaced(1 + fft_size / 2, 0., double(samplerate) / 2.)).tail(ncols);

    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> ramps (mels_f.size(), freqs.size());

    for(long long i = 0; i < mels_f.size(); i++)
    {
        for(long long j = 0; j < freqs.size(); j++)
        {
            ramps.coeffRef(i,j) = mels_f[i] - freqs[j];
        }
    }

    for(long long i = 0; i < nmels; i++)
    {
        // lower and upper slopes for all bins
        Eigen::ArrayXd lower = -ramps.row(i) / fdiffs[i];
        Eigen::ArrayXd upper = ramps.row(i+2) / fdiffs[i+1];

        // then intersect them with each other and zero
        weights.row(i) = (lower.min(upper)).max(0);
    }


    // Slaney-style mel is scaled to be approx constant energy per channel
    Eigen::ArrayXd enorm(2. / (mels_f.segment(2, nmels) - mels_f.head(nmels)));

    weights = weights.array().colwise() * enorm;

    return(weights.cast<float>());
}

Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>
melSpec(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> spec, size_t fft_size, std::size_t &samplerate)
{
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> filter_banks = filterBanks(
        fft_size,
        samplerate
    );

    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> mel_spec = filter_banks * spec;

    // Normalize
    mel_spec = mel_spec / mel_spec.maxCoeff();
    mel_spec = power2db(mel_spec);
    mel_spec = mel_spec.array().max(mel_spec.maxCoeff() - float(100.));

    return(mel_spec);
}

