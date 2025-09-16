#include <string>
#include <vector>
#include <unordered_map>

namespace ascii {
   struct Font {
      int symbolWidth;
      int characterHeight;
      std::unordered_map<char, std::string> fontMap;
   };

   ascii::Font parseFont(std::string fontName);

   std::vector<std::string> buildStaticBanner(std::string inputText, int maxWidth, ascii::Font font);

   void printStaticBanner(std::vector<std::string> banner);

   void outputStaticBanner(std::vector<std::string> banner, std::string outputFileName);

   void printScrollingBanner(std::string inputText, int maxWidth, int scrollingSpeed, ascii::Font font);
}

