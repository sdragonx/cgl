/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 spectrogram.hpp

 2020-11-09 02:00:54

 HZ范围 26个
 const int METER_FREQUENCY[] = {
 30, 60, 100, 160, 240, 300, 350, 400,
 440, 500, 600, 800, 1000, 1500, 2000, 2600,
 3000, 4000, 6000, 8000, 10000, 14000, 16000,18000,
 20000,24000 };

 男声频率大约为80－600Hz，女声频率大约在160-1200Hz

*/
#ifndef SPECTROGRAM_HPP_20201109020054
#define SPECTROGRAM_HPP_20201109020054

#include <cgl/math/fft.hpp>
#include "al.hpp"

namespace cgl{
namespace al{


/* 绘制音频频谱
 *
 */
template<typename T, int fft_size, int SIZE>
class spectrogram
{
public:
    typedef T value_type;

    const static int FFT_SIZE = fft_size;
    const static int GRAPH_SIZE = SIZE;

    int METER[GRAPH_SIZE];
    value_type graph[GRAPH_SIZE];
    math::complex<value_type> fft_buf[FFT_SIZE];
public:
    spectrogram()
    {
    }

    int size()const
    {
        return GRAPH_SIZE;
    }

    value_type operator[](int i)
    {
        return graph[i];
    }

    //计算每个频点对应坐标，samplerate为采样率
    //最小频率  20.0
    //最大频率  22000.0;
    void calc_meter(double samplerate, double min_sample = 80.0, double max_sample = 22000.0)
    {
        double sampleratePoint;
        for (int i = 0; i < GRAPH_SIZE; ++i) {
            double F = pow(max_sample / min_sample, 1.0 / GRAPH_SIZE);
            sampleratePoint = min_sample * pow(F, i);
            //计算每一个频点的位置
            METER[i] = static_cast<int>(sampleratePoint / (samplerate / FFT_SIZE));
            //(samplerate / (FFT_SIZE * 8.0)));
        }
    }

    void compute(short* buf)
    {
        int i;
        for (i = 0; i < FFT_SIZE; i++) {
            fft_buf[i].real = buf[i] * HammingWindow(i, FFT_SIZE);
            fft_buf[i].imag = 0.0;
        }

        //快速傅里叶处理
        fft(fft_buf, FFT_SIZE);

        //低频数据需要更多的采样，才能完美表达，有文章用的8倍数据

        math::complex<value_type> c;
        value_type value;
        value_type min_value = FLT_MAX;
        value_type max_value = FLT_MIN;
        for (i = 0; i < GRAPH_SIZE; ++i) {
            int k = METER[i];
            c = fft_buf[k];
            //结果乘以波形频率值，再除以32768.0
            c *= METER[i] * 0.000030517578125;
            value = c.real * c.real + c.imag * c.imag;
            //除以64000，不确定是否最终结果是1000最大值
            //将最终值
            graph[i] = sqrt(value) * 0.000015625;
            //if(graph[i] > 400)graph[i] = 400;
            if(graph[i] < min_value)min_value = graph[i];
            if(graph[i] > max_value)max_value = graph[i];
        }

//        if(max_value)
//        for(int i=0; i<GRAPH_SIZE; ++i){
//            graph[i] = graph[i] * 100.0 / max_value;
//        }

        //测试最大值
//        static int mm = 0;
//        if(max_value > mm)mm = max_value;
//        print(mm, max_value);
    }

    void compute(wavfile& wav, int offset)
    {
        const size_t size = FFT_SIZE;

        //范围检查
        if(wav.samples() - offset < FFT_SIZE){
            return ;
        }

        int data[size];
        int value;
        union{
            const int8_t*  data8;
            const int16_t* data16;
        };

        switch(wav.format()){
        case AL_FORMAT_MONO8:
            data8 = reinterpret_cast<const int8_t*>(wav.data());
            for (size_t i = 0; i < size; ++i){
                data[i] = static_cast<value_type>(data8[offset + i] << 6);// Out = (In-128)*64
            }
            break;
        case AL_FORMAT_MONO16:
            data16 = reinterpret_cast<const int16_t*>(wav.data());
            for (size_t i = 0; i < size; ++i){
                data[i] = data16[offset + i];
            }
            break;
        case AL_FORMAT_STEREO8:
            data8 = reinterpret_cast<const int8_t*>(wav.data());
            offset *= 2;
            for(size_t i=0; i < size; ++i){
                value = stereo_to_mono(data8[offset + i * 2], data8[offset + i * 2 + 1]);
                data[i] = pcm_8bit_to_16bit(value);
            }
            break;
        case AL_FORMAT_STEREO16:
            data16 = reinterpret_cast<const int16_t*>(wav.data());
            offset *= 2;
            for(size_t i=0; i < size; ++i){
                data[i] = stereo_to_mono(data16[offset + i * 2], data16[offset + i * 2 + 1]);
            }
            break;
        }

        //计算每个频段的频率值，这个只需要计算一次就行
        wav_view.calc_meter(wav.frequency());

        compute(data);
    }

    void compute(alsound& sound)
    {
        albuffer buf(sound.buffer());
        int sample_size = buf.channels() * buf.bits() / 8;
        void* data = alMapBufferSOFT(s.buffer(),
            s.sample_offset() * sample_size,
            FFT_SIZE * sample_size,
            AL_MAP_READ_BIT_SOFT|AL_MAP_PERSISTENT_BIT_SOFT);
        if(data){
            ALenum format = wave_format(buf.channels(), buf.bits());
            wav_view.compute(data, format, 0);
        }
        alUnmapBufferSOFT(s.buffer());
    }

    void compute(void* pvoid, int format, int offset)
    {
        const size_t size = FFT_SIZE;

        short data[size];
        int value;
        union{
            const int8_t*  data8;
            const int16_t* data16;
        };

        switch(format){
        case AL_FORMAT_MONO8:
            data8 = reinterpret_cast<const int8_t*>(pvoid);
            for (size_t i = 0; i < size; ++i){
                data[i] = static_cast<value_type>(data8[offset + i] << 6);// Out = (In-128)*64
            }
            break;
        case AL_FORMAT_MONO16:
            data16 = reinterpret_cast<const int16_t*>(pvoid);
            for (size_t i = 0; i < size; ++i){
                data[i] = data16[offset + i];
            }
            break;
        case AL_FORMAT_STEREO8:
            data8 = reinterpret_cast<const int8_t*>(pvoid);
            offset *= 2;
            for(size_t i=0; i < size; ++i){
                value = stereo_to_mono(data8[offset + i * 2], data8[offset + i * 2 + 1]);
                data[i] = pcm_8bit_to_16bit(value);
            }
            break;
        case AL_FORMAT_STEREO16:
            data16 = reinterpret_cast<const int16_t*>(pvoid);
            offset *= 2;
            for(size_t i=0; i < size; ++i){
                data[i] = stereo_to_mono(data16[offset + i * 2], data16[offset + i * 2 + 1]);
            }
            break;
        }

        //计算每个频段的频率值，这个只需要计算一次就行
        wav_view.calc_meter(wav.frequency());

        compute(data);
    }

private:
    // Applies a Hamming Window
    double HammingWindow(int n, int frameSize)
    {
        return 0.54 - 0.46 * cos((2 * M_PI * n) / (frameSize - 1));
    }

    // Applies a Hann Window
    double HannWindow(int n, int frameSize)
    {
        return 0.5 * (1 - cos((2 * M_PI * n) / (frameSize - 1)));
    }

    // Applies a Blackman-Harris Window
    double BlackmannHarrisWindow(int n, int frameSize)
    {
        return 0.35875 - (0.48829 * cos((2 * M_PI * n) / (frameSize - 1))) + (0.14128 * cos((4 * M_PI * n) / (frameSize - 1))) - (0.01168 * cos((6 * M_PI * n) / (frameSize - 1)));
    }
};

}//end namespace al
}//end namespace cgl

#endif //SPECTROGRAM_HPP_20201109020054
