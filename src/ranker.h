#pragma once
#include "index.h"
#include <vector>
#include <string>

struct SearchResult {
    int docId;
    std::string url;
    std::string snippet;
    double score;
};

class Ranker {
public:
    std::vector<SearchResult> rank(
        const std::vector<std::string>& terms,
        InvertedIndex& index
    );

private:
    double computeTFIDF(int termFreq, int docLength, int totalDocs, int docsWithTerm);
    std::string makeSnippet(const std::string& content, const std::string& term);
};