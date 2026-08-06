/******************************************************************************
 * @file test_resource.cpp
 * @brief Tests for the ResourceManager service and Banner component.
 * @author Adarsh Kumar
 * @date 2026
 *
 * Covers TC-016 (banner loading), TC-017 (missing resource handling), TC-018
 * (invalid file path handling) and TC-023 (corrupted resource file) from the
 * Testing Report.
 ******************************************************************************/

#include "TestFramework.hpp"

#include "Banner.hpp"
#include "ResourceManager.hpp"

TEST_CASE(TC_016, "Verify banner loading")
{
    ResourceManager resources;

    // The banner shipped with the project must be found and read.
    CHECK_TRUE(resources.exists("Resources/banner.txt"));
    CHECK_TRUE(resources.load("Resources/banner.txt"));
    CHECK_FALSE(resources.getContent().empty());
    CHECK_FALSE(resources.loadedPath().empty());

    Banner banner;

    CHECK_TRUE(banner.load("Resources/banner.txt"));
    CHECK_TRUE(banner.isVisible());
    CHECK_FALSE(banner.content().empty());
    CHECK_TRUE(banner.lineCount() > 0);

    // The logo is a separate resource and must also load.
    CHECK_TRUE(resources.load("Resources/logo.txt"));
    CHECK_CONTAINS(resources.getContent(), "DIGITAL CLOCK SYSTEM");
}

TEST_CASE(TC_017, "Verify missing resource handling")
{
    Banner banner;

    const std::string builtIn = banner.content();

    CHECK_FALSE(builtIn.empty());

    // A missing file must be reported, but the built-in artwork retained.
    CHECK_FALSE(banner.load("Resources/no-such-banner.txt"));
    CHECK_EQ(banner.content(), builtIn);
    CHECK_TRUE(banner.lineCount() > 0);

    // The application therefore always has a header to draw.
    CHECK_CONTAINS(banner.content(), "DIGITAL CLOCK SYSTEM");

    ResourceManager resources;

    CHECK_FALSE(resources.load("Resources/no-such-banner.txt"));
    CHECK_EQ(resources.getContent(), std::string(""));
    CHECK_FALSE(resources.exists("Resources/no-such-banner.txt"));
}

TEST_CASE(TC_018, "Verify invalid file path handling")
{
    ResourceManager resources;

    // Each of these is invalid in a different way; none may throw.
    CHECK_FALSE(resources.exists(""));
    CHECK_FALSE(resources.exists("   "));
    CHECK_FALSE(resources.exists("Resources/deeply/missing/path.txt"));
    CHECK_FALSE(resources.exists("/absolute/path/that/does/not/exist"));

    CHECK_FALSE(resources.load(""));
    CHECK_EQ(resources.resolve(""), std::string(""));
    CHECK_EQ(resources.read(""), std::string(""));

    // A directory is not a readable resource.
    CHECK_FALSE(resources.exists("Resources"));
    CHECK_FALSE(resources.load("Resources"));

    // read() on a missing file yields empty content rather than failing.
    CHECK_EQ(resources.read("Resources/missing.txt"), std::string(""));
}

TEST_CASE(TC_023, "Corrupted resource file is handled without crashing")
{
    // An empty banner file is treated as unusable so the header is never blank.
    const std::string emptyBanner =
        TestFramework::writeTempFile("empty-banner.txt", "");

    Banner banner;

    const std::string builtIn = banner.content();

    CHECK_FALSE(banner.load(emptyBanner));
    CHECK_EQ(banner.content(), builtIn);

    // Binary content is read without error; it is simply drawn as-is.
    const std::string binary = TestFramework::writeTempFile(
        "binary-banner.txt",
        std::string("\x01\x02\x03\xFF", 4) + "\nstill text\n");

    CHECK_TRUE(banner.load(binary));
    CHECK_CONTAINS(banner.content(), "still text");

    // Windows line endings must not leave stray carriage returns in the lines.
    const std::string crlf = TestFramework::writeTempFile(
        "crlf-banner.txt", "first\r\nsecond\r\n\r\n\r\n");

    CHECK_TRUE(banner.load(crlf));

    const std::vector<std::string> lines = banner.lines();

    CHECK_EQ(lines.size(), static_cast<std::size_t>(2));
    CHECK_EQ(lines[0], std::string("first"));
    CHECK_EQ(lines[1], std::string("second"));
}

TEST_CASE(UT_070, "Banner visibility can be toggled without losing artwork")
{
    Banner banner;

    CHECK_TRUE(banner.load("Resources/banner.txt"));

    const std::string artwork = banner.content();

    CHECK_FALSE(artwork.empty());

    banner.hide();

    CHECK_FALSE(banner.isVisible());
    CHECK_EQ(banner.content(), std::string(""));
    CHECK_EQ(banner.lineCount(), static_cast<std::size_t>(0));

    banner.show();

    CHECK_TRUE(banner.isVisible());
    CHECK_EQ(banner.content(), artwork);

    // reset() restores the built-in artwork.
    banner.reset();

    CHECK_TRUE(banner.isVisible());
    CHECK_CONTAINS(banner.content(), "DIGITAL CLOCK SYSTEM");
}

TEST_CASE(UT_071, "ResourceManager searches its configured directories")
{
    ResourceManager resources;

    const std::string path =
        TestFramework::writeTempFile("searched.txt", "found me");

    // The scratch directory is not on the default search path.
    CHECK_FALSE(resources.exists("searched.txt"));

    resources.addSearchPath(TestFramework::tempDirectory());

    CHECK_TRUE(resources.exists("searched.txt"));
    CHECK_TRUE(resources.load("searched.txt"));
    CHECK_EQ(resources.getContent(), std::string("found me"));

    // A path relative to the project root resolves through the "." entry
    // that every ResourceManager carries by default.
    ResourceManager direct;

    CHECK_TRUE(direct.exists(path));
    CHECK_EQ(direct.read(path), std::string("found me"));

    // clear() discards the buffer but leaves the search path intact.
    resources.clear();

    CHECK_EQ(resources.getContent(), std::string(""));
    CHECK_TRUE(resources.exists("searched.txt"));
}
