#include "mt/turing_machine.h"

#include <fstream>
#include <stdexcept>

void TuringMachine::AddState(const std::string& state) {
  states_.insert(state);
}

void TuringMachine::AddInputSymbol(char symbol) {
  input_alphabet_.insert(symbol);
}

void TuringMachine::AddTapeSymbol(char symbol) {
  tape_alphabet_.insert(symbol);
}

void TuringMachine::SetStartState(const std::string& state) {
  start_state_ = state;
}

void TuringMachine::SetBlank(char blank) {
  blank_ = blank;
}

void TuringMachine::AddFinalState(const std::string& state) {
  final_states_.insert(state);
}

void TuringMachine::SetTapeCount(std::size_t count) {
  tapes_.assign(count, Tape(blank_));
}

bool TuringMachine::AddTransition(const TransitionKey& key, const std::string& next_state,
                                  const std::vector<char>& write_symbols,
                                  const std::vector<char>& moves) {
  return transitions_.Add(key, next_state, write_symbols, moves);
}

const std::set<std::string>& TuringMachine::GetStates() const {
  return states_;
}

const std::set<char>& TuringMachine::GetInputAlphabet() const {
  return input_alphabet_;
}

const std::set<char>& TuringMachine::GetTapeAlphabet() const {
  return tape_alphabet_;
}

const std::string& TuringMachine::GetStartState() const {
  return start_state_;
}

char TuringMachine::GetBlank() const {
  return blank_;
}

const std::set<std::string>& TuringMachine::GetFinalStates() const {
  return final_states_;
}

std::size_t TuringMachine::GetTapeCount() const {
  return tapes_.size();
}

const TransitionTable& TuringMachine::GetTransitions() const {
  return transitions_;
}


std::string TuringMachine::ProcessFile(const std::string& in_path) {
  std::ifstream fichero(in_path);
  
  if (!fichero.is_open()) {
    throw std::runtime_error("No se pudo abrir el fichero de cadenas: " + in_path);
  }

  std::string report = "";
  std::string word = "";
  while(std::getline(fichero, word)) {
    SimulationResult result = Solve(word);
    report += "Entrada: \"" + word + "\"\n";
    report += std::string("Resultado: ") + (result.accepted ? "ACEPTADA" : "RECHAZADA") + "\n";
    report += "Estado final: " + result.final_state + "\n";
    
    for (size_t i{0}; i < tapes_.size(); ++i) {
      report += "Cinta " + std::to_string(i+1) + ": " + tapes_[i].ToString() + "\n";
    }
    report += "\n";
  }
  
  return report;
}

SimulationResult TuringMachine::Solve(const std::string& word) {
  ResetTapes(word);
  std::string state = start_state_;

  while (true) {
    std::string reads;
    for (const Tape& tape: tapes_) {
      reads += tape.Read();
    }

    TransitionKey key{state, reads};
    if (!transitions_.Contains(key)) {
      break;
    }

    Transition t = transitions_.Get(key).value();
    for (size_t i{0}; i < tapes_.size(); ++i) {
      tapes_[i].Write(t.write_symbols[i]);
      tapes_[i].Move(t.moves[i]);
    }

    state = t.next_state;
  }

  return SimulationResult{final_states_.count(state) > 0, state};
}

void TuringMachine::ResetTapes(const std::string& input) {
  if (tapes_.empty()) {
    return;
  }
  tapes_.front().Load(input);
  for (std::size_t i = 1; i < tapes_.size(); ++i) {
    tapes_[i].Reset();
  }
}
