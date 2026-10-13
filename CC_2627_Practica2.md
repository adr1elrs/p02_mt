# Complejidad Computacional — Curso 2026/27

## PRÁCTICA 2: Programar un simulador de una Máquina de Turing determinista

### Objetivo

El objetivo de la práctica consiste en programar un simulador de una Máquina de Turing,
realizando un diseño orientado a objetos y resolver dos problemas mediante una máquina de
Turing.

### Entrega

La ejecución de la práctica será revisada en la sesión de entrega en el laboratorio y la
entrega se realizará a través del aula virtual:

Es necesario proporcionar el código fuente en su totalidad, incluyendo todos los archivos y
librerías indispensables para su compilación y ejecución. Se incluirá un archivo README.

Las definiciones de las Máquinas de Turing (M = (Q, ∑, Γ, s, b, F, δ)) diseñadas para
resolver los dos problemas propuestos deben entregarse en dos formatos: fichero de
configuración para el simulador y la imagen del grafo. La ruta y nombres de los archivos
entregados deben aparecer en el fichero README.

Fecha límite de entrega: **23 de octubre de 2026**.

### Enunciado

En la práctica se desarrollará una MT multicinta determinista [1], donde la finalización de la
ejecución vendrá determinada únicamente por la ausencia de transiciones. Para realizar la
implementación se deberá elegir entre las siguientes variaciones posibles (seleccionar una
opción en cada fila). Las opciones seleccionadas deben indicarse en el README.

- Máquina de Turing con escritura y movimientos simultáneos o independientes.
- Máquina de Turing donde los únicos movimientos sean izquierda (L) y derecha (R) o que
  incluya también la posibilidad de no movimiento (S).
- Máquina de Turing con cinta infinita en una única dirección o en ambas direcciones.

Los elementos de la Máquina de Turing se introducirán en tiempo de ejecución del programa
utilizando un fichero de texto con el siguiente formato:

```text
# Comentarios
q1 q2 q3 …     # conjunto Q
a1 a2 a3 …     # conjunto Σ
A1 A2 A3 …     # conjunto Γ1

q1             # estado inicial
b              # símbolo blanco
q2 q3          # conjunto F
1              # número de cintas
q1 a1 q2 a2 m  # función de transición para 1 cinta2: δ (q1, a1) = (q2, a2, m)

...            # cada una de las transiciones en una línea distinta
```

> 1 Los símbolos contenidos en Γ estarán formados por un único carácter.
>
> 2 La estructura de las transiciones se modificará, si es necesario, para adaptarse a las
> características de la MT a implementar, pero siempre debe existir un espacio en blanco de
> separación entre cada elemento de la transición.

Se debe verificar que la información proporcionada en el fichero cumpla con las restricciones
de la definición formal de una Máquina de Turing, por ejemplo: s ∈ Q.

Una vez leída la definición de la Máquina de Turing, debe ser posible ejecutarla con
diferentes parámetros de entrada. Los diferentes valores de entrada podrán ser introducidos
por teclado o por fichero (no es necesario permitir los dos métodos).

Inicialmente, la cabeza de L/E debe encontrarse en el primer símbolo de los parámetros de
entrada. Para cada entrada, la salida del programa debe incluir:

- Si la MT se ha parado en un estado de aceptación o no.
- El contenido de la cinta, indicando la posición en la que se encuentra la cabeza de L/E.
  Por ejemplo, "ab[c]d" si la cabeza de L/E apunta al símbolo "c".

Cuando el número de cintas sea mayor que 1, inicialmente la entrada estará únicamente en la
primera cinta. Al finalizar la ejecución, el resultado debe estar también en la primera cinta.

### Notas de implementación

1. Los lenguajes de programación permitidos son C++ o Java.
2. Se debe utilizar un diseño orientado a objetos.
3. Para organizar el proyecto se va a utilizar una estructura estándar [2].
4. La compilación y ejecución completa del programa se debe realizar en consola (no se debe
   implementar un entorno gráfico).
5. El programa principal aceptará las siguientes opciones por línea de comandos:
   - `-config <f>`, f es un fichero de texto con la configuración de la MT.
   - `[-in <f>]`, f es un fichero de texto con las cadenas de entrada a comprobar.
     Opcional. Si no se incluye, las cadenas de entrada se introducirán por teclado.

### Máquinas de Turing a diseñar

1. MT que reconozca el lenguaje L = { aⁿbᵐ | m ≥ n, n > 0 }
2. MT que reciba como parámetro una cadena compuesta por símbolos 'a' y 'b'. La MT debe
   sustituir la cadena por el número de símbolos 'b', seguido del número de símbolos 'a'
   separados por un símbolo blanco. El número se codificará como n = 1ⁿ.

   La cabeza de L/E debe quedar al principio del resultado:

   Ejemplo:
   - Cinta inicial (con la cadena de entrada): `•abbabaabb•`
   - Cinta final (con el resultado): `•11111•1111•`

   Si la cadena está compuesta por símbolos del mismo tipo, se reflejará con un "0" la
   ausencia del otro símbolo:

   Ejemplos:
   - Cinta al inicio (con la cadena de entrada): `•aa•`
   - Cinta al final (con el resultado): `•0•11•`
   - Cinta al inicio (con la cadena de entrada): `•bb•`
   - Cinta al final (con el resultado): `•11•0•`

### Referencias

- [1] Material de clase [Moodle]:
  https://campusvirtual.ull.es/2627/ingenieriaytecnologia/course/view.php?id=2627090029
- [2] Organizar un Proyecto C++:
  https://www.studyplan.dev/cmake/organizing-a-cpp-project
