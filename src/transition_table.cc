#include "mt/transition_table.h"

bool TransitionTable::Add(const TransitionKey& key, const std::string& next_state,
                          const std::vector<char>& write_symbols,
                          const std::vector<char>& moves) {
  if (transitions_.find(key) != transitions_.end()) {
    return false;
  }
  transitions_.emplace(key, Transition{next_id_++, next_state, write_symbols, moves});
  return true;
}

bool TransitionTable::Contains(const TransitionKey& key) const {
  return transitions_.find(key) != transitions_.end();
}

std::optional<Transition> TransitionTable::Get(const TransitionKey& key) const {
  auto it = transitions_.find(key);
  if (it == transitions_.end()) {
    return std::nullopt;
  }
  return it->second;
}

size_t TransitionTable::Size() const {
  return transitions_.size();
}
