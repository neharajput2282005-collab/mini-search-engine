#pragma once
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include "parser.h"
#include "tokenizer.h"
#include "index.h"

class Crawler {
public:
    Crawler(int maxPages);
    void crawl(const std::string& seedUrl);
    InvertedIndex& getIndex();
    int getPagesCrawled();

private:
    int maxPages;
    int pagesCrawled;
    std::queue<std::string> frontier;
    std::unordered_set<std::string> visited;
    HTMLParser parser;
    Tokenizer tokenizer;
    InvertedIndex index;

    std::string fetchPage(const std::string& url);
    bool isValidUrl(const std::string& url);
};