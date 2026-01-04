//
//  InstalacaoN.c
//
//
//   Projecto_AP_
#include "InstalacaoN.h"
#include "Utils.h"
#include "Func_Tarifas.h"
#include "Func_ParqueEstac.h"
#include <locale.h>

//
//******************** PRIMEIRO ARRANQUE DA APLICAÇÃO**********************************
//

int personalizarAplicacaoF(TARIFA tarif[], ESTAC estac[], P_SYST_M_ESTAT_C *paramSystS){
    setlocale(LC_ALL, "Portuguese");
    DATA_SISTEMA dtSyst;  // variável necessária para ir buscar a data do sistema
    printf("\[SISTEMA] Primeira inicialização detetada!\n\n");
    printf("\n*** Personalização e Inicialização da Aplicação ***\n\n");
    (*paramSystS).nElemActT=0;   // Inicializar o valor do número de elementos actual do vector de tarifas
    // (o valor será 0, se for para inicializar o sistema) e
    // lido do fich. e actualizado, caso se esteja já em produção;
    (*paramSystS).nElemActE=0;   // Inicializar o valor do número de elementos actual do vector de estacionamentos
    // (o valor será 0, se for para inicializar o sistema) e
    // lido do fich. e actualizado, caso se esteja já em produção;
    (*paramSystS).ultIdEstacE=0;   // inicializar o valor do id do último registo de estacionamento a 0;
    // se for para inicializar o sistema, ficará realmente a 0
    // pois não haverá nenhum estacionamento;
    // se o sistema estiver em produção, o valor será
    // lido do ficheiro e guardada em memória.
    (*paramSystS).nRegImportadosT=0;  // número total de registos importados do fich. "Tarifas.txt"
    (*paramSystS).nRegImportadosE=0;  // número total de registos importados do fich. "Estacionamentos.txt"

    (*paramSystS).nMaxRegDefT=0;  // número total de registos definidos para os tarifários
    (*paramSystS).nMaxRegDefE=0;  // número total de registos definidos para os estacionamentos

    (*paramSystS).numPisos=0;  // número definido de pisos do parque
    (*paramSystS).numFilas=0;  // número definido de filas do parque
    (*paramSystS).nLugPFila=0;  // número definido de lugares por fila do parque

    do{
        printf("\nQual e o nome da empresa deste parque de estacionamento? ");
        fgets((*paramSystS).nomeEmpresa, 60, stdin);
        if(strlen((*paramSystS).nomeEmpresa)==0)
            printf("\nO tamanho do nome da empresa nao pode ser 0. P.f., insira-o novamente.");
    }while(strlen((*paramSystS).nomeEmpresa)==0);
    do{
        printf("\nQual e o seu nome (o instalador da aplicação?) ");
        fgets((*paramSystS).nomeInstalador, 50, stdin);
        if(strlen((*paramSystS).nomeInstalador)==0)
            printf("\nO tamanho do nome do instalador da aplicação não pode ser 0. P.f., insira-o novamente.");
    }while(strlen((*paramSystS).nomeInstalador)==0);
    //quantos pisos, lugares e filas, max registros para tarifarios, maximos de registros estacionamento,
    do{
        printf("\nQual e o número de pisos? (Máximo de 5 pisos!) ");
        scanf("%d", &(*paramSystS).numPisos);
        if(((*paramSystS).numPisos)> MAX_PISOS || ((*paramSystS).numPisos) <= 0)
            printf("\nErro ao registrar o número de pisos! Deve ser um número entre 1 e 5!\n");
    }while(((*paramSystS).numPisos)> MAX_PISOS || ((*paramSystS).numPisos) <= 0);
    do{
        printf("\nQual e o número de filas? (Máximo de 26 filas!) ");
        scanf("%d", &(*paramSystS).numFilas);
        if(((*paramSystS).numFilas)> MAX_FILAS || ((*paramSystS).numFilas) <= 0)
            printf("\nErro ao registrar o número de filas! Deve ser um número entre 1 e 26!\n");
    }while(((*paramSystS).numFilas)> MAX_FILAS || ((*paramSystS).numFilas) <= 0);
    do{
        printf("\nQual e o número de lugares por fila? (Máximo de 50 lugares!) ");
        scanf("%d", &(*paramSystS).nLugPFila);
        if(((*paramSystS).nLugPFila)> MAX_LUG_P_FILA || ((*paramSystS).nLugPFila) <= 0)
            printf("\nErro ao registrar o número de filas! Deve ser um número entre 1 e 50!\n");
    }while(((*paramSystS).nLugPFila)> MAX_LUG_P_FILA || ((*paramSystS).nLugPFila) <= 0);
    do{
        printf("\nQual e o numero máximo de registros para a Tárifas? (Deve ser entre 4 e 10!) ");
        scanf("%d", &(*paramSystS).nMaxRegDefT);
        if(((*paramSystS).nMaxRegDefT)> MAX_REG_TARIFAS || ((*paramSystS).nMaxRegDefT) < MIN_REG_TARIFAS )
            printf("\nErro ao registrar o numero de filas! Deve ser um número entre 4 e 10!\n");
    }while(((*paramSystS).nMaxRegDefT)> MAX_REG_TARIFAS || ((*paramSystS).nMaxRegDefT) < MIN_REG_TARIFAS);
    do{
        printf("\nQual e o numero máximo de registros para o estacionamento ? (Deve ser entre 3000 e 100000!) ");
        scanf("%d", &(*paramSystS).nMaxRegDefE);
        if(((*paramSystS).nMaxRegDefE)> MAX_REG_ESTAC || ((*paramSystS).nMaxRegDefE) < MIN_REG_ESTAC)
            printf("\nErro ao registrar o numero de filas! Deve ser um número entre 3000 e 100000!\n");
    }while(((*paramSystS).nMaxRegDefE)> MAX_REG_ESTAC || ((*paramSystS).nMaxRegDefE) < MIN_REG_ESTAC);
    // eliminar quando o código for adaptado}               // eliminar quando o código for adaptado

    printf("\nVai-se gravar o fich. de configuração...\n");
    // vai gravar a informação de configuração das requisições
    FILE *F1 = fopen("erros.txt", "w");
    if (!F1) {
        printf("\n\tImpossível abrir o Ficheiro erros.txt!!!\n\n");
        mostrarMensagem("\tTrata-se de um erro irrecuperável. A aplicação vai encerrar!");
        remove("infPersonSyst.txt");
        return 0;
    }
    FILE *F2 = fopen("infPersonSyst.txt", "w");
    if (!F2) { // o ficheiro de parâmetros não pôde ser criado. Vai mostrar-se uma mensagem e escrever o ficheiro de erros
        printf("\n\tImpossível criar o ficheiro com informação sobre a personalização do sistema para assegurar a sua persistência!!\n\n");
        mostrarMensagem("\tTrata-se de um erro irrecuperável. A aplicação vai encerrar!\n");
        fprintf(F2, "Impossível criar o ficheiro com informação sobre a personalização do sistema para assegurar a sua persistência!!\n");
        fprintf(F2, "Trata-se de um erro irrecuperável. A aplicação vai encerrar!\n");
        remove("infPersonSyst.txt");
        fclose(F1);
        return 0;
    }

    // Registar a informação base da personalização e parâmetros base
    obterDataSistema(&dtSyst);
    (*paramSystS).dtInstalacao.ano=dtSyst.anoS;
    (*paramSystS).dtInstalacao.mes=dtSyst.mesS;
    (*paramSystS).dtInstalacao.dia=dtSyst.diaS;
    (*paramSystS).dtUltProcess.ano=0;  // ainda não entrou em produção
    (*paramSystS).dtUltProcess.mes=0;
    (*paramSystS).dtUltProcess.dia=0;
    fprintf(F2, "%d/%d/%d\t%d/%d/%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\t\n",
            (*paramSystS).dtInstalacao.ano,
            (*paramSystS).dtInstalacao.mes,
            (*paramSystS).dtInstalacao.dia,
            (*paramSystS).dtUltProcess.ano,
            (*paramSystS).dtUltProcess.mes,
            (*paramSystS).dtUltProcess.dia,
            (*paramSystS).numPisos,
            (*paramSystS).numFilas,
            (*paramSystS).nLugPFila,
            (*paramSystS).nElemActT,
            (*paramSystS).ultIdEstacE,
            (*paramSystS).nMaxRegDefT,
            (*paramSystS).nMaxRegDefE,
            (*paramSystS).nRegImportadosT,
            (*paramSystS).nomeEmpresa,
            (*paramSystS).nomeInstalador);

    fclose(F1);
    fclose(F2);
    return 1;
}

//
//************************LER FICHEIRO TARIFA********************************************************************************************
//

int lerFicheiroTarif(TARIFA vTarifas[], P_SYST_M_ESTAT_C *paramSystS){

    FILE *F1 = fopen("erros.txt", "a");
    if (!F1) {
        printf("\n\n\tImpossível abrir o Ficheiro erros.txt\n\n");
        mostrarMensagem("Trata-se de um erro irrecuperável. A aplicação vai terminar!");
        remove("infPersonSyst.txt");
        return 0;
    }
    FILE *F2 = fopen("Tarifario.txt","r"); // os campos estão separados por \t
    if (!F2) {
        printf("\n\n\tImpossível abrir o Ficheiro Tarifario.txt\n\n");
        mostrarMensagem("Trata-se de um erro irrecuperável. A aplicação vai terminar!");
        fprintf(F1, "\n\n\tImpossível abrir o Ficheiro tarifas.txt. Trata-se de um erro irrecuperável. A aplicação vai terminar!");
        remove("infPersonSyst.txt");
        return 0;
    }
    int i = 0;

    // Variáveis Temporárias para leitura
    char tempTp;
    char tempCod[20];
    int th1, tm1, th2, tm2;
    int valInt, valDec;
    float tempVal;
    char lixo[10]; // Para apanhar o símbolo € ou caracteres extra

    // O formato de leitura inclui " %c" no fim para comer o simbolo €
    // Nota: O espaço antes do primeiro %c ignora espaços em branco/nova linha anteriores
    while (i < MAX_REG_TARIFAS &&
           fscanf(F2, " %c %s %d:%d %d:%d %d.%d%s",
                  &tempTp, tempCod, &th1, &tm1, &th2, &tm2, &valInt, &valDec, lixo) == 9) {

        int linhaValida = 1; // Assumimos que é válida até prova em contrário

        tempVal = valInt + (float)valDec/ 100.0;

        // --- VALIDAÇÃO 1: Tipo de Tarifa (H ou D) ---
        if (tempTp != 'H' && tempTp != 'D') {
            fprintf(F1, "Linha inválida (Tipo incorreto: %c): %s\n", tempTp, tempCod);
            linhaValida = 0;
        }

        // --- VALIDAÇÃO 2: Código começa por CT ---
        // Verifica se os 2 primeiros caracteres são "CT"
        if (strncmp(tempCod, "CT", 2) != 0) {
            fprintf(F1, "Linha inválida (Codigo deve ser do tipo CTX): %s\n", tempCod);
            linhaValida = 0;
        } else {
            // Opcional: Validar se o resto é número (ex: CT1, CT10)
            // Começamos a verificar a partir do índice 2
            int k = 2;
            while(tempCod[k] != '\0'){
                if(!isdigit(tempCod[k])){
                     fprintf(F1, "Linha inválida (Codigo não numérico após CT): %s\n", tempCod);
                     linhaValida = 0;
                     break;
                }
                k++;
            }
        }

        // --- VALIDAÇÃO 3: Horas e Minutos ---
        if (th1 < 0 || th1 > 23 || tm1 < 0 || tm1 > 59 ||
            th2 < 0 || th2 > 23 || tm2 < 0 || tm2 > 59) {
            fprintf(F1, "Linha inválida (Horario impossivel): %d:%d - %d:%d\n", th1, tm1, th2, tm2);
            linhaValida = 0;
        }

        // --- VALIDAÇÃO 4: Valor Monetário ---
        if (tempVal < 0) {
            fprintf(F1, "Linha inválida (Valor negativo): %.2f\n", tempVal);
            linhaValida = 0;
        }

        // SE PASSOU EM TUDO, GUARDA NA ESTRUTURA
        if (linhaValida) {
            vTarifas[i].tpTarif = tempTp;
            strcpy(vTarifas[i].codTarif, tempCod);
            vTarifas[i].inferior.hora = th1;
            vTarifas[i].inferior.min = tm1;
            vTarifas[i].superior.hora = th2;
            vTarifas[i].superior.min = tm2;
            vTarifas[i].valorUnit = tempVal;

            i++; // Só incrementa se gravou com sucesso
        }
    }

    fclose(F1);
    fclose(F2);
    (*paramSystS).nElemActT = i;
    return 1;
}

//
//****************************************** LER FICHEIRO ESTACIONAMENTO *********************************************************************
//

int lerFicheiroEstacionamento(ESTAC vEstac[], P_SYST_M_ESTAT_C *paramSystS) {

    FILE *F1 = fopen("erros_estac.txt", "w"); // Ficheiro de Log
    if (!F1) {
        printf("Erro ao criar log de erros.\n");
        remove("infPersonSyst.txt");
        return 0;
    }

    FILE *F2 = fopen("estacionamentos.txt", "r"); // Nome do ficheiro da imagem
    if (!F2) {
        printf("Impossível abrir estacionamentos.txt\n");
        fprintf(F1, "Erro fatal: Impossível abrir ficheiro de dados.\n");
        remove("infPersonSyst.txt");
        fclose(F1);
        return 0;
    }

    int i = 0;

    // --- VARIÁVEIS TEMPORÁRIAS ---
    char tempIdLixo[20]; // Para ler a 1ª coluna (L1, 2, 3...) que vamos ignorar
    char tempMat[20];    // Matrícula
    int a1, m1, d1;      // Data Entrada (Ano, Mês, Dia no ficheiro)
    int h1, min1;        // Hora Entrada
    char tempLugar[20];  // Lugar (cuidado com o tamanho 5 da struct)
    int a2, m2, d2;      // Data Saída
    int h2, min2;        // Hora Saída

    // O fscanf segue a ordem das colunas da imagem:
    // ID | Matrícula | Ano Ent | Mês Ent | Dia Ent | Hora Ent | Min Ent | Lugar | Ano Sai | Mês Sai | Dia Sai | Hora Sai | Min Sai
    while (i < MAX_REG_ESTAC &&
           fscanf(F2, "%s %s %d %d %d %d %d %s %d %d %d %d %d",
                  tempIdLixo, tempMat,
                  &a1, &m1, &d1, &h1, &min1,
                  tempLugar,
                  &a2, &m2, &d2, &h2, &min2) == 13) {

        int linhaValida = 1;

        // 1. Validar Datas (Entrada e Saída)
        // Nota: A tua função pede (dia, mes, ano), mas o ficheiro tem (ano, mes, dia). Atenção à ordem!
        if (!validar_data(d1, m1, a1)) {
            fprintf(F1, "Data Entrada invalida: %02d/%02d/%d (Matricula: %s)\n", d1, m1, a1, tempMat);
            linhaValida = 0;
        }
        if (!validar_data(d2, m2, a2)) {
            fprintf(F1, "Data Saida invalida: %02d/%02d/%d (Matricula: %s)\n", d2, m2, a2, tempMat);
            linhaValida = 0;
        }

        // 2. Validar Horários
        if (!validar_horario(h1, min1) || !validar_horario(h2, min2)) {
            fprintf(F1, "Horario invalido na linha da matricula %s\n", tempMat);
            linhaValida = 0;
        }

        // 3. Validar Tamanho das Strings (Buffer Overflow Protection)
        // A struct define matricula[15] e lugar[5]
        if (strlen(tempMat) >= 15) {
            fprintf(F1, "Matricula demasiado longa: %s\n", tempMat);
            linhaValida = 0;
        }
        if (strlen(tempLugar) >= 5) {
            fprintf(F1, "Codigo de Lugar demasiado longo: %s\n", tempLugar);
            linhaValida = 0;
        }

        // SE TUDO OK -> Gravar no Vetor
        if (linhaValida) {
            // Gerar ID sequencial automático (como pedido no comentário da struct)
            vEstac[i].numE = i + 1;

            strcpy(vEstac[i].matricVeiculo, tempMat);

            // Preencher Data/Hora Entrada
            vEstac[i].entradaDt.ano = a1;
            vEstac[i].entradaDt.mes = m1;
            vEstac[i].entradaDt.dia = d1;
            vEstac[i].entradaHm.hora = h1;
            vEstac[i].entradaHm.min = min1;

            strcpy(vEstac[i].lugar, tempLugar);

            // Preencher Data/Hora Saída
            vEstac[i].saidaDt.ano = a2;
            vEstac[i].saidaDt.mes = m2;
            vEstac[i].saidaDt.dia = d2;
            vEstac[i].saidaHm.hora = h2;
            vEstac[i].saidaHm.min = min2;

            // Inicializar campos que não estão no ficheiro
            vEstac[i].valPag = 0.0;
            strcpy(vEstac[i].obs, ""); // String vazia

            i++; // Incrementa contador de registos válidos
        }
    }

    fclose(F1);
    fclose(F2);
    (*paramSystS).nElemActE = i;

    // 2. Atualizar o último ID gerado.
    // Como geraste os IDs sequencialmente (i+1), o último ID é exatamente igual ao número de registos (i).
    (*paramSystS).ultIdEstacE = i;

    // Opcional: Atualizar nRegImportadosE se quiseres manter estatística
    (*paramSystS).nRegImportadosE = i;

    return 1;
}

int lerFicheiroParametros( FILE* F1, P_SYST_M_ESTAT_C *paramSist ){


    if(fscanf(F1, "%d/%d/%d\t%d/%d/%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%[^\t]\t%[^\t]",
                &paramSist->dtInstalacao.ano, &paramSist->dtInstalacao.mes, &paramSist->dtInstalacao.dia,
                &paramSist->dtUltProcess.ano, &paramSist->dtUltProcess.mes, &paramSist->dtUltProcess.dia,
                &paramSist->numPisos,
                &paramSist->numFilas,
                &paramSist->nLugPFila,
                &paramSist->nElemActT,
                &paramSist->ultIdEstacE,
                &paramSist->nMaxRegDefT,
                &paramSist->nMaxRegDefE,
                &paramSist->nRegImportadosT,
                paramSist->nomeEmpresa,
                paramSist->nomeInstalador)==16)
                return 1;

    else return 0;

}
