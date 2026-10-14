#include "mt/config_parser.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {

// true si la linea esta vacia o solo contiene espacios en blanco.
bool IsBlank(const std::string& line) {
  for (unsigned char ch : line) {
    if (!std::isspace(ch)) {
      return false;
    }
  }
  return true;
}

// true si el token es una secuencia no vacia de digitos.
bool IsUnsignedInteger(const std::string& token) {
  if (token.empty()) {
    return false;
  }
  for (unsigned char ch : token) {
    if (!std::isdigit(ch)) {
      return false;
    }
  }
  return true;
}

}  // namespace

std::vector<std::pair<int, std::string>> ConfigParser::ReadSignificantLines(
    const std::string& config_path) const {
  std::ifstream file(config_path);
  if (!file.is_open()) {
    throw std::runtime_error("No se pudo abrir el fichero de configuracion: " + config_path);
  }

  std::vector<std::pair<int, std::string>> lines;
  std::string line;
  int line_number = 0;
  while (std::getline(file, line)) {
    ++line_number;
    const std::size_t hash_pos = line.find('#');
    if (hash_pos != std::string::npos) {
      line = line.substr(0, hash_pos);
    }
    if (IsBlank(line)) {
      continue;
    }
    lines.emplace_back(line_number, line);
  }
  return lines;
}

std::vector<std::string> ConfigParser::SplitTokens(const std::string& line) const {
  std::istringstream iss(line);
  std::vector<std::string> tokens;
  std::string token;
  while (iss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

TuringMachine ConfigParser::Parse(const std::string& config_path) {
  const std::vector<std::pair<int, std::string>> lines = ReadSignificantLines(config_path);

  // Lanza un error indicando el numero de linea original del fichero.
  auto error = [](int line_number, const std::string& message) {
    throw std::runtime_error("Error [linea " + std::to_string(line_number) + "]: " + message);
  };

  // Se requieren las 7 lineas obligatorias de definicion.
  if (lines.size() < 7) {
    throw std::runtime_error(
        "Error: faltan lineas obligatorias de definicion (Q, Sigma, Gamma, s, b, F, k).");
  }

  TuringMachine machine;

  // --- Linea 1: estados Q ---
  const std::vector<std::string> states = SplitTokens(lines[0].second);
  if (states.empty()) {
    error(lines[0].first, "el conjunto Q esta vacio.");
  }
  for (const std::string& state : states) {
    machine.AddState(state);
  }

  // --- Linea 2: alfabeto de entrada Sigma ---
  const std::vector<std::string> sigma = SplitTokens(lines[1].second);
  if (sigma.empty()) {
    error(lines[1].first, "el alfabeto Sigma esta vacio.");
  }
  for (const std::string& symbol : sigma) {
    if (symbol.size() != 1) {
      error(lines[1].first, "el simbolo '" + symbol + "' de Sigma debe tener longitud 1.");
    }
    machine.AddInputSymbol(symbol[0]);
  }

  // --- Linea 3: alfabeto de cinta Gamma ---
  const std::vector<std::string> gamma = SplitTokens(lines[2].second);
  if (gamma.empty()) {
    error(lines[2].first, "el alfabeto Gamma esta vacio.");
  }
  for (const std::string& symbol : gamma) {
    if (symbol.size() != 1) {
      error(lines[2].first, "el simbolo '" + symbol + "' de Gamma debe tener longitud 1.");
    }
    machine.AddTapeSymbol(symbol[0]);
  }

  // --- Sigma debe estar contenido en Gamma ---
  for (char symbol : machine.GetInputAlphabet()) {
    if (machine.GetTapeAlphabet().count(symbol) == 0) {
      error(lines[1].first,
            std::string("el simbolo '") + symbol + "' de Sigma no pertenece a Gamma.");
    }
  }

  // --- Linea 4: estado inicial s ---
  const std::vector<std::string> start_tokens = SplitTokens(lines[3].second);
  if (start_tokens.size() != 1) {
    error(lines[3].first, "la linea del estado inicial debe contener un unico estado.");
  }
  if (machine.GetStates().count(start_tokens[0]) == 0) {
    error(lines[3].first, "el estado inicial '" + start_tokens[0] + "' no pertenece a Q.");
  }
  machine.SetStartState(start_tokens[0]);

  // --- Linea 5: simbolo blanco b ---
  const std::vector<std::string> blank_tokens = SplitTokens(lines[4].second);
  if (blank_tokens.size() != 1 || blank_tokens[0].size() != 1) {
    error(lines[4].first, "la linea del simbolo blanco debe contener un unico caracter.");
  }
  const char blank = blank_tokens[0][0];
  if (machine.GetTapeAlphabet().count(blank) == 0) {
    error(lines[4].first, std::string("el simbolo blanco '") + blank + "' no pertenece a Gamma.");
  }
  if (machine.GetInputAlphabet().count(blank) != 0) {
    error(lines[4].first, std::string("el simbolo blanco '") + blank + "' no puede pertenecer a Sigma.");
  }
  machine.SetBlank(blank);

  // --- Linea 6: conjunto de estados finales F ---
  const std::vector<std::string> finals = SplitTokens(lines[5].second);
  if (finals.empty()) {
    error(lines[5].first, "la linea de estados finales F esta vacia.");
  }
  for (const std::string& state : finals) {
    if (machine.GetStates().count(state) == 0) {
      error(lines[5].first, "el estado final '" + state + "' no pertenece a Q.");
    }
    machine.AddFinalState(state);
  }

  // --- Linea 7: numero de cintas k ---
  const std::vector<std::string> tape_tokens = SplitTokens(lines[6].second);
  if (tape_tokens.size() != 1 || !IsUnsignedInteger(tape_tokens[0])) {
    error(lines[6].first, "la linea del numero de cintas debe ser un entero >= 1.");
  }
  std::size_t tape_count = 0;
  try {
    tape_count = std::stoul(tape_tokens[0]);
  } catch (const std::out_of_range&) {
    error(lines[6].first, "la linea del numero de cintas debe ser un entero >= 1.");
  }
  if (tape_count < 1) {
    error(lines[6].first, "la linea del numero de cintas debe ser un entero >= 1.");
  }
  machine.SetTapeCount(tape_count);

  // --- Lineas siguientes: transiciones delta ---
  const std::size_t expected_tokens = 3 * tape_count + 2;
  for (std::size_t i = 7; i < lines.size(); ++i) {
    const int line_number = lines[i].first;
    const std::vector<std::string> tokens = SplitTokens(lines[i].second);

    if (tokens.size() != expected_tokens) {
      error(line_number, "la transicion debe tener 3k+2 = " + std::to_string(expected_tokens) +
                             " elementos (origen, lecturas, destino, escrituras/movimientos).");
    }

    const std::string& origin = tokens[0];
    const std::string& dest = tokens[tape_count + 1];
    if (machine.GetStates().count(origin) == 0) {
      error(line_number, "el estado origen '" + origin + "' no pertenece a Q.");
    }
    if (machine.GetStates().count(dest) == 0) {
      error(line_number, "el estado destino '" + dest + "' no pertenece a Q.");
    }

    // Lecturas: un simbolo de Gamma por cinta.
    std::string reads;
    for (std::size_t j = 1; j <= tape_count; ++j) {
      if (tokens[j].size() != 1 || machine.GetTapeAlphabet().count(tokens[j][0]) == 0) {
        error(line_number, "el simbolo leido '" + tokens[j] + "' no pertenece a Gamma.");
      }
      reads += tokens[j][0];
    }

    // Escrituras y movimientos, emparejados por cinta.
    std::vector<char> writes;
    std::vector<char> moves;
    std::size_t pos = tape_count + 2;
    for (std::size_t j = 0; j < tape_count; ++j) {
      const std::string& write = tokens[pos];
      const std::string& move = tokens[pos + 1];
      if (write.size() != 1 || machine.GetTapeAlphabet().count(write[0]) == 0) {
        error(line_number, "el simbolo escrito '" + write + "' no pertenece a Gamma.");
      }
      if (move.size() != 1 || (move[0] != 'L' && move[0] != 'R' && move[0] != 'S')) {
        error(line_number, "el movimiento '" + move + "' no es valido (L, R o S).");
      }
      writes.push_back(write[0]);
      moves.push_back(move[0]);
      pos += 2;
    }

    const TransitionKey key{origin, reads};
    if (!machine.AddTransition(key, dest, writes, moves)) {
      error(line_number,
            "la transicion '" + origin + " " + reads + "' ya fue declarada (no determinista).");
    }
  }

  return machine;
}
