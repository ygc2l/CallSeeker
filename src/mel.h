#ifndef MEL_H
#define MEL_H
#include <vector>
#include "third_party/eigen_archive/Eigen/Core"

Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> filterBanks(std::size_t &fft_size, std::size_t &samplerate);
double hz2mel(double x);
double mel2hz(double x);
Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>
melSpec(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> spec, size_t fft_size, std::size_t &samplerate);

#endif // MEL_H
