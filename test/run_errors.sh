#!/usr/bin/env bash
# Script de pruebas del simulador de Maquina de Turing.
# Comprueba los errores de CLI, la matriz de errores de configuracion y casos
# positivos. Uso: bash test/run_errors.sh

set -u

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BIN="$ROOT/mt_sim"
CONFIG_DIR="$ROOT/test/errors"
WORDS="$ROOT/input/MT1.txt"
OUT="$ROOT/output.txt"

# Compila si el binario no existe.
if [ ! -x "$BIN" ]; then
  echo "Compilando el proyecto..."
  make -C "$ROOT" >/dev/null 2>&1 || { echo "ERROR: no se pudo compilar."; exit 1; }
fi

PASS=0
FAIL=0

# run_check <nombre> <exit_esperado> <fragmento_mensaje> <comando...>
run_check() {
  local name="$1"
  local expected_exit="$2"
  local expected_msg="$3"
  shift 3
  local output exit_code
  output="$("$@" 2>&1)"
  exit_code=$?
  if [ "$exit_code" -eq "$expected_exit" ] && printf '%s' "$output" | grep -qF "$expected_msg"; then
    echo "PASS: $name"
    PASS=$((PASS + 1))
  else
    echo "FAIL: $name"
    echo "      exit=$exit_code (esperado $expected_exit); buscaba: $expected_msg"
    echo "      salida: $(printf '%s' "$output" | head -1)"
    FAIL=$((FAIL + 1))
  fi
}

# run_check_file <nombre> <exit_esperado> <fragmento> <fichero_salida> <comando...>
run_check_file() {
  local name="$1"
  local expected_exit="$2"
  local expected_msg="$3"
  local check_file="$4"
  shift 4
  local exit_code
  "$@" >/dev/null 2>&1
  exit_code=$?
  if [ "$exit_code" -eq "$expected_exit" ] && [ -f "$check_file" ] && grep -qF "$expected_msg" "$check_file"; then
    echo "PASS: $name"
    PASS=$((PASS + 1))
  else
    echo "FAIL: $name"
    echo "      exit=$exit_code (esperado $expected_exit); buscaba: $expected_msg"
    FAIL=$((FAIL + 1))
  fi
}

# run_check_file_strip: como run_check_file pero ignora los corchetes del cabezal
# ([ y ]) antes de buscar el fragmento, para poder comparar el contenido de la cinta.
run_check_file_strip() {
  local name="$1"
  local expected_exit="$2"
  local expected_msg="$3"
  local check_file="$4"
  shift 4
  local exit_code
  "$@" >/dev/null 2>&1
  exit_code=$?
  if [ "$exit_code" -eq "$expected_exit" ] && [ -f "$check_file" ] && tr -d '[]' < "$check_file" | grep -qF "$expected_msg"; then
    echo "PASS: $name"
    PASS=$((PASS + 1))
  else
    echo "FAIL: $name"
    echo "      exit=$exit_code (esperado $expected_exit); buscaba: $expected_msg"
    FAIL=$((FAIL + 1))
  fi
}

echo "=== Errores de CLI ==="
run_check "CLI C1: numero de parametros incorrecto" 1 "no es el correcto" \
  "$BIN" -config a -in b -extra c
run_check "CLI C2: falta bandera obligatoria" 1 "faltan parametros obligatorios" \
  "$BIN" -config x -config y
run_check "CLI C3: fichero de configuracion no existe" 1 "No se pudo abrir el fichero de configuracion" \
  "$BIN" -config "$ROOT/test/no_such_config.txt" -in "$WORDS"
run_check "CLI C4: fichero de cadenas no existe" 1 "No se pudo abrir el fichero de cadenas" \
  "$BIN" -config "$ROOT/config/MT1.txt" -in "$ROOT/test/no_such_words.txt"

# C5: no se puede abrir output.txt (lo convertimos en directorio dentro de un cwd temporal).
TMP_C5="$(mktemp -d)"
mkdir -p "$TMP_C5/output.txt"
run_check "CLI C5: fichero de salida no se puede abrir" 1 "No se pudo abrir el fichero de salida" \
  bash -c "cd '$TMP_C5' && '$BIN' -config '$ROOT/config/MT1.txt' -in '$WORDS'"
rm -rf "$TMP_C5"

# Nota: los errores E2, E3 y E4 (conjuntos vacios) son comprobaciones defensivas
# inalcanzables, porque las lineas en blanco se ignoran (una cabecera vacia
# desplazaria las posiciones y dispararia E1).
echo ""
echo "=== Matriz de errores de configuracion ==="
run_check "E1: faltan lineas obligatorias" 1 "faltan lineas obligatorias" \
  "$BIN" -config "$CONFIG_DIR/error_01.txt" -in "$WORDS"
run_check "E5: simbolo de Sigma con longitud != 1" 1 "de Sigma debe tener longitud 1" \
  "$BIN" -config "$CONFIG_DIR/error_05.txt" -in "$WORDS"
run_check "E6: simbolo de Gamma con longitud != 1" 1 "de Gamma debe tener longitud 1" \
  "$BIN" -config "$CONFIG_DIR/error_06.txt" -in "$WORDS"
run_check "E7: Sigma no contenido en Gamma" 1 "de Sigma no pertenece a Gamma" \
  "$BIN" -config "$CONFIG_DIR/error_07.txt" -in "$WORDS"
run_check "E8: blanco no pertenece a Gamma" 1 "no pertenece a Gamma" \
  "$BIN" -config "$CONFIG_DIR/error_08.txt" -in "$WORDS"
run_check "E9: blanco pertenece a Sigma" 1 "no puede pertenecer a Sigma" \
  "$BIN" -config "$CONFIG_DIR/error_09.txt" -in "$WORDS"
run_check "E10: estado inicial mal definido" 1 "unico estado" \
  "$BIN" -config "$CONFIG_DIR/error_10.txt" -in "$WORDS"
run_check "E11: estado inicial ajeno a Q" 1 "no pertenece a Q" \
  "$BIN" -config "$CONFIG_DIR/error_11.txt" -in "$WORDS"
run_check "E12: blanco mal definido" 1 "unico caracter" \
  "$BIN" -config "$CONFIG_DIR/error_12.txt" -in "$WORDS"
run_check "E13: estado final ajeno a Q" 1 "estado final" \
  "$BIN" -config "$CONFIG_DIR/error_13.txt" -in "$WORDS"
run_check "E14: numero de cintas invalido" 1 "entero >= 1" \
  "$BIN" -config "$CONFIG_DIR/error_14.txt" -in "$WORDS"
run_check "E15: aridad de transicion incorrecta" 1 "3k+2" \
  "$BIN" -config "$CONFIG_DIR/error_15.txt" -in "$WORDS"
run_check "E16: estado origen ajeno a Q" 1 "estado origen" \
  "$BIN" -config "$CONFIG_DIR/error_16.txt" -in "$WORDS"
run_check "E17: estado destino ajeno a Q" 1 "estado destino" \
  "$BIN" -config "$CONFIG_DIR/error_17.txt" -in "$WORDS"
run_check "E18: simbolo leido ajeno a Gamma" 1 "simbolo leido" \
  "$BIN" -config "$CONFIG_DIR/error_18.txt" -in "$WORDS"
run_check "E19: simbolo escrito ajeno a Gamma" 1 "simbolo escrito" \
  "$BIN" -config "$CONFIG_DIR/error_19.txt" -in "$WORDS"
run_check "E20: movimiento invalido" 1 "movimiento" \
  "$BIN" -config "$CONFIG_DIR/error_20.txt" -in "$WORDS"
run_check "E21: transicion duplicada" 1 "ya fue declarada" \
  "$BIN" -config "$CONFIG_DIR/error_21.txt" -in "$WORDS"

echo ""
echo "=== Casos positivos ==="
run_check_file "OK: MT1 se ejecuta y acepta" 0 "ACEPTADA" "$OUT" \
  "$BIN" -config "$ROOT/config/MT1.txt" -in "$ROOT/input/MT1.txt"
run_check_file "OK: MT1 se ejecuta y rechaza" 0 "RECHAZADA" "$OUT" \
  "$BIN" -config "$ROOT/config/MT1.txt" -in "$ROOT/input/MT1.txt"
run_check_file_strip "OK: MT2 obtiene 5 b's y 4 a's" 0 "11111.1111" "$OUT" \
  "$BIN" -config "$ROOT/config/MT2.txt" -in "$ROOT/input/MT2.txt"

echo ""
echo "Resultado: $PASS correctas, $FAIL fallidas"
[ "$FAIL" -eq 0 ]
