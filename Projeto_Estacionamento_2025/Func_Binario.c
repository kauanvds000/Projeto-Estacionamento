//
//  Func_Binario.c
//  Projecto_AP_
//
//  Funcoes para gravar e carregar dados em ficheiros binarios
//

#include "Func_Binario.h"
#include <stdio.h>
#include <string.h>

// ==================== FUNCOES DE GRAVACAO ====================

/**
 * Grava o vetor de estacionamentos num ficheiro binario
 */
int GravarEstacionamentosBinario(ESTAC vEstac[], int nEstac) {
    FILE *fp = fopen(FICHEIRO_BIN_ESTAC, "wb");

    if (fp == NULL) {
        printf("[ERRO] Nao foi possivel criar ficheiro %s\n", FICHEIRO_BIN_ESTAC);
        return 0;
    }

    // Primeiro gravar o numero de registos
    if (fwrite(&nEstac, sizeof(int), 1, fp) != 1) {
        printf("[ERRO] Falha ao gravar contador de estacionamentos\n");
        fclose(fp);
        return 0;
    }

    // Gravar todos os registos
    if (nEstac > 0) {
        size_t escritos = fwrite(vEstac, sizeof(ESTAC), nEstac, fp);
        if (escritos != nEstac) {
            printf("[ERRO] Falha ao gravar estacionamentos (%zu de %d)\n", escritos, nEstac);
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    printf("[PERSISTENCIA] %d estacionamentos gravados em %s\n", nEstac, FICHEIRO_BIN_ESTAC);
    return 1;
}

/**
 * Grava o vetor de tarifas num ficheiro binario
 */
int GravarTarifasBinario(TARIFA vTarifas[], int nTarifas) {
    FILE *fp = fopen(FICHEIRO_BIN_TARIFAS, "wb");

    if (fp == NULL) {
        printf("[ERRO] Nao foi possivel criar ficheiro %s\n", FICHEIRO_BIN_TARIFAS);
        return 0;
    }

    // Primeiro gravar o numero de registos
    if (fwrite(&nTarifas, sizeof(int), 1, fp) != 1) {
        printf("[ERRO] Falha ao gravar contador de tarifas\n");
        fclose(fp);
        return 0;
    }

    // Gravar todos os registos
    if (nTarifas > 0) {
        size_t escritos = fwrite(vTarifas, sizeof(TARIFA), nTarifas, fp);
        if (escritos != nTarifas) {
            printf("[ERRO] Falha ao gravar tarifas (%zu de %d)\n", escritos, nTarifas);
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    printf("[PERSISTENCIA] %d tarifas gravadas em %s\n", nTarifas, FICHEIRO_BIN_TARIFAS);
    return 1;
}

/**
 * Grava os parametros do sistema num ficheiro binario
 */
int GravarParametrosBinario(P_SYST_M_ESTAT_C *params) {
    FILE *fp = fopen(FICHEIRO_BIN_PARAMS, "wb");

    if (fp == NULL) {
        printf("[ERRO] Nao foi possivel criar ficheiro %s\n", FICHEIRO_BIN_PARAMS);
        return 0;
    }

    // Gravar a estrutura inteira
    if (fwrite(params, sizeof(P_SYST_M_ESTAT_C), 1, fp) != 1) {
        printf("[ERRO] Falha ao gravar parametros\n");
        fclose(fp);
        return 0;
    }

    fclose(fp);
    printf("[PERSISTENCIA] Parametros gravados em %s\n", FICHEIRO_BIN_PARAMS);
    return 1;
}

/**
 * Grava TODOS os dados de uma vez
 */
int GravarTodosDados(ESTAC vEstac[], TARIFA vTarifas[], P_SYST_M_ESTAT_C *params) {
    int sucesso = 1;

    printf("\n[SISTEMA] A gravar dados...\n");

    if (!GravarEstacionamentosBinario(vEstac, params->nElemActE)) {
        sucesso = 0;
    }

    if (!GravarTarifasBinario(vTarifas, params->nElemActT)) {
        sucesso = 0;
    }

    if (!GravarParametrosBinario(params)) {
        sucesso = 0;
    }

    if (sucesso) {
        printf("[SISTEMA] Todos os dados foram gravados com sucesso!\n");
    } else {
        printf("[AVISO] Alguns dados podem nao ter sido gravados corretamente.\n");
    }

    return sucesso;
}


// ==================== FUNCOES DE LEITURA ====================

/**
 * Carrega estacionamentos de ficheiro binario
 */
int CarregarEstacionamentosBinario(ESTAC vEstac[], int maxEstac) {
    FILE *fp = fopen(FICHEIRO_BIN_ESTAC, "rb");

    if (fp == NULL) {
        // Ficheiro nao existe - nao e erro, pode ser primeira execucao
        return -1;
    }

    // Ler o numero de registos
    int nEstac = 0;
    if (fread(&nEstac, sizeof(int), 1, fp) != 1) {
        printf("[ERRO] Falha ao ler contador de estacionamentos\n");
        fclose(fp);
        return -1;
    }

    // Validar
    if (nEstac < 0 || nEstac > maxEstac) {
        printf("[ERRO] Numero de estacionamentos invalido: %d\n", nEstac);
        fclose(fp);
        return -1;
    }

    // Ler todos os registos
    if (nEstac > 0) {
        size_t lidos = fread(vEstac, sizeof(ESTAC), nEstac, fp);
        if (lidos != nEstac) {
            printf("[ERRO] Falha ao ler estacionamentos (%zu de %d)\n", lidos, nEstac);
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    printf("[PERSISTENCIA] %d estacionamentos carregados de %s\n", nEstac, FICHEIRO_BIN_ESTAC);
    return nEstac;
}

/**
 * Carrega tarifas de ficheiro binario
 */
int CarregarTarifasBinario(TARIFA vTarifas[], int maxTarifas) {
    FILE *fp = fopen(FICHEIRO_BIN_TARIFAS, "rb");

    if (fp == NULL) {
        return -1;
    }

    // Ler o numero de registos
    int nTarifas = 0;
    if (fread(&nTarifas, sizeof(int), 1, fp) != 1) {
        printf("[ERRO] Falha ao ler contador de tarifas\n");
        fclose(fp);
        return -1;
    }

    // Validar
    if (nTarifas < 0 || nTarifas > maxTarifas) {
        printf("[ERRO] Numero de tarifas invalido: %d\n", nTarifas);
        fclose(fp);
        return -1;
    }

    // Ler todos os registos
    if (nTarifas > 0) {
        size_t lidos = fread(vTarifas, sizeof(TARIFA), nTarifas, fp);
        if (lidos != nTarifas) {
            printf("[ERRO] Falha ao ler tarifas (%zu de %d)\n", lidos, nTarifas);
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    printf("[PERSISTENCIA] %d tarifas carregadas de %s\n", nTarifas, FICHEIRO_BIN_TARIFAS);
    return nTarifas;
}

/**
 * Carrega parametros do sistema de ficheiro binario
 */
int CarregarParametrosBinario(P_SYST_M_ESTAT_C *params) {
    FILE *fp = fopen(FICHEIRO_BIN_PARAMS, "rb");

    if (fp == NULL) {
        return 0;
    }

    // Ler a estrutura inteira
    if (fread(params, sizeof(P_SYST_M_ESTAT_C), 1, fp) != 1) {
        printf("[ERRO] Falha ao ler parametros\n");
        fclose(fp);
        return 0;
    }

    fclose(fp);
    printf("[PERSISTENCIA] Parametros carregados de %s\n", FICHEIRO_BIN_PARAMS);
    return 1;
}


// ==================== FUNCOES DE VERIFICACAO ====================

/**
 * Verifica se existem ficheiros binarios de dados persistidos
 */
int ExistemDadosPersistidos() {
    FILE *fp = fopen(FICHEIRO_BIN_ESTAC, "rb");

    if (fp != NULL) {
        fclose(fp);
        return 1;
    }

    return 0;
}
