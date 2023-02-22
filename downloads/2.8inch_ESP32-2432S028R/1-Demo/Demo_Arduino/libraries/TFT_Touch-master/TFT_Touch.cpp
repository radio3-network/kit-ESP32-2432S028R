/*

Library for 2046 controller based TFT touch screen.

Significantly modified by Rowboteer 22/11/15
Renamed TFT_Touch

See original header text at end of file
*/


#include "Arduino.h"
#include "TFT_Touch.h"

/* Constructor to initialise the GPIO */
TFT_Touch::TFT_Touch(byte CS_Pin, byte Clk_Pin, byte Din_Pin, byte Dout_Pin)
{
  _CS = CS_Pin;
  _Clk = Clk_Pin;
  _Din = Din_Pin;
  _Dout = Dout_Pin;

  pinMode(_CS, OUTPUT); 
  pinMode(_Clk, OUTPUT); 
  pinMode(_Din, OUTPUT); 
  pinMode(_Dout, INPUT); 
  
  digitalWrite(_CS, HIGH);  
  digitalWrite(_Clk, LOW);
  digitalWrite(_Din, LOW);

  _hmin = 0;
  _hmax = 4095;
  _vmin = 0;
  _vmax = 4095;
  _hres = 320;
  _vres = 240;

  _xyswap = 0;
  _xflip = 0;
  _yflip = 0;

  _axis = 0;

  //_XYswap = 0;
  //_Xflip  = 0;
  //_Yflip  = 0;
}

/* Detects if the touch screen is currently pressed. Returns TRUE if pressed */
boolean TFT_Touch::Pressed(void)
{

  // Get the raw contact coordinates
  _xraw = _ReadAxis(_axis);
  _yraw = _ReadAxis(!_axis);

  // Now double check the touch is still near the initial contact point
  // This helps to debounce the touch contact
  // We are working with signed integers intentionally
  delay(1);
  if (abs(_xraw - _ReadAxis(_axis)) > _RAWERR) return false;
  if (abs(_yraw - _ReadAxis(!_axis)) > _RAWERR) return false;
  delay(1);
  if (abs(_xraw - _ReadAxis(_axis)) > _RAWERR) return false;
  if (abs(_yraw - _ReadAxis(!_axis)) > _RAWERR) return false;

/*
  // Get the raw contact coordinates, temporary test to emulate a flip and rotation
  _xraw = _ReadAxis(!_axis);
  _yraw = 4095 - _ReadAxis(_axis);

  // Now double check the touch is still near the initial contact point
  // This helps to debounce the touch contact
  // We are working with signed integers intentionally
  delay(1);
  if (abs(_xraw - _ReadAxis(!_axis)) > _RAWERR) return false;
  if (abs(_yraw - (4095 - _ReadAxis(_axis))) > _RAWERR) return false;
  delay(1);
  if (abs(_xraw - _ReadAxis(!_axis)) > _RAWERR) return false;
  if (abs(_yraw - (4095 - _ReadAxis(_axis))) > _RAWERR) return false;
*/

  // Check values are in calibration range
  //if((_yraw > _vmin && _yraw < _vmax) && (_xraw > _hmin && _xraw < _hmax))
  if((_yraw > 0 && _yraw < 4095) && (_xraw > 0 && _xraw < 4095))
  {
    if (_xyswap){
      _xcoord = constrain(map(_yraw, _vmin, _vmax, 30, _vres-30), 0, _vres);
      _ycoord = constrain(map(_xraw, _hmin, _hmax, 30, _hres-30), 0, _hres);
      if(_xflip) _xcoord = _vres - _xcoord;
      if(_yflip) _ycoord = _hres - _ycoord;
    }
    else
    {
      _xcoord = constrain(map(_xraw, _hmin, _hmax, 30, _hres-30), 0, _hres);
      _ycoord = constrain(map(_yraw, _vmin, _vmax, 30, _vres-30), 0, _vres);
      if(_xflip) _xcoord = _hres - _xcoord;
      if(_yflip) _ycoord = _vres - _ycoord;
    }

    return true; 
  }else
  {
    return false;
  } 
}

/* Reads one of the axis (XAXIS or YAXIS) raw coordinates. */
int16_t TFT_Touch::_ReadAxis(boolean Axis)
{
  int Data;
  
  digitalWrite(_CS, LOW);
  if (Axis)
  {
    _OutputData(0x90);   
  }else
  {
    _OutputData(0xD0);
  }

  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  Data = _ReadData();
  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  digitalWrite(_CS, HIGH); 
  digitalWrite(_Din, LOW);
  
  return Data;
}

/* Reads the raw data from the touch screen */
uint16_t TFT_Touch::_ReadData(void)
{
  byte index;
  int Data;
  Data = 0;
  for (index = 12; index > 0; index--)
  {
    Data += digitalRead(_Dout) << (index-1);
    digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  }
  return Data;
}  

/* Writes to the touch screen's configuration register */
void TFT_Touch::_OutputData(byte Data)
{
  byte index;
  
  for (index = 8; index > 0; index--)
  {
    digitalWrite(_Din, (Data >> (index -1)) & 1);
    digitalWrite(_Clk, HIGH); digitalWrite(_Clk, LOW); //_PulseClock();
  }
}

/* Read the current position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::ReadRawX(void)
{
  return _ReadAxis(_axis);;
}

/* Read the current position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::ReadRawY(void)
{
  return _ReadAxis(!_axis);;
}

/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::RawX(void)
{
  return _xraw;
}

/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::RawY(void)
{
  return _yraw;
}

/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::X(void)
{
  return _xcoord;
}

/* Read the last position from the touch screen and return it as a pixel position. */
uint16_t TFT_Touch::Y(void)
{
  return _ycoord;
}

/* Read the last zone from the touch screen and return it as a pixel position. */
uint32_t TFT_Touch::Zone(void)
{
  return _xcoord + (uint32_t)_ycoord *_hres;
}

/* Set the screen resolution in pixels. */
void TFT_Touch::setResolution(uint16_t hres, uint16_t vres)
{
  _hres = hres;
  _vres = vres;
}

/* Set the screen calibration values */
void TFT_Touch::setCal(uint16_t hmin, uint16_t hmax,
                       uint16_t vmin, uint16_t vmax,
                       uint16_t hres, uint16_t vres,
                       bool axis)//, bool xflip, bool yflip)
{
  _hmin = hmin;
  _hmax = hmax;
  _vmin = vmin;
  _vmax = vmax;
  _hres = hres;
  _vres = vres;

  _axis   = axis;
  //_Xflip  = xflip;
  //_Yflip  = yflip;
}

  int16_t TFT_Touch::ReadCal(byte param)
{
  switch(param) {
    case 1:
      return _hmin;
      break;
    case 2:
      return _hmax;
      break;
    case 3:
      return _vmin;
      break;
    case 4:
      return _vmax;
      break;
    case 5:
      return _hres;
      break;
    case 6:
      return _vres;
      break;
    case 7:
      return _axis;
      break;
    //case 8:
    //  return _xflip;
    //  break;
    //case 9:
    //  return _yflip;
    //  break;
  }
}
void TFT_Touch::setRotation(byte rotation)
{
  switch(rotation) {
    case 1:
      _xyswap  = 0;
      _xflip = 0;//_Xflip;
      _yflip = 0;//_Yflip;
      break;
    case 2:
      _xyswap  = 1;
      _xflip = 0;//_Xflip;
      _yflip = 1;//!_Yflip;
      break;
    case 3:
      _xyswap  = 0;
      _xflip = 1;//!_Xflip;
      _yflip = 1;//!_Yflip;
      break;
    case 0:
      _xyswap  = 1;
      _xflip = 1;//!_Xflip;
      _yflip = 0;//_Yflip;
      break;
  }
}


// Original header

/* FILE:    HCTFT_Touch.h
   DATE:    10/06/14
   VERSION: 0.1
   AUTHOR:  Andrew Davies

Library for 2046 controller based TFT touch screen.

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
