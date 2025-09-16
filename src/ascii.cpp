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
      std::unordered_map<char, std::string> defaultFontMap = defaultFont;
      int symbolCount = 3;
      int characterHeight = 7;

      if (!fontName.empty()) {
         toml::table fontFile = toml::parse_file("fonts/" + fontName + ".toml");

         symbolCount = fontFile["symbol_count"].value_or(1);
         characterHeight = fontFile["character_height"].value_or(1);

         for (auto pair : defaultFont) {
            std::string key(1, pair.first);
            defaultFontMap[pair.first] = fontFile[key].value_or(defaultFont[pair.first]);
         }
      }

      return {symbolCount, characterHeight, defaultFontMap};
   }

   std::vector<std::string> buildStaticBanner(std::string inputText, int maxWidth, ascii::Font font) {
      std::vector<std::string> bannerLines(font.characterHeight);
      std::vector<std::string> fullBanner;
      int currentLineWidth = 0;

      for (char inputChar : inputText) {
         std::string asciiChar = font.fontMap[inputChar];
         std::vector<std::string> asciiCharLines = splitString(asciiChar, '\n');
         int asciiCharWidth = asciiCharLines[0].length();

         // If the current line width will exceed the max width, pushback the current lines and continue
         if (currentLineWidth + asciiCharWidth + 2 > maxWidth) {
            for (std::string line : bannerLines) {
               fullBanner.push_back(line);
            }

            bannerLines.assign(font.characterHeight, "");
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

   void printScrollingBanner(std::string inputText, int maxWidth, int scrollingSpeed, ascii::Font font) {
      inputText = "  " + inputText + "  ";

      std::vector<std::vector<std::string>> characterLines(font.characterHeight);

      for (char inputChar : inputText) {
         std::string asciiChar = font.fontMap[inputChar];
         std::vector<std::string> asciiCharLines = splitString(asciiChar, '\n');


         for (int i = 0; i < asciiCharLines.size(); i++) {
            int index = 0;

            while (index < asciiCharLines[i].size()) {
               if (asciiCharLines[i].at(index) == ' ') {
                  characterLines[i].push_back(" ");
                  index++;
               } else {
                  int symbolWidth = font.symbolWidth;
                  characterLines[i].push_back(asciiCharLines[i].substr(index, symbolWidth));
                  index += symbolWidth;
               }
            }
         }
      }

      int offset = 0;

      while (true) {
         // Clear screen, scroll buffer, and reset cursor
         std::cout << "\033[2J" << "\033[3J" << "\033[H";
         std::this_thread::sleep_for(std::chrono::milliseconds(150 / scrollingSpeed));

         for (std::vector<std::string> line : characterLines) {
            for (int i = 0; i < maxWidth; i++) {
               std::cout << line[(i + offset) % line.size()];
            }
            std::cout << '\n';
         }

         offset++;
      }
   }
}

