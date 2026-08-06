/******************************************************************************
 * @file test_main.cpp
 * @brief Test harness implementation and entry point.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Implements the registry declared in TestFramework.hpp and provides main()
 * for the test executable. Test cases themselves live in the other files in
 * this directory, one per component.
 ******************************************************************************/

#include "TestFramework.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>

namespace TestFramework
{
    namespace
    {
        /**
         * @brief Returns the registry.
         *
         * Held inside a function so that registration from other translation
         * units cannot run before the container is constructed.
         */
        std::vector<TestCase> &registry()
        {
            static std::vector<TestCase> tests;
            return tests;
        }

        /** Failure messages recorded by the test currently running. */
        std::vector<std::string> currentFailures;

        /** Scratch directory path, created on first use. */
        std::string scratchDirectory;

        /** Strips the directory portion of a source path for readability. */
        std::string baseName(const std::string &path)
        {
            const std::size_t slash = path.find_last_of("/\\");

            if (slash == std::string::npos)
                return path;

            return path.substr(slash + 1);
        }
    }

    int registerTest(const std::string &id,
                     const std::string &objective,
                     TestFunction body)
    {
        // Identifiers are written as TC_003 in code but read as TC-003.
        std::string readable = id;
        std::replace(readable.begin(), readable.end(), '_', '-');

        registry().push_back(TestCase{readable, objective, body});

        return 0;
    }

    void recordFailure(const std::string &file,
                       int line,
                       const std::string &message)
    {
        currentFailures.push_back(
            baseName(file) + ":" + std::to_string(line) + ": " + message);
    }

    std::string tempDirectory()
    {
        if (scratchDirectory.empty())
        {
            const std::filesystem::path path =
                std::filesystem::path("Build") / "test-tmp";

            std::error_code code;
            std::filesystem::create_directories(path, code);

            scratchDirectory = path.string();
        }

        return scratchDirectory;
    }

    std::string writeTempFile(const std::string &name,
                              const std::string &content)
    {
        const std::string path =
            (std::filesystem::path(tempDirectory()) / name).string();

        std::ofstream file(path, std::ios::trunc);
        file << content;
        file.close();

        return path;
    }

    OutputCapture::OutputCapture()
        : savedBuffer(std::cout.rdbuf(buffer.rdbuf()))
    {
    }

    OutputCapture::~OutputCapture()
    {
        std::cout.rdbuf(savedBuffer);
    }

    std::string OutputCapture::text() const
    {
        return buffer.str();
    }

    int runAll(const std::string &filter)
    {
        std::vector<TestCase> &tests = registry();

        // Report in identifier order so the output tracks the Testing Report.
        std::sort(
            tests.begin(),
            tests.end(),
            [](const TestCase &left, const TestCase &right)
            { return left.id < right.id; });

        std::cout
            << "============================================================\n"
            << "Digital Clock System - Test Suite\n"
            << "============================================================\n\n";

        std::size_t passed = 0;
        std::size_t failed = 0;
        std::size_t skipped = 0;

        std::vector<std::string> failureReport;

        for (const TestCase &test : tests)
        {
            if (!filter.empty() && test.id.find(filter) == std::string::npos)
            {
                ++skipped;
                continue;
            }

            currentFailures.clear();

            try
            {
                test.body();
            }
            catch (const std::exception &error)
            {
                currentFailures.push_back(
                    std::string("unexpected exception: ") + error.what());
            }
            catch (...)
            {
                currentFailures.push_back("unexpected non-standard exception");
            }

            const bool ok = currentFailures.empty();

            std::cout
                << (ok ? "[ PASS ] " : "[ FAIL ] ")
                << std::left << std::setw(8) << test.id
                << test.objective << '\n';

            if (ok)
            {
                ++passed;
                continue;
            }

            ++failed;

            for (const std::string &failure : currentFailures)
            {
                std::cout << "         " << failure << '\n';

                failureReport.push_back(test.id + " - " + failure);
            }
        }

        std::cout
            << "\n------------------------------------------------------------\n"
            << "Total   : " << (passed + failed) << '\n'
            << "Passed  : " << passed << '\n'
            << "Failed  : " << failed << '\n';

        if (skipped > 0)
            std::cout << "Skipped : " << skipped << " (filtered out)\n";

        std::cout
            << "------------------------------------------------------------\n";

        if (!failureReport.empty())
        {
            std::cout << "\nFailures:\n";

            for (const std::string &failure : failureReport)
                std::cout << "  " << failure << '\n';
        }

        std::cout << '\n'
                  << (failed == 0 ? "RESULT: PASSED" : "RESULT: FAILED")
                  << '\n';

        if (!scratchDirectory.empty())
        {
            std::error_code code;
            std::filesystem::remove_all(scratchDirectory, code);
        }

        return (failed == 0) ? 0 : 1;
    }
}

/**
 * @brief Test executable entry point.
 *
 * @param argc Argument count.
 * @param argv Argument values. An optional first argument filters the run to
 *             tests whose identifier contains that text, for example
 *             <tt>./DigitalClockTests TC-01</tt>.
 * @return int 0 when every test passed.
 */
int main(int argc, char *argv[])
{
    const std::string filter = (argc > 1) ? argv[1] : "";

    return TestFramework::runAll(filter);
}
