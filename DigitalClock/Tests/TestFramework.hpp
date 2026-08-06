#ifndef TEST_FRAMEWORK_HPP
#define TEST_FRAMEWORK_HPP

/******************************************************************************
 * @file TestFramework.hpp
 * @brief Minimal self-registering test harness.
 * @author Adarsh Kumar
 * @date 2026
 *
 * The Digital Clock System has no third-party dependencies, and adding one
 * purely to run its tests would undermine that. This header therefore
 * provides just enough of a harness to express the test cases from the
 * Testing Report: self-registering cases, a handful of assertions, and a
 * runner that reports pass and fail counts.
 *
 * A test case is declared with its Testing Report identifier so that the
 * output can be read directly against that document:
 *
 * @code
 * TEST_CASE(TC_003, "Verify current system time")
 * {
 *     Clock clock;
 *     CHECK_TRUE(clock.update());
 * }
 * @endcode
 ******************************************************************************/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace TestFramework
{
    /** Signature of a test body. */
    using TestFunction = void (*)();

    /**
     * @struct TestCase
     * @brief One registered test.
     */
    struct TestCase
    {
        std::string id;        ///< Identifier, such as "TC-003".
        std::string objective; ///< Objective from the Testing Report.
        TestFunction body;     ///< Function implementing the test.
    };

    /**
     * @brief Adds a test to the registry.
     *
     * @param id        Test identifier.
     * @param objective Human-readable objective.
     * @param body      Function to run.
     * @return int Unused; returned so registration can occur in an initialiser.
     */
    int registerTest(const std::string &id,
                     const std::string &objective,
                     TestFunction body);

    /**
     * @brief Records an assertion failure against the running test.
     *
     * @param file    Source file of the assertion.
     * @param line    Line of the assertion.
     * @param message Description of what failed.
     */
    void recordFailure(const std::string &file,
                       int line,
                       const std::string &message);

    /**
     * @brief Runs every registered test.
     *
     * @param filter When non-empty, only tests whose identifier contains this
     *               text are run.
     * @return int 0 when every test passed, 1 otherwise.
     */
    int runAll(const std::string &filter = "");

    /**
     * @brief Returns a writable directory for temporary test files.
     *
     * The directory is created on first use and removed by runAll() once the
     * suite finishes.
     *
     * @return std::string Path of the scratch directory.
     */
    std::string tempDirectory();

    /**
     * @brief Writes a file into the scratch directory.
     *
     * @param name    File name, without a directory component.
     * @param content Text to write.
     * @return std::string Full path of the written file.
     */
    std::string writeTempFile(const std::string &name,
                              const std::string &content);

    /**
     * @brief Renders a value for an assertion message.
     *
     * @tparam T Type of the value.
     * @param value Value to render.
     * @return std::string Text form of @p value.
     */
    template <typename T>
    std::string describe(const T &value)
    {
        std::ostringstream out;
        out << value;
        return out.str();
    }

    /** Renders a bool as "true"/"false" rather than 1/0. */
    inline std::string describe(bool value)
    {
        return value ? "true" : "false";
    }

    /** Renders a string in quotes so trailing spaces are visible. */
    inline std::string describe(const std::string &value)
    {
        return "\"" + value + "\"";
    }

    /** Renders a C string in quotes. */
    inline std::string describe(const char *value)
    {
        return describe(std::string(value != nullptr ? value : "<null>"));
    }

    /**
     * @class OutputCapture
     * @brief Redirects std::cout for the lifetime of the object.
     *
     * Components in the presentation layer write to std::cout. Capturing that
     * output lets those components be tested without the escape sequences
     * disturbing the test report, and lets assertions inspect what was drawn.
     */
    class OutputCapture
    {
    public:
        OutputCapture();
        ~OutputCapture();

        OutputCapture(const OutputCapture &) = delete;
        OutputCapture &operator=(const OutputCapture &) = delete;

        /**
         * @brief Returns everything written to std::cout so far.
         * @return std::string Captured output.
         */
        std::string text() const;

    private:
        std::ostringstream buffer;   ///< Receives the captured output.
        std::streambuf *savedBuffer; ///< Original std::cout buffer.
    };
}

/**
 * @brief Declares and registers a test case.
 *
 * @param identifier Test identifier as a C++ token, such as @c TC_003.
 * @param objective  Objective text from the Testing Report.
 */
#define TEST_CASE(identifier, objective)                                   \
    static void identifier##_body();                                       \
    static const int identifier##_registration =                           \
        TestFramework::registerTest(                                       \
            #identifier, objective, &identifier##_body);                   \
    static void identifier##_body()

/** Fails the running test unconditionally with @p message. */
#define FAIL_TEST(message) \
    TestFramework::recordFailure(__FILE__, __LINE__, (message))

/** Fails the running test when @p expression is false. */
#define CHECK_TRUE(expression)                                             \
    do                                                                     \
    {                                                                      \
        if (!(expression))                                                 \
        {                                                                  \
            FAIL_TEST(std::string("expected true: ") + #expression);       \
        }                                                                  \
    } while (false)

/** Fails the running test when @p expression is true. */
#define CHECK_FALSE(expression)                                            \
    do                                                                     \
    {                                                                      \
        if ((expression))                                                  \
        {                                                                  \
            FAIL_TEST(std::string("expected false: ") + #expression);      \
        }                                                                  \
    } while (false)

/** Fails the running test when @p actual differs from @p expected. */
#define CHECK_EQ(actual, expected)                                         \
    do                                                                     \
    {                                                                      \
        const auto &checkActual = (actual);                                \
        const auto &checkExpected = (expected);                            \
        if (!(checkActual == checkExpected))                               \
        {                                                                  \
            FAIL_TEST(                                                     \
                std::string(#actual) + " == " + #expected +                \
                "\n      actual   : " +                                    \
                TestFramework::describe(checkActual) +                     \
                "\n      expected : " +                                    \
                TestFramework::describe(checkExpected));                   \
        }                                                                  \
    } while (false)

/** Fails the running test when @p actual equals @p unexpected. */
#define CHECK_NE(actual, unexpected)                                       \
    do                                                                     \
    {                                                                      \
        const auto &checkActual = (actual);                                \
        const auto &checkUnexpected = (unexpected);                        \
        if ((checkActual == checkUnexpected))                              \
        {                                                                  \
            FAIL_TEST(                                                     \
                std::string(#actual) + " != " + #unexpected +              \
                "\n      both were : " +                                   \
                TestFramework::describe(checkActual));                     \
        }                                                                  \
    } while (false)

/** Fails the running test when @p haystack does not contain @p needle. */
#define CHECK_CONTAINS(haystack, needle)                                   \
    do                                                                     \
    {                                                                      \
        const std::string checkHaystack = (haystack);                      \
        const std::string checkNeedle = (needle);                          \
        if (checkHaystack.find(checkNeedle) == std::string::npos)          \
        {                                                                  \
            FAIL_TEST(                                                     \
                std::string("expected to find ") +                         \
                TestFramework::describe(checkNeedle) + " in " +            \
                TestFramework::describe(checkHaystack));                   \
        }                                                                  \
    } while (false)

/** Fails the running test when @p haystack contains @p needle. */
#define CHECK_NOT_CONTAINS(haystack, needle)                               \
    do                                                                     \
    {                                                                      \
        const std::string checkHaystack = (haystack);                      \
        const std::string checkNeedle = (needle);                          \
        if (checkHaystack.find(checkNeedle) != std::string::npos)          \
        {                                                                  \
            FAIL_TEST(                                                     \
                std::string("did not expect to find ") +                   \
                TestFramework::describe(checkNeedle) + " in " +            \
                TestFramework::describe(checkHaystack));                   \
        }                                                                  \
    } while (false)

#endif // TEST_FRAMEWORK_HPP
