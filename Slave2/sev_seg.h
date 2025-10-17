/*
 * _7seg.h
 *
 * Created: 10/16/2023 2:29:33 PM
 *  Author: VTE
 */ 


#ifndef SEV_SEG_H_
#define SEV_SEG_H_
void seven_seg_vinit(unsigned char port_name);
void seven_seg_write(unsigned char portname,unsigned char number);
#endif /* SEV_SEG_H_ */