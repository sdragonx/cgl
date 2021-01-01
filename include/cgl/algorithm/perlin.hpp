/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 perlin.hpp

 2019-06-25 19:28:38

 柏林噪声处理2的n次方纹理，结果是可以平铺的

*/
#ifndef PERLIN_HPP_20190625192838
#define PERLIN_HPP_20190625192838

#include <cgl/array2d.hpp>
#include <cgl/math/random.hpp>

namespace cgl{

//线性插值
float Linear_Interpolate(float a, float b, float alpha)
{
    //return a * (1 - alpha) + alpha * b;
    return a + (b - a) * alpha;
}

//余弦插值
float Cosine_Interpolate(float a, float b, float alpha)
{
    alpha = (1 - std::cos(alpha * M_PI)) * 0.5f;
    return a + (b - a) * alpha;
}

//立方插值
float CubicLerp(float a, float b, float c, float d, float t)
{
	float p = (d - c) - (a - b);
	return t * t * t * p + t * t * ((a - b) - p) + t * (c - a) + b;
}

#ifdef CGL_PERLIN_COSINE
    #define Interpolate Cosine_Interpolate
#else
    #define Interpolate Linear_Interpolate
#endif

//随机数
float random_xyz(int seed, int x, int y, int z)
{
    int n = seed + x*73 + y*179 + z*283;
    n = (n << 13) ^ n;
    int m = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
    return 1.0f - ((float)m / 1073741824.0f);
}

//根据octave级别，分为2^octave次方块
//octave马赛克
//这个算法，会使地图首位相接
void GenerateSmoothNoise(array2d<float>& smoothNoise, const array2d<float>& baseNoise, int octave)
{
    int width = baseNoise.size_x();
    int height = baseNoise.size_y();

    smoothNoise.resize(width, height);

    int samplePeriod = 1 << octave;
    float sampleFrequency = 1.0f / samplePeriod;
    int mask = samplePeriod - 1;

    int x0, x1, y0, y1;
    for (int y = 0; y < height; ++y){
        //calculate the horizontal sampling indices
        //y0 = (y / samplePeriod) * samplePeriod;
        //y0 = y >> octave << octave;
        y0 = y & ~mask;
        y1 = (y0 + samplePeriod) % height; //wrap around
//        y1 = (y + samplePeriod)%height;
        float vertical_blend = (y - y0) * sampleFrequency;

        for (int x = 0; x < width; ++x){
            //calculate the vertical sampling indices
            //x0 = (x / samplePeriod) * samplePeriod;
            //x0 = x >> octave << octave;
            x0 = x & ~mask;
            x1 = (x0 + samplePeriod) % width; //wrap around
//            x1 = (x + samplePeriod)%width;
            float horizontal_blend = (x - x0) * sampleFrequency;

            //上下左右混合对接
            //blend the top two corners
            float top = Interpolate(baseNoise[y0][x0], baseNoise[y1][x0], vertical_blend);

            //blend the bottom two corners
            float bottom = Interpolate(baseNoise[y0][x1], baseNoise[y1][x1], vertical_blend);

            //final blend
            smoothNoise[y][x] = Interpolate(top, bottom, horizontal_blend);
        }
    }
}

//计算每个级别单独函数
float SmoothNoise(int x, int y, const array2d<float>& baseNoise, int octave)
{
    int width = baseNoise.size_x();
    int height = baseNoise.size_y();

    int samplePeriod = 1 << octave;
    float sampleFrequency = 1.0f / samplePeriod;

    int sample_i0 = y >> octave << octave;
    int sample_i1 = (sample_i0 + samplePeriod) % width; //wrap around
    float horizontal_blend = (y - sample_i0) * sampleFrequency;
    //CGL_LOG("%f", horizontal_blend);

    int sample_j0 = x >> octave << octave;
    int sample_j1 = (sample_j0 + samplePeriod) % height; //wrap around
    float vertical_blend = (x - sample_j0) * sampleFrequency;

    float top = Linear_Interpolate(baseNoise[sample_i0][sample_j0],
                baseNoise[sample_i1][sample_j0], horizontal_blend);

    //blend the bottom two corners
    float bottom = Linear_Interpolate(baseNoise[sample_i0][sample_j1],
        baseNoise[sample_i1][sample_j1], horizontal_blend);

    return Linear_Interpolate(top, bottom, vertical_blend);

}

void GeneratePerlinNoise(array2d<float>& perlinNoise, const array2d<float>& baseNoise,
    int octaveCount,
    float persistance = 0.5f)
{
    int width = baseNoise.size_x();
    int height = baseNoise.size_y();

    perlinNoise.resize(width, height);

    //smooth noise
    array2d<float> smoothNoise;

    // 不同采样级别的 Noise 的叠加系数
    //float persistance = 0.80;//75f;
    // 不同采样级别的 Noise 的叠加比重
    float amplitude = 1.0f;
    // 所有采样级别的 Noise 的叠加总比重
    float totalAmplitude = 0.0f;

    //blend noise together
    for (int octave = octaveCount - 1; octave >= 0; --octave){
        amplitude *= persistance;
        totalAmplitude += amplitude;

        // 生成不同采样级别的 Noise
        GenerateSmoothNoise(smoothNoise, baseNoise, octave);

        // 混合采样级别的 Noise
        for (int i = 0; i <width; i++){
            for (int j = 0; j < height; j++){
                perlinNoise[i][j] += smoothNoise[i][j] * amplitude;
                //perlinNoise[i][j] += SmoothNoise(j, i, baseNoise, octave) * amplitude;
            }
        }
    }

    //normalisation
    float invTotalAmplitude = 1.0f / totalAmplitude;
    for (int i = 0; i <width; i++){
        for (int j = 0; j <height; j++){
            perlinNoise[i][j] *= invTotalAmplitude;
        }
    }
}

//生成随机噪声
void perlin_noise_init(array2d<float>& base, size_t width, size_t height, int seed)
{
    base.resize(width, height);

    //*
    math::basic_random rnd(seed);
    for (size_t y = 0; y < height; ++y){
        for (size_t x = 0; x < width; ++x){
            base[y][x] = float(rnd.get()) / CGL_RANDOM_MAX;
        }
    }
    /*/
    srand(seed);
    for (size_t y = 0; y < base.size_y(); ++y){
        for (size_t x = 0; x < base.size_x(); ++x){
            base[y][x] = float(rand()) / RAND_MAX;
        }
    }
    //*/
}


//这个算法，归一化是个问题，分部不均匀
//滤波结果，首尾是不重复的
class perlin_noise
{
public:
    byte_t p[512];

    void init(int seed)
    {
        math::basic_random random(seed);
        for(int i=0; i<256; ++i){
            p[i] = random.uniform(256);
        }
        std::copy(p, p + 256, p + 256);
    }

    //noise 1D
    float noise(float x)
    {
        int X = (int) floor(x) & 255;
        x -= floor(x);
        float u = fade(x);
        return lerp(u, grad(p[X], x), grad(p[X+1], x-1)) * 2;
    }

    //noise 2D
    float noise(float x, float y)
    {
        int X = (int) floor(x) & 255;
        int Y = (int) floor(y) & 255;
        x -= floor(x);
        y -= floor(y);

        float u = fade(x);
        float v = fade(y);

        int A = (p[X  ] + Y) & 0xff;
        int B = (p[X+1] + Y) & 0xff;

        float n = lerp(v,
            lerp(u, grad(p[A  ], x, y  ), grad(p[B  ], x-1, y  )),
            lerp(u, grad(p[A+1], x, y-1), grad(p[B+1], x-1, y-1)));

        return n * 0.5 + 0.5; //[0-1]
    }

    //noise 3D
    float noise(float x, float y, float z)
    {
        // Find the unit cube that contains the point
        int X = (int) floor(x) & 255;
        int Y = (int) floor(y) & 255;
        int Z = (int) floor(z) & 255;

        // Find relative x, y,z of point in cube
        x -= floor(x);
        y -= floor(y);
        z -= floor(z);

        // Compute fade curves for each of x, y, z
        float u = fade(x);
        float v = fade(y);
        float w = fade(z);

        // Hash coordinates of the 8 cube corners
        int A  = p[X    ] + Y;
        int AA = p[A    ] + Z;
        int AB = p[A + 1] + Z;
        int B  = p[X + 1] + Y;
        int BA = p[B    ] + Z;
        int BB = p[B + 1] + Z;

        // Add blended results from 8 corners of cube
        float n =
            lerp(w,
                lerp(v,
                    lerp(u, grad(p[AA], x, y, z), grad(p[BA], x-1, y, z)),
                    lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z))
                ),
                lerp(v,
                    lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1)),
                    lerp(u, grad(p[AB+1], x, y-1, z-1),	grad(p[BB+1], x-1, y-1, z-1))
                )
            );
        return n * 0.5 + 0.5;
    }

    float Fbm(float x, int octave)
    {
        int f = 0.0f;
        int w = 0.5f;
        for (int i = 0; i < octave; i++) {
            f += w * noise(x);
            x *= 2.0f;
            w *= 0.5f;
        }
        return f;
    }

    float octave_noise(float x, float y, int octave)
    {
        float f = 0.0f;
        float amplitude = 1.0f;// 0.5f;
        float total = 0;
        for (int i = 0; i < octave; i++) {
            f += noise(x, y) * amplitude;
            x *= 2.0f;
            y *= 2.0f;
            total += amplitude;
            amplitude *= 0.5;
        }
        return f / total;
    }

    float Fbm(float x, float y, float z, int octave)
    {
        float f = 0.0f;
        float w = 0.5f;
        for (int i = 0; i < octave; i++) {
            f += w * noise(x, y, z);
            x *= 2.0f;
            y *= 2.0f;
            z *= 2.0f;
            w *= 0.5f;
        }
        return f;
    }

private:
    float fade(float t)
    {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    float lerp(float t, float a, float b) {
        return a + t * (b - a);
//        return Cosine_Interpolate(a, b, t);
    }

    float grad(int hash, float x)
    {
        return (hash & 1) == 0 ? x : -x;
    }

    float grad(int hash, float x, float y)
    {
        return ((hash & 1) == 0 ? x : -x) + ((hash & 2) == 0 ? y : -y);
    }

    /*
    float grad(int hash, float x, float y, float z)
    {
        int h = hash & 15;
        // Convert lower 4 bits of hash into 12 gradient directions
        float u = h < 8 ? x : y;
        float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }
    /*/

    float grad(int hash, float x, float y, float z)
    {
        switch(hash & 0xF){
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        case 0x4: return  x + z;
        case 0x5: return -x + z;
        case 0x6: return  x - z;
        case 0x7: return -x - z;
        case 0x8: return  y + z;
        case 0x9: return -y + z;
        case 0xA: return  y - z;
        case 0xB: return -y - z;
        case 0xC: return  y + x;
        case 0xD: return -y + z;
        case 0xE: return  y - x;
        case 0xF: return -y - z;
        default: return 0; // never happens
        }
    }
    //*/

};

}//end namespace cgl

#endif //PERLIN_HPP_20190625192838
