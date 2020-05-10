
#ifndef __FSMTRAIN_H__
#define __FSMTRAIN_H__


#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "sapi.h"

typedef enum{
	STATE_DETENIDO,
	STATE_CABECERA,
	STATE_CIRCULANDO,
	STATE_ARRIBANDO,
	STATE_ESTACION
} fsmTrainState_t;

void fsmTrainInit(void);
void fsmTrainUpdate(void);

#ifdef __cplusplus
}
#endif


#endif /* __FSMTRAIN_H__ */
