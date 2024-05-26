#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tdas/list.h"
#include "tdas/stack.h"
#include "tdas/heap.h"
#include "tdas/queue.h"
#include "tdas/set.h"
#include "tdas/map.h"
#include "tdas/extra.h"
#include <string.h>
#define MAXPROFUNDIDAD 16

// Definición de la estructura para el estado del puzzle
typedef struct {
    int square[3][3]; // Matriz 3x3 que representa el tablero
    int x;    // Posición x del espacio vacío
    int y;    // Posición y del espacio vacío
    List* list_actions; //Secuencia de movimientos para llegar al estado (no se usa)
    int nodos; // Nodos generados
    int profundidad; // Profundidad del nodo
} State;

int distancia_L1(State* state) {
    int ev=0;
    int k=1;
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++){
            int val=state->square[i][j];
            if (val==0) continue;
            int ii=(val)/3;
            int jj=(val)%3;
            ev+= abs(ii-i) + abs(jj-j);
        }
    return ev;
}
// Función para imprimir el estado del puzzle
void imprimirEstado(const State *estado) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (estado->square[i][j] == 0)
                printf("x "); // Imprime un espacio en blanco para el espacio vacío
            else
                printf("%d ", estado->square[i][j]);
        }
        printf("\n");
    }
}
// Función para obtener los estados adjacentes según un estado dado
State* transition(State* state, int mov){
    State* newState = malloc(sizeof(State));
    newState->x = -1;
    newState->y = -1; // Inicializar posicion x e y;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            newState->square[i][j] = state->square[i][j]; // encontrar casilla vacia
        }
    }
    int x = state->x; // actualizar posicion x
    int y = state->y; // actualizar posicion y
    // casos de movimiento: 1 = arriba, 2 = abajo, 3 = izquierda, 4 = derecha;

    switch (mov) {
        case 1:
            if (x > 0) {
                newState->square[x][y] = newState->square[x - 1][y];
                newState->square[x - 1][y] = 0;
                newState->x = x - 1;
                newState->y = y;
            }
            break;
        case 2: 
            if (x < 2) {
                newState->square[x][y] = newState->square[x + 1][y];
                newState->square[x + 1][y] = 0;
                newState->x = x + 1;
                newState->y = y;
            }
            break;
        case 3: 
            if (y > 0) {
                newState->square[x][y] = newState->square[x][y - 1];
                newState->square[x][y - 1] = 0;
                newState->x = x;
                newState->y = y - 1;
            }
            break;
        case 4:
            if (y < 2) {
                newState->square[x][y] = newState->square[x][y + 1];
                newState->square[x][y + 1] = 0;
                newState->x = x;
                newState->y = y + 1;
            }
            break;
    }
    if (newState->x  < 0 || newState->x  >= 3 || newState->y < 0 || newState->y >= 3) {
        return NULL; // Validar limites
    }
    return newState;
}

void get_adjacent_states(State* state, List* adyacentes){
    for(int i=1; i<=4; i++){ // recorrer movimientos
        State* newState = transition(state, i); // validar cada movimiento
        if(newState != NULL){
            list_pushBack(adyacentes, newState);
        }    
    }
}
// Funcion para comparar estados
int stateCmp(void *state1,void *state2) { // funcion para hash del mapa
    State *a = (State*)state1; // convertir de void a state
    State *b = (State*)state2;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (a->square[i][j] != b->square[i][j]) { // verificar si son iguales
                return 0;
            }
        }
    }
    return 1;
}
// Función para determinar si un estado es un estado objetivo
int finalState(State* state){
    int solucion[3][3]= { // definir solucion
    {0,1,2},
    {3,4,5},
    {6,7,8}
    };
    for(int i=0; i<3 ;i++){ // recorrer estado
        for(int j = 0; j < 3; j++){
            if(state->square[i][j] != solucion[i][j]) return 0; // comparar casillas
        }
    }
    return 1;
}
// Función de busqueda por profundidad
void dfs(State* state) {
    Stack* pila = stack_create();
    Map* explorados = map_create(stateCmp); // Crear mapa de estados explorados
    int total = 0;

    stack_push(pila, state); // Insertar estado inicial en la pila
    map_insert(explorados, state, NULL);

    while (list_size(pila) != 0) { // Mientras nuestra pila no está vacía
        State* stateActual = stack_pop(pila); // Extrae un estado 
        int profundidad = stateActual->profundidad; // Obtener profundidad

        if (finalState(stateActual)) {
            printf("\nEstado final encontrado\n");
            printf("Nodos explorados: %d\n", total);
            printf("Profundidad del nodo: %d\n\n", stateActual->profundidad);

            List* solucion = list_create();
            while(stateActual != NULL){
                list_pushFront(solucion, stateActual);
                MapPair* pair = map_search(explorados, stateActual);
                stateActual = pair ? pair->value : NULL;
            }

            State* aux = list_first(solucion);
            int cont = 1;
            while(aux != NULL){
                if(cont == 1){
                    printf("Estado inicial\n");
                    imprimirEstado(aux);
                    aux = list_next(solucion);
                    puts("");
                }
                printf("Paso %d\n", cont);
                imprimirEstado(aux);
                aux = list_next(solucion);
                puts("");
                cont++;
            }

            printf("\nRequiere %d pasos para llegar a la solución\n", cont-1);
            list_clean(solucion);
            break; // terminar busqueda
        }  
        // para regresar y no seguir generando nodos despues de cierta altura
        if(profundidad >= MAXPROFUNDIDAD) continue;

        List* adyacentes = list_create(); // crear lista de adyacentes
        get_adjacent_states(stateActual, adyacentes); 

        State* newState = (State*) list_first(adyacentes); // recorrer adyacentes
        while(newState != NULL){
            newState->profundidad = profundidad + 1; // con cada iteracion baja un nivel

            if(map_search(explorados, newState) == NULL){ // si se encuentra un estado nuevo
                stack_push(pila, newState);
                map_insert(explorados, newState, stateActual);
            }
            else stack_push(pila, newState);

            newState = (State*)list_next(adyacentes); // avanzar siguiente adyacente
        }
        list_clean(adyacentes); // limpiar lista de adyacentes
        total++;

    }
    if(list_size(pila) == 0){ // si no se encuentra el estado final;
        printf("No se encontró una solución al puzzle después de %d nodos\n", total);
    }
    stack_clean(pila); // limpiar pila y mapa
    map_clean(explorados);
    free(explorados);
}
// Función de busqueda por anchura
void bfs(State* state) {
    Queue* cola = queue_create();
    Map* explorados = map_create(stateCmp);
    int total = 0;

    queue_insert(cola, state);
    map_insert(explorados, state, NULL);

    while (list_size(cola) != 0) {
        State* stateActual = queue_remove(cola);
        int profundidad = stateActual->profundidad;

        if (finalState(stateActual)) {
            printf("\nSolución encontrada!\n");
            printf("Cantdad de iteraciones realizadas: %d\n\n", total);
            List* solucion = list_create();
            while (stateActual != NULL) {
                list_pushFront(solucion, stateActual);
                MapPair* pair = map_search(explorados, stateActual);
                stateActual = pair ? pair->value : NULL;
            }
            State* aux = list_first(solucion);
            int cont = 1;
            while (aux != NULL) {
                if(cont == 1){
                    printf("Estado inicial:\n");
                    imprimirEstado(aux);
                    aux = list_next(solucion);
                    puts("");
                }
                printf("Paso N°%d:\n", cont);
                imprimirEstado(aux);
                aux = list_next(solucion);
                puts("");

                cont++;
            }
            list_clean(solucion);
            break;
        }
        // para regresar y no seguir generando nodos despues de cierta altura
        if (profundidad >= MAXPROFUNDIDAD) continue; 

        List* adyacentes = list_create(); // crear lista
        get_adjacent_states(state, adyacentes);
        State* newState = (State*)list_first(adyacentes); // recorrer adyacentes
        while (newState != NULL) {
            newState->profundidad = profundidad + 1; // con cada iteracion baja un nivel
            if(map_search(explorados, newState) == NULL){
                queue_insert(cola, newState);
                map_insert(explorados, newState, stateActual);
            }
            //else queue_insert(cola, newState);
            newState = (State*)list_next(adyacentes); // avanzar con siguiente adyacente
        }
        list_clean(adyacentes);
        total++;
    }

    if (list_size(cola) == 0) {
        printf("No se encontró solución después de %d nodos\n", total);
    }
    queue_clean(cola); // limpiar pila y mapa
    map_clean(explorados);
    free(explorados);
}

int main() {
    // Estado inicial del puzzle
    State estado_inicial = {
        {{0, 2, 8}, // Primera fila (0 representa espacio vacío)
         {1, 3, 4}, // Segunda fila
         {6, 5, 7}, // Tercera fila
         },  
        0, 0  // Posición del espacio vacío (fila 0, columna 1)
    };
    estado_inicial.list_actions = list_create();

    // Imprime el estado inicial
    printf("Estado inicial del puzzle:\n");
    imprimirEstado(&estado_inicial);
    printf("Distancia L1:%d\n", distancia_L1(&estado_inicial));


    //Ejemplo de heap (cola con prioridad)
        printf("\n***** EJEMPLO USO DE HEAP ******\nCreamos un Heap e insertamos 3 elementos con distinta prioridad\n");
        Heap* heap = heap_create();
        char* data = strdup("Cinco");
        printf("Insertamos el elemento %s con prioridad -5\n", data);
        heap_push(heap, data, -5 /*prioridad*/);
        data = strdup("Seis");
        printf("Insertamos el elemento %s con prioridad -6\n", data);
        heap_push(heap, data, -6 /*prioridad*/);
        data = strdup("Siete");
        printf("Insertamos el elemento %s con prioridad -7\n", data);
        heap_push(heap, data, -7 /*prioridad*/);

        printf("\nLos elementos salen del Heap ordenados de mayor a menor prioridad\n");
        while (heap_top(heap) != NULL){
            printf("Top: %s\n", (char*) heap_top(heap));      
            heap_pop(heap);
        }
        printf("No hay más elementos en el Heap\n");

        char opcion;
        do {
            printf("\n***** EJEMPLO MENU ******\n");
            puts("========================================");
            puts("     Escoge método de búsqueda");
            puts("========================================");

            puts("1) Búsqueda en Profundidad");
            puts("2) Buscar en Anchura");
            puts("3) Buscar Mejor Primero");
            puts("4) Salir");

            printf("Ingrese su opción: ");
            scanf(" %c", &opcion);

            switch (opcion) {
            case '1':
              dfs(&estado_inicial);
              break;
            case '2':
              bfs(&estado_inicial);
              break;
            case '3':
              //best_first(estado_inicial);
              break;
            }
            presioneTeclaParaContinuar();
            limpiarPantalla();

      } while (opcion != '4');

  return 0;
}
