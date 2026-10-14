#ifndef MT_REPORTER_H
#define MT_REPORTER_H

#include <ostream>
#include <string>

/**
 * @brief Gestiona el destino de la salida (consola o fichero).
 *
 * Recibe el texto ya construido y lo vuelca tal cual, sin formatear contenido.
 */
class Reporter {
public:
  /**
   * @brief Construye el reporter con el stream de salida deseado.
   *
   * @param os Destino de la salida (std::cout, un std::ofstream, etc.).
   */
  explicit Reporter(std::ostream& os) : out_(os) {}

  /**
   * @brief Vuelca el texto completo al destino gestionado.
   *
   * @param full_text Texto del informe ya construido.
   */
  void Write(const std::string& full_text);

private:
  std::ostream& out_;  ///< Destino de la salida.
};

#endif // MT_REPORTER_H
