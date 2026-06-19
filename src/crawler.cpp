#include "crawler.h"
#include <iostream>
#include <windows.h>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")

Crawler::Crawler(int maxPages) : maxPages(maxPages), pagesCrawled(0) {}

std::string Crawler::fetchPage(const std::string& url) {
    std::string result;

    HINTERNET hInternet = InternetOpenA("MiniSearchBot/1.0",
        INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return "";

    HINTERNET hUrl = InternetOpenUrlA(hInternet, url.c_str(),
        NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hUrl) {
        InternetCloseHandle(hInternet);
        return "";
    }

    char buffer[4096];
    DWORD bytesRead;
    while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        result.append(buffer, bytesRead);
    }

    InternetCloseHandle(hUrl);
    InternetCloseHandle(hInternet);
    return result;
}

bool Crawler::isValidUrl(const std::string& url) {
    if (url.empty()) return false;
    if (url.substr(0, 4) != "http") return false;
    if (url.find(".pdf")  != std::string::npos) return false;
    if (url.find(".jpg")  != std::string::npos) return false;
    if (url.find(".png")  != std::string::npos) return false;
    if (url.find(".ico")  != std::string::npos) return false;
    if (url.find(".css")  != std::string::npos) return false;
    if (url.find(".js")   != std::string::npos) return false;
    if (url.find(".xml")  != std::string::npos) return false;
    if (url.find(".gif")  != std::string::npos) return false;
    if (url.find(".svg")  != std::string::npos) return false;
    if (url.find(".woff") != std::string::npos) return false;
    if (url.find("books.toscrape.com") == std::string::npos) return false;
    // Skip category listing pages, only crawl individual book pages
if (url.find("/category/") != std::string::npos) return false;
if (url.find("index.html") != std::string::npos && 
    url.find("/catalogue/") == std::string::npos) return false;
    return true;
}

void Crawler::crawl(const std::string& seedUrl) {
    frontier.push(seedUrl);

    while (!frontier.empty() && pagesCrawled < maxPages) {
        std::string url = frontier.front();
        frontier.pop();

        if (visited.count(url)) continue;
        if (!isValidUrl(url)) continue;

        std::cout << "[Crawling] " << url << std::endl;

        std::string html = fetchPage(url);
        if (html.empty()) {
            std::cout << "  -> Failed to fetch" << std::endl;
            continue;
        }

        visited.insert(url);
        pagesCrawled++;

        ParseResult parsed = parser.parse(html, url);
        std::cout << "  Title: " << parsed.title << std::endl;
        std::cout << "  Links: " << parsed.links.size() << std::endl;

        Document doc = {pagesCrawled - 1, url, parsed.cleanText};
        auto tokens = tokenizer.tokenize(parsed.cleanText);
        index.addDocument(doc, tokens);

        for (auto& link : parsed.links) {
            if (!visited.count(link) && isValidUrl(link)) {
                frontier.push(link);
            }
        }
    }  // ← this closes the while loop

    std::cout << "\nCrawling done! Pages indexed: " << pagesCrawled << std::endl;
}

InvertedIndex& Crawler::getIndex() { return index; }
int Crawler::getPagesCrawled() { return pagesCrawled; }