//
//  Func_Estacionamentos.c
//  Projecto_AP_
//
//  Funcoes para gestao de estacionamentos e calculo de tarifas
//

#include "Func_Estacionamentos.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// ==================== FUNCOES DE REGISTO ====================

void RegistarEstacionamento(ESTAC E[], int idx, unsigned int _numE, char _mat[],
                            DATA _dtEnt, HORA_MIN _hmE, DATA _dtSaida,
                            HORA_MIN _hmS, float _valPag, char _obs[])
{
    E[idx].numE = _numE;
    strcpy(E[idx].matricVeiculo, _mat);
    E[idx].entradaDt = _dtEnt;
    E[idx].entradaHm = _hmE;
    E[idx].saidaDt = _dtSaida;
    E[idx].saidaHm = _hmS;
    E[idx].valPag = _valPag;
    strcpy(E[idx].obs, _obs);
}

// ==================== FUNCOES DE VISUALIZACAO ====================

void MostrarEstacionamento(ESTAC E[], int idx)
{
    printf("\nNum. Estac: %u | Matricula: [%s] | Lugar: [%s]\n",
           E[idx].numE,
           E[idx].matricVeiculo,
           E[idx].lugar);
    printf("   Entrada: %02d/%02d/%d %02d:%02d | Saida: %02d/%02d/%d %02d:%02d\n",
           E[idx].entradaDt.dia, E[idx].entradaDt.mes, E[idx].entradaDt.ano,
           E[idx].entradaHm.hora, E[idx].entradaHm.min,
           E[idx].saidaDt.dia, E[idx].saidaDt.mes, E[idx].saidaDt.ano,
           E[idx].saidaHm.hora, E[idx].saidaHm.min);
    printf("   Valor Pago: %.2f EUR | Obs: %s\n",
           E[idx].valPag,
           E[idx].obs);
}

void MostrarTicketEntrada(unsigned int numE, char *matricula, char *lugar,
                          DATA dtEnt, HORA_MIN hmEnt)
{
    printf("\n");
    printf("+========================================================+\n");
    printf("|              TICKET DE ESTACIONAMENTO                  |\n");
    printf("|                     ENTRADA                            |\n");
    printf("+========================================================+\n");
    printf("|  No Entrada:    %-10u                             |\n", numE);
    printf("|  Matricula:     %-15s                        |\n", matricula);
    printf("|  Lugar:         %-5s                                  |\n", lugar);
    printf("|  Data Entrada:  %02d/%02d/%d                             |\n",
           dtEnt.dia, dtEnt.mes, dtEnt.ano);
    printf("|  Hora Entrada:  %02d:%02d                                  |\n",
           hmEnt.hora, hmEnt.min);
    printf("+========================================================+\n");
    printf("|  GUARDE ESTE TICKET - Necessario para a saida          |\n");
    printf("+========================================================+\n");
}

void MostrarTicketSaida(ESTAC *estac, float valorPago)
{
    printf("\n");
    printf("+========================================================+\n");
    printf("|              TICKET DE ESTACIONAMENTO                  |\n");
    printf("|                      SAIDA                             |\n");
    printf("+========================================================+\n");
    printf("|  No Entrada:    %-10u                             |\n", estac->numE);
    printf("|  Matricula:     %-15s                        |\n", estac->matricVeiculo);
    printf("|  Lugar:         %-5s                                  |\n", estac->lugar);
    printf("+--------------------------------------------------------+\n");
    printf("|  Data Entrada:  %02d/%02d/%d  Hora: %02d:%02d               |\n",
           estac->entradaDt.dia, estac->entradaDt.mes, estac->entradaDt.ano,
           estac->entradaHm.hora, estac->entradaHm.min);
    printf("|  Data Saida:    %02d/%02d/%d  Hora: %02d:%02d               |\n",
           estac->saidaDt.dia, estac->saidaDt.mes, estac->saidaDt.ano,
           estac->saidaHm.hora, estac->saidaHm.min);
    printf("+--------------------------------------------------------+\n");
    printf("|                                                        |\n");
    printf("|  VALOR A PAGAR:    %8.2f EUR                       |\n", valorPago);
    printf("|                                                        |\n");
    printf("+========================================================+\n");
    printf("|              Obrigado pela preferencia!                |\n");
    printf("+========================================================+\n");
}

// ==================== FUNCOES DE CALCULO DE TARIFAS ====================

/**
 * Arredonda hora de entrada para o proximo multiplo de 15 minutos
 * Se ja estiver num multiplo de 15, mantem
 * Ex: 8:10 -> 8:15, 8:00 -> 8:00, 8:16 -> 8:30
 */
HORA_MIN ArredondarHoraEntrada(HORA_MIN hm) {
    HORA_MIN resultado;
    resultado.hora = hm.hora;

    // Se ja e multiplo de 15, manter
    if (hm.min % 15 == 0) {
        resultado.min = hm.min;
    } else {
        // Arredondar para o proximo multiplo de 15
        resultado.min = ((hm.min / 15) + 1) * 15;

        // Se passou de 60 minutos, ajustar hora
        if (resultado.min >= 60) {
            resultado.min = 0;
            resultado.hora++;
            if (resultado.hora >= 24) {
                resultado.hora = 0;
            }
        }
    }

    return resultado;
}

/**
 * Arredonda hora de saida para o proximo multiplo de 15 minutos
 * Se ja estiver num multiplo de 15, manter
 * Ex: 9:55 -> 10:00, 9:45 -> 9:45, 9:01 -> 9:15
 */
HORA_MIN ArredondarHoraSaida(HORA_MIN hm) {
    // Mesma logica que entrada - arredondar para cima
    return ArredondarHoraEntrada(hm);
}

/**
 * Verifica se uma hora esta no periodo diurno (08:00-21:59)
 * Retorna: 1 se diurno (08:00-21:59), 0 se noturno (22:00-07:59)
 */
int EhPeriodoDiurno(int hora) {
    return (hora >= HORA_INICIO_DIURNO && hora < HORA_INICIO_NOTURNO);
}

/**
 * Calcula quantos dias de diferenca entre duas datas
 * Usa uma aproximacao simples contando dias do ano
 */
int CalcularNumeroDias(DATA dtEntrada, DATA dtSaida) {
    // Se mesmo dia
    if (dtEntrada.ano == dtSaida.ano &&
        dtEntrada.mes == dtSaida.mes &&
        dtEntrada.dia == dtSaida.dia) {
        return 1;
    }

    // Calcular dias do ano para cada data (aproximacao)
    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Ajustar fevereiro para ano bissexto
    if (ano_bissexto(dtEntrada.ano)) diasPorMes[1] = 29;

    // Calcular dia do ano para entrada
    int diaAnoEntrada = dtEntrada.dia;
    for (int m = 0; m < dtEntrada.mes - 1; m++) {
        diaAnoEntrada += diasPorMes[m];
    }

    // Resetar para ano de saida se diferente
    if (ano_bissexto(dtSaida.ano)) diasPorMes[1] = 29;
    else diasPorMes[1] = 28;

    // Calcular dia do ano para saida
    int diaAnoSaida = dtSaida.dia;
    for (int m = 0; m < dtSaida.mes - 1; m++) {
        diaAnoSaida += diasPorMes[m];
    }

    // Se mesmo ano
    if (dtEntrada.ano == dtSaida.ano) {
        return diaAnoSaida - diaAnoEntrada + 1;
    }

    // Se anos diferentes
    int diasRestantesAnoEntrada = (ano_bissexto(dtEntrada.ano) ? 366 : 365) - diaAnoEntrada;
    int diasNoAnoSaida = diaAnoSaida;
    int anosIntermedio = dtSaida.ano - dtEntrada.ano - 1;
    int diasAnosIntermedio = anosIntermedio * 365; // Simplificacao

    return diasRestantesAnoEntrada + diasNoAnoSaida + diasAnosIntermedio + 1;
}

/**
 * Converte data/hora para minutos totais desde meia-noite
 * Util para calcular diferencas de tempo no mesmo dia
 */
int HoraParaMinutos(HORA_MIN hm) {
    return hm.hora * 60 + hm.min;
}

/**
 * Calcula minutos num periodo especifico (diurno ou noturno) para um intervalo de tempo
 * Apenas para calculos dentro do MESMO DIA
 */
void CalcularMinutosPorPeriodo(HORA_MIN hmInicio, HORA_MIN hmFim,
                                int *minutosDiurnos, int *minutosNoturnos) {
    *minutosDiurnos = 0;
    *minutosNoturnos = 0;

    int minInicio = HoraParaMinutos(hmInicio);
    int minFim = HoraParaMinutos(hmFim);

    // Limites dos periodos em minutos
    int inicioDiurno = HORA_INICIO_DIURNO * 60;      // 8:00 = 480 min
    int fimDiurno = HORA_INICIO_NOTURNO * 60;        // 22:00 = 1320 min

    // Percorrer cada minuto e classificar
    for (int m = minInicio; m < minFim; m++) {
        int horaAtual = m / 60;
        if (EhPeriodoDiurno(horaAtual)) {
            (*minutosDiurnos)++;
        } else {
            (*minutosNoturnos)++;
        }
    }
}

/**
 * Calcula o valor total a pagar pelo estacionamento
 * Aplica as regras do tarifario conforme o enunciado
 */
float CalcularValorEstacionamento(DATA dtEntrada, HORA_MIN hmEntrada,
                                   DATA dtSaida, HORA_MIN hmSaida) {

    // Arredondar horas para periodos de 15 minutos
    HORA_MIN hmEntArredondada = ArredondarHoraEntrada(hmEntrada);
    HORA_MIN hmSaiArredondada = ArredondarHoraSaida(hmSaida);

    // Calcular numero de dias
    int numDias = CalcularNumeroDias(dtEntrada, dtSaida);

    // CASO 1: Varios dias (>=2 passagens de dia) -> Tarifa T4
    if (numDias >= 2) {
        float valorMultiDia = TARIFA_MULTI_DIA * numDias;
        return valorMultiDia;
    }

    // CASO 2: Mesmo dia -> Calcular por horas (T1/T2) ou dia completo (T3)
    int minutosDiurnos = 0;
    int minutosNoturnos = 0;

    CalcularMinutosPorPeriodo(hmEntArredondada, hmSaiArredondada,
                              &minutosDiurnos, &minutosNoturnos);

    // Converter minutos para horas (fracionario)
    float horasDiurnas = minutosDiurnos / 60.0;
    float horasNoturnas = minutosNoturnos / 60.0;

    // Calcular valor por tarifa horaria
    float valorHorario = (horasDiurnas * TARIFA_DIURNA) + (horasNoturnas * TARIFA_NOTURNA);

    // Se o valor exceder o dia completo, aplicar tarifa de dia completo
    if (valorHorario > TARIFA_DIA_COMPLETO) {
        return TARIFA_DIA_COMPLETO;
    }

    return valorHorario;
}

// ==================== FUNCOES DE VALIDACAO ====================

/**
 * Valida formato de matricula
 * Aceita formatos: XX-XX-XX onde X pode ser letra ou numero
 * Tambem aceita formatos sem hifen ou com outros separadores
 */
int ValidarMatricula(char *matricula) {
    int len = strlen(matricula);

    // Verificar tamanho minimo e maximo
    if (len < 4 || len > 14) {
        return 0;
    }

    // Verificar se tem pelo menos alguns caracteres alfanumericos
    int numAlfanum = 0;
    for (int i = 0; i < len; i++) {
        if (isalnum((unsigned char)matricula[i])) {
            numAlfanum++;
        }
    }

    // Deve ter pelo menos 4 caracteres alfanumericos
    if (numAlfanum < 4) {
        return 0;
    }

    return 1;
}

/**
 * Verifica se a data/hora de saida e posterior a data/hora de entrada
 */
int ValidarSaidaPosteriorEntrada(DATA dtEnt, HORA_MIN hmEnt, DATA dtSai, HORA_MIN hmSai) {
    // Comparar anos
    if (dtSai.ano < dtEnt.ano) return 0;
    if (dtSai.ano > dtEnt.ano) return 1;

    // Anos iguais - comparar meses
    if (dtSai.mes < dtEnt.mes) return 0;
    if (dtSai.mes > dtEnt.mes) return 1;

    // Meses iguais - comparar dias
    if (dtSai.dia < dtEnt.dia) return 0;
    if (dtSai.dia > dtEnt.dia) return 1;

    // Dias iguais - comparar horas
    if (hmSai.hora < hmEnt.hora) return 0;
    if (hmSai.hora > hmEnt.hora) return 1;

    // Horas iguais - comparar minutos
    if (hmSai.min <= hmEnt.min) return 0;

    return 1;
}

// ==================== FUNCOES DE PESQUISA ====================

/**
 * Procura um estacionamento pelo numero de entrada
 */
int ProcurarEstacionamentoPorNumero(ESTAC vEstac[], int nEstac, unsigned int numE) {
    for (int i = 0; i < nEstac; i++) {
        if (vEstac[i].numE == numE) {
            return i;
        }
    }
    return -1; // Nao encontrado
}

/**
 * Procura um estacionamento ativo (ainda no parque) pela matricula
 * Um estacionamento esta ativo se saidaDt.ano == 0
 */
int ProcurarEstacionamentoAtivoPorMatricula(ESTAC vEstac[], int nEstac, char *matricula) {
    for (int i = 0; i < nEstac; i++) {
        if (strcmp(vEstac[i].matricVeiculo, matricula) == 0 &&
            vEstac[i].saidaDt.ano == 0) {
            return i;
        }
    }
    return -1; // Nao encontrado
}
