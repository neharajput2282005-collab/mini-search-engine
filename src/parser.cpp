#include "parser.h"
#include <algorithm>

ParseResult HTMLParser::parse(const std::string& html, const std::string& baseUrl) {
    ParseResult result;
    result.title = extractTitle(html);
    result.links = extractLinks(html, baseUrl);
    result.cleanText = stripTags(html);
    result.cleanText = decodeEntities(result.cleanText);
    return result;
}

std::string HTMLParser::extractTitle(const std::string& html) {
    size_t start = html.find("<title>");
    size_t end = html.find("</title>");
    if (start == std::string::npos || end == std::string::npos) return "No Title";
    start += 7;
    return html.substr(start, end - start);
}

std::string HTMLParser::stripTags(const std::string& html) {
    std::string result;
    bool inTag = false;
    bool inScript = false;
    bool inStyle = false;

    for (size_t i = 0; i < html.size(); i++) {
        if (i + 7 <= html.size() && html.substr(i, 7) == "<script") inScript = true;
        if (i + 9 <= html.size() && html.substr(i, 9) == "</script>") {
            inScript = false; i += 8; continue;
        }
        if (inScript) continue;

        if (i + 6 <= html.size() && html.substr(i, 6) == "<style") inStyle = true;
        if (i + 8 <= html.size() && html.substr(i, 8) == "</style>") {
            inStyle = false; i += 7; continue;
        }
        if (inStyle) continue;

        if (html[i] == '<') { inTag = true; result += ' '; continue; }
        if (html[i] == '>') { inTag = false; continue; }
        if (!inTag) result += html[i];
    }
    return result;
}

std::vector<std::string> HTMLParser::extractLinks(const std::string& html, const std::string& baseUrl) {
    std::vector<std::string> links;
    size_t pos = 0;

    while ((pos = html.find("href=\"", pos)) != std::string::npos) {
        pos += 6;
        size_t end = html.find("\"", pos);
        if (end == std::string::npos) break;
        std::string href = html.substr(pos, end - pos);
        if (!href.empty() && href[0] != '#' && href.find("mailto:") == std::string::npos) {
            std::string resolved = resolveUrl(href, baseUrl);
            if (!resolved.empty()) links.push_back(resolved);
        }
        pos = end + 1;
    }
    return links;
}

std::string HTMLParser::resolveUrl(const std::string& href, const std::string& baseUrl) {
    // Already absolute URL
    if (href.size() >= 4 && href.substr(0, 4) == "http") return href;

    // Get base directory by stripping everything after last /
    std::string baseDir = baseUrl;
    size_t lastSlash = baseDir.rfind('/');
    if (lastSlash != std::string::npos)
        baseDir = baseDir.substr(0, lastSlash + 1);

    // Build raw resolved path
    std::string raw;
    if (!href.empty() && href[0] == '/')  {
        // Root-relative: attach to domain only
        size_t domainEnd = baseUrl.find('/', 8); // skip https://
        if (domainEnd == std::string::npos)
            raw = baseUrl + href;
        else
            raw = baseUrl.substr(0, domainEnd) + href;
    } else {
        raw = baseDir + href;
    }

    // Normalize: resolve every ../ segment
    std::string result;
    size_t i = 0;
    // Preserve scheme://
    size_t schemeEnd = raw.find("://");
    if (schemeEnd != std::string::npos) {
        schemeEnd += 3;
        result = raw.substr(0, schemeEnd);
        i = schemeEnd;
    }

    std::vector<std::string> segments;
    while (i <= raw.size()) {
        size_t slash = raw.find('/', i);
        if (slash == std::string::npos) slash = raw.size();
        std::string seg = raw.substr(i, slash - i);
        if (seg == "..") {
            if (!segments.empty()) segments.pop_back();
        } else if (seg != "." && !seg.empty()) {
            segments.push_back(seg);
        }
        i = slash + 1;
    }

    for (size_t j = 0; j < segments.size(); j++) {
        if (j > 0) result += '/';
        result += segments[j];
    }

    return result;
}

std::string HTMLParser::decodeEntities(const std::string& text) {
    std::string result = text;
    size_t pos;
    while ((pos = result.find("&amp;"))  != std::string::npos) result.replace(pos, 5, "&");
    while ((pos = result.find("&lt;"))   != std::string::npos) result.replace(pos, 4, "<");
    while ((pos = result.find("&gt;"))   != std::string::npos) result.replace(pos, 4, ">");
    while ((pos = result.find("&nbsp;")) != std::string::npos) result.replace(pos, 6, " ");
    return result;
}