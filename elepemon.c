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
#include "inih/ini.h"
#include "elepemon.h"
 #include <dlfcn.h>
 
/*-------------------------------------------------------------------------------*/

char elepemon_actual[20];
int contador;
struct handler_node* GLOBAL;

/*-------------------------------------------------------------------------------*/

/*
 *FUNCIONES OBLIGATORIAS
 *
 *Estas son las funciones requeridas por la tarea
 *
 */

void print_elepemon(const struct elepemon* elepemon)
{
	printf("\n");
	printf("\t---------------------------------\n");

    printf("\t        %s Stats:\n",elepemon->name);
    printf("\t\tHp = %d\n",elepemon->hp);

    /*Imprimimos el tipo*/
    if(elepemon->type==NORMAL){
        printf("\t\ttipo = NORMAL\n");
    }else if(elepemon->type==WATER){
        printf("\t\ttipo = WATER\n");
    }else if(elepemon->type==FIRE){
        printf("\t\ttipo = FIRE\n");
    }else if(elepemon->type==PLANT){
        printf("\t\ttipo = PLANT\n");
    }else{
        printf("\t\ttipo = NORMAL\n");
    }

    printf("\t\tPower = %d\n",elepemon->power);
    printf("\t\tDefense = %d\n",elepemon->defense);
    printf("\t\tNum. Ataques = %d\n",elepemon->attack.atack_count);

    /*Imprimos los ataques*/
    for(int i=0;i<elepemon->attack.atack_count;i++)
    {
        printf("\t\t   -Ataque %d: %s\n",i,elepemon->attack.attack_ids[i]);
    }

    printf("\t---------------------------------\n");
    printf("\n");
}

void init_elepemon(struct elepemon* elepemon)
{
	/*Inicializamos elepemon. Primero pedimos memoria*/
    elepemon=malloc(sizeof(struct elepemon));
    elepemon->name="";
    elepemon->hp=0;
    elepemon->type=NORMAL;
    elepemon->power=0;
    elepemon->defense=0;
    elepemon->attack.atack_count=0;
    elepemon->attack.attack_ids=NULL;
    elepemon->attack.attacks=NULL;
}

void free_elepemon(struct elepemon* elepemon)
{   
	/*Liberamos el puntero al nombre*/
    free(elepemon->name);

    /*Liberamos cada ataques*/
    for(int i=0;i<elepemon->attack.atack_count;i++){
        free(elepemon->attack.attack_ids[i]);
    }

    /*Liberamos el puntero al array de ataques*/
    free(elepemon->attack.attack_ids);

    /*Liberamos puntero al string de ataques*/
    free(elepemon->attack.attacks);
}

char** get_attack_ids(const char* str, int* n)
{
	/*Asignamos en 0 la cantidad 
	 *inicial de ataques*/
	*n = 0;

	/*Variables auxiliares*/
	int i = 0;
	char** ids_array;
	char* auxiliar;
	char* auxiliar2;

	/*Asignamos al auxiliar el string entregado*/
	auxiliar = strdup(str);

	/*Determinamos cuantos ataques tenemos
	 *contando las comas*/
	for (i = 0; i < strlen(str); i++){
		if (str[i] == ',')
			(*n)++;
	}
	(*n)++;

	/*Asignamos la memoria de acuerdo a la 
	 *cantidad de ataques*/
	ids_array = malloc(sizeof(char *) * (*n));
	
	/*Reiniciamos contador para volver a utilizarlo*/
	i = 0;

	/*Agregamos ataques al arreglo*/
	while ((auxiliar2 = strsep(&auxiliar, ",")) != NULL){
		*(ids_array+i) = strdup(auxiliar2);
		i++;
	}

	/*Liberamos memoria*/
	free(auxiliar); 
	free(auxiliar2);

	return ids_array;
}

int load_attacks(const char* attacks_filedir, struct elepemon* elepemon)
{

    void *handler;
    char *file;

    elepemon->attack.attacks=malloc(sizeof(attack_t)*elepemon->attack.atack_count);

    for(int contador=0; contador < elepemon->attack.atack_count; contador++)
    {

        file=malloc(sizeof(char)*(strlen(elepemon->attack.attack_ids[contador])+strlen("lib")+strlen(".so")+strlen(attacks_filedir)+strlen("1")));
        
        file=strcpy(file,attacks_filedir);
        file=strcat(file,"lib");
        file=strcat(file,elepemon->attack.attack_ids[contador]);
        file=strcat(file,".so");

        handler=dlopen(file,RTLD_NOW);

        if(handler==NULL){
            file=strcat(file,"1");
            handler=dlopen(file,RTLD_NOW);
            if(handler==NULL){
                printf("%s\n",dlerror());
                return 0;
            }
        }

        free(file);
        push_handler(&GLOBAL,handler);

        elepemon->attack.attacks[contador]=dlsym(handler,ATTACK_FN);
    }
    return 1;
}


void unload_attacks(struct handler_node** stack)
{
    while(*(stack)!=NULL)
    {
        struct handler_node *auxiliar = *stack;
        *stack=(*stack)->next;
        dlclose(auxiliar->handler);
        free(auxiliar);
    }
}

struct elepemon_node* push_elepemon(struct elepemon_node** stack, const char* name)
{
    /*Se pide memoria*/
    struct elepemon_node* nuevo = malloc(sizeof(struct elepemon_node));

    /*Asignamos el nombre del elepemon al nodo para
     *que la funcion handler luego agregue los stast
     *en él*/
    nuevo->elepemon.name = strdup(name);

    /*Se agrega el nodo al stack*/
    nuevo->next = *stack;
    *stack = nuevo;

    /*Se retorna el puntero al nodo agregado*/
    return *stack;
}

struct elepemon* get_elepemon(struct elepemon_node* stack, const char* name)
{	
	/*Si el stack esta vacío retornamos NULL*/
	if (stack_vacio(stack)==1){
		return NULL;

	/*Si no, procedemos a buscar el elepemon*/
	}else{

		/*Si al analozar el nodo lo encuentra,
		 *lo retorna*/
		if (strcmp(stack->elepemon.name,name) == 0){
			return &(stack->elepemon);

		/*Si no, vuelve a llamar la función*/
		}else{
			get_elepemon(stack->next,name);
		}
	}
}

void free_elepemon_stack(struct elepemon_node *stack)
{   
    /*Pedimos memoria para variable auxiliar*/
    struct elepemon_node *auxiliar = malloc(sizeof(struct elepemon_node));

    /*Si el stack no esta vacio, procedemos a ingresar a el*/
    if(stack_vacio(stack)==0){

        /*Guardamos el stack en el auxiliar*/
        auxiliar = stack;

        /*Corremos un nodo en el stack*/
        stack = stack->next;

        /*Liberamos la memoria del nodo que habiamos
         *retirado y habiamos guardado en el auxiliar*/
        free_elepemon(&(auxiliar->elepemon));

        /*Liberamos la memoria del auxiliar*/
        free(auxiliar);

        /*Volvemos a llamar la funcion para pasar
         *al sgte nodo*/
        free_elepemon_stack(stack);

    }else{
        return;
    }
}

/*-------------------------------------------------------------------------------*/

/*
 *FUNCIONES ANEXAS
 *
 *Estas son las funciones creadas anexamente para
 *complementar la tarea. Su documentacion se puede
 *encontrar en el fichero 'elepemon.h'
 *
 */

/*FUNCIONES APLICADAS A LOS STACKS DE ELEPEMONES*/

struct elepemon_node* pop_elepemon(struct elepemon_node** stack, const char* name)
{   
    /*Pedimos la memoria del nodo a agregar*/
    struct elepemon_node* auxiliar;

    /*Si encuentra un nodo con el nombre del elepemon
     *solo modifica el puntero next para pasar al sgte
     *nodo*/
    if (strcmp((*stack)->elepemon.name,name)==0){
        auxiliar = *stack;
        (*stack) = auxiliar->next;
        
        return auxiliar;
    }
    /*Si encuentra el nombre en el nodo sgte, gacemos lo
     *mismo que antes*/
    else if (strcmp((*stack)->next->elepemon.name,name)==0){
        auxiliar = (*stack)->next;
        (*stack)->next = auxiliar->next;
        
        return auxiliar;
    /*Si no, volvemos a llamar la funcion con el sgte nodo*/
    }else{
        pop_elepemon(&((*stack)->next),name);
    }
}

int stack_vacio(struct elepemon_node* stack)
{
    /*Si el stack apunta a NULL es que está vacio*/
    if(stack==NULL){
        return 1;
    /*Si no, contiene elementos*/
    }else{
        return 0;
    }
}

void load_type(struct elepemon_node** elepemon, const char* type)
{   

    /*La funcion compara el string con el tipo con el nombre del tipo
     *que se desea agregar. Cuando lo encuentra modifica elepemon.type
     *con este tipo*/
    if(strcmp(type,"NORMAL")==0){
        (*elepemon)->elepemon.type = NORMAL;
    }
    else if(strcmp(type,"WATER")==0){
        (*elepemon)->elepemon.type = WATER;
    }
    else if(strcmp(type,"FIRE")==0){
        (*elepemon)->elepemon.type = FIRE;
    }
    else if(strcmp(type,"PLANT")==0){
        (*elepemon)->elepemon.type = PLANT;
    }
}

int length_stack(struct elepemon_node* stack)
{
    /*Cuando la pila este vacia
     *retorna 0*/
    if(stack_vacio(stack)==1){
        return 0;
    /*Si no, procede a sumar uno y evaluar
     *el sgte nodo de forma recursiva*/
    }else{
        return (1 + length_stack(stack->next));
    }
}

void go_over_stack(struct elepemon_node** stack)
{
    
    /*Variables auxiliares*/
    struct elepemon_node* auxiliar;
    int posicion = 1;

    /*Mientras el nodo que se
     *esta recorriendo apunte a
     *un sgte nodo, el ciclo for 
     *sigue*/
    for(auxiliar = *stack;auxiliar != 0;auxiliar = auxiliar->next){
        printf("\t        %d. %s\n",posicion,auxiliar->elepemon.name);
        posicion++;
    }

    free(auxiliar);
}

int check_elepemon(struct elepemon_node** stack, const char* name)
{
    /*Variable auxiliar*/
    struct elepemon_node* auxiliar;
    auxiliar = *stack;

    /*Si el stack esta vacio
     *retornamos 0
     */
    if(stack_vacio(auxiliar)==1){
        return 0;
    }

    /*Empezamos a recorrer los
     *nodos del stack retornando 1
     *cuando encuentre al elepemon*/
    if(strcmp(auxiliar->elepemon.name,name)==0){
        return 1;
    }else{
        check_elepemon(&(auxiliar->next),name);
    }   
}

void elepemones_disponibles(struct elepemon_node** stack)
{

    /*Se imprimen los elepemones disponibles del stack*/
    printf("|--------------------------------------------------|\n");
    printf("|             Elepemones disponibles               |\n");
    printf("|--------------------------------------------------|\n\n");
    go_over_stack(stack);
    printf("\n");
}

void elepemones_disponibles_jugador(struct elepemon_node** stack, const char* name)
{

    /*Se imprimen los elepemones disponibles del stack del jugador*/
    printf("|--------------------------------------------------|\n");
    printf("                  Equipo de %s             \n",name);
    printf("|--------------------------------------------------|\n\n");
    go_over_stack(stack);
    printf("\n");
}

struct elepemon_node* choice_elepemon(struct elepemon_node** stack_principal, struct elepemon_node** stack_jugador, const char* name)
{   
    /*Primeros imprimimos los elepemones disponibles*/
    elepemones_disponibles(stack_principal);

    /*Se pregunta el elepemon que
     *se desea elegir*/
    printf("> %s elije a tu elepemon n°%i. Ingresa el nombre: ",name,contador);
    scanf("%s",elepemon_actual);

    /*Si el elepemon se encuentra disponible se
     *procede a sacarlo del stack principal e
     *ingresarlo al stack del jugador*/
    if(check_elepemon(stack_principal,elepemon_actual)){

        struct elepemon_node* auxiliar = malloc(sizeof(struct elepemon_node));

        /*Sacamos el nodo del stack principal y lo guardamos
         *en una variable auxiliar*/
        auxiliar = pop_elepemon(stack_principal,elepemon_actual);

        if(stack_vacio(*stack_jugador)==1){
            auxiliar->next = NULL;
            *stack_jugador = auxiliar;

        }/*else{
            auxiliar->next  = *stack_jugador;
            *stack_jugador = auxiliar;
        }*/

        printf("\n");
        printf("\t[!] %s ha sido ingresado con exito. Ya no puede ser elegido.\n",elepemon_actual);
        printf("\n");

        elepemones_disponibles_jugador(stack_jugador,name);

    /*Si el elepemon no existe o ya no esta disponible,
     *se vuelve a llamar a la funcion*/
    }else{
        printf("\n");
        printf("\t[!] El elepemon no existe o ya no se encuentra disponible\n");
        printf("\n");
        choice_elepemon(stack_principal,stack_jugador,name);
    }
}

/*FUNCIONES APLICADAS AL STACK DE HANDLER*/

void push_handler(struct handler_node** stack, void* handler)
{
    /*Creamos nodo auxiliar y pedimos memoria*/
    struct handler_node *auxiliar;
    auxiliar = malloc(sizeof(struct handler_node));

    /*Agregamos el nodo al handler siguiendo la 
     *misma lógica usada en otras funciones*/
    auxiliar->handler = handler;
    auxiliar->next = *stack;
    *stack = auxiliar;
}