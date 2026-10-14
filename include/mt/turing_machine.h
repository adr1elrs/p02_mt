#ifndef MT_TURING_MACHINE_H
#define MT_TURING_MACHINE_H

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "mt/tape.h"
#include "mt/transition_table.h"

/**
 * @brief Resultado de simular una cadena.
 */
struct SimulationResult {
  bool accepted;           ///< true si la MT se detuvo en un estado de aceptacion.
  std::string final_state; ///< Estado en el que se detuvo la MT.
};

/**
 * @brief Modelo formal de la Maquina de Turing multicinta determinista y su simulacion.
 *
 * Almacena la definicion formal (Q, Sigma, Gamma, s, b, F, delta) junto con el
 * numero de cintas y el conjunto de cintas de trabajo. La comprobacion de cadenas
 * se apoya en ResetTapes y en los metodos de ejecucion.
 */
class TuringMachine {
public:
  // --- Setters: usados por ConfigParser para construir el modelo ---
  void AddState(const std::string& state);
  void AddInputSymbol(char symbol);
  void AddTapeSymbol(char symbol);
  void SetStartState(const std::string& state);
  void SetBlank(char blank);
  void AddFinalState(const std::string& state);
  void SetTapeCount(std::size_t count);

  /**
   * @brief Registra una transicion en la tabla de transiciones.
   *
   * @param key Clave (estado_origen, lecturas).
   * @param next_state Estado destino.
   * @param write_symbols Simbolo a escribir en cada cinta (tamano k).
   * @param moves Movimiento de cada cabezal (tamano k): 'L', 'R' o 'S'.
   * @return true si se inserto; false si la clave ya existia.
   */
  bool AddTransition(const TransitionKey& key, const std::string& next_state,
                     const std::vector<char>& write_symbols,
                     const std::vector<char>& moves);

  // --- Getters: lectura de la configuracion ---
  const std::set<std::string>& GetStates() const;
  const std::set<char>& GetInputAlphabet() const;
  const std::set<char>& GetTapeAlphabet() const;
  const std::string& GetStartState() const;
  char GetBlank() const;
  const std::set<std::string>& GetFinalStates() const;
  std::size_t GetTapeCount() const;
  const TransitionTable& GetTransitions() const;

  // --- Comprobacion de cadenas ---
  /**
   * @brief Lee las cadenas de un fichero y construye el informe completo.
   *
   * @param in_path Ruta del fichero con una cadena de entrada por linea.
   * @return Texto del informe listo para volcar con Reporter.
   */
  std::string ProcessFile(const std::string& in_path);

  /**
   * @brief Ejecuta la MT sobre una cadena.
   *
   * @param word Cadena de entrada a comprobar.
   * @return Resultado de la simulacion (aceptada y estado de parada).
   */
  SimulationResult Solve(const std::string& word);

private:
  /**
   * @brief Reinicia las cintas antes de simular una cadena.
   *
   * La cinta 1 recibe la entrada; el resto vuelve a una celda blanca.
   *
   * @param input Cadena a cargar en la primera cinta.
   */
  void ResetTapes(const std::string& input);

  std::set<std::string> states_;      ///< Conjunto de estados Q.
  std::set<char> input_alphabet_;     ///< Alfabeto de entrada Sigma.
  std::set<char> tape_alphabet_;      ///< Alfabeto de cinta Gamma (incluye el blanco).
  std::string start_state_;           ///< Estado inicial s.
  char blank_ = '\0';                 ///< Simbolo blanco b.
  std::set<std::string> final_states_;///< Conjunto de estados finales F.
  TransitionTable transitions_;       ///< Funcion de transicion delta.
  std::vector<Tape> tapes_;           ///< Cintas de trabajo (tamano k).
};

#endif // MT_TURING_MACHINE_H
