#ifndef ATTACK_H
#define ATTACK_H

#include "../elepemon.h"

/*
* Firma de funcion que cualquier ataque debiera implementar
* @param 1 atacante
* @param 2 defensor
* @return el resultado del ataque, alojado en el heap. debe ser liberado
*/
struct atack_result* attack(struct elepemon*, struct elepemon*);

#endif // ATTACK_H
