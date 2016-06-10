#include "error.hpp"
#include "parser.hpp"

#include <iostream>

#define DEF_MAX_NESTING 16
#define EMPTY_STRING ""

using namespace std;

namespace {
inline char *copy_std_str(const std::string str) {
  char *copy = new char[str.size() + 1];
  std::copy(str.begin(), str.end(), copy);
  copy[str.size()] = '\0';
  return copy;
}
}

namespace crowd {
MarkdownParser::MarkdownParser(MarkdownType type, int nest_level) {
  render_ = type == MarkdownType::NORMAL
                ? hoedown_html_renderer_new((hoedown_html_flags)0, 0)
                : hoedown_html_toc_renderer_new(nest_level);
  doc_ = hoedown_document_new(render_, (hoedown_extensions)0, DEF_MAX_NESTING);
}

MarkdownParser::~MarkdownParser() {
  hoedown_document_free(doc_);
  hoedown_html_renderer_free(render_);
}

std::string MarkdownParser::parse(const std::string &str) const {
  if (str.empty()) {
    return EMPTY_STRING;
  } else {
    char *c_str = copy_std_str(str);
    std::size_t size = strlen(c_str);
    hoedown_buffer *html = hoedown_buffer_new(size);
    hoedown_document_render(doc_, html, (uint8_t *)c_str, size);
    std::string ret(reinterpret_cast<char *>(html->data));
    delete[] c_str;
    hoedown_buffer_free(html);
    return ret;
  }
}

string MarkdownParser::parse(ifstream &file) const {
  if (!file.is_open()) {
    throw IOException("Can not open file.");
  }
  string md;
  string line;
  while (getline(file, line)) {
    md += line;
    md.push_back('\n');
  }
  return parse(md);
}
}
