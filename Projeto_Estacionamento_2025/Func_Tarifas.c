//
//  Func_Tarifas.c
//  Projecto_AP_
//


#include "Func_Tarifas.h"
#include "InstalacaoN.h"


void MostrarTarifa(TARIFA T[], int idx) // idx é o índice do vector corespondente ao requisitante a mostrar
{
    printf("\tNumero da Tarifa: %d - \tTarifa-> TIPO: %c Codigo: [%s] Hora entrada: %02d:%02d Hora saida: %02d:%02d Valor: %.2f.\n",
            idx+1,
            T[idx].tpTarif,
            T[idx].codTarif,
            T[idx].inferior.hora,
            T[idx].inferior.min,
            T[idx].superior.hora,
            T[idx].superior.min,
            T[idx].valorUnit);
}

void GuardarTarifasEmFicheiro(TARIFA vTarifas[], int nTarifas) {
    FILE *f = fopen("Tarifario.txt", "w");
    if (!f) {
        printf("[ERRO] Nao foi possivel abrir o ficheiro para gravacao!\n");
        return;
    }

    // Reescreve todas as tarifas com a formatação correta
    for (int i = 0; i < nTarifas; i++) {
        fprintf(f, "%c\t%s\t%02d:%02d\t%02d:%02d\t%.2f€\n",
                vTarifas[i].tpTarif,
                vTarifas[i].codTarif,
                vTarifas[i].inferior.hora, vTarifas[i].inferior.min,
                vTarifas[i].superior.hora, vTarifas[i].superior.min,
                vTarifas[i].valorUnit);
    }
    fclose(f);
    printf("Alteracoes guardadas no ficheiro Tarifario.txt!\n");
}

void ModificarTarifa(TARIFA vTarifas[], int nTarifas) { // nTarifas = paramSist.nElemActT = numeros de linhas ( tarifas ) lidas no ficheiro
    char codPesquisa[5];
    printf("\n--- MODIFICAR TARIFA ---\n");
    printf("Qual o codigo da tarifa a alterar? (ex: CT1): ");
    scanf("%s", codPesquisa);

    // ---PARA CORRIGIR O MENU DUPLO ---
    /*
    1.Quando digita o código da tarifa (ex: ct2) e carrega no Enter, a função scanf("%s", ...) lê o "ct2", mas deixa o "Enter" (\n) no buffer do teclado.

    2.A função termina e volta ao Menu.

    3.O Menu usa fgets. O fgets vai ao buffer e encontra o "Enter" que sobrou. Ele "pensa" que você digitou uma linha vazia.

    4.loop roda uma vez "em falso" (imprime o menu), vê que a linha vazia não é válida, e só na segunda volta é que para e espera que você digite de novo.
    */
    while (getchar() != '\n'); // Limpa o Enter que o scanf deixou para trás

    int idx = -1; // define como -1  = nao encontrado
    // Procura a tarifa no vetor
    for (int i = 0; i < nTarifas; i++) {
        if (strcmp(vTarifas[i].codTarif, codPesquisa) == 0) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {// se continuar como -1 = nao encontrado
        printf("[ERRO] Tarifa com codigo '%s' nao encontrada.\n", codPesquisa);
    } else {
        printf("Dados Atuais: Tipo: %c | Valor: %.2f\n", vTarifas[idx].tpTarif, vTarifas[idx].valorUnit);
        printf("\nInsira os NOVOS dados:\n");


        //----------------////// FAZER VERIFICACÕES //////------------------//

        printf("Novo Tipo (H=Hora, D=Dia): ");
        scanf(" %c", &vTarifas[idx].tpTarif); // Espaço antes de %c para limpar buffer

        printf("Novo Inicio (Hora Minuto): ");
        scanf("%d %d", &vTarifas[idx].inferior.hora, &vTarifas[idx].inferior.min);

        printf("Novo Fim (Hora Minuto): ");
        scanf("%d %d", &vTarifas[idx].superior.hora, &vTarifas[idx].superior.min);

        printf("Novo Valor (Use ponto, ex: 0.60): ");
        scanf("%f", &vTarifas[idx].valorUnit);

        //----------------////// FAZER VERIFICACÕES //////------------------//

        while (getchar() != '\n'); // Limpa o Enter que sobrou do scanf anterior

        // Gravar no ficheiro imediatamente
        GuardarTarifasEmFicheiro(vTarifas, nTarifas); // nTarifas = paramSist.nElemActT = numeros de linhas ( tarifas ) lidas no ficheiro
    }
}

void CriarNovaTarifa(TARIFA vTarifas[], P_SYST_M_ESTAT_C *paramSystS) {
    // 1. Verificar se há espaço no vetor
    if (paramSystS->nElemActT >= paramSystS->nMaxRegDefT) {
        printf("[ERRO] O limite máximo de tarifas (%d) já foi atingido!\n", paramSystS->nMaxRegDefT);
        return;
    }

    int idx = paramSystS->nElemActT; // A nova posição é o fim atual do vetor
    char novoCod[5];

    printf("\n--- CRIAR NOVA TARIFA ---\n");
    printf("Código (ex: CT5): ");
    scanf("%s", novoCod);

    // Verificar se já existe
    for(int i=0; i<paramSystS->nElemActT; i++){
        if(strcmp(vTarifas[i].codTarif, novoCod) == 0){
             printf("[ERRO] Já existe uma tarifa com o código %s!\n", novoCod);
             return;
        }
    }

    strcpy(vTarifas[idx].codTarif, novoCod);

    printf("Tipo (H=Hora, D=Dia): ");
    scanf(" %c", &vTarifas[idx].tpTarif);

    printf("Inicio (Hora Minuto): ");
    scanf("%d %d", &vTarifas[idx].inferior.hora, &vTarifas[idx].inferior.min);

    printf("Fim (Hora Minuto): ");
    scanf("%d %d", &vTarifas[idx].superior.hora, &vTarifas[idx].superior.min);

    printf("Valor (ex: 5.00): ");
    scanf("%f", &vTarifas[idx].valorUnit);

    // Atualizar contadores
    paramSystS->nElemActT++;

    while (getchar() != '\n'); // Limpa o Enter que sobrou do scanf anterior

    // Gravar tudo no ficheiro
    GuardarTarifasEmFicheiro(vTarifas, paramSystS->nElemActT); // nTarifas = paramSist.nElemActT = numeros de linhas ( tarifas ) lidas no ficheiro
}

