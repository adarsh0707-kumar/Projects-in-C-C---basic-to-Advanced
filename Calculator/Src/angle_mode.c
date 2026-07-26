#include "angle_mode.h"

/* Radians is the mathematical default and matches existing behavior,
   so switching this module in doesn't change any pre-existing result
   unless the user explicitly runs 'mode deg'. */
static AngleMode currentMode = MODE_RADIAN;

void setAngleMode(AngleMode mode)
{
    currentMode = mode;
}

AngleMode getAngleMode(void)
{
    return currentMode;
}

const char *angleModeName(void)
{
    return (currentMode == MODE_DEGREE) ? "DEG" : "RAD";
}