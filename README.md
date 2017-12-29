**[TAREA RAMO UNIVERSIDAD]**

========================================
TAREA 1 / LENGUAJES DE PROGRAMACIÓN / 2016-2
========================================

INTEGRANTES
-------------
- **Gabriela Sepúlveda Bravo**
	- **Usuario DI**: gsepulve
	- **Rol**: 201573012-1
- **Beatriz Segura Pastén** 
	- **Usuario DI**: bsegura
	- **Rol**: 201573041-5

CONSIDERACIONES DEL JUEGO
------------------
- Ambos usuarios lucharán con la misma cantidad de elepemones.
- La elección de ellos será por turnos.
- Una vez seleccionado un elepemon en batalla, este no podrá ser cambiado a menos que sea totalmente debilitado.

SUPUESTOS
------------------
- Los Usuarios ingresaran nombres de no más de 20 carácteres.
- Cuando se pregunte una cantidad de elepemones, los usuarios ingresaran un número y no letras.
- El archivo ELEPEMON.ini no contendrá elepemones repetidos.

INSTALACIÓN DE ELEPEMON GO
------------------
- Lo primero que debes hacer es clonar el repositorio del proyecto en tu ordenador. Para ello utiliza el siguiente comando en la carpeta donde deseas guardar el juego:
```
git clone https://github.com/Gabitak9/ELEPEMON
```

- Para compilar ElepemonGO:
```
make COMPILE
```
- Para ejecutar ElepemonGo:
```
make RUN
```
- Para limpiar los directorios de archivos basura:
```
make CLEAN
```
La compilacion incluye la compilacion de la libreria `inih` (Librería proporcionada por el usuario [benhoyt](https://github.com/benhoyt/inih)) y la libreria `attacks`. Si usted quisiera compilar estas librerias por separado deberá seguir los siguientes comandos:

- Para la libreria  `inih` 
```
make INIH
```
- Para la libreria de ataques `attacks`
```
make ATTACKS
```
Se insiste en que el comando `make COMPILE` incluye la compilación de estas librerías, por lo que su uso no es necesario. Pero se explica su funcionamiento.

EJEMPLO DE OUTPUT
------------------

```
|--------------------------------------------------|
|             Tarea1LP:  ELEPEMON GO!              |
|--------------------------------------------------|
 BIENVENIDOS valientes entrenadores al LP Stadium! 

> Ingresa el nombre del entrenador 1: Red 
> Ingresa el nombre del entrenador 2: Ash


	[!] Cargando elepemones disponibles ...

	[!] Se han cargado 11 elepemones con exito

>¡ENTRENADORES! ¿Con cuantos elepemones luchara cada uno?: 1

	[!] Cantidad ingresada con exito.

> Cada entrenador/a luchara con 1 elepemon/es. ¡ES TIEMPO DE ELEGIR! 
|--------------------------------------------------|
|             Elepemones disponibles               |
|--------------------------------------------------|

	        1. Raichu
	        2. Pikachu
	        3. Charizard
	        4. Charmeleon
	        5. Charmander
	        6. Blastoise
	        7. Wartortle
	        8. Squirtle
	        9. Venusaur
	        10. Ivysaur
	        11. Bulbasaur

> Red elije a tu elepemon n°1. Ingresa el nombre: Pikachu

	[!] Pikachu ha sido ingresado con exito. Ya no puede ser elegido.

|--------------------------------------------------|
                  Equipo de Red             
|--------------------------------------------------|

	        1. Pikachu

|--------------------------------------------------|
|             Elepemones disponibles               |
|--------------------------------------------------|

	        1. Raichu
	        2. Charizard
	        3. Charmeleon
	        4. Charmander
	        5. Blastoise
	        6. Wartortle
	        7. Squirtle
	        8. Venusaur
	        9. Ivysaur
	        10. Bulbasaur

> Ash elije a tu elepemon n°1. Ingresa el nombre: Bulbasaur

	[!] Bulbasaur ha sido ingresado con exito. Ya no puede ser elegido.

|--------------------------------------------------|
                  Equipo de Ash             
|--------------------------------------------------|

	        1. Bulbasaur


	[!] Los ataques de los elepemones fueron cargados con éxito!

|--------------------------------------------------|
|----------- LA BATALLA COMENZARA AHORA -----------|
|--------------------------------------------------|

```

AVANCE TAREA
------------------
**FUNCIONES OBLIGATORIAS**

|FUNCIÓN|ESTADO|
|:----:|:---:|
|`print_elepemon()`|:D|
|`init_elepmon()`|:D|
|`free_elepemon()`|:D|
|`get_attacks_ids()`|:D|
|`load_attacks()`|:triumph:|
|`unload_attacks()`|:triumph:|
|`push_elepemon()`|:D|
|`get_elepemon()`|:D|
|`free_elepemon_stack()`|:D

**OBJETIVOS IMPLÍCITOS**

|OBJETIVO|ESTADO|
|:---:|:---:|
|Cargado y manejo de la liberia `inih` | Logrado |
|Creación de un Makefile | Logrado |
|Cargado dinámico de los ataques | Pendiente |

**PENDIENTE**

-  [x] Arreglar carga de elepemons al stack principal.
-  [ ] Corregir error en funcion `choice_elepemon()`  que permite solo ingresar un elepemon por jugador.

**ESTRATEGIA SEGUIDA/A SEGUIR**
- Se utilizo una estructura de **stack** para almacenar elepemones y manejarlos. Al iniciar el programa se cargaban todos los elepemones disponibles en un stack.
- Mientras el jugador elegía un elepemon, este se iba pasando a un stack destinado a él.
- Se cargarían los ataques de los elepemones en batalla.
- Se procedería a efectuar la batalla por turnos. Mientras ninguno de los stacks de los jugadores estuviera vacío, se estaría en un bucle de batalla.
- Se preguntaría al jugador que elepemon ataca y se efectuaría la funcion de ataque.
- Luego de cada ataque se corrobaria la vida de cada elepemon. Si alguno quedaba sin puntos de vida (o puntos de vida negrativos) este sería retirado del stack.
- Cuando un stack quedara vacío, la batalla terminaba y se anunciaba el ganador.
