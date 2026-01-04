//
//  Func_Binario.h
//  Projecto_AP_
//
//  Funcoes para gravar e carregar dados em ficheiros binarios
//

#ifndef Func_Binario_h
#define Func_Binario_h

#include "Func_Estacionamentos.h"
#include "Func_Tarifas.h"
#include "InstalacaoN.h"

// Nomes dos ficheiros binarios
#define FICHEIRO_BIN_ESTAC "estacionamentos.bin"
#define FICHEIRO_BIN_TARIFAS "tarifas.bin"
#define FICHEIRO_BIN_PARAMS "parametros.bin"

// ==================== FUNCOES DE GRAVACAO ====================

/**
 * Grava o vetor de estacionamentos num ficheiro binario
 * @return 1 se sucesso, 0 se erro
 */
int GravarEstacionamentosBinario(ESTAC vEstac[], int nEstac);

/**
 * Grava o vetor de tarifas num ficheiro binario
 * @return 1 se sucesso, 0 se erro
 */
int GravarTarifasBinario(TARIFA vTarifas[], int nTarifas);

/**
 * Grava os parametros do sistema num ficheiro binario
 * @return 1 se sucesso, 0 se erro
 */
int GravarParametrosBinario(P_SYST_M_ESTAT_C *params);

/**
 * Grava TODOS os dados (estacionamentos + tarifas + parametros)
 * @return 1 se sucesso, 0 se erro
 */
int GravarTodosDados(ESTAC vEstac[], TARIFA vTarifas[], P_SYST_M_ESTAT_C *params);


// ==================== FUNCOES DE LEITURA ====================

/**
 * Carrega estacionamentos de ficheiro binario
 * @return numero de registos lidos, ou -1 se erro/ficheiro nao existe
 */
int CarregarEstacionamentosBinario(ESTAC vEstac[], int maxEstac);

/**
 * Carrega tarifas de ficheiro binario
 * @return numero de registos lidos, ou -1 se erro/ficheiro nao existe
 */
int CarregarTarifasBinario(TARIFA vTarifas[], int maxTarifas);

/**
 * Carrega parametros do sistema de ficheiro binario
 * @return 1 se sucesso, 0 se erro/ficheiro nao existe
 */
int CarregarParametrosBinario(P_SYST_M_ESTAT_C *params);


// ==================== FUNCOES DE VERIFICACAO ====================

/**
 * Verifica se existem ficheiros binarios de dados persistidos
 * @return 1 se existem, 0 se nao existem
 */
int ExistemDadosPersistidos();

#endif /* Func_Binario_h */
