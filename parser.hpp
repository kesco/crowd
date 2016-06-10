#pragma once

#include "html.h"
#include "parser.hpp"

#include <fstream>
#include <string>

namespace crowd {
struct Parser {
  virtual ~Parser() {}

  virtual std::string parse(const std::string&) const = 0;
  
  virtual std::string parse(std::ifstream&) const = 0;
};

enum class MarkdownType { NORMAL, TOC };

struct MarkdownParser : Parser {
  MarkdownParser(MarkdownType type = MarkdownType::NORMAL, int nest_level = 16);

  virtual ~MarkdownParser() override;

  virtual std::string parse(const std::string&) const override;
  
  virtual std::string parse(std::ifstream&) const override;

 private:
  hoedown_renderer* render_;
  hoedown_document* doc_;
};
}