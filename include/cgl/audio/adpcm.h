/* 
** adpcm.h - include file for adpcm coder. 
** 
** Version 1.0, 7-Jul-92. 
*/

//#include <stdint.h>

#ifdef __cplusplus
extern "C"{
#endif

struct adpcm_state { 
	short      valprev;        /* Previous output value */
    char       index;          /* Index into stepsize table */
}__attribute__((packed)); 

typedef struct adpcm_state adpcm_state_t;

void adpcm_coder(const short *, char *, int, struct adpcm_state *);
void adpcm_decoder(const char *, short *, int, struct adpcm_state *);

#ifdef __cplusplus
}
#endif
