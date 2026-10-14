#include "mt/reporter.h"

void Reporter::Write(const std::string& full_text) {
  out_ << full_text;
}
