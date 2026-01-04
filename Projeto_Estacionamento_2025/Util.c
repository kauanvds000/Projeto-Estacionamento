//
//  Util.c
//
// //   Projecto_AP_

#include "Utils.h"
#include "InstalacaoN.h"
void MostrarParametros(P_SYST_M_ESTAT_C p) {
    printf("\n=======================================================\n");
    printf("               PARAMETROS DO SISTEMA                   \n");
    printf("=======================================================\n");
    printf("Empresa:            %s\n", p.nomeEmpresa);
    printf("Instalado por:      %s\n", p.nomeInstalador);
    printf("Data Instalação:    %02d/%02d/%d\n", p.dtInstalacao.dia, p.dtInstalacao.mes, p.dtInstalacao.ano);
    printf("-------------------------------------------------------\n");
    printf("Capacidade Máxima:  %d veiculos\n", p.nMaxRegDefE);
    printf("Ocupacão Atual:     %d veiculos\n", p.nElemActE);
    printf("Ultimo ID Gerado:   %d\n", p.ultIdEstacE);
    printf("-------------------------------------------------------\n");
    printf("Dimensões Parque:   %d Pisos x %d Filas x %d Lugares\n", p.numPisos, p.numFilas, p.nLugPFila);
    printf("=======================================================\n");
}

/** \brief Função que vai retirar o Enter que fica no final da string aceite pela função fgets(),
 *         algo que não acontece com a função gets(), mas que está "deprecated", por motivos de segurança;
 *         A função procura o final de string ('\0'), e coloca outro ('\0') a sobrepôr o caracter anterior,
 *		   onde está o Enter, eliminando-o.
 *		   Parâmetros: S é o ponteiro para a string a "tratar"
 *
 * \param S *char
 */
void retiraEnterString(char *S){
	while(*S!='\0')
		S++;
	*(--S)='\0';
}

/** \brief Função que converte uma string num unsigned int que é retornado à função chamante
 *		   Parâmetros: st é o ponteiro para a string a converter;
 *		               err é um ponteiro para uma "flag" (caracter) que assinala o sucesso da operação:
 *		                        será 'e' se houver erro, 'n', de contrário
 *
 * \param st *char
 * \param err *char
 * \return unsigned int
 *
 */
unsigned int convertUInt(char *st, char *err) {
  char *x;
  for (x = st ; *x ; x++) {
	  if (!isdigit(*x)){
		  *err='e';
		  return 0;
	  }
  }
  *err='n';
  return (unsigned int) (strtoul(st, 0L, 10));
}

/** \brief Função para mostrar uma mensagem e esperar que o utilizador digite <enter> significando isso que já leu a mensagem
 *         (algo semelhante à função system("pause") do Visual Studio)
 * 		   Parâmetros: mens é o ponteiro para a string a mostrar no ecrã;
 *
 * \param mens *char - contém a mensagem que se pretende seja mostrada no ecrã
 */
void mostrarMensagem(char *mens){
	printf("\n%s", mens);
	printf("\nDigite <Enter> para continuar... ");
    getchar();
	getchar();
}

/** \brief Função verificar se um ano é bissexto
 * 		   Parâmetros: o ano a verificar;
 *	       É retornado true ou false, atendendo ao ano ser ou não bissexto
 * \param ano int
 * \return bool
 */
bool ano_bissexto(int ano) {
	if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
		return true;
	}
	return false;
}

/** \brief Função para verificar se uma data é válida
 * 		   Parâmetros: a data a validar - o dia, mês e ano;
 *	       É retornado true ou false, atendendo à validade da data verificada
 * \param dia int
 * \param mes int
 * \param ano int
 * \return bool
 */
bool validar_data(int dia, int mes, int ano) {
	// Verificar se o mês é válido
	if (mes < 1 || mes > 12) {
		return false;
	}

	// Número de dias por mês
	int dias_por_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	// Ajuste para anos bissextos: fevereiro tem 29 dias
	if (ano_bissexto(ano)) {
		dias_por_mes[1] = 29;
	}

	// Verificar se o dia é válido para o mês
	if (dia < 1 || dia > dias_por_mes[mes - 1]) {
		return false;
	}

	return true;
}

bool validar_horario(int hora, int min) {
    if (hora < 0 || hora > 23) return false;
    if (min < 0 || min > 59) return false;
    return true;
}

/**
Nome-da-função: Read_Split_Line_File
Descrição: Permite ler o conteúdo de uma linha do ficheiro e devolve
		   uma tabela de strings com os vários campos lidos;
Parâmetros:
 - F:    ficheiro onde vai ser feita a leitura
 - n_campos_max:    número máximo de campos a ler
 - n_campos_lidos:    devolve o numero de campos que a linha contém
 - separadores :    permite especificar os delimitadores dos campos que estão na linha do ficheiro
Retorno:
 - Retorna uma tabela de strings
Observação:
 - Dentro desta função é alocado espaço de memória, ficando a cargo
   de quem invoca a função a libertação da memória alocada
Autor: Francisco Morgado
**/
STRING *Read_Split_Line_File(FILE *f, int n_campos_max, int *n_campos_lidos, char *separadores)
{
	LinhaTexto LT;
	*n_campos_lidos = 0;
	if (!f) return NULL;
	if (fgets(LT , MAX_LINHA_FICHEIRO , f) != NULL)    // fgets lê uma linha do ficheiro de texto para a string LT
	{												   // se o resultado da leitura for NULL, significa que nada foi lido
													   // sendo retornado NULL
		// "partir" a linha lida, usando os separadores definidos
		STRING *Res = (STRING *)malloc(n_campos_max*sizeof(STRING));  // alocação de um array com n_campos_max ponteiros para STRING
		char *pch = strtok (LT, separadores);
		int cont = 0;
		while (pch != NULL)
		{
			Res[cont] = (char *)malloc((strlen(pch)+1)*sizeof(char)); // alocação do espaço necessário para
													// guardar a string correspondente ao campo
			strcpy(Res[cont++], pch);
			pch = strtok (NULL, separadores);
		}
		*n_campos_lidos = cont;
		return Res;  // retorna o vector de ponteiros para cada uma das strings
					 // resultantes da divisão da string lida do ficheiro
	}
	return NULL;
}

void obterDataSistema(DATA_SISTEMA *d) {
    struct timespec ts;
    struct tm *tm_info;

    // C11: obtém segundos + nanossegundos (portável)
    timespec_get(&ts, TIME_UTC);

    // Converte para data/hora local (NÃO thread-safe, mas 100% portável)
    tm_info = localtime(&ts.tv_sec);

    d->anoS  = tm_info->tm_year + 1900;
    d->mesS  = tm_info->tm_mon + 1;
    d->diaS  = tm_info->tm_mday;
    d->horaS = tm_info->tm_hour;
    d->minutoS  = tm_info->tm_min;
    d->segundoS  = tm_info->tm_sec;

    d->milissegundoS = (int) ts.tv_nsec / 1000000;
}

void LimparEcra(){
    printf("\nPressione <Enter> para retornar ao menu...\n");
    getchar();
    system("cls");

}
