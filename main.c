/*
 *
 *Tarea1 - Lenguajes de Programacion - 2016-2
 *Repositorio en gitlab: git@gitlab.labcomp.cl:gsepulve/tarea1LP-2016-2-gsepulve.git
 *
 */

/*-------------------------------------------------------------------------------*/

/*Librerias*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "inih/ini.h"
#include "elepemon.h"
#include "attacks/attack.h"

/*-------------------------------------------------------------------------------*/

/*Funcion MATCH*/
#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

/*Funcion Handler*/
static int handler(void* puntero_generico, const char* section, const char* name, const char* value)
{
    /*Casteo de la estructura void*/
    struct elepemon_node** auxiliar = (struct elepemon_node**)puntero_generico;

    /*Si el stack esta vacio, se ingresa el primer
     *elepemon*/
    if (stack_vacio(*auxiliar) == 1){
        /*Se pide memoria, y se inicializa con los valores
        por defecto, exepto el nombre*/
        (*auxiliar) = malloc(sizeof(struct elepemon_node));
        init_elepemon(&((*auxiliar)->elepemon));
        (*auxiliar)->elepemon.name = strdup(section);
        /*Se proceden a agregar los stats*/
        if (MATCH(section, "hp")) {
                (*auxiliar)->elepemon.hp = strtol(value,NULL,0);
            } else if (MATCH(section, "type")) {
                load_type(auxiliar,strdup(value));
            } else if (MATCH(section, "attacks")) {
                int contador = 0;
                (*auxiliar)->elepemon.attack.attack_ids = get_attack_ids(value,&contador);
                (*auxiliar)->elepemon.attack.atack_count = contador;
            } else if (MATCH(section, "power")){
                (*auxiliar)->elepemon.power = strtol(value,NULL,0);
            } else if (MATCH(section, "defense")){
                (*auxiliar)->elepemon.defense = strtol(value,NULL,0);
            } else {
                return 0;  /* No se encuentra section/name. Error */
            }
    }
    /*Si aun no se ha cambiado de seccion, se sigue
     *ingresando stats en el mismo nodo*/
    else if (strcmp(section,(*auxiliar)->elepemon.name)==0){
        if (MATCH(section, "hp")) {
                (*auxiliar)->elepemon.hp = strtol(value,NULL,0);
            } else if (MATCH(section, "type")) {
                load_type(auxiliar,strdup(value));
            } else if (MATCH(section, "attacks")) {
                int contador = 0;
                (*auxiliar)->elepemon.attack.attack_ids = get_attack_ids(value,&contador);
                (*auxiliar)->elepemon.attack.atack_count = contador;
            } else if (MATCH(section, "power")){
                (*auxiliar)->elepemon.power = strtol(value,NULL,0);
            } else if (MATCH(section, "defense")){
                (*auxiliar)->elepemon.defense = strtol(value,NULL,0);
            } else {
                return 0;  /* No se encuentra section/name. Error */
            }
    }
    /*Si no, se procede a agregar un nuevo elepemon
     *al stack*/
    else{
        push_elepemon(auxiliar,section);
        if (MATCH(section, "hp")) {
                (*auxiliar)->elepemon.hp = strtol(value,NULL,0);
            } else if (MATCH(section, "type")) {
                load_type(auxiliar,strdup(value));
            } else if (MATCH(section, "attacks")) {
                int contador = 0;
                (*auxiliar)->elepemon.attack.attack_ids = get_attack_ids(value,&contador);
                (*auxiliar)->elepemon.attack.atack_count = contador;
            } else if (MATCH(section, "power")){
                (*auxiliar)->elepemon.power = strtol(value,NULL,0);
            } else if (MATCH(section, "defense")){
                (*auxiliar)->elepemon.defense = strtol(value,NULL,0);
            } else {
                return 0;  /* No se encuentra section/name. Error */
            }
    }
    return 1;
}

/*-------------------------------------------------------------------------------*/

/*Variables a nivel Global*/
char P1[20];
char P2[20];
char elepemon_actual[20];
int cantidad_elepemones;
int contador;
int elepemon_por_jugador;
struct handler_node* GLOBAL;

/*-------------------------------------------------------------------------------*/

/*Funciones extras*/

void bienvenida()
{

    /*Menu de Inicio*/
    printf("|--------------------------------------------------|\n");
    printf("|             Tarea1LP:  ELEPEMON GO!              |\n");
    printf("|--------------------------------------------------|\n");
    printf(" BIENVENIDOS valientes entrenadores al LP Stadium! \n\n");
    printf("> Ingresa el nombre del entrenador 1: ");
    scanf("%s",P1);
    printf("> Ingresa el nombre del entrenador 2: ");
    scanf("%s",P2);
    printf("\n");
}

int validar(int cantidad)
{   
    int opcion;
    /*Se pide una opcion numerica*/
    printf(">¡ENTRENADORES! ¿Con cuantos elepemones luchara cada uno?: ");
    scanf("%i",&opcion);

    /*Se procede a evaluar si es posible*/
    if(((cantidad/2)>=opcion) && opcion<=6){
        printf("\n");
        printf("\t[!] Cantidad ingresada con exito.\n");
        printf("\n");
        printf("> Cada entrenador/a luchara con %i elepemon/es. ¡ES TIEMPO DE ELEGIR! \n",opcion);
        return opcion;
    }else{
        printf("\n");
        printf("\t[!] No es posible luchar con esa cantidad de elepemones.\n");
        printf("\n");
        validar(cantidad);
    }
}

void batalla(struct elepemon_node* stack_P1, struct elepemon_node* stack_P2)
{
    printf("|--------------------------------------------------|\n");
    printf("|----------- LA BATALLA COMENZARA AHORA -----------|\n");
    printf("|--------------------------------------------------|\n");
    printf("\n");

    /*Mientras alguno de los stack de los jugadores
     *NO este vacio, la batalla estará ejecutandose*/
    while(stack_vacio(stack_P1)==0 || stack_vacio(stack_P2)==0){

        contador = 0;
        if(contador==0){
            printf("¿%s que elepemon ataca?\n",P1);
        }else{
            printf("¿%s que elepemon ataca?\n",P2);
        }
        contador = 1;
    
    }
}
/*-------------------------------------------------------------------------------*/

int main(int argc, char* argv[])
{

    /*Damos la bienvenida al juego y pedimos
     *el nombre de los jugadores*/
    bienvenida();

    /*Creamos el primer nodo del stack donde
     *se almacenaran todos los elepemones
     *disponibles*/
    struct elepemon_node* stack;

    printf("\n");
    printf("\t[!] Cargando elepemones disponibles ...\n");

    /*Se verifica que el archivo ELEPEMONS.ini exista y se
     *procede a cargar todos los elepemones a un arreglo y
     *un stack*/
    if (ini_parse("ELEPEMONS.ini", handler, &stack) < 0) {
        printf("Can't load 'ELEPEMONS.ini'\n");
        return 1;
    }
    
    /*Se calcula la cantidad de elepemones en
     *el stack*/
    cantidad_elepemones = length_stack(stack);
    printf("\n");
    printf("\t[!] Se han cargado %i elepemones con exito\n",cantidad_elepemones);
    printf("\n");

    /*Se pide una cantidad de elepemones por jugador
     *y se valida*/
    elepemon_por_jugador = validar(cantidad_elepemones);

    /*Creamos el primero nodo de los stacks de
     *cada jugador*/
    struct elepemon_node* stack_P1 = NULL;
    struct elepemon_node* stack_P2 = NULL;

    contador = 1;
    while(contador<=elepemon_por_jugador)
    {   
        stack_P1 = choice_elepemon(&stack,&stack_P1,P1);
        stack_P2 = choice_elepemon(&stack,&stack_P2,P2);
        contador++;
    }


    /*Liberamos la memoria del stack principal
     *ya que ya no se usara*/
    free_elepemon_stack(stack);

    contador = 0;

    /*Cargamos los ataques*/
    load_attacks("attacks/",&(stack_P1->elepemon));
    load_attacks("attacks/",&(stack_P2->elepemon));
    printf("\n");
    printf("\t[!] Los ataques de los elepemones fueron cargados con éxito!\n");
    printf("\n");

    /*Inicio de batalla*/
    batalla(stack_P1,stack_P2);


    
}