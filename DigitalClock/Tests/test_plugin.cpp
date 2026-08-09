/******************************************************************************
 * @file test_plugin.cpp
 * @brief Tests for the plugin loader.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-094 to TC-097, closing KI-006.
 *
 * Most of these are about refusal. Loading a plugin means running code the
 * application did not compile, and the ways that goes wrong -- a library that
 * is not a plugin, one built against a different interface, one that declines
 * -- all end in the same place if unchecked: a call through a pointer into
 * something that is not what it claims to be. So the loader's job is mostly
 * saying no, and that is mostly what is tested here.
 *
 * POSIX-only, like the pseudo-terminal tests: the fixtures are shared
 * libraries built by the Makefile, and the Windows equivalent is covered by
 * the loader compiling and reporting itself unsupported where it is.
 ******************************************************************************/

#include "TestFramework.hpp"

#include <cstddef>
#include <filesystem>
#include <string>

#include "PluginManager.hpp"

namespace
{
    /** Host services the tests lend to a plugin under test. */
    int loggedLines = 0;

    extern "C" void testLog(const char *message)
    {
        (void)message;
        ++loggedLines;
    }

    extern "C" long long testNow(void)
    {
        return 1000;
    }

    DigitalClockHost makeHost(unsigned int abi = DIGITALCLOCK_PLUGIN_ABI)
    {
        DigitalClockHost host;

        host.abiVersion = abi;
        host.log = testLog;
        host.monotonicNow = testNow;

        return host;
    }

    /** Where the Makefile puts the built fixtures. */
    std::string fixture(const std::string &name)
    {
        return "Build/" + name + PluginManager::librarySuffix();
    }

    bool anyRejectionMentions(const PluginManager &manager,
                              const std::string &text)
    {
        for (const std::string &reason : manager.rejections())
        {
            if (reason.find(text) != std::string::npos)
                return true;
        }

        return false;
    }
}

TEST_CASE(TC_094, "A missing plugin directory is normal, not an error")
{
    PluginManager manager;

    const DigitalClockHost host = makeHost();

    // The usual state of an installation with no plugins. It must not warn,
    // fail, or leave the application in a different shape than before.
    CHECK_EQ(manager.loadDirectory("Plugins/no-such-directory", host),
             static_cast<std::size_t>(0));

    CHECK_EQ(manager.count(), static_cast<std::size_t>(0));
    CHECK_TRUE(manager.at(0) == nullptr);
    CHECK_EQ(manager.nameAt(0), std::string(""));

    // Unloading nothing is safe, and safe twice.
    manager.unloadAll();
    manager.unloadAll();

    CHECK_EQ(manager.count(), static_cast<std::size_t>(0));
}

TEST_CASE(TC_095, "A library that is not a plugin is refused by name")
{
    PluginManager manager;

    const DigitalClockHost host = makeHost();

    /*
    A shared library that loads perfectly well and simply is not a plugin --
    the ordinary case of something stray in the directory. Refusing by
    looking for a named symbol is what stops the loader guessing at an entry
    point and calling whatever it finds.
    */
    const std::string stray = fixture("notaplugin");

    if (!std::filesystem::exists(stray))
    {
        FAIL_TEST("the not-a-plugin fixture was not built: " + stray);
        return;
    }

    CHECK_FALSE(manager.loadPlugin(stray, host));
    CHECK_EQ(manager.count(), static_cast<std::size_t>(0));
    CHECK_TRUE(anyRejectionMentions(manager, "digitalclock_plugin_init"));

    // A path that is not a file at all is refused with the loader's own
    // reason rather than a crash.
    CHECK_FALSE(manager.loadPlugin("Build/definitely-not-here.so", host));
    CHECK_EQ(manager.count(), static_cast<std::size_t>(0));
    CHECK_TRUE(anyRejectionMentions(manager, "definitely-not-here"));

    // And something that is a file but not a loadable library at all.
    CHECK_FALSE(manager.loadPlugin("Config/config.ini", host));
    CHECK_EQ(manager.count(), static_cast<std::size_t>(0));
}

TEST_CASE(TC_096, "A plugin built for another ABI is refused, not called")
{
    const std::string path = fixture("badabi");

    if (!std::filesystem::exists(path))
    {
        FAIL_TEST("the bad-ABI fixture was not built: " + path);
        return;
    }

    PluginManager manager;

    const DigitalClockHost host = makeHost();

    /*
    The fixture reports an interface version this build does not implement.
    Refusing it before touching anything else is the whole point: its struct
    may not have the shape this build expects, so reading further fields --
    let alone calling through them -- is not safe.
    */
    CHECK_FALSE(manager.loadPlugin(path, host));
    CHECK_EQ(manager.count(), static_cast<std::size_t>(0));

    CHECK_TRUE(anyRejectionMentions(manager, "ABI"));

    // The reason names both versions, so the message is actionable.
    CHECK_TRUE(anyRejectionMentions(
        manager, std::to_string(DIGITALCLOCK_PLUGIN_ABI)));
}

TEST_CASE(TC_097, "The bundled plugin loads, renders and shuts down")
{
    const std::string path = "Plugins/pomodoro" +
                             PluginManager::librarySuffix();

    if (!std::filesystem::exists(path))
    {
        FAIL_TEST("the pomodoro plugin was not built: " + path);
        return;
    }

    loggedLines = 0;

    {
        PluginManager manager;

        const DigitalClockHost host = makeHost();

        CHECK_TRUE(manager.loadPlugin(path, host));
        CHECK_EQ(manager.count(), static_cast<std::size_t>(1));

        const DigitalClockPlugin *plugin = manager.at(0);

        CHECK_TRUE(plugin != nullptr);

        if (plugin == nullptr)
            return;

        CHECK_EQ(manager.nameAt(0), std::string("Pomodoro"));
        CHECK_EQ(plugin->abiVersion,
                 static_cast<unsigned int>(DIGITALCLOCK_PLUGIN_ABI));

        // It used the host it was lent, rather than printing anywhere.
        CHECK_TRUE(loggedLines > 0);

        // A fresh Pomodoro is ready, and shows its first interval's length.
        char readout[128] = {0};
        char secondary[192] = {0};

        plugin->render(0, readout, static_cast<int>(sizeof(readout)),
                       secondary, static_cast<int>(sizeof(secondary)));

        CHECK_EQ(std::string(readout), std::string("25:00"));
        CHECK_CONTAINS(std::string(secondary), "Work");
        CHECK_CONTAINS(std::string(secondary), "Ready");

        // Space starts it; a later reading shows less time remaining.
        CHECK_TRUE(plugin->handleKey != nullptr);
        CHECK_TRUE(plugin->handleKey(' ', 0) != 0);

        plugin->render(61000, readout, static_cast<int>(sizeof(readout)),
                       secondary, static_cast<int>(sizeof(secondary)));

        CHECK_EQ(std::string(readout), std::string("23:59"));
        CHECK_CONTAINS(std::string(secondary), "Running");

        // A key it does not want is declined, so the application can offer
        // it elsewhere rather than having it swallowed.
        CHECK_TRUE(plugin->handleKey('z', 61000) == 0);

        // N advances to the break without waiting out the work interval.
        CHECK_TRUE(plugin->handleKey('n', 61000) != 0);

        plugin->render(61000, readout, static_cast<int>(sizeof(readout)),
                       secondary, static_cast<int>(sizeof(secondary)));

        CHECK_EQ(std::string(readout), std::string("05:00"));
        CHECK_CONTAINS(std::string(secondary), "Break");

        CHECK_TRUE(plugin->footerHint != nullptr);
        CHECK_CONTAINS(std::string(plugin->footerHint()), "[N] Next");
    }

    // The manager's destructor shut the plugin down and closed the library.
    // Reaching here without a crash is the assertion.
    CHECK_TRUE(true);
}
