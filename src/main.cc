#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "mt/config_parser.h"
#include "mt/reporter.h"
#include "mt/turing_machine.h"

/**
 * @brief Busca el valor de una bandera en los argumentos de la linea de comandos.
 *
 * Recorre los argumentos desde el indice 1 y devuelve el valor que sigue a la
 * bandera indicada. Pone `found` a true si la bandera aparece.
 *
 * @param argc Numero de argumentos.
 * @param argv Vector de argumentos.
 * @param flag Bandera a buscar (p. ej. "-config").
 * @param found Se pone a true si la bandera existe.
 * @return Valor de la bandera, o cadena vacia si no se encuentra.
 */
std::string FindArg(int argc, char* argv[], const std::string& flag, bool& found) {
  found = false;
  for (int i = 1; i + 1 < argc; ++i) {
    if (std::string(argv[i]) == flag) {
      found = true;
      return argv[i + 1];
    }
  }
  return "";
}

/**
 * @brief Punto de entrada del simulador de Maquina de Turing.
 *
 * Lee los argumentos obligatorios (-config, -in), construye y valida la MT desde
 * el fichero de configuracion, procesa las cadenas de entrada y escribe el
 * informe en el fichero de salida output.txt.
 *
 * @param argc Numero de argumentos.
 * @param argv Argumentos de la linea de comandos.
 * @return 0 si la ejecucion es correcta; 1 ante cualquier error.
 */
int main(int argc, char* argv[]) {
  const std::string output_path = "output.txt";

  // Se requieren exactamente 4 parametros: 2 banderas y 2 valores.
  if (argc != 5) {
    std::cerr << "Error: el numero de parametros no es el correcto (se esperan -config <f> -in <f>)"
              << std::endl;
    return 1;
  }

  bool has_config = false;
  bool has_in = false;
  const std::string config_path = FindArg(argc, argv, "-config", has_config);
  const std::string in_path = FindArg(argc, argv, "-in", has_in);

  if (!has_config || !has_in) {
    std::cerr << "Error: faltan parametros obligatorios (-config, -in)" << std::endl;
    return 1;
  }

  try {
    ConfigParser parser;
    TuringMachine machine = parser.Parse(config_path);

    const std::string report = machine.ProcessFile(in_path);

    std::ofstream out(output_path);
    if (!out.is_open()) {
      throw std::runtime_error("No se pudo abrir el fichero de salida: " + output_path);
    }
    Reporter reporter(out);
    reporter.Write(report);
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  return 0;
}
