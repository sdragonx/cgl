#ifndef ADPCM_H
#define ADPCM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* IMA ADPCM Stepsize table */
static const long IMAStep_size[89] = {
       7,    8,    9,   10,   11,   12,   13,   14,   16,   17,   19,
      21,   23,   25,   28,   31,   34,   37,   41,   45,   50,   55,
      60,   66,   73,   80,   88,   97,  107,  118,  130,  143,  157,
     173,  190,  209,  230,  253,  279,  307,  337,  371,  408,  449,
     494,  544,  598,  658,  724,  796,  876,  963, 1060, 1166, 1282,
    1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660,
    4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493,10442,
   11487,12635,13899,15289,16818,18500,20350,22358,24633,27086,29794,
   32767
};

/* IMA4 ADPCM Codeword decode table */
static const long IMA4Codeword[16] = {
    1, 3, 5, 7, 9, 11, 13, 15,
   -1,-3,-5,-7,-9,-11,-13,-15,
};

/* IMA4 ADPCM Step index adjust decode table */
static const long IMA4Index_adjust[16] = {
   -1,-1,-1,-1, 2, 4, 6, 8,
   -1,-1,-1,-1, 2, 4, 6, 8
};

inline int32_t clampi(int32_t val, int32_t min, int32_t max)
{
	return val < min ? min : (val > max ? max : val);
}

static void DecodeIMA4Block(int16_t *dst, const int8_t *src, int32_t numchans)
{
	const int MAXCHANNELS = 10;
	int32_t sample[MAXCHANNELS], index[MAXCHANNELS];
	uint32_t code[MAXCHANNELS];
	int32_t j,k,c;

    for(c = 0;c < numchans;c++)
    {
        sample[c]  = *(src++);
        sample[c] |= *(src++) << 8;
        sample[c]  = (sample[c]^0x8000) - 32768;
        index[c]  = *(src++);
        index[c] |= *(src++) << 8;
        index[c]  = (index[c]^0x8000) - 32768;

		index[c] = clampi(index[c], 0, 88);

        dst[c] = sample[c];
    }

    j = 1;
    while(j < 65)
    {
        for(c = 0;c < numchans;c++)
        {
            code[c]  = *(src++);
            code[c] |= *(src++) << 8;
            code[c] |= *(src++) << 16;
            code[c] |= *(src++) << 24;
        }

        for(k = 0;k < 8;k++,j++)
        {
            for(c = 0;c < numchans;c++)
            {
                int nibble = code[c]&0xf;
                code[c] >>= 4;

                sample[c] += IMA4Codeword[nibble] * IMAStep_size[index[c]] / 8;
                sample[c] = clampi(sample[c], -32768, 32767);

                index[c] += IMA4Index_adjust[nibble];
                index[c] = clampi(index[c], 0, 88);

                dst[j*numchans + c] = sample[c];
            }
        }
    }
}

static void EncodeIMA4Block(int8_t *dst, const int16_t *src, int32_t *sample, int32_t *index, int32_t numchans)
{
	int32_t j,k,c;

    for(c = 0;c < numchans;c++)
    {
        int diff = src[c] - sample[c];
        int step = IMAStep_size[index[c]];
        int nibble;

        nibble = 0;
        if(diff < 0)
        {
            nibble = 0x8;
            diff = -diff;
        }

		diff = std::min(step*2, diff);
        nibble |= (diff*8/step - 1) / 2;

        sample[c] += IMA4Codeword[nibble] * step / 8;
        sample[c] = clampi(sample[c], -32768, 32767);

        index[c] += IMA4Index_adjust[nibble];
        index[c] = clampi(index[c], 0, 88);

        *(dst++) = sample[c] & 0xff;
        *(dst++) = (sample[c]>>8) & 0xff;
        *(dst++) = index[c] & 0xff;
        *(dst++) = (index[c]>>8) & 0xff;
    }

    j = 1;
    while(j < 65)
    {
        for(c = 0;c < numchans;c++)
        {
            for(k = 0;k < 8;k++)
            {
                int diff = src[(j+k)*numchans + c] - sample[c];
                int step = IMAStep_size[index[c]];
                int nibble;

                nibble = 0;
                if(diff < 0)
                {
                    nibble = 0x8;
                    diff = -diff;
                }

				diff = std::min(step*2, diff);
                nibble |= (diff*8/step - 1) / 2;

                sample[c] += IMA4Codeword[nibble] * step / 8;
                sample[c] = clampi(sample[c], -32768, 32767);

                index[c] += IMA4Index_adjust[nibble];
                index[c] = clampi(index[c], 0, 88);

                if(!(k&1)) *dst = nibble;
                else *(dst++) |= nibble<<4;
            }
        }
        j += 8;
    }
}

#ifdef __cplusplus
}
#endif

#endif /* ADPCM_H*/

