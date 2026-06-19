#pragma once
#include <string>
#include <vector>

struct ParseResult {
    std::string cleanText;
    std::vector<std::string> links;
    std::string title;
};

class HTMLParser {
public:
    ParseResult parse(const std::string& html, const std::string& baseUrl);

private:
    std::string extractTitle(const std::string& html);
    std::string stripTags(const std::string& html);
    std::vector<std::string> extractLinks(const std::string& html, const std::string& baseUrl);
    std::string resolveUrl(const std::string& href, const std::string& baseUrl);
    std::string decodeEntities(const std::string& text);
};