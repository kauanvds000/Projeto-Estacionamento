//
//  main.c
//  Projecto_AP
//
//  Aplicacao de Gestao de Parque de Estacionamento
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#include "Utils.h"
#include "Func_Estacionamentos.h"
#include "Func_Tarifas.h"
#include "InstalacaoN.h"
#include "Func_ParqueEstac.h"
#include "Func_Binario.h"

// VARIAVEIS GLOBAIS - Estruturas de dados principais
TARIFA vTarifas[MAX_REG_TARIFAS];
ESTAC vEstac[MAX_REG_ESTAC];
LUGAR_E parqueEstac[MAX_PISOS][MAX_FILAS][MAX_LUG_P_FILA];
P_SYST_M_ESTAT_C paramSist;

// ==================== FUNCOES AUXILIARES ====================

void ParaMaiusculas(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void LimparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void LerString(char *destino, int tamanhoMax, const char *mensagem) {
    printf("%s", mensagem);
    if (fgets(destino, tamanhoMax, stdin) != NULL) {
        size_t len = strlen(destino);
        if (len > 0 && destino[len-1] == '\n') {
            destino[len-1] = '\0';
        }
    }
}

int LerInteiro(const char *mensagem, int min, int max) {
    int valor;
    char buffer[50];

    while (1) {
        printf("%s", mensagem);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            if (sscanf(buffer, "%d", &valor) == 1) {
                if (valor >= min && valor <= max) {
                    return valor;
                }
                printf("[ERRO] Valor deve estar entre %d e %d!\n", min, max);
            } else {
                printf("[ERRO] Por favor, insira um numero valido!\n");
            }
        }
    }
}

// ==================== FUNCAO DE INICIALIZACAO ====================

int InicializarSistema() {

    printf("\n");
    printf("=======================================================\n");
    printf("       SISTEMA DE GESTAO DE PARQUE DE ESTACIONAMENTO   \n");
    printf("=======================================================\n\n");

    // ============================================================
    // CASO 1: Existem dados binarios persistidos -> Carregar deles
    // ============================================================
    if (ExistemDadosPersistidos()) {
        printf("[SISTEMA] Dados persistidos encontrados. A carregar...\n\n");

        // Carregar parametros
        if (!CarregarParametrosBinario(&paramSist)) {
            printf("[ERRO] Falha ao carregar parametros!\n");
            return 0;
        }

        // Carregar estacionamentos
        int nEstac = CarregarEstacionamentosBinario(vEstac, MAX_REG_ESTAC);
        if (nEstac >= 0) {
            paramSist.nElemActE = nEstac;
        } else {
            printf("[AVISO] A usar dados de estacionamentos do ficheiro texto...\n");
            lerFicheiroEstacionamento(vEstac, &paramSist);
        }

        // Carregar tarifas
        int nTarifas = CarregarTarifasBinario(vTarifas, MAX_REG_TARIFAS);
        if (nTarifas >= 0) {
            paramSist.nElemActT = nTarifas;
        } else {
            printf("[AVISO] A usar tarifas do ficheiro texto...\n");
            lerFicheiroTarif(vTarifas, &paramSist);
        }

        // Inicializar e sincronizar parque
        InicializarParque(parqueEstac, &paramSist);
        SincronizarEstacionamentos(parqueEstac, vEstac, &paramSist);

        printf("\n[SISTEMA] Dados carregados com sucesso!\n");
        return 1;
    }

    // ============================================================
    // CASO 2: Verificar se existe ficheiro de configuracao (instalacao anterior)
    // ============================================================
    FILE *F1 = fopen("infPersonSyst.txt", "r");

    if (F1 != NULL) {
        printf("[SISTEMA] Configuracao encontrada. A recuperar...\n\n");

        if (lerFicheiroParametros(F1, &paramSist) == 0) {
            fclose(F1);
            printf("[ERRO] Falha na leitura de parametros!\n");
            remove("infPersonSyst.txt");
            return 0;
        }
        fclose(F1);

        // Carregar dados dos ficheiros texto
        lerFicheiroTarif(vTarifas, &paramSist);
        lerFicheiroEstacionamento(vEstac, &paramSist);

        // Inicializar e sincronizar parque
        InicializarParque(parqueEstac, &paramSist);
        SincronizarEstacionamentos(parqueEstac, vEstac, &paramSist);

        printf("\n[SISTEMA] Dados carregados com sucesso!\n");
        return 1;
    }

    // ============================================================
    // CASO 3: Primeira execucao - Instalacao do sistema
    // ============================================================
    printf("[SISTEMA] Primeira execucao detetada. A iniciar instalacao...\n\n");

    if (personalizarAplicacaoF(vTarifas, vEstac, &paramSist) == 0) {
        printf("[ERRO] Falha na personalizacao!\n");
        return 0;
    }

    printf("\nA ler ficheiros de dados...\n");

    if (lerFicheiroTarif(vTarifas, &paramSist) == 0) {
        printf("[ERRO] Falha na leitura de tarifas!\n");
        return 0;
    }

    if (lerFicheiroEstacionamento(vEstac, &paramSist) == 0) {
        printf("[ERRO] Falha na leitura de estacionamentos!\n");
        return 0;
    }

    // Inicializar e sincronizar parque
    InicializarParque(parqueEstac, &paramSist);
    SincronizarEstacionamentos(parqueEstac, vEstac, &paramSist);

    printf("\n[SISTEMA] Instalacao concluida com sucesso!\n");
    printf("Pressione <Enter> para continuar...\n");
    getchar(); getchar();

    return 1;
}

// ==================== FUNCAO PRINCIPAL ====================

int main(void)
{
    setlocale(LC_ALL, "Portuguese");
    setlocale(LC_NUMERIC, "C");

    // Inicializar sistema
    if (!InicializarSistema()) {
        printf("\n[ERRO FATAL] Nao foi possivel inicializar o sistema.\n");
        printf("Pressione <Enter> para sair...\n");
        getchar();
        return EXIT_FAILURE;
    }

    // ==================== MENU PRINCIPAL ====================

    int opcao;
    char buffer[50];

    do {
        printf("\n");
        MostrarLugaresDisponiveis(parqueEstac, &paramSist);

        printf("=======================================================\n");
        printf("         GESTAO DE PARQUE DE ESTACIONAMENTO            \n");
        printf("=======================================================\n");
        printf("    1. Registar Entrada                                \n");
        printf("    2. Registar Saida                                  \n");
        printf("    3. Mapa de Ocupacao                                \n");
        printf("    4. Gestao de Tarifas                               \n");
        printf("    5. Consultar Estacionamentos                       \n");
        printf("    6. Parametros do Sistema                           \n");
        printf("    7. Gerir Lugares Indisponiveis                     \n");
        printf("    8. Reiniciar Sistema                               \n");
        printf("    0. Sair (Gravar e Sair)                            \n");
        printf("=======================================================\n");
        printf("Opcao: ");

        fgets(buffer, 50, stdin);
        if (sscanf(buffer, "%d", &opcao) != 1) {
            opcao = -1;
        }

        switch(opcao)
        {
            // ============================================================
            // OPCAO 1: REGISTAR ENTRADA
            // ============================================================
            case 1:
            {
                printf("\n");
                printf("=======================================================\n");
                printf("               REGISTO DE ENTRADA                      \n");
                printf("=======================================================\n");

                if (paramSist.nElemActE >= paramSist.nMaxRegDefE) {
                    printf("[ERRO] Limite de registos atingido!\n");
                    break;
                }

                int lugaresLivres = ContarLugaresLivresTotais(parqueEstac, &paramSist);
                if (lugaresLivres == 0) {
                    printf("[ERRO] Parque cheio! Nao ha lugares disponiveis.\n");
                    break;
                }

                printf("Lugares disponiveis: %d\n\n", lugaresLivres);

                // --- PEDIR MATRICULA ---
                char matTemp[15];
                int matriculaValida = 0;

                while (!matriculaValida) {
                    LerString(matTemp, sizeof(matTemp), "Matricula do veiculo: ");
                    ParaMaiusculas(matTemp);

                    if (!ValidarMatricula(matTemp)) {
                        printf("[ERRO] Formato de matricula invalido!\n");
                        continue;
                    }

                    int idx = ProcurarEstacionamentoAtivoPorMatricula(vEstac, paramSist.nElemActE, matTemp);
                    if (idx >= 0) {
                        printf("[ERRO] Veiculo %s ja esta no parque (Lugar: %s)!\n",
                               matTemp, vEstac[idx].lugar);
                        continue;
                    }

                    matriculaValida = 1;
                }

                // --- PEDIR DATA ---
                DATA dtEntTemp;
                int dataValida = 0;

                while (!dataValida) {
                    printf("Data de Entrada (DD MM AAAA): ");
                    if (scanf("%d %d %d", &dtEntTemp.dia, &dtEntTemp.mes, &dtEntTemp.ano) != 3) {
                        printf("[ERRO] Formato invalido! Use: DD MM AAAA\n");
                        LimparBuffer();
                        continue;
                    }
                    LimparBuffer();

                    if (!validar_data(dtEntTemp.dia, dtEntTemp.mes, dtEntTemp.ano)) {
                        printf("[ERRO] Data invalida!\n");
                        continue;
                    }
                    dataValida = 1;
                }

                // --- PEDIR HORA ---
                HORA_MIN hmEntTemp;
                int horaValida = 0;

                while (!horaValida) {
                    printf("Hora de Entrada (HH MM): ");
                    if (scanf("%d %d", &hmEntTemp.hora, &hmEntTemp.min) != 2) {
                        printf("[ERRO] Formato invalido! Use: HH MM\n");
                        LimparBuffer();
                        continue;
                    }
                    LimparBuffer();

                    if (!validar_horario(hmEntTemp.hora, hmEntTemp.min)) {
                        printf("[ERRO] Hora invalida!\n");
                        continue;
                    }
                    horaValida = 1;
                }

                // --- ESCOLHER PISO ---
                printf("\nPisos disponiveis:\n");
                for (int p = 0; p < paramSist.numPisos; p++) {
                    int livres = ContarLugaresLivresPiso(parqueEstac, p, &paramSist);
                    printf("  Piso %d: %d lugares livres\n", p + 1, livres);
                }

                int pisoEscolhido = LerInteiro("\nEscolha o piso (0 para automatico): ",
                                               0, paramSist.numPisos);

                // --- ATRIBUIR LUGAR ---
                char lugarAtribuido[5] = "";

                if (pisoEscolhido >= 1 && pisoEscolhido <= paramSist.numPisos) {
                    if (!EncontrarLugarLivrePiso(parqueEstac, pisoEscolhido - 1, &paramSist, lugarAtribuido)) {
                        printf("[AVISO] Piso %d cheio! A procurar noutro...\n", pisoEscolhido);
                        if (!EncontrarLugarLivreParque(parqueEstac, &paramSist, lugarAtribuido)) {
                            printf("[ERRO] Parque cheio!\n");
                            break;
                        }
                    }
                } else {
                    if (!EncontrarLugarLivreParque(parqueEstac, &paramSist, lugarAtribuido)) {
                        printf("[ERRO] Parque cheio!\n");
                        break;
                    }
                }

                // --- REGISTAR ---
                unsigned int novoNumE = paramSist.ultIdEstacE + 1;
                DATA dtSaidaZero = {0, 0, 0};
                HORA_MIN hmSaidaZero = {0, 0};

                int idx = paramSist.nElemActE;
                vEstac[idx].numE = novoNumE;
                strcpy(vEstac[idx].matricVeiculo, matTemp);
                vEstac[idx].entradaDt = dtEntTemp;
                vEstac[idx].entradaHm = hmEntTemp;
                strcpy(vEstac[idx].lugar, lugarAtribuido);
                vEstac[idx].saidaDt = dtSaidaZero;
                vEstac[idx].saidaHm = hmSaidaZero;
                vEstac[idx].valPag = 0.0;
                strcpy(vEstac[idx].obs, "");

                OcuparLugar(parqueEstac, lugarAtribuido, matTemp, novoNumE, &paramSist);

                paramSist.nElemActE++;
                paramSist.ultIdEstacE = novoNumE;

                MostrarTicketEntrada(novoNumE, matTemp, lugarAtribuido, dtEntTemp, hmEntTemp);
                printf("\n[SUCESSO] Entrada registada! Dirija-se ao lugar %s\n", lugarAtribuido);
            }
            break;

            // ============================================================
            // OPCAO 2: REGISTAR SAIDA
            // ============================================================
            case 2:
            {
                printf("\n");
                printf("=======================================================\n");
                printf("                REGISTO DE SAIDA                       \n");
                printf("=======================================================\n");

                printf("Como deseja identificar o veiculo?\n");
                printf("  1. Por matricula\n");
                printf("  2. Por numero de entrada\n");
                int opcaoId = LerInteiro("Opcao: ", 1, 2);

                int idxEstac = -1;

                if (opcaoId == 1) {
                    char matPesquisa[15];
                    LerString(matPesquisa, sizeof(matPesquisa), "Matricula: ");
                    ParaMaiusculas(matPesquisa);

                    idxEstac = ProcurarEstacionamentoAtivoPorMatricula(vEstac, paramSist.nElemActE, matPesquisa);

                    if (idxEstac < 0) {
                        printf("[ERRO] Veiculo %s nao encontrado no parque!\n", matPesquisa);
                        break;
                    }
                } else {
                    int numPesquisa = LerInteiro("Numero de entrada: ", 1, paramSist.ultIdEstacE);

                    idxEstac = ProcurarEstacionamentoPorNumero(vEstac, paramSist.nElemActE, numPesquisa);

                    if (idxEstac < 0) {
                        printf("[ERRO] Estacionamento #%d nao encontrado!\n", numPesquisa);
                        break;
                    }

                    if (vEstac[idxEstac].saidaDt.ano != 0) {
                        printf("[ERRO] Este veiculo ja saiu em %02d/%02d/%d!\n",
                               vEstac[idxEstac].saidaDt.dia,
                               vEstac[idxEstac].saidaDt.mes,
                               vEstac[idxEstac].saidaDt.ano);
                        break;
                    }
                }

                printf("\n--- Veiculo Encontrado ---\n");
                printf("Matricula: %s\n", vEstac[idxEstac].matricVeiculo);
                printf("Lugar: %s\n", vEstac[idxEstac].lugar);
                printf("Entrada: %02d/%02d/%d as %02d:%02d\n",
                       vEstac[idxEstac].entradaDt.dia,
                       vEstac[idxEstac].entradaDt.mes,
                       vEstac[idxEstac].entradaDt.ano,
                       vEstac[idxEstac].entradaHm.hora,
                       vEstac[idxEstac].entradaHm.min);

                printf("\nConfirma a saida? (S/N): ");
                char confirma[10];
                fgets(confirma, sizeof(confirma), stdin);

                if (confirma[0] != 'S' && confirma[0] != 's') {
                    printf("Operacao cancelada.\n");
                    break;
                }

                // --- PEDIR DATA DE SAIDA ---
                DATA dtSaiTemp;
                HORA_MIN hmSaiTemp;
                int dadosValidos = 0;

                while (!dadosValidos) {
                    printf("Data de Saida (DD MM AAAA): ");
                    if (scanf("%d %d %d", &dtSaiTemp.dia, &dtSaiTemp.mes, &dtSaiTemp.ano) != 3) {
                        LimparBuffer();
                        printf("[ERRO] Formato invalido!\n");
                        continue;
                    }
                    LimparBuffer();

                    if (!validar_data(dtSaiTemp.dia, dtSaiTemp.mes, dtSaiTemp.ano)) {
                        printf("[ERRO] Data invalida!\n");
                        continue;
                    }

                    printf("Hora de Saida (HH MM): ");
                    if (scanf("%d %d", &hmSaiTemp.hora, &hmSaiTemp.min) != 2) {
                        LimparBuffer();
                        printf("[ERRO] Formato invalido!\n");
                        continue;
                    }
                    LimparBuffer();

                    if (!validar_horario(hmSaiTemp.hora, hmSaiTemp.min)) {
                        printf("[ERRO] Hora invalida!\n");
                        continue;
                    }

                    if (!ValidarSaidaPosteriorEntrada(vEstac[idxEstac].entradaDt,
                                                       vEstac[idxEstac].entradaHm,
                                                       dtSaiTemp, hmSaiTemp)) {
                        printf("[ERRO] Saida deve ser posterior a entrada (%02d/%02d/%d %02d:%02d)!\n",
                               vEstac[idxEstac].entradaDt.dia,
                               vEstac[idxEstac].entradaDt.mes,
                               vEstac[idxEstac].entradaDt.ano,
                               vEstac[idxEstac].entradaHm.hora,
                               vEstac[idxEstac].entradaHm.min);
                        continue;
                    }

                    dadosValidos = 1;
                }

                // --- CALCULAR E REGISTAR ---
                float valorPagar = CalcularValorEstacionamento(
                    vEstac[idxEstac].entradaDt, vEstac[idxEstac].entradaHm,
                    dtSaiTemp, hmSaiTemp
                );

                vEstac[idxEstac].saidaDt = dtSaiTemp;
                vEstac[idxEstac].saidaHm = hmSaiTemp;
                vEstac[idxEstac].valPag = valorPagar;

                LibertarLugar(parqueEstac, vEstac[idxEstac].lugar, &paramSist);

                MostrarTicketSaida(&vEstac[idxEstac], valorPagar);
                printf("\n[SUCESSO] Saida registada! Lugar %s libertado.\n", vEstac[idxEstac].lugar);
            }
            break;

            // ============================================================
            // OPCAO 3: MAPA DE OCUPACAO
            // ============================================================
            case 3:
            {
                printf("\n");
                printf("=======================================================\n");
                printf("                 MAPA DE OCUPACAO                      \n");
                printf("=======================================================\n");
                printf("    1. Ver estado ATUAL                                \n");
                printf("    2. Ver estado numa DATA/HORA especifica            \n");
                printf("    0. Voltar                                          \n");
                printf("=======================================================\n");

                int opcaoMapa = LerInteiro("Opcao: ", 0, 2);

                if (opcaoMapa == 1) {
                    InicializarParque(parqueEstac, &paramSist);
                    SincronizarEstacionamentos(parqueEstac, vEstac, &paramSist);

                    int pisoMapa = LerInteiro("\nPiso a visualizar (0 para todos): ", 0, paramSist.numPisos);

                    if (pisoMapa == 0) {
                        for (int p = 0; p < paramSist.numPisos; p++) {
                            MostrarMapaOcupacaoPiso(parqueEstac, p, &paramSist);
                        }
                    } else {
                        MostrarMapaOcupacaoPiso(parqueEstac, pisoMapa - 1, &paramSist);
                    }

                } else if (opcaoMapa == 2) {
                    DATA dataConsulta;
                    HORA_MIN horaConsulta;

                    printf("\n--- Consulta Historica ---\n");
                    printf("Data (DD MM AAAA): ");
                    scanf("%d %d %d", &dataConsulta.dia, &dataConsulta.mes, &dataConsulta.ano);
                    printf("Hora (HH MM): ");
                    scanf("%d %d", &horaConsulta.hora, &horaConsulta.min);
                    LimparBuffer();

                    int veiculos = SincronizarPorDataHora(parqueEstac, vEstac, &paramSist,
                                                          dataConsulta, horaConsulta);

                    printf("\nVeiculos no parque: %d\n", veiculos);

                    int pisoMapa = LerInteiro("Piso (0 para todos): ", 0, paramSist.numPisos);

                    if (pisoMapa == 0) {
                        for (int p = 0; p < paramSist.numPisos; p++) {
                            MostrarMapaOcupacaoPiso(parqueEstac, p, &paramSist);
                        }
                    } else {
                        MostrarMapaOcupacaoPiso(parqueEstac, pisoMapa - 1, &paramSist);
                    }

                    InicializarParque(parqueEstac, &paramSist);
                    SincronizarEstacionamentos(parqueEstac, vEstac, &paramSist);
                }

                printf("\nPressione <Enter> para continuar...\n");
                getchar();
            }
            break;

            // ============================================================
            // OPCAO 4: GESTAO DE TARIFAS
            // ============================================================
            case 4:
            {
                printf("\n=== TARIFAS ===\n");
                for (int i = 0; i < paramSist.nElemActT; i++)
                    MostrarTarifa(vTarifas, i);

                printf("\n1. Modificar | 2. Criar Nova | 0. Voltar\n");
                int opcaoT = LerInteiro("Opcao: ", 0, 2);

                if (opcaoT == 1) ModificarTarifa(vTarifas, paramSist.nElemActT);
                else if (opcaoT == 2) CriarNovaTarifa(vTarifas, &paramSist);
            }
            break;

            // ============================================================
            // OPCAO 5: CONSULTAR ESTACIONAMENTOS
            // ============================================================
            case 5:
            {
                printf("\n=== CONSULTAR ESTACIONAMENTOS ===\n");
                printf("1. Listar todos | 2. Por matricula | 3. Por numero | 4. No parque | 0. Voltar\n");
                int opcaoC = LerInteiro("Opcao: ", 0, 4);

                if (opcaoC == 1) {
                    for (int j = 0; j < paramSist.nElemActE; j++)
                        MostrarEstacionamento(vEstac, j);
                } else if (opcaoC == 2) {
                    char matPesq[15];
                    LerString(matPesq, sizeof(matPesq), "Matricula: ");
                    ParaMaiusculas(matPesq);
                    for (int j = 0; j < paramSist.nElemActE; j++)
                        if (strstr(vEstac[j].matricVeiculo, matPesq))
                            MostrarEstacionamento(vEstac, j);
                } else if (opcaoC == 3) {
                    int n = LerInteiro("Numero: ", 1, paramSist.ultIdEstacE);
                    int idx = ProcurarEstacionamentoPorNumero(vEstac, paramSist.nElemActE, n);
                    if (idx >= 0) MostrarEstacionamento(vEstac, idx);
                    else printf("Nao encontrado!\n");
                } else if (opcaoC == 4) {
                    printf("\n--- Veiculos no Parque ---\n");
                    for (int j = 0; j < paramSist.nElemActE; j++)
                        if (vEstac[j].saidaDt.ano == 0)
                            MostrarEstacionamento(vEstac, j);
                }
                printf("\nPressione <Enter>...\n"); getchar();
            }
            break;

            // ============================================================
            // OPCAO 6: PARAMETROS
            // ============================================================
            case 6:
                MostrarParametros(paramSist);
                printf("\nPressione <Enter>...\n"); getchar();
                break;

            // ============================================================
            // OPCAO 7: LUGARES INDISPONIVEIS
            // ============================================================
            case 7:
            {
                printf("\n1. Marcar indisponivel | 2. Reverter | 0. Voltar\n");
                int opcaoI = LerInteiro("Opcao: ", 0, 2);

                if (opcaoI == 1) {
                    char cod[10]; LerString(cod, sizeof(cod), "Codigo (ex: 1A01): "); ParaMaiusculas(cod);
                    printf("Motivo: i=inadequado, o=obras, r=reservado, m=outros\n");
                    char mot[5]; LerString(mot, sizeof(mot), "Motivo: ");
                    if (MarcarLugarIndisponivel(parqueEstac, cod, mot[0], &paramSist))
                        printf("[OK] Lugar %s indisponivel.\n", cod);
                } else if (opcaoI == 2) {
                    char cod[10]; LerString(cod, sizeof(cod), "Codigo: "); ParaMaiusculas(cod);
                    if (ReverterLugarIndisponivel(parqueEstac, cod, &paramSist))
                        printf("[OK] Lugar %s disponivel.\n", cod);
                }
                printf("\nPressione <Enter>...\n"); getchar();
            }
            break;

            // ============================================================
            // OPCAO 8: REINICIAR
            // ============================================================
            case 8:
            {
                printf("\n[ATENCAO] Isto apaga TODOS os dados!\n");
                printf("Confirma? (S/N): ");
                char c[5]; fgets(c, sizeof(c), stdin);
                if (c[0] == 'S' || c[0] == 's') {
                    remove("infPersonSyst.txt");
                    remove(FICHEIRO_BIN_ESTAC);
                    remove(FICHEIRO_BIN_TARIFAS);
                    remove(FICHEIRO_BIN_PARAMS);
                    printf("[OK] Sistema reiniciado. Reinicie a aplicacao.\n");
                    return 0;
                }
            }
            break;

            // ============================================================
            // OPCAO 0: SAIR - GRAVAR DADOS
            // ============================================================
            case 0:
            {
                // ***** GRAVAR TODOS OS DADOS EM FICHEIROS BINARIOS *****
                GravarTodosDados(vEstac, vTarifas, &paramSist);
                printf("\n[SISTEMA] Aplicacao encerrada. Ate breve!\n");
            }
            break;

            default:
                printf("\n[ERRO] Opcao invalida!\n");
                break;
        }

    } while (opcao != 0);

    return EXIT_SUCCESS;
}
