/**
 * @file angle_mode.h
 * @brief Global degree/radian mode for trigonometric functions.
 */
#ifndef ANGLE_MODE_H
#define ANGLE_MODE_H

/** The unit trig functions interpret their arguments in. */
typedef enum
{
    MODE_RADIAN, /**< Default; matches the C math library's convention. */
    MODE_DEGREE  /**< e.g. sin(90) == 1 instead of sin(pi/2) == 1. */
} AngleMode;

/** @brief Sets the current global angle mode. */
void setAngleMode(AngleMode mode);
/** @brief Returns the current global angle mode. */
AngleMode getAngleMode(void);
/** @brief Returns a short display label: "DEG" or "RAD". */
const char *angleModeName(void);

#endif
