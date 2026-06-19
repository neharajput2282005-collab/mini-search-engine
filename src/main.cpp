#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "crawler.h"
#include "ranker.h"
#include "tokenizer.h"

struct Query {
    std::vector<std::string> terms;
    std::string op;
};

Query parseQuery(const std::string& input) {
    Query q;
    q.op = "SINGLE";
    std::istringstream ss(input);
    std::string word;
    while (ss >> word) {
        // Convert word to uppercase for comparison
        std::string upper = word;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        
        if (upper == "AND") q.op = "AND";
        else if (upper == "OR") q.op = "OR";
        else {
            // Lowercase the actual search term
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            q.terms.push_back(word);
        }
    }
    return q;
}

int main() {
    std::cout << "=== Mini Search Engine ===" << std::endl;
    std::cout << "Crawling web pages..." << std::endl;

    Crawler crawler(20);
crawler.crawl("https://books.toscrape.com/catalogue/page-1.html");
    InvertedIndex& index = crawler.getIndex();
    Ranker ranker;

    std::cout << "\n=== Search Engine Ready ===" << std::endl;
    std::cout << "Commands:" << std::endl;
    std::cout << "  Single word : mystery" << std::endl;
    std::cout << "  AND search  : mystery AND fiction" << std::endl;
    std::cout << "  OR search   : travel OR mystery" << std::endl;
    std::cout << "  quit        : exit" << std::endl;

    std::string input;
    while (true) {
        std::cout << "\n> ";
        std::getline(std::cin, input);
        if (input == "quit") break;
        if (input.empty()) continue;

        Query q = parseQuery(input);
        if (q.terms.empty()) {
            std::cout << "Please enter a valid search term." << std::endl;
            continue;
        }

        auto results = ranker.rank(q.terms, index);

        // Filter for AND: doc must contain ALL terms
        if (q.op == "AND" && q.terms.size() > 1) {
            results.erase(std::remove_if(results.begin(), results.end(),
                [&](const SearchResult& r) {
                    for (auto& term : q.terms) {
                        auto postings = index.lookup(term);
                        bool found = false;
                        for (auto& p : postings)
                            if (p.docId == r.docId) { found = true; break; }
                        if (!found) return true;
                    }
                    return false;
                }), results.end());
        }

        if (results.empty()) {
            std::cout << "No results found." << std::endl;
        } else {
            int show = std::min((int)results.size(), 5);
            std::cout << "Top " << show << " result(s):" << std::endl;
            for (int i = 0; i < show; i++) {
                auto& r = results[i];
                std::cout << "\n[" << i+1 << "] " << r.url << std::endl;
                std::cout << "    Score : " << r.score << std::endl;
                std::cout << "    " << r.snippet << std::endl;
            }
        }
    }

    std::cout << "\nGoodbye!" << std::endl;
    return 0;
}