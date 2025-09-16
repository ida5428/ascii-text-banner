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

auto AvailableFont = CLI::Validator([](std::string &fontName) {
   namespace fs = std::filesystem;

   for (fs::path file : fs::directory_iterator("fonts/")) {
      if (!fontName.compare(file.filename().replace_extension("").string())) {
         return "";
      }
   }

   throw CLI::ValidationError("Font cannot be found", fontName);
}, "FONT", "AvailableFont");

int main(int argc, char** argv) {
   CLI::App app{"A CLI tool to generate ASCII text banners."};

   std::string inputText;
   int scrollingSpeed;
   std::string fontName;
   std::string outputFileName;
   int maxWidth;
   // Get the maximum width of the terminal
   struct winsize size;
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
   maxWidth = size.ws_col;

   CLI::App* listFonts = app.add_subcommand("list-fonts", "List the available fonts to use for the ASCII text.");

   CLI::Option* optionText = app.add_option(
      "-i,--input,input", inputText,
      "The input text string to render as an ASCII banner."
   );

   CLI::Option* optionWidth = app.add_option(
      "-w,--width", maxWidth,
      "The maximum width of the banner. Defaults to the terminal width."
   )->check(CLI::Range(15, maxWidth));

   CLI::Option* optionScroll = app.add_option(
      "-s,--scroll", scrollingSpeed,
      "Display the text with a scrolling animation. Specify a value to change the speed."
   )->check(CLI::Range(1, 10))->expected(0, 1)->default_val(5);

   CLI::Option* optionFont = app.add_option(
      "-f,--font", fontName,
      "The font to render the ASCII text as."
   )->check(AvailableFont);

   CLI::Option* optionOutput = app.add_option(
      "-o,--output", outputFileName,
      "Write the ASCII banner to a file in addition to printing. Overwrites existing files."
   )->check(ExistingParentDirectory);

   CLI11_PARSE(app, argc, argv);

   if (listFonts->parsed()) {
      namespace fs = std::filesystem;

      for (fs::path file : fs::directory_iterator("fonts/")) {
         std::cout << file.filename().replace_extension("").string() << '\n';
      }

      return 0;
   }

   // Check if the input is empty after parsing since input takes precendence over the `list-fonts` subcommand
   if (inputText.empty()) {
      std::cerr << "--input: 1 required TEXT missing" << '\n' <<
                   "Run with --help for more information." << std::endl;
      return 0;
   }
   
   std::transform(inputText.begin(), inputText.end(), inputText.begin(), ::tolower);
   ascii::Font font = ascii::parseFont(fontName);

   if (optionScroll->count() > 0) {
      ascii::printScrollingBanner(inputText, maxWidth, scrollingSpeed, font);
   } else {
      std::vector<std::string> banner = ascii::buildStaticBanner(inputText, maxWidth, font);
      ascii::printStaticBanner(banner);
   }

   if (!outputFileName.empty()) {
      std::vector<std::string> banner = ascii::buildStaticBanner(inputText, maxWidth, font);
      ascii::outputStaticBanner(banner, outputFileName);
   }

   return 0;
}

