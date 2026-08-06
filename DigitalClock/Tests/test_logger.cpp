/******************************************************************************
 * @file test_logger.cpp
 * @brief Tests for the Logger service.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-014 (log file creation), TC-015 (log entry generation), TC-019
 * (permission error handling) and TC-024 (log directory unavailable) from the
 * Testing Report.
 ******************************************************************************/

#include "TestFramework.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

#ifndef _WIN32
#include <unistd.h>
#endif

#include "Logger.hpp"

namespace
{
    /**
     * @brief Reads a whole file into a string.
     * @param path File to read.
     * @return std::string Contents, empty when the file cannot be read.
     */
    std::string readFile(const std::string &path)
    {
        std::ifstream file(path);

        if (!file)
            return "";

        std::ostringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }
}

TEST_CASE(TC_014, "Verify log file creation")
{
    const std::string path =
        TestFramework::tempDirectory() + "/created/nested/app.log";

    // Remove any residue from an earlier run.
    std::error_code code;
    std::filesystem::remove_all(
        TestFramework::tempDirectory() + "/created", code);

    Logger logger;

    // Missing parent directories must be created rather than reported.
    CHECK_TRUE(logger.initialize(path));
    CHECK_TRUE(logger.isOpen());
    CHECK_TRUE(std::filesystem::exists(path));

    logger.close();
    CHECK_FALSE(logger.isOpen());

    // Every session writes a header so runs can be told apart.
    CHECK_CONTAINS(readFile(path), "NEW LOGGING SESSION");
}

TEST_CASE(TC_015, "Verify log entry generation")
{
    const std::string path =
        TestFramework::tempDirectory() + "/entries.log";

    std::error_code code;
    std::filesystem::remove(path, code);

    {
        Logger logger;

        CHECK_TRUE(logger.initialize(path));

        logger.setMinimumLevel(Logger::Level::DEBUG);

        logger.debug("a debug entry");
        logger.info("Application Started");
        logger.warning("Theme Missing");
        logger.error("Configuration File Not Found");

        logger.close();
    }

    const std::string contents = readFile(path);

    // Each severity must appear with its label and message.
    CHECK_CONTAINS(contents, "[DEBUG] a debug entry");
    CHECK_CONTAINS(contents, "[INFO] Application Started");
    CHECK_CONTAINS(contents, "[WARNING] Theme Missing");
    CHECK_CONTAINS(contents, "[ERROR] Configuration File Not Found");

    // Entries carry a timestamp ahead of the level, per the FR-007 criteria.
    CHECK_CONTAINS(contents, "] [INFO]");
    CHECK_CONTAINS(contents, "[20");
}

TEST_CASE(UT_050, "Logger discards entries below the minimum level")
{
    const std::string path =
        TestFramework::tempDirectory() + "/levels.log";

    std::error_code code;
    std::filesystem::remove(path, code);

    {
        Logger logger;

        CHECK_TRUE(logger.initialize(path));

        logger.setMinimumLevel(Logger::Level::WARNING);

        logger.debug("suppressed debug");
        logger.info("suppressed info");
        logger.warning("recorded warning");
        logger.error("recorded error");

        logger.close();
    }

    const std::string contents = readFile(path);

    CHECK_NOT_CONTAINS(contents, "suppressed debug");
    CHECK_NOT_CONTAINS(contents, "suppressed info");
    CHECK_CONTAINS(contents, "recorded warning");
    CHECK_CONTAINS(contents, "recorded error");
}

TEST_CASE(UT_051, "Logger parses and names severity levels")
{
    CHECK_EQ(Logger::levelToString(Logger::Level::DEBUG), std::string("DEBUG"));
    CHECK_EQ(Logger::levelToString(Logger::Level::INFO), std::string("INFO"));
    CHECK_EQ(
        Logger::levelToString(Logger::Level::WARNING), std::string("WARNING"));
    CHECK_EQ(Logger::levelToString(Logger::Level::ERROR), std::string("ERROR"));

    CHECK_TRUE(Logger::levelFromString("debug") == Logger::Level::DEBUG);
    CHECK_TRUE(Logger::levelFromString("  Info ") == Logger::Level::INFO);
    CHECK_TRUE(Logger::levelFromString("WARN") == Logger::Level::WARNING);
    CHECK_TRUE(Logger::levelFromString("ERROR") == Logger::Level::ERROR);

    // Unknown names keep the caller's fallback.
    CHECK_TRUE(
        Logger::levelFromString("verbose", Logger::Level::INFO) ==
        Logger::Level::INFO);
}

TEST_CASE(TC_019, "Verify permission error handling")
{
    // A path whose parent is a regular file can never be opened, which
    // exercises the same failure path as a permission denial portably.
    const std::string blocker =
        TestFramework::writeTempFile("blocker.txt", "not a directory");

    Logger logger;

    CHECK_FALSE(logger.initialize(blocker + "/app.log"));
    CHECK_FALSE(logger.isOpen());

    // Logging to a closed logger must be a no-op, not a crash.
    logger.info("this has nowhere to go");
    logger.error("neither does this");

    CHECK_FALSE(logger.isOpen());

    // An empty path is rejected rather than creating a stray file.
    CHECK_FALSE(logger.initialize(""));

#ifndef _WIN32
    /*
    Where the platform actually enforces permissions, verify a real denial as
    well. Two situations make that impossible: running as root, and working
    from a filesystem that does not implement POSIX permissions at all --
    NTFS and exFAT mounts silently accept chmod and then ignore it. Probing
    for a write that should have been refused distinguishes a genuine pass
    from a filesystem that cannot express the condition.
    */
    if (::geteuid() != 0)
    {
        const std::string locked =
            TestFramework::tempDirectory() + "/locked";

        std::error_code code;
        std::filesystem::create_directories(locked, code);
        std::filesystem::permissions(
            locked,
            std::filesystem::perms::owner_read |
                std::filesystem::perms::owner_exec,
            std::filesystem::perm_options::replace,
            code);

        std::ofstream probe(locked + "/probe");
        const bool permissionsEnforced = !probe.is_open();
        probe.close();

        if (permissionsEnforced)
        {
            Logger restricted;

            CHECK_FALSE(restricted.initialize(locked + "/denied.log"));
            CHECK_FALSE(restricted.isOpen());
        }

        // Restore write permission so the scratch directory can be removed.
        std::filesystem::permissions(
            locked,
            std::filesystem::perms::owner_all,
            std::filesystem::perm_options::replace,
            code);
    }
#endif
}

TEST_CASE(TC_024, "Log directory unavailable disables logging safely")
{
    const std::string blocker =
        TestFramework::writeTempFile("blocker2.txt", "not a directory");

    Logger logger;

    // initialize() reports the problem instead of throwing or aborting.
    CHECK_FALSE(logger.initialize(blocker + "/nested/app.log"));
    CHECK_FALSE(logger.isOpen());

    // The logger stays usable as a silent sink for the rest of the run.
    logger.setMinimumLevel(Logger::Level::DEBUG);
    logger.debug("still safe");
    logger.info("still safe");
    logger.warning("still safe");
    logger.error("still safe");

    // close() on a logger that was never opened must also be safe.
    logger.close();
    logger.close();

    CHECK_FALSE(logger.isOpen());
}
