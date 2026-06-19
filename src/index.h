#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

struct Document {
    int id;
    std::string url;
    std::string content;
};

struct Posting {
    int docId;
    std::vector<int> positions;
};

class InvertedIndex {
public:
    void addDocument(const Document& doc, const std::vector<std::string>& tokens);
    std::vector<Posting> lookup(const std::string& term);
    void printIndex();
    int getDocCount();
    Document getDocument(int docId);
    int getDocLength(int docId);

private:
    std::unordered_map<std::string, std::vector<Posting>> index;
    std::vector<Document> documents;
    std::unordered_map<int, int> docTokenCounts;  // ← added
};