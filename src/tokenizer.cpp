#include "tokenizer.h"
#include <algorithm>

Tokenizer::Tokenizer() {
    stopWords = {
        "a", "an", "the", "is", "it", "in", "on", "at", "to",
        "for", "of", "and", "or", "but", "not", "with", "this",
        "that", "are", "was", "be", "as", "by", "from", "has",
        "have", "had", "he", "she", "they", "we", "you", "i",
        "its", "into", "than", "then", "so", "if", "about"
    };
}

std::string Tokenizer::toLower(const std::string& word) {
    std::string result = word;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool Tokenizer::isStopWord(const std::string& word) {
    return stopWords.count(word) > 0;
}

bool Tokenizer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

std::vector<std::string> Tokenizer::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::string word = "";

    for (char c : text) {
        if (isAlpha(c)) {
            word += c;
        } else {
            if (!word.empty()) {
                std::string lower = toLower(word);
                if (!isStopWord(lower) && lower.length() > 1) {
                    tokens.push_back(lower);
                }
                word = "";
            }
        }
    }

    if (!word.empty()) {
        std::string lower = toLower(word);
        if (!isStopWord(lower) && lower.length() > 1) {
            tokens.push_back(lower);
        }
    }

    return tokens;
}