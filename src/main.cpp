#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <array>
#include <thread>
#include <chrono>

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

void printNormalBanner(std::string inputText, int maxWidth) {
   std::array<std::string, 7> lines;
   int currentLineWidth = 0;

   for (char textChar : inputText) {
      char inputText = std::tolower(textChar);
      std::string asciiChar = ascii::character[inputText];
      std::vector<std::string> asciiCharLines = splitString(asciiChar, '\n');
      int asciiCharWidth = asciiCharLines[0].length();

      if (currentLineWidth + asciiCharWidth + 2 > maxWidth) {
         for (std::string line : lines) {
            std::cout << line << '\n';
         }

         lines.fill("");
         currentLineWidth = 0;
      }

      for (int i = 0; i < lines.size(); i++) {
         lines[i] += asciiCharLines[i];
      }
      currentLineWidth += asciiCharWidth;
   }

   for (std::string line : lines) {
      std::cout << line << '\n';
   }
}

void printScrollingBanner(std::string inputText, int maxWidth, int scrollingSpeed) {
   std::array<std::string, 7> lines;

   for (int i = 0; i < lines.size(); i++) {
      lines[i] += "                ";
   }

   for (char textChar : inputText) {
      char inputChar = std::tolower(textChar);
      std::string asciiChar = ascii::character[inputChar];
      std::vector<std::string> asciiCharLines = splitString(asciiChar, '\n');

      for (int i = 0; i < lines.size(); i++) {
         lines[i] += asciiCharLines[i];
      }
   }

   std::vector<std::vector<std::string>> lineCharacters;

   for (std::string line : lines) {
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

   bool canOutputFile = false;
   bool canScrollText = false;

   CLI::Option* flagOutput = app.add_flag(
      "-o,--output", canOutputFile,
      "Write the ASCII text to an output file."
   );

   CLI::Option* flagScroll = app.add_flag(
      "-s,--scroll", canScrollText,
      "Display the ASCII text as a scrolling animation."
   );

   std::string inputText;
   int maxWidth;
   int scrollingSpeed = 5;

   // Get the maximum terminal width
   struct winsize size;
   ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
   maxWidth = size.ws_col;

   CLI::Option* optionText = app.add_option(
      "-i,--input", inputText,
      "The text to convert to ASCII."
   )->required();

   CLI::Option* optionWidth = app.add_option(
      "-w,--width", maxWidth,
      "The maximum width of the ASCII text."
   )->transform(CLI::Range(15, maxWidth));

   CLI::Option* optionSpeed = app.add_option(
      "-v,--speed", scrollingSpeed,
      "The speed of the scrolling animation."
   )->transform(CLI::Range(1, 10))->needs(flagScroll);

   CLI11_PARSE(app, argc, argv);

   if (canScrollText) {
      printScrollingBanner(inputText, maxWidth, scrollingSpeed);
   } else {
      printNormalBanner(inputText, maxWidth);
   }

   return 0;
}

