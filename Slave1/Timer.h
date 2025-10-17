/*
 * Timer.h
 *
 * Created: 2/6/2024 4:30:02 AM
 *  Author: VTE
 */ 


#ifndef TIMER_H_
#define TIMER_H_
#include "std_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
void timer0_overflow(void);
void timer_CTC_init_interrupt(void);
void timer1_overflow(void);
void timer1_ctc(void);
void timer2_overflow(void);
void timer2_ctc(void);
void timer2_overflow_External_clock(void);
void timer_wave_nonPWM(void);
void timer_wave_fastPWM(void);
void timer_wave_phasecorrectPWM(void);
void timer1_wave_fastPWM_B(double value);
void timer1_wave_fastPWM_A(double value);
void timer2_overflow_init_interrupt(void);

#endif /* TIMER_H_ */