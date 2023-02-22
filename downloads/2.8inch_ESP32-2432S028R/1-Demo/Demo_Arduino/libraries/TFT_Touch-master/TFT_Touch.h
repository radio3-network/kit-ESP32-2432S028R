/*

Library for 2046 controller based TFT touch screen.

Significantly modified by Rowboteer 22/11/15
Renamed TFT_Touch

See original header text at end of file

*/

#ifndef _TFT_Touch_H
#define _TFT_Touch_H

#include "Arduino.h"
#include "Calibration.h"

class TFT_Touch
{
  public:
  TFT_Touch(byte CS_Pin, byte Clk_Pin, byte Din_DIO, byte Dout_DIO);
  boolean Pressed(void);
  uint16_t ReadRawX(void);
  uint16_t ReadRawY(void);
  uint16_t RawX(void);
  uint16_t RawY(void);
  uint16_t X(void);
  uint16_t Y(void);
  uint32_t Zone(void);

  void setResolution(uint16_t xres, uint16_t yres);
  void setCal(uint16_t xmin, uint16_t xmax, uint16_t ymin, uint16_t ymax, uint16_t xres, uint16_t yres, boolean axis);//, boolean xflip, boolean yflip);
  void setRotation(byte rotation);

  int16_t ReadCal(byte param);

  uint16_t _hmin, _hmax, _vmin, _vmax, _hres, _vres;
  boolean _axis, _xyswap, _xflip, _yflip;

  private:

  int16_t _ReadAxis(boolean Axis);

  byte _CS;
  byte _Clk;
  byte _Din;
  byte _Dout;

  int16_t _xraw, _yraw, _xcoord, _ycoord;
  //boolean _XYswap, _Xflip, _Yflip;
  
  void _PulseClock(void);
  uint16_t _ReadData(void);
  void _OutputData(byte Data);
  };

#endif

// Original header

/* FILE:    HCTFT_Touch.h
   DATE:    10/06/14
   VERSION: 0.1
   AUTHOR:  Andrew Davies

Library header for 2046 controller based TFT touch screen.

You may copy, alter and reuse this code in any way you like, but please leave
reference to HobbyComponents.com in your comments if you redistribute this code.
This software may not be used directly for the purpose of selling products that
directly compete with Hobby Components Ltd's own range of products.

THIS SOFTWARE IS PROVIDED "AS IS". HOBBY COMPONENTS MAKES NO WARRANTIES, WHETHER
EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE, ACCURACY OR LACK OF NEGLIGENCE.
HOBBY COMPONENTS SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR ANY DAMAGES,
INCLUDING, BUT NOT LIMITED TO, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY
REASON WHATSOEVER.
*/