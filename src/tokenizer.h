#pragma once
#include <string>
#include <vector>
#include <unordered_set>

class Tokenizer {
public:
    Tokenizer();
    std::vector<std::string> tokenize(const std::string& text);

private:
    std::unordered_set<std::string> stopWords;
    std::string toLower(const std::string& word);
    bool isStopWord(const std::string& word);
    bool isAlpha(char c);
};
