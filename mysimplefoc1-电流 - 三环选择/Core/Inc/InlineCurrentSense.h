#ifndef INLINE_CS_LIB_H
#define INLINE_CS_LIB_H

#include "foc_utils.h" 


/******************************************************************************/
void InlineCurrentSense(float _shunt_resistor, float _gain, int _pinA, int _pinB, int _pinC);
void InlineCurrentSense_Init(void);
void Current_calibrateOffsets(void);
float _readADCVoltageInline(int ch);
PhaseCurrent_s getPhaseCurrents(void);
/******************************************************************************/


#endif

