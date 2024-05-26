# Tarea-Estructura-3

Descripcion:
Este programa se centra en la búsqueda y resolución de un puzzle-8 utilizando grafos implícitos. Entrega 2 opciones de algoritmos para recorrer cada estado hasta encontrar una solución y muestra cada paso con su tablero correspondiente, además de la cantidad de movimientos.

## Cómo compilar y ejecutar:

1. Visita [Repl.it](https://repl.it/).
2. Crea una nueva cuenta o inicia sesión si ya tienes una.
3. Una vez en tu dashboard, selecciona "New Repl" y elige "Import from GitHub".
4. Pega la URL del repositorio: `(https://github.com/littlenegrito/Tarea-Estructura-3.git)`.
5. [Repl.it](http://repl.it/) clonará el repositorio y preparará un entorno de ejecución.
6. Antes de ejecutar, copia el codigo `(gcc -o main main.c tdas/extra.c tdas/list.c tdas/map.c && ./main)` en la configuración de compilar
7. Presiona el botón "Run" para compilar y ejecutar la aplicación.

## Funcionalidades

### Funcionando correctamente:

- Distancia de estado actual a solución.
- Función de obtener estados adyacentes
- Mostrar estado de cada movimiento de tablero.
- Búsqueda de solución por profundidad
- Búsqueda de solución por anchura

### Problemas conocidos:

- La función de best_first sigue en desarrollo.
- Es recomendado utilizar la búsqueda por anchura, ya que la primera opción depende del límite recibido para cada caso y los resultados pueden variar.
