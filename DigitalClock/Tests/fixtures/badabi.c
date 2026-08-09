/******************************************************************************
 * @file badabi.c
 * @brief A plugin built for the wrong ABI, so the loader's refusal is tested.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Reports an interface version the application does not implement. The point
 * is that the loader must refuse it *before* calling anything through the
 * struct: the fields it would read may not be where this build expects them,
 * so trusting it is how a plugin system crashes inside somebody else's code.
 *
 * Deliberately broken, and only ever built into Build/ for the test suite.
 ******************************************************************************/

#include <stddef.h>

#include "Plugin.h"

static void render(long long nowMs,
                   char *readout, int readoutSize,
                   char *secondary, int secondarySize)
{
    (void)nowMs;
    (void)readout;
    (void)readoutSize;
    (void)secondary;
    (void)secondarySize;
}

static const DigitalClockPlugin BAD = {
    DIGITALCLOCK_PLUGIN_ABI + 99, /* the whole point */
    "Should Never Load",
    "0.0.0",
    render,
    NULL,
    NULL,
    NULL
};

const DigitalClockPlugin *digitalclock_plugin_init(
    const DigitalClockHost *host)
{
    (void)host;

    return &BAD;
}
