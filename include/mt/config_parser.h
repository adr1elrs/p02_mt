#ifndef MT_CONFIG_PARSER_H
#define MT_CONFIG_PARSER_H

#include <string>
#include <utility>
#include <vector>

#include "mt/turing_machine.h"

/**
 * @brief Lee y valida el fichero de configuracion de una Maquina de Turing.
 *
 * Aisla la E/S y el parsing: interpreta el fichero, aplica la matriz de errores
 * de la especificacion y construye el modelo TuringMachine correspondiente.
 */
class ConfigParser {
public:
  ConfigParser() = default;

  /**
   * @brief Construye la Maquina de Turing a partir de un fichero de configuracion.
   *
   * @param config_path Ruta del fichero de configuracion.
   * @return La maquina construida y validada.
   * @throws std::runtime_error si hay errores de E/S, sintacticos o semanticos.
   */
  TuringMachine Parse(const std::string& config_path);

private:
  /**
   * @brief Lee el fichero y devuelve las lineas utiles con su numero original.
   *
   * Descarta comentarios (todo lo que sigue a '#') y lineas en blanco.
   *
   * @param config_path Ruta del fichero de configuracion.
   * @return Pares (numero de linea original, contenido de la linea).
   */
  std::vector<std::pair<int, std::string>> ReadSignificantLines(const std::string& config_path) const;

  /**
   * @brief Divide una linea por espacios en blanco en tokens.
   *
   * @param line Linea a procesar.
   * @return Vector con los tokens de la linea.
   */
  std::vector<std::string> SplitTokens(const std::string& line) const;
};

#endif // MT_CONFIG_PARSER_H
