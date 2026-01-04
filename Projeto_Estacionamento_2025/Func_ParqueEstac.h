//
//  Func_ParqueEstac.h
//  Projecto_AP_
//
//  Header com estruturas e prototipos para gestao do parque de estacionamento
//

#ifndef Func_ParqueEstac_h
#define Func_ParqueEstac_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utils.h"
#include "Func_Estacionamentos.h"

// Constantes maximas para dimensoes do parque
#define MAX_PISOS 5
#define MAX_FILAS 26
#define MAX_LUG_P_FILA 50

/**
 * Estrutura que representa um lugar de estacionamento
 *
 * matricAut - Matricula do veiculo estacionado (vazio se livre)
 * sit - Situacao do lugar:
 *            'L' = Livre (disponivel para estacionamento)
 *            'O' = Ocupado (veiculo estacionado)
 *            'i' = Indisponivel - condicoes inadequadas
 *            'o' = Indisponivel - obras
 *            'r' = Indisponivel - reservado
 *            'm' = Indisponivel - outros motivos
 * numE - Numero do registo de estacionamento (0 se livre/indisponivel)
 */
typedef struct lugarE {
    char matricAut[16];  // Matricula do automovel (max 15 chars + '\0')
    char sit;            // Situacao: 'L', 'O', 'i', 'o', 'r', 'm'
    int numE;            // Numero do estacionamento associado
} LUGAR_E;


// ==================== PROTOTIPOS DAS FUNCOES ====================
// Nota: Usamos void* para paramSist para evitar dependencia circular
// O ponteiro e convertido para o tipo correto dentro das funcoes

// --- Funcoes de Inicializacao ---
void InicializarParque(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], void *paramSist);
void SincronizarEstacionamentos(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], ESTAC vEstac[], void *paramSist);

// --- NOVA: Sincronizar por data/hora especifica ---
int SincronizarPorDataHora(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                            ESTAC vEstac[],
                            void *paramSist,
                            DATA dataConsulta,
                            HORA_MIN horaConsulta);

// --- Funcoes de Conversao de Codigos ---
int ConverterCodigoParaIndices(char *codigoLugar, int *piso, int *fila, int *lugar);
void ConverterIndicesParaCodigo(int piso, int fila, int lugar, char *codigoLugar);

// --- Funcoes de Contagem ---
int ContarLugaresLivresPiso(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], int piso, void *paramSist);
int ContarLugaresLivresTotais(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], void *paramSist);
void MostrarLugaresDisponiveis(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], void *paramSist);

// --- Funcoes de Procura de Lugares ---
int EncontrarLugarLivrePiso(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], int piso, void *paramSist, char *codigoLugar);
int EncontrarLugarLivreParque(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], void *paramSist, char *codigoLugar);

// --- Funcoes de Ocupacao/Libertacao ---
int OcuparLugar(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], char *codigoLugar, char *matricula, int numEstac, void *paramSist);
int LibertarLugar(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], char *codigoLugar, void *paramSist);

// --- Funcoes de Visualizacao ---
void MostrarMapaOcupacaoPiso(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], int piso, void *paramSist);

// --- Funcoes de Indisponibilidade ---
int MarcarLugarIndisponivel(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], char *codigoLugar, char motivo, void *paramSist);
int ReverterLugarIndisponivel(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], char *codigoLugar, void *paramSist);

// --- Funcoes auxiliares de comparacao de datas ---
int CompararDataHora(DATA d1, HORA_MIN h1, DATA d2, HORA_MIN h2);

#endif /* Func_ParqueEstac_h */
