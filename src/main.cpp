#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <array>
#include <thread>
#include <chrono>
#include <fstream>
#include <filesystem>

#include <sys/ioctl.h>
#include <unistd.h>

#include "CLI/CLI11.hpp"
#include "ascii.hpp"

std::vector<std::string> splitString(const std::string& text, char delimiter) {
   std::vector<std::string> tokens;
   std::stringstream ss(text);
   std::string item;

   while (std::getline(ss, item, delimiter)) {
      tokens.push_back(std::move(item));
   }
   return tokens;
}

std::vector<std::string> buildStaticBanner(std::string inputText, int maxWidth) {
   std::array<std::string, 7> bannerLines;
   std::vector<std::string> fullBanner;
   int currentLineWidth = 0;

   for (char inputChar : inputText) {
      std::string asciiChar = ascii::character[inputChar];
      std::vector<std::string> asciiCharLines = splitString(asciiChar, '\n');
      int asciiCharWidth = asciiCharLines[0].length();

      // If the current line width will exceed the max width, pushback the current lines and reset
      if (currentLineWidth + asciiCharWidth + 2 > maxWidth) {
         for (std::string line : bannerLines) {
            fullBanner.push_back(line);
         }

         bannerLines.fill("");
         currentLineWidth = 0;
      }

      for (int i = 0; i < asciiCharLines.size(); i++) {
         bannerLines[i] += asciiCharLines[i];
      }
      currentLineWidth += asciiCharWidth;
   }

   // Pushback any remaining lines
   for (std::string line : bannerLines) {
      fullBanner.push_back(line);
   }

   return fullBanner;
}

void printStaticBanner(std::vector<std::string> banner) {
   for (std::string line : banner) {
      std::cout << line << '\n';
   }
}

void outputStaticBanner(std::vector<std::string> banner, std::string outputFileName) {
   std::ofstream outputFile(outputFileName);
   for (std::string line : banner) {
      outputFile << line << '\n';
   }
   outputFile.close();
}

auto ExistingParentDirectory = CLI::Validator([](std::string &outputFileName) {
   namespace fs = std::filesystem;
   fs::path outputFilePath = outputFileName;
   fs::path parentPath = outputFilePath.parent_path();

   // Empty path means that no directory is given
   // Return success if the parent path exists or the file is in the current dir.
   if (parentPath.empty() || fs::exists(parentPath)) {
      return "";
   }

   throw CLI::ValidationError("Parent path does not exist", parentPath);
}, "Check if the parent directory for a given path exists", "ExistingParentDirectory");

void printScrollingBanner(std::string inputText, int maxWidth, int scrollingSpeed) {
   std::array<std::string, 7> bannerLines;

   for (int i = 0; i < bannerLines.size(); i++) {
      bannerLines[i] += "                ";
   }

   for (char inputChar : inputText) {
      std::string asciiChar = ascii::character[inputChar];
      std::vector<std::string> asciiCharLines = splitString(asciiChar, '\n');

      for (int i = 0; i < bannerLines.size(); i++) {
         bannerLines[i] += asciiCharLines[i];
      }
   }

   std::vector<std::vector<std::string>> lineCharacters;

   for (std::string line : bannerLines) {
      std::vector<std::string> textLine;
      int index = 0;
      
      while (index < line.size()) {
         if (line.at(index) == ' ') {
            textLine.push_back(" ");
            index++;
         } else {
            textLine.push_back(line.substr(index, 3));
            index += 3;
         }
      }

      lineCharacters.push_back(textLine);
   }

   int offset = 0;

   while (true) {
      // Clear screen, scroll buffer, and reset cursor
      std::cout << "\033[2J" << "\033[3J" << "\033[H";
      std::this_thread::sleep_for(std::chrono::milliseconds(200 / scrollingSpeed));

      for (std::vector<std::string> line : lineCharacters) {
         for (int i = 0; i < maxWidth; i++) {
            std::cout << line[(i + offset) % line.size()];
         }
         std::cout << '\n';
      }

      offset++;
   }
}

int main(int argc, char** argv) {
   CLI::App app{"A CLI tool to generate ASCII text banners."};

   std::string inputText;
   CLI::Option* optionText = app.add_option(
      "-i,--input,input", inputText,
      "The input text string to render as an ASCII banner."
   )->required();

   int maxWidth;
   // Get the maximum width of the terminal
   struct winsize size;
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
   maxWidth = size.ws_col;
   CLI::Option* optionWidth = app.add_option(
      "-w,--width", maxWidth,
      "The maximum width of the banner. Default to the terminal width."
   )->check(CLI::Range(15, maxWidth));

   int scrollingSpeed;
   CLI::Option* optionScroll = app.add_option(
      "-s,--scroll", scrollingSpeed,
      "Display the text with a scrolling animation. Specify a value to change the speed."
   )->check(CLI::Range(1, 10))->expected(0, 1)->default_val(5);

   std::string outputFileName;
   CLI::Option* optionOutput = app.add_option(
      "-o,--output", outputFileName,
      "Write the ASCII banner to a file in addition to printing. Overwrites existing files."
   )->check(ExistingParentDirectory);

   CLI11_PARSE(app, argc, argv);
   
   std::transform(inputText.begin(), inputText.end(), inputText.begin(), ::tolower);

   if (optionScroll->count() > 0) {
      printScrollingBanner(inputText, maxWidth, scrollingSpeed);
   } else {
      std::vector<std::string> banner = buildStaticBanner(inputText, maxWidth);
      printStaticBanner(banner);
   }

   if (!outputFileName.empty()) {
      std::vector<std::string> banner = buildStaticBanner(inputText, maxWidth);
      outputStaticBanner(banner, outputFileName);
   }

   return 0;
}

