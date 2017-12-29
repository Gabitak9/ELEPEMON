#include "attack.h"

struct atack_result* attack(struct elepemon* attacker, struct elepemon* defensor)
{
    /*Variables auxiliares*/
    int damage_extra;

    /*Inicializamos damage_extra con el valor
     *de acuerdo al nivel de ataque*/
    damage_extra = 15;

    /*Formula para calcular el daño
     *segun el ataque y defensa de los
     *elepemones*/
    float damage = damage_extra + (attacker->power/defensor->defense)*10;

    /*Creamos la estructura a retornar y le asignamos
     *memoria*/
    struct atack_result* attack_result;
    attack_result = malloc(sizeof(struct atack_result));

    attack_result->atack_id = "Hoja Afilada";
    attack_result->defensor = defensor;
    attack_result->attacker = attacker;
    attack_result->effect = NOEFFECT;

    if (defensor->type == FIRE) {
        damage = damage - 10;
    } else if (defensor->type == WATER) {
        damage = damage + 20;
    } else {
        damage = damage;
    }

    /*Agregamos el daño a la estructura
     *de resultado*/
    attack_result->damage_done = damage;

    /*Finalmente aplicamos el daño al hp del
     *elepemon defensor*/
    defensor->hp = (defensor->hp) - (attack_result->damage_done);
    return attack_result;
}