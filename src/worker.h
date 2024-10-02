#ifndef WORKER_THREAD_H
#define WORKER_THREAD_H

#include <algorithm>
#include <ctime>
#include <fstream>
#include <QDir>
#include <QObject>
#include <QFileInfo>
#include <QMutex>
#include <QSettings>
#include <QStringList>
#include <QThread>
#ifdef _WIN32
    #include <windows.h>
    #define ENABLE_SNDFILE_WINDOWS_PROTOTYPES 1
#endif
#include <libsndfile/sndfile.hh>
#include "third_party/eigen_archive/Eigen/Core"
#include "third_party/eigen_archive/unsupported/Eigen/CXX11/Tensor"
#include "tensorflow/core/protobuf/config.pb.h"
#include "tensorflow/cc/saved_model/loader.h"
#include "tensorflow/cc/saved_model/tag_constants.h"
#include "tensorflow/core/public/session.h"
#include "fft.h"
#include "mel.h"
#include "settings.h"
#include "utils.h"
#include <QDebug>

template<typename T>
using  matrix_type = Eigen::Matrix<T,Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>;

template<typename Scalar, typename... Dims>
auto mat2Tensor(const matrix_type<Scalar> &matrix, Dims... dims)
{
    constexpr int rank = sizeof... (Dims);
    return Eigen::TensorMap<Eigen::Tensor<const Scalar, rank, Eigen::ColMajor>>(matrix.data(), {dims...});
}

tensorflow::Tensor prepInputs(Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> &spec, size_t nmels);

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker() : stop(false) {}

public slots:
    void loadModel()
    {
        tensorflow::SessionOptions session_options;
        tensorflow::RunOptions run_options;

        auto status = tensorflow::LoadSavedModel(session_options, run_options, "model", {tensorflow::kSavedModelTagServe}, &model);

        if (!status.ok())
        {
           std::cerr << status.ToString();
           std::terminate();
        }
    }

    void run_impl (QStringList filepaths, QSettings* qsettings_ptr, QDir output_dir_path)
    {
        mutex.lock();
        stop = false;
        mutex.unlock();

        // Parameters
        double audio_chunk_duration = 1.5;
        float bin_res = 25;
        constexpr size_t nmels = 224;
        const std::vector<std::string> classes {"CATBIC_CALL", "CATMIN_CALL", "CATBIC_SONG", "CATMIN_SONG"};

        // Get current time
        time_t analysis_start_time, analysis_end_time;
        struct tm *timeinfo;
        char buffer[80];
        std::time (&analysis_start_time);
        timeinfo = localtime(&analysis_start_time);
        strftime(buffer,80,"%Y%m%d %H%M%S",timeinfo);
        QString cur_time (buffer);

        QString software = "CallSeeker21";

        // Summary results
        QString output_summary_results_csv_filepath = output_dir_path.filePath(software + "_" + cur_time + "_SummaryResults.csv");
        // QString md_output_f_path = output_dir_path.filePath("Metadata_" + cur_time + ".csv");        

        std::ofstream output_summary_results_csv_ofstream;

        if (qsettings_ptr->value("output_summary_csv").toBool())
        {
            output_summary_results_csv_ofstream.open(output_summary_results_csv_filepath.toStdString());
            output_summary_results_csv_ofstream << "Filepath,ID,Chunk start (HMS.ms),Chunk end (HMS.ms),Class,Confidence";
            output_summary_results_csv_ofstream << std::endl;
        }

        // Files analyzed
        QString output_summary_analyzed_files_csv_filepath = output_dir_path.filePath(software + "_" + cur_time + "_FileProcessingStatus.csv");

        std::ofstream output_summary_analyzed_files_csv_ofstream;

        if (qsettings_ptr->value("output_summary_analyzed_files_csv").toBool())
        {
            output_summary_analyzed_files_csv_ofstream.open(output_summary_analyzed_files_csv_filepath.toStdString());
            output_summary_analyzed_files_csv_ofstream << "Filepath,Status,NbIdentifications";
            output_summary_analyzed_files_csv_ofstream << std::endl;
        }

        float progress = 0;

        float progress_inc_read_file = 0.25;
        float progress_inc_make_spec = 0.25;
        float progress_inc_nn_pred = 0.5;

        std::vector<size_t> fsizes(size_t(filepaths.size()));

        for (int i = 0; i < filepaths.size(); ++i)
        {
            fsizes[i] = size_t(QFileInfo(filepaths[i]).size());
        }

        size_t total_size = std::accumulate(fsizes.begin(), fsizes.end(), size_t(0));

        size_t n_total_id = 0;

        // Loop over files
        for (size_t i = 0; i < size_t(filepaths.size()); ++i)
        {
            time_t this_file_analysis_start_time, this_file_analysis_end_time;
            std::time (&this_file_analysis_start_time);

            float progress_inc_file = float(100) * float(fsizes[i]) / float(total_size);

            QString filepath = filepaths[int(i)];
            QString filename_w_ext = QFileInfo(filepath).fileName();
            QString filename_wo_ext = QFileInfo(filepath).baseName();

            std::ofstream output_audacity_ofstream;

#if ENABLE_SNDFILE_WINDOWS_PROTOTYPES
            SndfileHandle snd_file (filepath.toStdWString().c_str(), SFM_READ);
#else
            SndfileHandle snd_file (filepath.toStdString().c_str(), SFM_READ);
#endif

            size_t n_audio_samples = size_t(snd_file.frames());
            size_t samplerate = size_t(snd_file.samplerate());

            size_t fft_size = size_t(samplerate / bin_res);
            size_t hop_length = audio_chunk_duration * double(samplerate) / double(nmels);

            emit read_file ("Reading " + filename_w_ext);

            // Update CSV that keeps track of every file analysed
            output_summary_analyzed_files_csv_ofstream << filepath.toStdString();

            // Fetch up to 2 seconds from this file
            size_t n_samples_to_read = std::min(nmels * hop_length + fft_size - hop_length, n_audio_samples);

            std::vector<double> audio_samples (n_samples_to_read, 0);
            std::vector<double>::iterator audio_samples_seek_it = audio_samples.begin();

            size_t n_audio_chunks = size_t(1 + std::ceil(float(n_audio_samples - n_samples_to_read) / float(nmels * hop_length)));
            size_t samples_read_to_date = 0;

            bool read_error = false;
            size_t id = 0;

            for (size_t audio_chunk_i = 0; audio_chunk_i < n_audio_chunks; ++audio_chunk_i)
            {
                if(samples_read_to_date + n_samples_to_read > n_audio_samples)
                {
                    n_samples_to_read = size_t(std::floor(float(n_audio_samples - fft_size) / float(hop_length))) * hop_length + fft_size - samples_read_to_date;
                    size_t new_size = std::max(n_samples_to_read + fft_size - hop_length, fft_size);
                    audio_samples.resize(new_size, 0);
                }

                if (read_audio(snd_file, audio_samples_seek_it, n_samples_to_read))
                {
                    progress += progress_inc_file / float(n_audio_chunks) * progress_inc_read_file;
                    emit read_file(progress);

                    if (stop_requested())
                    {
                        output_summary_analyzed_files_csv_ofstream << ",Processing stoped by user.,";
                        output_summary_analyzed_files_csv_ofstream << id;
                        output_summary_analyzed_files_csv_ofstream << std::endl;
                        emit finished();
                        return;
                    }

                    emit process_file("Analysing " + filename_w_ext);

                    fft fft;
                    fft.setPlan(fft_size);


                    long long n_frames = static_cast<long long>(std::floor(float(audio_samples.size() - fft_size) / float(hop_length))) + 1;


                    size_t n_bins = fft.getNBins();
                    size_t ncols = size_t(std::ceil(float(n_frames) / float(nmels))) * nmels;


                    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor> spec = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>::Zero(n_bins, ncols);


                    // STFT
                    for(long long i = 0, seek = 0; i < n_frames; ++i, seek += hop_length)
                    {
                        fft.process(seek, audio_samples);
                        spec.col(i) = fft.mag;
                    }

                    // Create the mel-spectrogram
                    spec = melSpec(spec, fft_size, samplerate);

                    // Calculate chunk start time
                    float chunk_start_time = float(audio_chunk_i * hop_length * nmels) / float(samplerate);
                    float chunk_end_time = float(std::min(audio_chunk_i * hop_length * nmels + hop_length * nmels, n_audio_samples)) / float(samplerate);

                    tensorflow::Tensor spec_tf_tensormap = prepInputs(spec, nmels);
                    spec.resize(0,0);

                    progress += progress_inc_file / float(n_audio_chunks) * progress_inc_make_spec;
                    emit process_file(progress);

                    if (stop_requested())
                    {
                        output_summary_analyzed_files_csv_ofstream << ",Processing stopped by user.,";
                        output_summary_analyzed_files_csv_ofstream << id;
                        output_summary_analyzed_files_csv_ofstream << std::endl;
                        emit finished();
                        return;
                    }
                    tensorflow::Tensor learning_phase(tensorflow::DT_BOOL, tensorflow::TensorShape());
                    learning_phase.scalar<float>()() = false;

                    std::vector<std::string> output_tensor_names { "probs/Sigmoid:0" };

                    std::vector<std::pair<std::string, tensorflow::Tensor>> nn_inputs = {
                        { "input_1:0", spec_tf_tensormap},
                        { "keras_learning_phase:0", learning_phase }
                    };
                    std::vector<tensorflow::Tensor> nn_outputs;
                    auto status = model.session->Run(nn_inputs, output_tensor_names, {}, &nn_outputs);

                    if (!status.ok())
                    {
                        std::cerr << status.ToString();
                        std::terminate();
                    }

                    if (stop_requested())
                    {
                        output_summary_analyzed_files_csv_ofstream << ",Processing stopped by user.,";
                        output_summary_analyzed_files_csv_ofstream << id;
                        output_summary_analyzed_files_csv_ofstream << std::endl;
                        emit finished();
                        return;
                    }


                    for(size_t output_prob_cl_i = 0; output_prob_cl_i < classes.size(); ++output_prob_cl_i)
                    {
                        float prob = nn_outputs[0].matrix<float>()(0, output_prob_cl_i);

                        if(prob >= float(.5))
                        {
                            if (output_summary_results_csv_ofstream.is_open())
                            {
                                id++;
                                output_summary_results_csv_ofstream << filepath.toStdString();
                                output_summary_results_csv_ofstream << "," << id;
                                output_summary_results_csv_ofstream << "," << s2dhmsms(chunk_start_time) \
                                                                    << "," << s2dhmsms(chunk_end_time) \
                                                                    << "," << classes[output_prob_cl_i] \
                                                                    << "," << prob << std::endl;
                            }

                            if (qsettings_ptr->value("output_audacity").toBool())
                            {
                                if(!output_audacity_ofstream.is_open())
                                {
                                    output_audacity_ofstream.open( output_dir_path.filePath(filename_wo_ext + "_" + software + "_" + cur_time + "_AudacityLabels.txt").toStdString() );
                                }
                                output_audacity_ofstream << chunk_start_time \
                                                         << "\t" << chunk_end_time \
                                                         << "\t" << classes[output_prob_cl_i] <<  " (" \
                                                         << std::trunc(std::round(prob * 100)) / float(100) << ")" << std::endl;
                            }
                        }
                    }

                    if (stop_requested())
                    {
                        output_summary_analyzed_files_csv_ofstream << ",Processing stoped by user.,";
                        output_summary_analyzed_files_csv_ofstream << id;
                        output_summary_analyzed_files_csv_ofstream << std::endl;
                        emit finished();
                        return;
                    }

                    samples_read_to_date += n_samples_to_read;

                    if (samples_read_to_date < n_audio_samples)
                    {
                        n_samples_to_read = nmels * hop_length;
                        std::rotate(audio_samples.rbegin(), audio_samples.rbegin() + int(fft_size - hop_length), audio_samples.rend()); // Shift to the right by fft_size - hop_length
                        audio_samples_seek_it = audio_samples.begin() + int(fft_size - hop_length);
                    }

                    progress += progress_inc_file / float(n_audio_chunks) * progress_inc_nn_pred;
                    emit process_file(progress);
                }
                else
                {
                    output_summary_analyzed_files_csv_ofstream << ",A read error occurred while reading this file.,";
                    output_summary_analyzed_files_csv_ofstream << id;
                    output_summary_analyzed_files_csv_ofstream << std::endl;
                    read_error = true;
                    break;
                }
            }

            n_total_id += id;

            if (!read_error)
            {
                std::time (&this_file_analysis_end_time);
                output_summary_analyzed_files_csv_ofstream << ",Processed in ";
                output_summary_analyzed_files_csv_ofstream << std::difftime(this_file_analysis_end_time, this_file_analysis_start_time);
                output_summary_analyzed_files_csv_ofstream << "s,";
                output_summary_analyzed_files_csv_ofstream << id;
                output_summary_analyzed_files_csv_ofstream << " identifications";
                output_summary_analyzed_files_csv_ofstream << std::endl;
            }

            // Free memory used by audio_samples
            std::vector<double>().swap(audio_samples);

            if (output_audacity_ofstream.is_open())
            {
                output_audacity_ofstream.close();
            }

            emit process_file(progress);
        }

        if (output_summary_results_csv_ofstream.is_open())
        {
           output_summary_results_csv_ofstream.close();
        }

        if (output_summary_analyzed_files_csv_ofstream.is_open())
        {
           std::time(&analysis_end_time);
           output_summary_analyzed_files_csv_ofstream << "Analysis finished in ";
           output_summary_analyzed_files_csv_ofstream << std::difftime(analysis_end_time, analysis_start_time);
           output_summary_analyzed_files_csv_ofstream << "s. Total identifications : ";
           output_summary_analyzed_files_csv_ofstream << n_total_id;
           output_summary_analyzed_files_csv_ofstream << std::endl;
           output_summary_analyzed_files_csv_ofstream.close();
        }

        emit process_file(100);
        emit finished();
    }

    void received_stop_signal()
    {
        QMutexLocker locker(&mutex);
        stop = true;
    }

signals:
    void finished();
    void read_file(float value);
    void read_file(QString filename);
    void process_file(float value);
    void process_file(QString filename);

private:
    bool stop_requested()
    {
        QMutexLocker locker(&mutex);
        return stop;
    }

    bool read_audio(SndfileHandle snd_file, std::vector<double>::iterator audio_samples_seek_it, size_t n_samples_to_read);

    tensorflow::SavedModelBundle model;
    QMutex mutex;
    bool stop;
};

#endif // WORKER_THREAD_H
