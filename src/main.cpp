#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <array>

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

int main(int argc, char** argv) {
   int termWidth;
   {
      struct winsize size;
      ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
      termWidth = size.ws_col;
   }

   CLI::App app{"A CLI tool to generate ASCII text banners."};

   std::string inputText;
   app.add_option("-i,--input", inputText, "The text to convert to ASCII.")->required();
   int optionWidth = 0;
   app.add_option("-w,--width", optionWidth, "The maximum width of the ASCII text.")->transform(CLI::Range(15,  termWidth));
   int optionSpeed = 5;
   app.add_option("-v,--speed", optionSpeed, "The speed of scrolling.")->transform(CLI::Range(1, 10));

   bool flagOutput = false;
   app.add_flag("-o,--output", flagOutput, "Output a text file with the ASCII text.");
   bool flagScroll = false;
   app.add_flag("-s,--scroll", flagScroll, "Scroll through the ASCII text as an animation.");

   CLI11_PARSE(app, argc, argv);


   std::array<std::string, 7> lines;
   int currentLineWidth = 0;
   int maxWidth = (optionWidth != 0) ? optionWidth : termWidth;

   for (char textChar : inputText) {
      char inputText = std::tolower(textChar);
      std::string asciiChar = ascii::character[inputText];
      std::vector<std::string> asciiCharLines = splitString(asciiChar, '\n');
      int asciiCharWidth = asciiCharLines[0].length();

      if (currentLineWidth + asciiCharWidth + 2 > maxWidth || inputText == '\n') {
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

   return 0;
}

