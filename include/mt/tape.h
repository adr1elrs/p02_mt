#ifndef MT_TAPE_H
#define MT_TAPE_H

#include <deque>
#include <string>

/**
 * @brief Representa una cinta y su cabezal de lectura/escritura.
 *
 * La cinta es infinita en ambas direcciones, pero solo se materializan las celdas
 * ya usadas. Se usa std::deque<char> porque permite crecer en O(1) por ambos
 * extremos; todo lo que queda fuera del deque se considera blanco.
 *
 * Invariante: 0 <= head_ < cells_.size().
 */
class Tape {
public:
  /**
   * @brief Construye una cinta con el simbolo blanco indicado y una celda blanca.
   *
   * @param blank Simbolo blanco de la cinta.
   */
  explicit Tape(char blank);

  /**
   * @brief Carga una cadena como contenido inicial y situa el cabezal al principio.
   *
   * Si la cadena es vacia, la cinta queda con una unica celda blanca.
   *
   * @param content Contenido inicial de la cinta.
   */
  void Load(const std::string& content);

  /**
   * @brief Restaura la cinta a su contenido inicial y coloca el cabezal a la izquierda.
   */
  void Reset();

  /**
   * @brief Lee el simbolo bajo el cabezal.
   *
   * @return Simbolo apuntado por el cabezal.
   */
  char Read() const;

  /**
   * @brief Escribe un simbolo bajo el cabezal.
   *
   * @param symbol Simbolo a escribir.
   */
  void Write(char symbol);

  /**
   * @brief Mueve el cabezal, creando celdas blancas si sale por un extremo.
   *
   * @param direction 'L' (izquierda), 'R' (derecha) o 'S' (sin movimiento).
   */
  void Move(char direction);

  /**
   * @brief Indice actual del cabezal dentro de las celdas materializadas.
   *
   * @return Posicion del cabezal.
   */
  int HeadIndex() const;

  /**
   * @brief Devuelve las celdas materializadas de la cinta.
   *
   * @return Referencia constante al deque de celdas.
   */
  const std::deque<char>& Cells() const;

  /**
   * @brief Devuelve el contenido de la cinta con el cabezal entre corchetes.
   *
   * Ejemplo: "ab[c]d".
   *
   * @return Representacion textual de la cinta.
   */
  std::string ToString() const;

private:
  std::deque<char> cells_;  ///< Celdas materializadas (fuera de aqui todo es blanco).
  int head_ = 0;            ///< Indice del cabezal dentro de cells_.
  char blank_;              ///< Simbolo blanco.
  std::string initial_;     ///< Contenido inicial, para poder reiniciar con Reset().
};

#endif // MT_TAPE_H
