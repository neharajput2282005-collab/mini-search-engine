#include "index.h"
#include <algorithm>

void InvertedIndex::addDocument(const Document& doc, const std::vector<std::string>& tokens) {
    documents.push_back(doc);
    docTokenCounts[doc.id] = tokens.size();  // ← store token count

    for (int i = 0; i < (int)tokens.size(); i++) {
        const std::string& term = tokens[i];
        auto& postings = index[term];

        auto it = std::find_if(postings.begin(), postings.end(),
            [&](const Posting& p) { return p.docId == doc.id; });

        if (it != postings.end()) {
            it->positions.push_back(i);
        } else {
            Posting p;
            p.docId = doc.id;
            p.positions.push_back(i);
            postings.push_back(p);
        }
    }
}

std::vector<Posting> InvertedIndex::lookup(const std::string& term) {
    auto it = index.find(term);
    if (it != index.end()) return it->second;
    return {};
}

void InvertedIndex::printIndex() {
    std::cout << "\n=== Inverted Index ===" << std::endl;
    for (auto& pair : index) {
        std::cout << "[" << pair.first << "] -> ";
        for (auto& posting : pair.second) {
            std::cout << "doc" << posting.docId << "(pos:";
            for (int pos : posting.positions) std::cout << pos << " ";
            std::cout << ") ";
        }
        std::cout << std::endl;
    }
}

int InvertedIndex::getDocCount() {
    return documents.size();
}

Document InvertedIndex::getDocument(int docId) {
    return documents[docId];
}

int InvertedIndex::getDocLength(int docId) {
    return docTokenCounts.count(docId) ? docTokenCounts[docId] : 1;
}