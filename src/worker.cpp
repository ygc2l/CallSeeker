#include <vector>
#include "worker.h"
#ifdef _WIN32
    #include <windows.h>
    #define ENABLE_SNDFILE_WINDOWS_PROTOTYPES 1
#endif
#include <libsndfile/sndfile.hh>

tensorflow::Tensor prepInputs(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> &spec, size_t nmels)
{
    // Rescale spectrogram values between -1 and 1
    spec /= 100.;
    spec *= 2.;
    spec.array() -= 1;

    size_t ncols = spec.cols();

    // Prepare tensorflow inputs
    tensorflow::Tensor spec_tf_tensormap(tensorflow::DT_FLOAT, tensorflow::TensorShape({int(ncols / nmels), int(nmels), int(nmels), 1}));
    Eigen::TensorMap<Eigen::Tensor<float, 4, Eigen::ColMajor>> spec_eigen_tensormap(spec_tf_tensormap.tensor<float, 4>().data(), int(ncols / nmels), int(nmels), int(nmels), 1);

    Eigen::Tensor<float, 2, Eigen::ColMajor> spec_eigen_tensor_2D = mat2Tensor(spec, spec.rows(), ncols);
    Eigen::array<int, 4> new_dims{{ int(ncols / nmels), int(nmels), int(nmels), 1}};
    spec_eigen_tensormap = spec_eigen_tensor_2D.reshape(new_dims);

    return(spec_tf_tensormap);
}

bool Worker::read_audio(SndfileHandle snd_file, std::vector<double>::iterator audio_samples_seek_it, size_t n_samples_to_read)
{
    int n_channels = snd_file.channels();
    size_t read_len = 0;

    if(n_channels == 1)
    {
        snd_file.read(audio_samples_seek_it.base(), static_cast<long long>(n_samples_to_read));
    }
    else if (n_channels == 2)
    {
        std::vector<double> tmp (n_samples_to_read * 2);
        read_len = size_t(snd_file.read(tmp.begin().base(), static_cast<long long>(n_samples_to_read * 2)));

        // Mix channels together
        for (size_t k = 0; k < read_len; k += 2)
        {
            *audio_samples_seek_it = tmp[k] * .5 + tmp[k + 1] * .5;
            audio_samples_seek_it++;
        }
    }
    else
    {
        return false;
    }

    return true;
}
