/*
 Copyright (c) 2005-2020 sdragonx (mail:sdragonx@foxmail.com)

 fft.hpp

 2020-11-09 01:05:42

 ���ٸ���Ҷ�任
 Fast Fourier Transformation

*/
#ifndef FFT_HPP_20201109010542
#define FFT_HPP_20201109010542

#include <cgl/math/complex.hpp>

namespace cgl{

/*
void fft(cp *a,int n,int inv)
{
    int bit=0;
    while ((1<<bit)<n)bit++;
    fo(i,0,n-1)
    {
        rev[i]=(rev[i>>1]>>1)|((i&1)<<(bit-1));
        if (i<rev[i])swap(a[i],a[rev[i]]);//��������if�ύ�����Σ�����û������
    }
    for (int mid=1;mid<n;mid*=2)//mid��׼���ϲ����еĳ��ȵĶ���֮һ
    {
        cp temp(cos(pi/mid),inv*sin(pi/mid));//��λ����pi��ϵ��2�Ѿ�Լ����
        for (int i=0;i<n;i+=mid*2)//mid*2��׼���ϲ����еĳ��ȣ�i�Ǻϲ�������һλ
        {
            cp omega(1,0);
            for (int j=0;j<mid;j++,omega*=temp)//ֻɨ��벿�֣��õ��Ұ벿�ֵĴ�
            {
                cp x=a[i+j],y=omega*a[i+j+mid];
                a[i+j]=x+y,a[i+j+mid]=x-y;//������Ǻ����任ʲô��
            }
        }
    }
}
*/

int fft(math::complex<double> buf[], int n)
{
    int i, j, k;
    math::complex<double> s;
    math::complex<double> t;
    math::complex<double> u;

    //int M = (int) (log(n) / log(2));

    // bit reversal sorting
    j = n / 2;
    // ���Ʒ�ת�����򣬴ӵ�Ƶ����Ƶ
    for (i = 1; i <= n - 2; i++) {
        if (i < j) {
            std::swap(buf[i], buf[j]);
        }
        k = n / 2;
        while (k <= j) {
            j = j - k;
            k = k / 2;
        }
        j = j + k;
    }

    // For Loops
    for(int step = 1; step <= n; step *= 2){
        k = step / 2;
        u = 1.0;
        s.real = cos(M_PI / k);// cos��sin���Ĵ�����ʱ�䣬�����ö�ֵ
        s.imag = -sin(M_PI / k);
        for (j = 0; j < k; ++j) { // loop for each sub DFT
            for (i = j; i < n; i += step) {// loop for each butterfly
                t = buf[i + k] * u;
                buf[i + k] = buf[i] - t;
                buf[i] += t;
            }
            u *= s;
        }
    }

    return 0;
}

}//end namespace cgl

#endif //FFT_HPP_20201109010542