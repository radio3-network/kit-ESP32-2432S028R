#ifndef _Calibration_H
#define _Calibration_H

// When a touch is detected the library samples x and y twice
// The two x and two y values must be within RAWERR of each other
// otherwise the touch is rejected as noisy.
// Define +/- raw coord error limit for a press
// A value of about 10 seems good

#define _RAWERR 10

#endif