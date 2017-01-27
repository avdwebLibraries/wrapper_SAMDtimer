/*
Copyright (C) 2012  Albert van Dalen http://www.avdweb.nl
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License at http://www.gnu.org/licenses .

Version 26-1-2017

16 bit timer: max period_us = 1398080us (0.7Hz), min period_us = 1us (1MHz)

note *1 SCK, MOSI, MISO are on Arduino Zero SPI header

Timer              Arduino Zero pins              SAM15x15 pins
SAMDtimer 0 8bit   D3 D18/A4 D17 D4               d[9]  d[5]  d[4]  d[8]              timer not yet implemented
SAMDtimer 0 16bit  D3 D18/A4                      d[9]  d[5]                          timer not yet implemented
SAMDtimer 1 8bit   D0 D9 D33 D8 D1 D30            d[11] d[7]  d[31] d[6] d[10] d[27]  timer not yet implemented                
SAMDtimer 1 16bit  D0 D9 D33                      d[11] d[7]  d[31]                   timer not yet implemented
SAMDtimer 2 8bit   D13 (D28) D22/MISO*1 D16       d[19] d[15] d[14] d[3]              timer not yet implemented
SAMDtimer 2 16bit  D13 (D28)                      d[19] d[15]                         timer not yet implemented

SAMDtimer 3 8bit   D5 D12 D2 D10                  d[17] d[21] d[16] d[20]             timer not yet implemented
SAMDtimer 3 16bit  D5 D12                         d[17] d[21]
SAMDtimer 4 8bit   D16/A2 D21/SCL D15/A1 D20/SDA  d[3]  d[25] d[2]  d[24]             timer not yet implemented
SAMDtimer 4 16bit  D16/A2 D21/SCL                 d[3]  d[25] 
SAMDtimer 5 8bit   D24/SCK*1 D23/MOSI*1           d[13] d[12]                         timer not yet implemented
SAMDtimer 5 16bit  D24/SCK*1                      d[13] 
*/

#include "avdweb_SAMDtimer.h"

SAMDtimer::SAMDtimer(byte timerNr, tc_counter_size countersize, byte pin, unsigned period_us, int pulseWidth_us, bool timerEnable): 
Adafruit_ZeroTimer(timerNr), pin(pin), countersize(countersize), period_us(period_us)  
{ if(pulseWidth_us==-1) calc(period_us, period_us/2);
  else calc(period_us, pulseWidth_us);
  init(timerEnable);    
}

SAMDtimer::SAMDtimer(byte timerNr, tc_callback_t _ISR, unsigned period_us, bool ISRenable):
Adafruit_ZeroTimer(timerNr)  
{ ISR = _ISR;
  countersize = TC_COUNTER_SIZE_16BIT; 
  calc(period_us, period_us/2);
  init(1);
  setCallback(ISRenable, TC_CALLBACK_CC_CHANNEL1, ISR); 
}

void SAMDtimer::setPulseWidth(unsigned pulseWidth_us)
{ calc(period_us, pulseWidth_us);
  setPeriodMatch(periodCounter, PWcounter, 1); 
}

void SAMDtimer::attachInterrupt(tc_callback_t _ISR, bool interruptEnable)
{ ISR = _ISR;
  setCallback(interruptEnable, TC_CALLBACK_CC_CHANNEL1, ISR); 
}

void SAMDtimer::enableTimer(bool timerEnable)
{ enable(timerEnable);
}

void SAMDtimer::enableInterrupt(bool interruptEnable)
{ setCallback(interruptEnable, TC_CALLBACK_CC_CHANNEL1, ISR); 
}

void SAMDtimer::init(bool enabled)
{ configure(prescale, countersize, TC_WAVE_GENERATION_MATCH_PWM);
  PWMout(true, 1, pin); // must be ch1 for 16bit
  setPeriodMatch(periodCounter, PWcounter, 1);
  enable(enabled); 
}

void SAMDtimer::calc(unsigned period_us, unsigned pulseWidth_us)
{ periodCounter = (F_CPU * (signed)period_us) / 1000000; // why signed?
  PWcounter = (F_CPU * (signed)pulseWidth_us) / 1000000; 
  if(periodCounter < 65536) prescale = TC_CLOCK_PRESCALER_DIV1; 
  else if((PWcounter >>= 1, periodCounter >>= 1) < 65536) prescale = TC_CLOCK_PRESCALER_DIV2; // = 256
  else if((PWcounter >>= 1, periodCounter >>= 1) < 65536) prescale = TC_CLOCK_PRESCALER_DIV4; 
  else if((PWcounter >>= 1, periodCounter >>= 1) < 65536) prescale = TC_CLOCK_PRESCALER_DIV8; 
  else if((PWcounter >>= 1, periodCounter >>= 1) < 65536) prescale = TC_CLOCK_PRESCALER_DIV16; 
  else if((PWcounter >>= 2, periodCounter >>= 2) < 65536) prescale = TC_CLOCK_PRESCALER_DIV64; 
  else if((PWcounter >>= 2, periodCounter >>= 2) < 65536) prescale = TC_CLOCK_PRESCALER_DIV256; 
  else if((PWcounter >>= 2, periodCounter >>= 2) < 65536) prescale = TC_CLOCK_PRESCALER_DIV1024; 
}





