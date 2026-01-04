//
//  Func_ParqueEstac.c
//  Projecto_AP_
//
//  Funcoes para gerir a matriz 3D do parque de estacionamento
//  A matriz representa: parque[PISO][FILA][LUGAR]
//

#include "Func_ParqueEstac.h"
#include "InstalacaoN.h"

/**
 * Compara duas datas/horas
 *
 * Retorna:
 *   -1 se d1/h1 < d2/h2  (primeira e anterior)
 *    0 se d1/h1 == d2/h2 (iguais)
 *    1 se d1/h1 > d2/h2  (primeira e posterior)
 */
int CompararDataHora(DATA d1, HORA_MIN h1, DATA d2, HORA_MIN h2) {
    // Comparar anos
    if (d1.ano < d2.ano) return -1;
    if (d1.ano > d2.ano) return 1;

    // Anos iguais, comparar meses
    if (d1.mes < d2.mes) return -1;
    if (d1.mes > d2.mes) return 1;

    // Meses iguais, comparar dias
    if (d1.dia < d2.dia) return -1;
    if (d1.dia > d2.dia) return 1;

    // Dias iguais, comparar horas
    if (h1.hora < h2.hora) return -1;
    if (h1.hora > h2.hora) return 1;

    // Horas iguais, comparar minutos
    if (h1.min < h2.min) return -1;
    if (h1.min > h2.min) return 1;

    // Tudo igual
    return 0;
}

/**
 * Inicializa todos os lugares do parque como livres
 */
void InicializarParque(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA], void *paramSist) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    for (int piso = 0; piso < p->numPisos; piso++) {
        for (int fila = 0; fila < p->numFilas; fila++) {
            for (int lugar = 0; lugar < p->nLugPFila; lugar++) {
                strcpy(parque[piso][fila][lugar].matricAut, "");
                parque[piso][fila][lugar].sit = 'L';
                parque[piso][fila][lugar].numE = 0;
            }
        }
    }

    printf("[PARQUE] Inicializado com %d pisos x %d filas x %d lugares = %d lugares totais\n",
           p->numPisos, p->numFilas, p->nLugPFila,
           p->numPisos * p->numFilas * p->nLugPFila);
}

/**
 * Converte um codigo de lugar (ex: "1A01" ou "1A1") para indices da matriz
 * Aceita formatos: "1A01", "1A1", "3I1", "3I15", etc.
 * Retorna: 1 se OK, 0 se invalido
 */
int ConverterCodigoParaIndices(char *codigoLugar, int *piso, int *fila, int *lugar) {

    int len = strlen(codigoLugar);

    // Minimo 3 caracteres (ex: "1A1"), maximo 4 (ex: "1A50")
    if (len < 3 || len > 4) {
        return 0;
    }

    // Primeiro caracter: piso (deve ser digito 1-5)
    if (codigoLugar[0] < '1' || codigoLugar[0] > '5') {
        return 0;
    }
    *piso = codigoLugar[0] - '1';  // '1' -> 0, '2' -> 1, etc.

    // Segundo caracter: fila (deve ser letra A-Z)
    if (codigoLugar[1] < 'A' || codigoLugar[1] > 'Z') {
        return 0;
    }
    *fila = codigoLugar[1] - 'A';  // 'A' -> 0, 'B' -> 1, etc.

    // Restantes caracteres: numero do lugar (1-50)
    *lugar = atoi(&codigoLugar[2]) - 1;  // "01" ou "1" -> 0, "50" -> 49

    // Validar intervalos
    if (*piso < 0 || *piso >= MAX_PISOS) return 0;
    if (*fila < 0 || *fila >= MAX_FILAS) return 0;
    if (*lugar < 0 || *lugar >= MAX_LUG_P_FILA) return 0;

    return 1;
}

/**
 * Converte indices da matriz para codigo de lugar (ex: "1A01")
 */
void ConverterIndicesParaCodigo(int piso, int fila, int lugar, char *codigoLugar) {
    sprintf(codigoLugar, "%d%c%02d", piso + 1, 'A' + fila, lugar + 1);
}

/**
 * Sincroniza a matriz com veiculos que ainda NAO sairam (saidaDt.ano == 0)
 * Usa-se para o estado ATUAL do parque em tempo real
 */
void SincronizarEstacionamentos(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                                 ESTAC vEstac[],
                                 void *paramSist) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    int veiculosNoParque = 0;
    int piso, fila, lugar;

    for (int i = 0; i < p->nElemActE; i++) {

        // Veiculo ainda no parque = data de saida a zeros
        if (vEstac[i].saidaDt.ano == 0) {

            if (ConverterCodigoParaIndices(vEstac[i].lugar, &piso, &fila, &lugar)) {

                if (piso < p->numPisos && fila < p->numFilas && lugar < p->nLugPFila) {

                    strcpy(parque[piso][fila][lugar].matricAut, vEstac[i].matricVeiculo);
                    parque[piso][fila][lugar].sit = 'O';
                    parque[piso][fila][lugar].numE = vEstac[i].numE;

                    veiculosNoParque++;
                }
            }
        }
    }

    printf("[PARQUE] Sincronizado (atual): %d veiculos atualmente no parque\n", veiculosNoParque);
}

/**
 * NOVA FUNCAO: Sincroniza o parque para uma data/hora especifica
 *
 * Mostra o estado do parque como estava naquela data/hora.
 * Um veiculo estava no parque se:
 *   - Data/hora de consulta >= Data/hora de entrada
 *   - Data/hora de consulta < Data/hora de saida (ou saida = 0, ainda la)
 *
 * Retorna: numero de veiculos que estavam no parque nessa data/hora
 */
int SincronizarPorDataHora(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                            ESTAC vEstac[],
                            void *paramSist,
                            DATA dataConsulta,
                            HORA_MIN horaConsulta) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    // Primeiro, reinicializar todos os lugares como livres
    for (int piso = 0; piso < p->numPisos; piso++) {
        for (int fila = 0; fila < p->numFilas; fila++) {
            for (int lugar = 0; lugar < p->nLugPFila; lugar++) {
                // Manter lugares indisponiveis como estao
                if (parque[piso][fila][lugar].sit != 'i' &&
                    parque[piso][fila][lugar].sit != 'o' &&
                    parque[piso][fila][lugar].sit != 'r' &&
                    parque[piso][fila][lugar].sit != 'm') {

                    strcpy(parque[piso][fila][lugar].matricAut, "");
                    parque[piso][fila][lugar].sit = 'L';
                    parque[piso][fila][lugar].numE = 0;
                }
            }
        }
    }

    int veiculosNoParque = 0;
    int lugaresForaDimensao = 0;
    int aindaNaoEntraram = 0;
    int jaSairam = 0;
    int piso, fila, lugar;

    // DEBUG: Mostrar info da consulta
    printf("\n[DEBUG] Consultando data: %02d/%02d/%d as %02d:%02d\n",
           dataConsulta.dia, dataConsulta.mes, dataConsulta.ano,
           horaConsulta.hora, horaConsulta.min);
    printf("[DEBUG] Total de estacionamentos em memoria: %d\n", p->nElemActE);
    printf("[DEBUG] Dimensoes do parque: %d pisos x %d filas x %d lugares\n\n",
           p->numPisos, p->numFilas, p->nLugPFila);

    // Percorrer todos os estacionamentos
    for (int i = 0; i < p->nElemActE; i++) {

        // Verificar se o veiculo JA TINHA ENTRADO na data/hora de consulta
        // (dataConsulta >= dataEntrada)
        int jaEntrou = CompararDataHora(dataConsulta, horaConsulta,
                                         vEstac[i].entradaDt, vEstac[i].entradaHm) >= 0;

        // Verificar se o veiculo AINDA NAO TINHA SAIDO na data/hora de consulta
        // (dataConsulta < dataSaida) OU (dataSaida == 0, ainda nao saiu)
        int aindaNaoSaiu = 0;

        if (vEstac[i].saidaDt.ano == 0) {
            // Veiculo ainda nao saiu (data saida a zeros)
            aindaNaoSaiu = 1;
        } else {
            // Veiculo ja saiu - verificar se a consulta e anterior a saida
            aindaNaoSaiu = CompararDataHora(dataConsulta, horaConsulta,
                                            vEstac[i].saidaDt, vEstac[i].saidaHm) < 0;
        }

        // Contadores de debug
        if (!jaEntrou) aindaNaoEntraram++;
        if (jaEntrou && !aindaNaoSaiu) jaSairam++;

        // Se ja tinha entrado E ainda nao tinha saido -> estava no parque
        if (jaEntrou && aindaNaoSaiu) {

            if (ConverterCodigoParaIndices(vEstac[i].lugar, &piso, &fila, &lugar)) {

                if (piso < p->numPisos && fila < p->numFilas && lugar < p->nLugPFila) {

                    strcpy(parque[piso][fila][lugar].matricAut, vEstac[i].matricVeiculo);
                    parque[piso][fila][lugar].sit = 'O';
                    parque[piso][fila][lugar].numE = vEstac[i].numE;

                    veiculosNoParque++;
                } else {
                    // Lugar fora das dimensoes configuradas
                    lugaresForaDimensao++;
                    printf("[AVISO] Lugar %s fora das dimensoes! (Piso %d, Fila %c, Lugar %d)\n",
                           vEstac[i].lugar, piso+1, 'A'+fila, lugar+1);
                }
            } else {
                printf("[AVISO] Codigo de lugar invalido: %s (Matricula: %s)\n",
                       vEstac[i].lugar, vEstac[i].matricVeiculo);
            }
        }
    }

    // Mostrar estatisticas
    printf("\n[DEBUG] === ESTATISTICAS ===\n");
    printf("[DEBUG] Ainda nao tinham entrado: %d\n", aindaNaoEntraram);
    printf("[DEBUG] Ja tinham saido: %d\n", jaSairam);
    printf("[DEBUG] Estavam no parque: %d\n", veiculosNoParque);
    if (lugaresForaDimensao > 0) {
        printf("[AVISO] Lugares fora das dimensoes: %d\n", lugaresForaDimensao);
    }
    printf("\n");

    return veiculosNoParque;
}

/**
 * Conta lugares livres num piso
 */
int ContarLugaresLivresPiso(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                           int piso,
                           void *paramSist) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    int livres = 0;

    for (int fila = 0; fila < p->numFilas; fila++) {
        for (int lugar = 0; lugar < p->nLugPFila; lugar++) {
            if (parque[piso][fila][lugar].sit == 'L') {
                livres++;
            }
        }
    }

    return livres;
}

/**
 * Conta lugares livres em todo o parque
 */
int ContarLugaresLivresTotais(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                               void *paramSist) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    int total = 0;

    for (int piso = 0; piso < p->numPisos; piso++) {
        total += ContarLugaresLivresPiso(parque, piso, p);
    }

    return total;
}

/**
 * Mostra lugares disponiveis por piso (para o menu)
 */
void MostrarLugaresDisponiveis(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                                void *paramSist) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    printf("-------------------------------------------------------\n");
    printf("  LUGARES DISPONIVEIS:  ");

    for (int piso = 0; piso < p->numPisos; piso++) {
        int livres = ContarLugaresLivresPiso(parque, piso, p);
        printf("Piso %d: %3d  ", piso + 1, livres);
    }

    printf("\n");
    printf("  TOTAL DISPONIVEL: %d lugares\n", ContarLugaresLivresTotais(parque, p));
    printf("-------------------------------------------------------\n");
}

/**
 * Encontra primeiro lugar livre num piso (politica A01)
 */
int EncontrarLugarLivrePiso(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                             int piso,
                             void *paramSist,
                             char *codigoLugar) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    for (int fila = 0; fila < p->numFilas; fila++) {
        for (int lugar = 0; lugar < p->nLugPFila; lugar++) {
            if (parque[piso][fila][lugar].sit == 'L') {
                ConverterIndicesParaCodigo(piso, fila, lugar, codigoLugar);
                return 1;
            }
        }
    }

    return 0;
}

/**
 * Encontra lugar livre em qualquer piso
 */
int EncontrarLugarLivreParque(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                               void *paramSist,
                               char *codigoLugar) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    for (int piso = 0; piso < p->numPisos; piso++) {
        if (EncontrarLugarLivrePiso(parque, piso, p, codigoLugar)) {
            return 1;
        }
    }

    return 0;
}

/**
 * Ocupa um lugar
 */
int OcuparLugar(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                char *codigoLugar,
                char *matricula,
                int numEstac,
                void *paramSist) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    int piso, fila, lugar;

    if (!ConverterCodigoParaIndices(codigoLugar, &piso, &fila, &lugar)) {
        printf("[ERRO] Codigo de lugar invalido: %s\n", codigoLugar);
        return 0;
    }

    if (piso >= p->numPisos || fila >= p->numFilas || lugar >= p->nLugPFila) {
        printf("[ERRO] Lugar fora das dimensoes do parque: %s\n", codigoLugar);
        return 0;
    }

    if (parque[piso][fila][lugar].sit != 'L') {
        printf("[ERRO] Lugar %s nao esta livre (situacao: %c)\n",
               codigoLugar, parque[piso][fila][lugar].sit);
        return 0;
    }

    strcpy(parque[piso][fila][lugar].matricAut, matricula);
    parque[piso][fila][lugar].sit = 'O';
    parque[piso][fila][lugar].numE = numEstac;

    return 1;
}

/**
 * Liberta um lugar
 */
int LibertarLugar(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                  char *codigoLugar,
                  void *paramSist) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    int piso, fila, lugar;

    if (!ConverterCodigoParaIndices(codigoLugar, &piso, &fila, &lugar)) {
        printf("[ERRO] Codigo de lugar invalido: %s\n", codigoLugar);
        return 0;
    }

    if (piso >= p->numPisos || fila >= p->numFilas || lugar >= p->nLugPFila) {
        printf("[ERRO] Lugar fora das dimensoes do parque: %s\n", codigoLugar);
        return 0;
    }

    strcpy(parque[piso][fila][lugar].matricAut, "");
    parque[piso][fila][lugar].sit = 'L';
    parque[piso][fila][lugar].numE = 0;

    return 1;
}

/**
 * Mostra mapa de ocupacao de um piso
 */
void MostrarMapaOcupacaoPiso(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                              int piso,
                              void *paramSist) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    printf("\n============== MAPA DE OCUPACAO - PISO %d ==============\n\n", piso + 1);

    // Cabecalho com numeros dos lugares
    printf("     ");
    for (int lugar = 0; lugar < p->nLugPFila; lugar++) {
        printf("%02d ", lugar + 1);
    }
    printf("\n");

    // Linha separadora
    printf("     ");
    for (int lugar = 0; lugar < p->nLugPFila; lugar++) {
        printf("---");
    }
    printf("\n");

    // Mostrar cada fila
    for (int fila = 0; fila < p->numFilas; fila++) {
        printf("  %c |", 'A' + fila);

        for (int lugar = 0; lugar < p->nLugPFila; lugar++) {
            char sit = parque[piso][fila][lugar].sit;

            char simbolo;
            switch (sit) {
                case 'L': simbolo = '-'; break;
                case 'O': simbolo = 'X'; break;
                case 'i': simbolo = 'i'; break;
                case 'o': simbolo = 'o'; break;
                case 'r': simbolo = 'r'; break;
                case 'm': simbolo = 'm'; break;
                default:  simbolo = '?'; break;
            }

            printf(" %c ", simbolo);
        }
        printf("\n");
    }

    // Legenda
    printf("\n---------------------------------------------------------\n");
    printf("LEGENDA: [-] Livre  [X] Ocupado  [i] Inadequado\n");
    printf("         [o] Obras  [r] Reservado  [m] Outros motivos\n");
    printf("---------------------------------------------------------\n");

    // Estatisticas
    int livres = ContarLugaresLivresPiso(parque, piso, p);
    int total = p->numFilas * p->nLugPFila;
    int ocupados = total - livres;
    printf("Ocupados: %d | Livres: %d | Total: %d\n", ocupados, livres, total);
}

/**
 * Marca lugar como indisponivel
 */
int MarcarLugarIndisponivel(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                             char *codigoLugar,
                             char motivo,
                             void *paramSist) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    int piso, fila, lugar;

    if (motivo != 'i' && motivo != 'o' && motivo != 'r' && motivo != 'm') {
        printf("[ERRO] Motivo invalido. Use: i, o, r ou m\n");
        return 0;
    }

    if (!ConverterCodigoParaIndices(codigoLugar, &piso, &fila, &lugar)) {
        printf("[ERRO] Codigo de lugar invalido: %s\n", codigoLugar);
        return 0;
    }

    if (piso >= p->numPisos || fila >= p->numFilas || lugar >= p->nLugPFila) {
        printf("[ERRO] Lugar fora das dimensoes do parque: %s\n", codigoLugar);
        return 0;
    }

    if (parque[piso][fila][lugar].sit == 'O') {
        printf("[ERRO] Lugar %s esta ocupado. Nao pode ser marcado como indisponivel.\n", codigoLugar);
        return 0;
    }

    parque[piso][fila][lugar].sit = motivo;
    strcpy(parque[piso][fila][lugar].matricAut, "");
    parque[piso][fila][lugar].numE = 0;

    return 1;
}

/**
 * Reverte lugar indisponivel para livre
 */
int ReverterLugarIndisponivel(LUGAR_E parque[][MAX_FILAS][MAX_LUG_P_FILA],
                               char *codigoLugar,
                               void *paramSist) {

    P_SYST_M_ESTAT_C *p = (P_SYST_M_ESTAT_C *)paramSist;

    int piso, fila, lugar;

    if (!ConverterCodigoParaIndices(codigoLugar, &piso, &fila, &lugar)) {
        printf("[ERRO] Codigo de lugar invalido: %s\n", codigoLugar);
        return 0;
    }

    if (piso >= p->numPisos || fila >= p->numFilas || lugar >= p->nLugPFila) {
        printf("[ERRO] Lugar fora das dimensoes do parque: %s\n", codigoLugar);
        return 0;
    }

    char sit = parque[piso][fila][lugar].sit;

    if (sit != 'i' && sit != 'o' && sit != 'r' && sit != 'm') {
        printf("[ERRO] Lugar %s nao esta marcado como indisponivel (situacao: %c)\n",
               codigoLugar, sit);
        return 0;
    }

    parque[piso][fila][lugar].sit = 'L';

    return 1;
}
