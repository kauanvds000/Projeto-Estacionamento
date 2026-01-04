//
//  Func_Tarifas.h
//    Projecto_AP_
//
//

#ifndef Func_Tarifas_h
#define Func_Tarifas_h


#include "Utils.h"

typedef struct tarifa  // Estrutura das Tarifas a usar no vector de tamanho fixo
{
    char tpTarif;  // tipo de tarifa: pode ser H, se o valor a pagar for calculado em termos de horas
                   //                 pode ser D, se o cálculo vai ser efectuado em número de dias do estacionamenrto
    char codTarif[4]; // código da tarifa
    HORA_MIN inferior; // limite inferior do período da tarifa
    HORA_MIN superior; // limite superior do período da tarifa
    DATA dataNasc;
    float valorUnit;  // valor a pagar por cada hora ou dia de estacionamento
    DATA dtUltRequis;   // colocar a 0, quando se carrega do fich. fornecido
}TARIFA;

// Protótipos relativos às funções existentes no ficheiro .c

void MostrarTarifa(TARIFA T[], int idx);
#endif /* Func_Tarifas_h */
