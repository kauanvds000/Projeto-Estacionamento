//
//  InstalacaoN.h
//  Projecto_AP_
//
//  Header com estruturas e funcoes de instalacao/configuracao do sistema
//

#ifndef InstalacaoN_h
#define InstalacaoN_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "Func_Estacionamentos.h"
#include "Func_Tarifas.h"
#include "Utils.h"

// Limites para registos
#define MAX_REG_ESTAC 100000    // Maximo de estacionamentos
#define MIN_REG_ESTAC 3000      // Minimo de estacionamentos

#define MAX_REG_TARIFAS 10      // Maximo de tarifas
#define MIN_REG_TARIFAS 4       // Minimo de tarifas

/**
 * Estrutura que centraliza todos os parametros do sistema
 *
 * Esta estrutura guarda:
 * - Informacao de instalacao (datas, nomes)
 * - Configuracao do parque (pisos, filas, lugares)
 * - Contadores de tarifas
 * - Contadores de estacionamentos
 */
typedef struct paramSystMemStatAbCentraliz {
    // --- Informacao de Instalacao ---
    DATA dtInstalacao;          // Data em que o sistema foi instalado
    DATA dtUltProcess;          // Data do ultimo processamento/utilizacao
    char nomeEmpresa[61];       // Nome da empresa do parque (max 60 chars)
    char nomeInstalador[51];    // Nome de quem instalou (max 50 chars)

    // --- Configuracao do Parque de Estacionamento ---
    int numPisos;               // Numero de pisos configurados (1 a 5)
    int numFilas;               // Numero de filas por piso (1 a 26, A-Z)
    int nLugPFila;              // Numero de lugares por fila (1 a 50)

    // --- Parametros de Tarifarios ---
    int nElemActT;              // Numero atual de tarifas em memoria
    int nMaxRegDefT;            // Maximo de tarifas permitido
    int nRegImportadosT;        // Tarifas importadas do ficheiro .txt

    // --- Parametros de Estacionamentos ---
    int nElemActE;              // Numero atual de estacionamentos em memoria
    int nMaxRegDefE;            // Maximo de estacionamentos permitido
    int nRegImportadosE;        // Estacionamentos importados do ficheiro .txt
    int ultIdEstacE;            // ID do ultimo estacionamento (gerado sequencialmente)

} P_SYST_M_ESTAT_C;


// ==================== PROTOTIPOS DAS FUNCOES ====================

// --- Funcoes de Leitura de Ficheiros ---
int lerFicheiroTarif(TARIFA vTarifas[], P_SYST_M_ESTAT_C *paramSystS);
int lerFicheiroEstacionamento(ESTAC vEstac[], P_SYST_M_ESTAT_C *paramSystS);
int lerFicheiroParametros(FILE* F1, P_SYST_M_ESTAT_C *paramSist);

// --- Funcoes de Instalacao/Personalizacao ---
int personalizarAplicacaoF(TARIFA tarif[], ESTAC estac[], P_SYST_M_ESTAT_C *paramSystS);

// --- Funcoes de Visualizacao ---
void MostrarParametros(P_SYST_M_ESTAT_C p);

// --- Funcoes de Gestao de Tarifas ---
void CriarNovaTarifa(TARIFA vTarifas[], P_SYST_M_ESTAT_C *paramSystS);
void ModificarTarifa(TARIFA vTarifas[], int nTarifas);
void GuardarTarifasEmFicheiro(TARIFA vTarifas[], int nTarifas);


#endif /* InstalacaoN_h */
