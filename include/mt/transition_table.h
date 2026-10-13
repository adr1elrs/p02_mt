#ifndef MT_TRANSITION_TABLE_H
#define MT_TRANSITION_TABLE_H

#include <map>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "mt/transition.h"

/**
 * @brief Clave de busqueda de una transicion: (estado_origen, lecturas).
 *
 * El segundo elemento es una cadena de longitud k con un simbolo leido por cinta.
 * Se usa std::string (y no std::vector<char>) porque el std::map necesita comparar
 * las claves y std::string ya define el operador <.
 */
using TransitionKey = std::tuple<std::string, std::string>;

/**
 * @brief Almacena la funcion de transicion de la Maquina de Turing.
 *
 * Como la MT es determinista, para cada clave (estado_origen, lecturas) existe como
 * mucho una transicion. Por eso se usa std::map y no std::multimap.
 */
class TransitionTable {
public:
  /**
   * @brief Inserta una transicion asignandole el siguiente id disponible.
   *
   * Actua como red de seguridad frente a no determinismo: si ya existia una
   * transicion con la misma clave, no la sobrescribe y devuelve false.
   *
   * @param key Clave (estado_origen, lecturas).
   * @param next_state Estado destino de la transicion.
   * @param write_symbols Simbolo a escribir en cada cinta (tamano k).
   * @param moves Movimiento de cada cabezal (tamano k): 'L', 'R' o 'S'.
   * @return true si la transicion se inserto; false si la clave ya existia.
   */
  bool Add(const TransitionKey& key, const std::string& next_state,
           const std::vector<char>& write_symbols, const std::vector<char>& moves);

  /**
   * @brief Comprueba si existe una transicion para una clave.
   *
   * @param key Clave de busqueda.
   * @return true si existe una transicion con esa clave.
   */
  bool Contains(const TransitionKey& key) const;

  /**
   * @brief Devuelve la transicion asociada a una clave.
   *
   * @param key Clave de busqueda.
   * @return La transicion, o std::nullopt si no existe ninguna con esa clave.
   */
  std::optional<Transition> Get(const TransitionKey& key) const;

  /**
   * @brief Numero total de transiciones registradas.
   *
   * @return Cantidad de transiciones en la tabla.
   */
  size_t Size() const;

private:
  std::map<TransitionKey, Transition> transitions_;  ///< Transiciones indexadas por clave.
  int next_id_ = 0;                                  ///< Siguiente id a asignar.
};

#endif // MT_TRANSITION_TABLE_H
