/******************************************************************************
 * @file notaplugin.c
 * @brief A perfectly good shared library that is not a plugin.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Loads fine and exports nothing the application is looking for. This is the
 * ordinary case of a stray library sitting in the plugin directory, and the
 * loader must refuse it by *name* rather than guess at an entry point and
 * call whatever it finds.
 *
 * Only ever built into Build/ for the test suite.
 ******************************************************************************/

#include "Plugin.h"

DIGITALCLOCK_PLUGIN_EXPORT int digitalclock_something_else(void)
{
    return 42;
}
