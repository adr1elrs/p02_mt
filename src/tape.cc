#include "mt/tape.h"

Tape::Tape(char blank) : blank_(blank) {
  Reset();
}

void Tape::Load(const std::string& content) {
  initial_ = content;
  Reset();
}

void Tape::Reset() {
  cells_.clear();
  if (initial_.empty()) {
    cells_.push_back(blank_);
  } else {
    for (char symbol : initial_) {
      cells_.push_back(symbol);
    }
  }
  head_ = 0;
}

char Tape::Read() const {
  return cells_[head_];
}

void Tape::Write(char symbol) {
  cells_[head_] = symbol;
}

void Tape::Move(char direction) {
  if (direction == 'R') {
    ++head_;
    if (head_ == static_cast<int>(cells_.size())) {
      cells_.push_back(blank_);
    }
  } else if (direction == 'L') {
    --head_;
    if (head_ < 0) {
      cells_.push_front(blank_);
      head_ = 0;
    }
  }
}

int Tape::HeadIndex() const {
  return head_;
}

const std::deque<char>& Tape::Cells() const {
  return cells_;
}

std::string Tape::ToString() const {
  std::string result;
  for (int i = 0; i < static_cast<int>(cells_.size()); ++i) {
    if (i == head_) {
      result += '[';
    }
    result += cells_[i];
    if (i == head_) {
      result += ']';
    }
  }
  return result;
}
