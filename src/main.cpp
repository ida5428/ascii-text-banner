#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <filesystem>

#include <sys/ioctl.h>
#include <unistd.h>

#include "CLI/CLI11.hpp"
#include "ascii.h"

auto ExistingParentDirectory = CLI::Validator([](std::string &outputFileName) {
   namespace fs = std::filesystem;
   fs::path outputFilePath = outputFileName;
   fs::path parentPath = outputFilePath.parent_path();

   // Empty path means that no directory is given
   // Return success if the parent path exists or the file is in the current working directory
   if (parentPath.empty() || fs::exists(parentPath)) {
      return "";
   }

   throw CLI::ValidationError("Parent path does not exist", parentPath);
}, "PATH", "ExistingParentDirectory");

// Validator to check if the provided font exists
auto AvailableFont = CLI::Validator([](std::string &fontName) {
   namespace fs = std::filesystem;
   fs::path fontsDir = "fonts/";
   std::string fontFile = fontName + ".toml";

   for (const auto entry : fs::directory_iterator(fontsDir)) {
      if (fontFile == entry.path().filename()) {
         return "";
      }
    }

   throw CLI::ValidationError("Font cannot be found", fontName);
}, "FONT", "AvailableFont");

int maxTerminalWidth() {
   struct winsize size;
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
   return size.ws_col;
}

void listAvailableFonts() {
   namespace fs = std::filesystem;

   fs::path fontsDir = "fonts";

   if (!fs::exists(fontsDir)) {
      std::cerr << "The 'fonts/' directory does not exist." << std::endl;
      return;
   }

   bool foundFont = false;

   for (const auto entry : fs::directory_iterator(fontsDir)) {
      if (entry.path().extension() == ".toml") {
         std::cout << entry.path().stem().string() << "\n";
         foundFont = true;
      }
   }

   if (!foundFont) {
      std::cerr << "No fonts found in the 'fonts/' directory." << std::endl;
   }
}

int main(int argc, char** argv) {
   CLI::App app{"A CLI tool to generate ASCII text banners."};

   std::string inputText;
   int scrollingSpeed;
   std::string fontName;
   std::string outputFileName;
   int maxWidth;

   // Subcommand to list available fonts
   CLI::App* listFonts = app.add_subcommand("list-fonts", "List the available fonts to use for the ASCII text.");

   // Normal CLI options
   CLI::Option* optionText = app.add_option(
      "-i,--input,input", inputText,
      "The input text string to render as an ASCII banner."
   );

   CLI::Option* optionWidth = app.add_option(
      "-w,--width", maxWidth,
      "The maximum width of the banner. Defaults to the terminal width."
   )->check(CLI::Range(15, maxTerminalWidth()))->default_val(maxTerminalWidth());

   CLI::Option* optionScroll = app.add_option(
      "-s,--scroll", scrollingSpeed,
      "Display the text with a scrolling animation. Specify a value to change the speed."
   )->check(CLI::Range(1, 10))->expected(0, 1)->default_val(5);
   // Ranges from 1 - 10, expectes either 0 or 1 argument(s), default value is 5

   CLI::Option* optionFont = app.add_option(
      "-f,--font", fontName,
      "The font to render the ASCII text as."
   )->check(AvailableFont);

   CLI::Option* optionOutput = app.add_option(
      "-o,--output", outputFileName,
      "Write the ASCII banner to a file in addition to printing. Overwrites existing files."
   )->check(ExistingParentDirectory);

   CLI11_PARSE(app, argc, argv);

   // Check if the 'list-fonts' subcommand was parsed
   if (listFonts->parsed()) {
      listAvailableFonts();
      return 0;
   }

   // Check if the input is empty after parsing since input would precendence over the 'list-fonts' subcommand
   if (inputText.empty()) {
      std::cerr << "--input: 1 required TEXT missing" << '\n' <<
                   "Run with --help for more information." << std::endl;
      return 1;
   }
   
   std::transform(inputText.begin(), inputText.end(), inputText.begin(), ::tolower);
   ascii::Font font = ascii::parseFont(fontName);

   // Output banner to file if specified
   if (*optionOutput) {
      std::vector<std::string> banner = ascii::buildStaticBanner(inputText, maxWidth, font);
      ascii::outputStaticBanner(banner, outputFileName);
   }

   if (*optionScroll) {
      ascii::printScrollingBanner(inputText, maxWidth, scrollingSpeed, font);
   } else {
      std::vector<std::string> banner = ascii::buildStaticBanner(inputText, maxWidth, font);
      ascii::printStaticBanner(banner);
   }

   return 0;
}

