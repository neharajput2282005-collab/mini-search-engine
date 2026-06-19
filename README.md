# Mini Search Engine

A fully functional search engine built in **C++17** from scratch — no external libraries.

## Architecture

```
Web Crawler → HTML Parser → Tokenizer → Inverted Index → Query Engine → TF-IDF Ranker
```

## Features

- **BFS Web Crawler** — fetches real web pages using Windows WinINet API
- **Custom HTML Parser** — extracts clean text and hyperlinks from raw HTML
- **Inverted Index** — maps terms to documents with positional posting lists
- **TF-IDF Ranking** — scores documents by relevance using term frequency × inverse document frequency
- **Boolean Queries** — supports AND / OR / single-term search

## How to Build & Run

### Requirements
- Windows OS
- g++ / MinGW-w64 (C++17 or later)

### Build
```bash
g++ -std=c++17 -o search_engine.exe src/main.cpp src/tokenizer.cpp src/index.cpp src/parser.cpp src/crawler.cpp src/ranker.cpp -Wall -lwininet
```

### Run
```bash
.\search_engine.exe
```

## Example Usage

```
=== Mini Search Engine ===
Crawling web pages...
[Crawling] https://books.toscrape.com/catalogue/page-1.html
[Crawling] https://books.toscrape.com/catalogue/sharp-objects_997/index.html
...
Crawling done! Pages indexed: 20

=== Search Engine Ready ===
> mystery
Top 2 result(s):
[1] https://books.toscrape.com/catalogue/sharp-objects_997/index.html
    Score: 0.0196

> dark AND mystery
Top 1 result(s):
[1] https://books.toscrape.com/catalogue/the-black-maria_991/index.html
    Score: 0.0331

> love OR travel
Top 5 result(s):
[1] https://books.toscrape.com/catalogue/a-light-in-the-attic_1000/index.html
    Score: 0.0174
```

## Project Structure

```
mini-search-engine/
├── src/
│   ├── main.cpp          # Entry point, query loop
│   ├── crawler.cpp/.h    # BFS web crawler using WinINet
│   ├── parser.cpp/.h     # HTML parser, link extractor
│   ├── tokenizer.cpp/.h  # Lowercase, stop word removal
│   ├── index.cpp/.h      # Inverted index with posting lists
│   ├── ranker.cpp/.h     # TF-IDF scoring and ranking
└── libs/
    └── nlohmann/         # JSON header (for future persistence)
```

## Modules

| Module | Description |
|--------|-------------|
| Tokenizer | Lowercases text, removes stop words, extracts tokens |
| Inverted Index | Maps each term to docIDs and positions |
| HTML Parser | Strips tags, decodes entities, resolves relative URLs |
| Web Crawler | BFS frontier, visited set, fetches real HTTP pages |
| Query Engine | Parses AND / OR / single queries |
| TF-IDF Ranker | Scores and sorts results by relevance |

## Tech Stack

- **Language:** C++17
- **HTTP Client:** Windows WinINet API
- **Ranking Algorithm:** TF-IDF (Term Frequency × Inverse Document Frequency)
- **Compiler:** g++ / MinGW-w64 16.1.0
- **Editor:** Visual Studio Code
