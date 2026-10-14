# Simulador de Máquina de Turing multicinta determinista

Implementación en C++20 de un simulador de **Máquina de Turing multicinta determinista**. El
programa lee la definición formal de la MT desde un fichero de configuración, la valida
rigurosamente y comprueba un conjunto de cadenas de entrada, indicando para cada una si la MT
se detuvo en un estado de aceptación y el contenido final de todas las cintas con la posición
de sus cabezales.

## Variaciones elegidas

De las variaciones propuestas en el enunciado se han seleccionado:

| Variación | Opción elegida |
|---|---|
| Escritura y movimiento | **Simultáneos** (una transición escribe *y* mueve en un paso) |
| Movimientos permitidos | **`L`, `R` y `S`** (izquierda, derecha, sin movimiento) |
| Cinta | **Infinita en ambas direcciones** |

Además, la MT es **determinista** y la ejecución **termina únicamente por ausencia de
transición** (no se detectan bucles).

## Requisitos

- Compilador compatible con **C++20** (`g++` recomendado).
- `make` para la compilación.

## Estructura del proyecto

```text
p02_mt/
├── include/mt/            # Cabeceras públicas
│   ├── transition.h       # struct Transition
│   ├── transition_table.h # clase TransitionTable (delta)
│   ├── tape.h             # clase Tape (una cinta)
│   ├── turing_machine.h   # clase TuringMachine (modelo + simulación)
│   ├── config_parser.h    # clase ConfigParser (E/S + validación)
│   └── reporter.h         # clase Reporter (destino de salida)
├── src/                   # Implementación
│   ├── transition_table.cc
│   ├── tape.cc
│   ├── turing_machine.cc
│   ├── config_parser.cc
│   ├── reporter.cc
│   └── main.cc            # Punto de entrada (CLI)
├── config/                # Definiciones formales de las MT
│   ├── MT1.txt            # L = {aⁿbᵐ | m ≥ n, n > 0}
│   └── MT2.txt            # Cuenta b's y a's (unario)
├── input/                 # Cadenas a probar por máquina
│   ├── MT1.txt
│   └── MT2.txt
├── test/                  # Script de pruebas + ficheros de error
│   ├── run_errors.sh
│   └── errors/error_*.txt
├── Makefile
└── README.md
```

## Compilación

```bash
make          # compila y genera el binario mt_sim en la raíz
make clean    # elimina build/ y el binario mt_sim
```

Los artefactos intermedios (`.o`, `.d`) se generan en `build/` (*out-of-source build*); el
binario final **`mt_sim`** se deposita en la raíz del proyecto.

## Uso

```bash
./mt_sim -config <fichero_config> -in <fichero_cadenas>
```

| Parámetro | Descripción |
|---|---|
| `-config <f>` | Fichero con la definición formal de la MT. |
| `-in <f>` | Fichero con las cadenas de entrada (una por línea). |

Ambos parámetros son **obligatorios**. Los resultados se escriben en el fichero **`output.txt`**
en el directorio de ejecución (se sobrescribe en cada ejecución). Cualquier parámetro
incorrecto, fichero no accesible o definición inválida aborta con un mensaje por `std::cerr` y
código de retorno `1`.

## Fichero de configuración

Las líneas que empiezan por `#` son comentarios. Las líneas en blanco se ignoran. Las **siete
primeras líneas significativas** son obligatorias, en este orden:

1. **Estados Q** (separados por espacios): `qs q0 q1 q2 q3`
2. **Alfabeto de entrada Σ** (símbolos de un carácter): `a b`
3. **Alfabeto de cinta Γ** (símbolos de un carácter, incluye el blanco): `a b .`
4. **Estado inicial s** (un único estado): `qs`
5. **Símbolo blanco b** (un único carácter): `.`
6. **Estados finales F** (al menos uno): `q3`
7. **Número de cintas k** (entero ≥ 1): `2`

Las líneas siguientes son **transiciones**, una por línea. Para `k` cintas cada transición tiene
exactamente `3k + 2` tokens:

```text
origen  read_1 ... read_k  destino  write_1 move_1  ...  write_k move_k
```

La **clave** de una transición es `(estado_origen, read_1 ... read_k)`. Ejemplos:

```text
# k = 1
q0 0 q1 0 R
# k = 2
q0 a . q0 a R a R
# k = 3
q0 a . . q0 a R 1 R . S
```

### Máquinas incluidas

| Fichero | Problema | Estados | Cintas | Finales | Transiciones |
|---|---|---|---|---|---|
| `config/MT1.txt` | `L = { aⁿbᵐ \| m ≥ n, n > 0 }` | `qs, q0..q3` | 2 | `{q3}` | 9 |
| `config/MT2.txt` | Cuenta b's y a's (unario) | `q0..q15` | 3 | `{q15}` | 41 |

Ambas se obtuvieron de los ficheros JFLAP `MT1.jff` / `MT2.jff` (el blanco vacío de JFLAP se
traduce al símbolo `.`). La imagen de cada grafo se entrega junto con la práctica.

## Matriz de errores de configuración

El parser detecta (y aborta con mensaje indicando la línea) los siguientes errores:

1. **E1** Ausencia de líneas obligatorias (`Q`, `Σ`, `Γ`, `s`, `b`, `F`, `k`).
2. **E2–E4** Conjuntos `Q`, `Σ` o `Γ` vacíos (comprobaciones defensivas).
3. **E5/E6** Símbolo de `Σ` o `Γ` con longitud distinta de 1.
4. **E7** Algún símbolo de `Σ` no pertenece a `Γ`.
5. **E8** El blanco `b` no pertenece a `Γ`.
6. **E9** El blanco `b` pertenece a `Σ`.
7. **E10/E11** Línea de estado inicial mal formada o estado ajeno a `Q`.
8. **E12** Línea del blanco que no es un único carácter.
9. **E13** Conjunto `F` vacío o con estados ajenos a `Q`.
10. **E14** El número de cintas `k` no es un entero ≥ 1.
11. **E15** Transición con número de tokens distinto de `3k + 2`.
12. **E16/E17** Estado origen o destino de una transición ajeno a `Q`.
13. **E18/E19** Símbolo leído o escrito ajeno a `Γ`.
14. **E20** Movimiento distinto de `L`, `R` o `S`.
15. **E21** Transición duplicada (rompe el determinismo).

## Formato de salida

Por cada cadena del fichero de entrada se escribe en `output.txt` un bloque:

```text
Entrada: "abbabaabb"
Resultado: ACEPTADA
Estado final: q15
Cinta 1: .[1]1111.1111.
Cinta 2: .[1]111.
Cinta 3: .[1]1111.

```

- `Resultado` es `ACEPTADA` si el estado de parada ∈ `F`, y `RECHAZADA` en caso contrario.
- Se muestran **todas** las cintas. El contenido es el rango materializado de la cinta y el
  símbolo bajo el cabezal aparece entre corchetes: `ab[c]d`.
- La cabeza de L/E parte del primer símbolo de la entrada (o de la celda blanca si la cadena es
  vacía). Al finalizar, el resultado se encuentra en la primera cinta.

## Algoritmo de simulación

- **Representación de la cinta:** `std::deque<char>` con un índice de cabezal. Solo se
  materializan las celdas usadas; lo que queda fuera, por cualquier extremo, es blanco. El
  `deque` crece en O(1) por ambos lados.
- **Ejecución (`Solve`):** se reinician las cintas (cinta 1 = entrada, resto = blanco) y se
  aplican transiciones **mientras existan**: se leen las `k` cintas, se forma la clave y, si hay
  transición, se escribe y se mueve cada cabezal de forma simultánea. Al no hallar transición,
  se acepta si el estado actual ∈ `F`.
- **Determinismo:** a lo sumo una transición por `(estado, tupla de lecturas)`.

## Arquitectura (clases)

| Clase | Responsabilidad |
|---|---|
| `Transition` | Una regla (`id`, `next_state`, `write_symbols`, `moves`). |
| `TransitionTable` | La función δ: `std::map<(origen, lecturas) -> Transition>`; asigna ids y detecta duplicados. |
| `Tape` | Una cinta (`std::deque<char>` + cabezal); `Load`, `Reset`, `Read`, `Write`, `Move`, `ToString`. |
| `TuringMachine` | Modelo formal y simulación (`ProcessFile`, `Solve`); guarda `Q, Σ, Γ, s, b, F, k`, δ y las cintas. |
| `ConfigParser` | E/S y validación del fichero de configuración; construye el `TuringMachine`. |
| `Reporter` | Vuelca el texto ya montado al destino (fichero). |

## Pruebas

```bash
bash test/run_errors.sh
```

El script compila si es necesario y comprueba:

- 5 errores de CLI (parámetros, banderas, apertura de los tres ficheros).
- Los errores de configuración (ficheros en `test/errors/`).
- 3 casos positivos (MT1 acepta/rechaza y MT2), verificando el contenido de `output.txt`.

## Ejemplos rápidos

```bash
# Reconocedor L = {a^n b^m | m >= n, n > 0}
./mt_sim -config config/MT1.txt -in input/MT1.txt
cat output.txt

# Transductora: número de b's y de a's en unario
./mt_sim -config config/MT2.txt -in input/MT2.txt
cat output.txt
```
