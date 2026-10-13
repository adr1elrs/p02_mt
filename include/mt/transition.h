#ifndef MT_TRANSITION_H
#define MT_TRANSITION_H

#include <string>
#include <vector>

/**
 * @brief Representa una regla de transicion ya interpretada de la Maquina de Turing.
 *
 * Es un contenedor de datos sin logica. No almacena el estado origen ni los simbolos
 * leidos, porque ambos forman la clave con la que la transicion se indexa en
 * TransitionTable.
 */
struct Transition {
  int id;                           ///< Id secuencial (orden de lectura del fichero).
  std::string next_state;           ///< Estado destino.
  std::vector<char> write_symbols;  ///< Un simbolo a escribir por cinta (tamano k).
  std::vector<char> moves;          ///< Un movimiento por cinta (tamano k): 'L', 'R' o 'S'.
};

#endif // MT_TRANSITION_H
