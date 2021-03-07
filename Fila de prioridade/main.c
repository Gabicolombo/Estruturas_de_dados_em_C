#include "filaDePrioridade.h"

PFILA criarFila(int max){
  PFILA res = (PFILA) malloc(sizeof(FILADEPRIORIDADE));
  res->maxElementos = max;
  res->arranjo = (PONT*) malloc(sizeof(PONT)*max);
  int i;
  for (i=0;i<max;i++) res->arranjo[i] = NULL;
  PONT cabeca = (PONT) malloc(sizeof(ELEMENTO));
  res->fila = cabeca;
  cabeca->ant = cabeca;
  cabeca->prox = cabeca;
  cabeca->id = -1;
  cabeca->prioridade = 1000000;
  return res;
}

void exibirLog(PFILA f){
  printf("Log [elementos: %i (alem do no cabeca)]\n", tamanho(f));
  PONT atual = f->fila;
  printf("%p[%i;%f;%p]%p ", atual->ant, atual->id, atual->prioridade, atual, atual->prox);
  atual = atual->prox;
  while (atual != f->fila){
    printf("%p[%i;%f;%p]%p ", atual->ant, atual->id, atual->prioridade, atual, atual->prox);
    atual = atual->prox;
  }
  printf("\nElementos validos: ");
  atual = atual->prox;
  while (atual != f->fila){
    printf("[%i;%f;%p] ", atual->id, atual->prioridade, atual);
    atual = atual->prox;
  }

  printf("\nValores do arrajo:\n\[ ");
  int x;
  for (x=0;x<f->maxElementos;x++) printf("%p ",f->arranjo[x]);
  printf("]\n\n");
}

int tamanho(PFILA f){
  int tam = 0;
  // começa pelo o proximo do cabeca
  PONT end = f->fila->prox;
  while(end != f->fila){
  	tam+=1;
  	end = end->prox;
  }
  return tam;

}
// Essa função foi feita para saber o anterior e o posterior do elemento
PONT buscaSeqExc(PFILA f, float x, PONT* ant, PONT* post){
    *ant = f->fila;
    *post = f->fila->prox;
    PONT ini = f->fila->prox;
    while(ini != f->fila && ini->prioridade > x){
        *ant = ini;
        *post = ini->prox;
        ini = ini->prox;
    }
    if(ini != f->fila && ini->prioridade == x){
        return ini;
    }
    return NULL;
    
}

bool inserirElemento(PFILA f, int id, float prioridade){
  bool resposta = false;
  PONT i, ant, post;
  if((id <0 || id >= f->maxElementos) || (f->arranjo[id] != NULL)){
  	return resposta;
  }
  // por meio dessa chamada de função saberemos o anterior e o posterior
  i = buscaSeqExc(f, prioridade, &ant, &post);
  // reservando memoria
  i = (PONT) malloc(sizeof(ELEMENTO));
  // colocando o endereço do elemento no arranjo
  f->arranjo[id] = i;
  i->ant = ant;
  ant->prox = i;
  i->prox = post;
  post->ant = i;
  // preenchendo o id e a prioridade
  i->prioridade = prioridade;
  i->id = id;
  
  return true;
}

bool aumentarPrioridade(PFILA f, int id, float novaPrioridade){
  bool resposta = false;
  if(id<0 || id >= f->maxElementos || f->arranjo[id] == NULL || f->arranjo[id]->prioridade >= novaPrioridade){
      return resposta;
  }
  PONT i, ant, post;
  // reservamos a propriedade anterior em uma variavel
  i = f->arranjo[id];
  ant = i->ant;
  post = i->prox;
  
  ant->prox = post;
  post->ant = ant;
  buscaSeqExc(f, novaPrioridade, &ant, &post);
  post = ant->prox;
  ant->prox = i;
  i->ant = ant;
  post->ant = i;
  i->prox = post;
  i = f->arranjo[id];
  i->prioridade = novaPrioridade;
  return true;
}

bool reduzirPrioridade(PFILA f, int id, float novaPrioridade){
  bool resposta = false;
  if(id<0 || id >= f->maxElementos || f->arranjo[id] == NULL || f->arranjo[id]->prioridade <= novaPrioridade){
      return resposta;
  }
  PONT i, ant, post;
  float propAnterior = f->arranjo[id]->prioridade;
  // Na linha abaixo i recebeu o endereço por meio do arranjo
  i = f->arranjo[id];
  // Declarei o anterior e o posterior
  ant = i ->ant;
  post = i->prox;
  // Atualizei os ponteiros
  ant->prox = post;
  post->ant = ant;
  buscaSeqExc(f, novaPrioridade, &ant, &post);
  // Arrumando os posteiros do novo anterior e posterior do elemento
  post = ant->prox;
  ant->prox = i;
  i->ant = ant;
  post->ant = i;
  i->prox = post;
  i = f->arranjo[id];
  i->prioridade = novaPrioridade;
  
  return true;
}

PONT removerElemento(PFILA f){
  PONT resposta = NULL;
  if(f->fila->prox == f->fila){
      return resposta;
  }
  PONT i;
  // Sempre aquele que tem a prioriedade maior será removido primeiro
  i = f->fila->prox;
  // Atualizando os ponteiros
  i->prox ->ant = f->fila;
  f->fila->prox = i->prox;
  // Atualizando no arranjo
  f->arranjo[i->id] = NULL;
  return i;
}

bool consultarPrioridade(PFILA f, int id, float* resposta){
  bool resp = false;
  if(id < 0 || id>=f->maxElementos || f->arranjo[id] == NULL){
      return resp;
  }
  // É consultado por meio do endereço deixado pelo o número do id
  *resposta = f->arranjo[id]->prioridade;
  return true;
}

int main(){
  return 0;
}