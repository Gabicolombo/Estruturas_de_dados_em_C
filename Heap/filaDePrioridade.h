#include <stdlib.h>
#include <stdio.h>
#define true 1
#define false 0

typedef int bool;

typedef struct {
  int id;
  float prioridade;
  int posicao;
} ELEMENTO, * PONT;

typedef struct {
  int maxElementos;
  int elementosNoHeap;
  PONT* heap;
  PONT* arranjo;
} FILADEPRIORIDADE, * PFILA;

PFILA criarFila(int max);

int tamanho(PFILA f);

bool inserirElemento(PFILA f, int id, float prioridade);

bool aumentarPrioridade(PFILA f, int id, float novaPrioridade);

bool reduzirPrioridade(PFILA f, int id, float novaPrioridade);

PONT removerElemento(PFILA f);

bool consultarPrioridade(PFILA f, int id, float* resposta);

/* As seguintes funcoes nao precisam ser implementadas, mas podem ser 
   utilizadas como funcoes recursivas auxiliares para a insercao,
   remocao ou alteracao de prioridade                                  */
void reduzirPrioridadeAux(PFILA f, int posicao);
void reduzirPrioridadeAux2(PFILA f, PONT atual);

void aumentarPrioridadeAux(PFILA f, int posicao);
void aumentarPrioridadeAux2(PFILA f, PONT atual);

void refazHeapMaximo(PFILA f, PONT atual);