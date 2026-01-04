//
//  Utils.h
//  LerFichELoadEmMemoria
//
//  Created by Kauan Victor on 01/12/2025.
//

#ifndef Utils_h
#define Utils_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
// para usar o tipo bool
#include <stdbool.h>
// usar para usar as funções de tempo
#include <time.h>

#include <locale.h>

// Definem-se aqui os tipos básicos para usar na função ReadSplitLine
#define STRING char *
#define MAX_LINHA_FICHEIRO 150
typedef char LinhaTexto[MAX_LINHA_FICHEIRO];

// Outros tipos de dados usados

// struct para a data do sistema
typedef struct {
    int diaS;
    int mesS;
    int anoS;
    int horaS;
    int minutoS;
    int segundoS;
    int milissegundoS;
} DATA_SISTEMA;


typedef struct data{
	int dia, mes, ano;
} DATA;

typedef struct horaMin{
    int hora, min;
} HORA_MIN;

// Protótipos relativos às funções existentes no ficheiro .c
void retiraEnterString(char *S);
unsigned int convertUInt(char *st, char *err);
void mostrarMensagem(char *mens);
STRING *Read_Split_Line_File(FILE *f, int n_campos_max, int *n_campos_lidos, char *separadores);
bool ano_bissexto(int ano);
bool validar_data(int dia, int mes, int ano);
void obterDataSistema(DATA_SISTEMA *d);
bool validar_horario(int hora, int min);
void LimparEcra();

#endif /* Utils_h */
