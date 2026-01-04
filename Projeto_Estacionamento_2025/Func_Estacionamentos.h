//
//  Func_Estacionamentos.h
//  Projecto_AP_
//
//  Funcoes para gestao de estacionamentos e calculo de tarifas
//

#ifndef Func_Estacionamentos_h
#define Func_Estacionamentos_h

#include "Utils.h"

// Constantes de Tarifas (podem ser facilmente alteradas)
#define TARIFA_DIURNA 0.60      // T1: 08:00-21:59 em euros/hora
#define TARIFA_NOTURNA 0.30     // T2: 22:00-07:59 em euros/hora
#define TARIFA_DIA_COMPLETO 8.00 // T3: dia completo (sem mudar de dia)
#define TARIFA_MULTI_DIA 6.00   // T4: varios dias (euros/dia)

// Hora de inicio do periodo diurno (08:00)
#define HORA_INICIO_DIURNO 8
// Hora de inicio do periodo noturno (22:00)
#define HORA_INICIO_NOTURNO 22

/**
 * Estrutura de um registo de estacionamento
 *
 * numE - Numero sequencial do estacionamento (gerado automaticamente)
 * matricVeiculo - Matricula do veiculo (max 14 chars)
 * entradaDt - Data de entrada
 * entradaHm - Hora/minuto de entrada
 * lugar - Codigo do lugar (ex: "1A01")
 * saidaDt - Data de saida (0/0/0 se ainda no parque)
 * saidaHm - Hora/minuto de saida
 * valPag - Valor pago
 * obs - Observacoes
 */
typedef struct estacionamento {
    unsigned int numE;
    char matricVeiculo[15];
    DATA entradaDt;
    HORA_MIN entradaHm;
    char lugar[5];
    DATA saidaDt;
    HORA_MIN saidaHm;
    float valPag;
    char obs[31];
} ESTAC;


// ==================== PROTOTIPOS DAS FUNCOES ====================

// --- Funcoes de Registo ---
void RegistarEstacionamento(ESTAC E[], int idx, unsigned int _numE, char _mat[],
                            DATA _dtEnt, HORA_MIN _hmE, DATA _dtSaida, HORA_MIN _hmS,
                            float _valPag, char obs[]);

// --- Funcoes de Visualizacao ---
void MostrarEstacionamento(ESTAC E[], int idx);
void MostrarTicketEntrada(unsigned int numE, char *matricula, char *lugar,
                          DATA dtEnt, HORA_MIN hmEnt);
void MostrarTicketSaida(ESTAC *estac, float valorPago);

// --- Funcoes de Calculo de Tarifas ---

/**
 * Arredonda hora de entrada para o proximo periodo de 15 min (para baixo)
 * Ex: 8:10 -> 8:15, 8:00 -> 8:00, 8:14 -> 8:15
 */
HORA_MIN ArredondarHoraEntrada(HORA_MIN hm);

/**
 * Arredonda hora de saida para o proximo periodo de 15 min (para cima)
 * Ex: 9:55 -> 10:00, 9:45 -> 9:45, 9:01 -> 9:15
 */
HORA_MIN ArredondarHoraSaida(HORA_MIN hm);

/**
 * Calcula o numero de dias entre duas datas
 * Retorna o numero de dias (incluindo dias incompletos)
 */
int CalcularNumeroDias(DATA dtEntrada, DATA dtSaida);

/**
 * Verifica se uma hora esta no periodo diurno (08:00-21:59)
 * Retorna: 1 se diurno, 0 se noturno
 */
int EhPeriodoDiurno(int hora);

/**
 * Calcula o valor total a pagar pelo estacionamento
 * Aplica as regras do tarifario:
 * - T1 (0.60/h) para periodo diurno
 * - T2 (0.30/h) para periodo noturno
 * - T3 (8.00) se exceder e for mesmo dia
 * - T4 (6.00/dia) se >= 2 dias
 */
float CalcularValorEstacionamento(DATA dtEntrada, HORA_MIN hmEntrada,
                                   DATA dtSaida, HORA_MIN hmSaida);

// --- Funcoes de Validacao ---

/**
 * Valida formato de matricula portuguesa
 * Aceita: XX-XX-XX, XX-XX-00, 00-XX-XX (letras e numeros)
 * Retorna: 1 se valida, 0 se invalida
 */
int ValidarMatricula(char *matricula);

/**
 * Verifica se a data/hora de saida e posterior a data/hora de entrada
 * Retorna: 1 se valida, 0 se invalida
 */
int ValidarSaidaPosteriorEntrada(DATA dtEnt, HORA_MIN hmEnt, DATA dtSai, HORA_MIN hmSai);

// --- Funcoes de Pesquisa ---

/**
 * Procura um estacionamento pelo numero de entrada
 * Retorna: indice no vetor, ou -1 se nao encontrado
 */
int ProcurarEstacionamentoPorNumero(ESTAC vEstac[], int nEstac, unsigned int numE);

/**
 * Procura um estacionamento ativo (sem saida) pela matricula
 * Retorna: indice no vetor, ou -1 se nao encontrado
 */
int ProcurarEstacionamentoAtivoPorMatricula(ESTAC vEstac[], int nEstac, char *matricula);

#endif /* Func_Estacionamentos_h */
