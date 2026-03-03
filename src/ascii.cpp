#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <unordered_map>

#include "toml++/toml.hpp"
#include "ascii.h"
#include "defaultFont.h"

std::vector<std::string> splitString(const std::string& text, char delimiter) {
   std::vector<std::string> tokens;
   std::stringstream ss(text);
   std::string item;

   while (std::getline(ss, item, delimiter)) {
      tokens.push_back(std::move(item));
   }
   return tokens;
}

namespace ascii {
   ascii::Font parseFont(std::string fontName) {
      // The default font values if no font is provided
      std::unordered_map<char, std::string> defaultFontMap = defaultFont;
      int symbolWidth = 1;
      int characterHeight = 7;

      if (!fontName.empty()) {
         toml::table fontFile = toml::parse_file("fonts/" + fontName + ".toml");

         symbolWidth = fontFile["symbol_width"].value_or(1);
         characterHeight = fontFile["character_height"].value_or(1);

         for (auto pair : defaultFont) {
            std::string key(1, pair.first);
            defaultFontMap[pair.first] = fontFile[key].value_or(defaultFont[pair.first]);
         }
      }

      return { symbolWidth, characterHeight, defaultFontMap };
   }

   std::vector<std::string> buildStaticBanner(std::string inputText, int maxWidth, ascii::Font font) {
      // Create a temporary vector the size of the height of the ASCII character
      std::vector<std::string> bannerLines(font.characterHeight);
      std::vector<std::string> fullBanner;
      int currentLineWidth = 0;

      for (char inputChar : inputText) {
         // Get the ASCII character of the current text character, split the ascii character to individual lines and get the width of the character
         std::string asciiChar = font.fontMap[inputChar];
         std::vector<std::string> asciiCharLines = splitString(asciiChar, '\n');
         int asciiCharWidth = asciiCharLines[0].length();

         // If the current line width will exceed the max width, pushback the current lines and continue
         if (currentLineWidth + asciiCharWidth + 2 > maxWidth) {
            for (std::string line : bannerLines) {
               fullBanner.push_back(line);
            }

            // Reset the banner and line width
            bannerLines.assign(font.characterHeight, "");
            currentLineWidth = 0;
         }

         // Add the ASCII character lines to the banner and update the line width
         for (int i = 0; i < asciiCharLines.size(); i++) {
            bannerLines[i] += asciiCharLines[i];
         }
         currentLineWidth += asciiCharWidth;
      }

      // Add any remaining lines to the banner
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

   void printScrollingBanner(std::string inputText, int maxWidth, int scrollingSpeed, ascii::Font font) {
      // Add some padding
      inputText = "  " + inputText + "  ";

      // A 2D vector to hold each individual character of the ASCII art as a grid
      std::vector<std::vector<std::string>> fullBanner(font.characterHeight);

      for (char inputChar : inputText) {
         std::string asciiChar = font.fontMap[inputChar];
         std::vector<std::string> asciiCharLines = splitString(asciiChar, '\n');
         int asciiCharHeight = asciiCharLines.size();

         // For each line in the ASCII art 
         for (int i = 0; i < asciiCharHeight; i++) {
            int index = 0;
            int asciiCharWidth = asciiCharLines[i].length();

            while (index < asciiCharWidth) {
               if (asciiCharLines[i].at(index) == ' ') {
                  fullBanner[i].push_back(" ");
                  index++;
               } else {
                  fullBanner[i].push_back(asciiCharLines[i].substr(index, font.symbolWidth));
                  index += font.symbolWidth;
               }
            }
         }
      }

      int offset = 0;

      while (true) {
         std::this_thread::sleep_for(std::chrono::milliseconds(150 / scrollingSpeed));
         // Clear screen, scroll buffer, reset cursor, and add some padding
         std::cout << "\033[2J" << "\033[3J" << "\033[H" << "\n\n";

         for (std::vector<std::string> line : fullBanner) {
            for (int i = 0; i < maxWidth; i++) {
               std::cout << line[(i + offset) % line.size()];
            }
            std::cout << '\n';
         }

         offset++;
      }
   }
}

