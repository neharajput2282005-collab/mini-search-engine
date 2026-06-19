#include "ranker.h"
#include <cmath>
#include <algorithm>
#include <unordered_map>

double Ranker::computeTFIDF(int termFreq, int docLength, int totalDocs, int docsWithTerm) {
    if (docsWithTerm == 0 || docLength == 0) return 0.0;
    double tf = (double)termFreq / docLength;
    double idf = log((double)totalDocs / docsWithTerm) + 1.0;
    return tf * idf;
}

std::string Ranker::makeSnippet(const std::string& content, const std::string& term) {
    std::string clean;
    bool lastSpace = false;
    for (char c : content) {
        if (c == '\n' || c == '\r' || c == '\t') c = ' ';
        if (c == ' ') {
            if (!lastSpace) clean += c;
            lastSpace = true;
        } else {
            clean += c;
            lastSpace = false;
        }
    }

    size_t pos = clean.find(term);
    if (pos == std::string::npos) return clean.substr(0, 120) + "...";
    size_t start = (pos > 60) ? pos - 60 : 0;
    size_t end = std::min(clean.size(), pos + 120);
    return "..." + clean.substr(start, end - start) + "...";
}

std::vector<SearchResult> Ranker::rank(
    const std::vector<std::string>& terms,
    InvertedIndex& index)
{
    int totalDocs = index.getDocCount();
    std::unordered_map<int, double> scores;

    for (const std::string& term : terms) {
        auto postings = index.lookup(term);
        int docsWithTerm = postings.size();

        for (auto& posting : postings) {
            int docId = posting.docId;
            int docLength = index.getDocLength(docId);
            int termFreq = posting.positions.size();
            double score = computeTFIDF(termFreq, docLength, totalDocs, docsWithTerm);
            scores[docId] += score;
        }
    }

    std::vector<SearchResult> results;
    for (auto& [docId, score] : scores) {
        Document doc = index.getDocument(docId);
        std::string snippet = makeSnippet(doc.content, terms[0]);
        results.push_back({docId, doc.url, snippet, score});
    }

    std::sort(results.begin(), results.end(),
        [](const SearchResult& a, const SearchResult& b) {
            return a.score > b.score;
        });

    return results;
}