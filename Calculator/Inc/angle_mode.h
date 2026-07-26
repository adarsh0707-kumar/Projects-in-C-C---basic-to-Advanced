#ifndef ANGLE_MODE_H
#define ANGLE_MODE_H

typedef enum
{
    MODE_RADIAN,
    MODE_DEGREE
} AngleMode;

void setAngleMode(AngleMode mode);
AngleMode getAngleMode(void);
const char *angleModeName(void);

#endif