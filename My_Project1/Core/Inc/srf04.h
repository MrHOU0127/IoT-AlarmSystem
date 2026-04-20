#ifndef __SRF04_H
#define __SRF04_H

#include "main.h"

extern uint32_t SRF04_Time;
extern uint8_t SRF04_Flag;

void SRF04_Start(void);
float SRF04_GetDistance(void);
void SRF04_EXTI_Callback(void);

#endif
