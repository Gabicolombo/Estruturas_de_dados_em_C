#include "filaDePrioridade.h"

PFILA criarFila(int max){
  PFILA res = (PFILA) malloc(sizeof(FILADEPRIORIDADE));
  res->maxElementos = max;
  res->arranjo = (PONT*) malloc(sizeof(PONT)*max);
  res->heap = (PONT*) malloc(sizeof(PONT)*max);
  int i;
  for (i=0;i<max;i++) {
    res->arranjo[i] = NULL;
    res->heap[i] = NULL;
  }
  res->elementosNoHeap = 0;
  return res;
}

void exibirLog(PFILA f){
  printf("Log [elementos: %i]\n", f->elementosNoHeap);
  PONT atual;
  int i;
  for (i=0;i<f->elementosNoHeap;i++){
    atual = f->heap[i];
    printf("[%i;%f;%i] ", atual->id, atual->prioridade, atual->posicao);
  }
  printf("\n\n");
}

int tamanho(PFILA f){
  int tam = 0;
  int i = 0;
  // vai navegar pelo o heap
  while(i < f->maxElementos){
      if(f->heap[i] != NULL) tam++;
      i++;
  }
  return tam;
}
bool verificarExistencia(PFILA f, int id){
    int nav = 0;
    while(nav < f->maxElementos){
        if(f->arranjo[nav]->id == id) return true;
        nav++;
    }
    return false;
}

int pai(int indice){
    // retorna o indice do pai
    return (indice-1)/2;
}
int filhoEsq(int indice){
    // retorna o indice do filho a esquerda
    return 2*indice + 1;
}
int filhoDir(int indice){
    // retorna o indice do filho a direita
    return 2*indice + 2;
}
void maxHeap(PFILA f, int tam, int indice){
    // nessa funcao será organizado os ponteiros
    // prevalendo o pai sempre com a prioridade maior que seus filhos
    int esq = filhoEsq(indice);
    int dir = filhoDir(indice);
    int maior = indice;
    PONT aux;
    if(esq<=tam && f->heap[esq] != NULL){
        if(f->heap[esq]->prioridade > f->heap[indice]->prioridade) maior = esq;
    }
    else maior = indice;
    if(dir<=tam && f->heap[dir] != NULL){
        if(f->heap[dir]->prioridade > f->heap[maior]->prioridade) maior = dir;
    }
    if(maior != indice){
        // mexendo nos indices dos pais, para aquele que tiver a prioridade maior
        // seja o novo pai
        aux = f->heap[indice];
        f->heap[indice] = f->heap[maior];
        f->heap[maior] = aux;
        f->heap[indice]->posicao = indice;
        f->heap[maior]->posicao = maior;
        maxHeap(f, tam, maior);
    }
    
}
bool inserirElemento(PFILA f, int id, float prioridade){
  bool res = false;
  if(id <0 || id>= f->maxElementos || f->arranjo[id] != NULL) return res;
  PONT i = (PONT) malloc(sizeof(ELEMENTO));
  // guardando as informacoes no arranjo
  i->id = id;
  i->prioridade = prioridade;
  f->arranjo[id] = i;
  // preparando para inseri-lo no heap
  PONT aux;
  int indice = tamanho(f);
  f->heap[indice] = i;
  i->posicao = indice;
  while(indice > 0){
      // reorganizando a arvore heap
      if(f->heap[indice]->prioridade > f->heap[pai(indice)]->prioridade){
        aux = f->heap[indice];
        f->heap[indice] = f->heap[pai(indice)];
        f->heap[pai(indice)] = aux;
        f->heap[indice]->posicao = indice;
        f->heap[pai(indice)]->posicao = pai(indice);
        indice = pai(indice);
      }else break;
  }
  // aumentando os elementos no heap
  f->elementosNoHeap ++;
  
  return true;
}

bool aumentarPrioridade(PFILA f, int id, float novaPrioridade){
  bool res = false;
  if(id < 0 || id>= f->maxElementos || f->arranjo[id] == NULL || novaPrioridade <= f->arranjo[id]->prioridade ) return res;
  f->arranjo[id]->prioridade = novaPrioridade;
  int i =0;
  int indice = 0;
  // procurando o indice daquele id que aumentou a prioridade
  while (i <= f->maxElementos){
      if(f->heap[i]->id == id) {
          f->heap[i]->prioridade = novaPrioridade;
          indice = i;
          break;
      }
      i++;
  }
  PONT aux;
  while(indice > 0){
      // reorganizando a arvore heap com as suas especificacoes
      // considerando o pai sempre com a prioridade maior que seus filhos
      if(f->heap[indice]->prioridade > f->heap[pai(indice)]->prioridade){
        aux = f->heap[indice];
        f->heap[indice] = f->heap[pai(indice)];
        f->heap[pai(indice)] = aux;
        f->heap[indice]->posicao = indice;
        f->heap[pai(indice)]->posicao = pai(indice);
        indice = pai(indice);
      }else break;
  }
  return true;
}

bool reduzirPrioridade(PFILA f, int id, float novaPrioridade){
  bool res = false;
  if(id<0 || id>= f->maxElementos || f->arranjo[id] == NULL || f->arranjo[id]->prioridade <= novaPrioridade) return res;
  f->arranjo[id]->prioridade = novaPrioridade;
  int posicao = 0;
  // procurando a posicao até o id que o usuario pede para reduzir a prioridade
  while(f->heap[posicao]->id != id){
      posicao +=1;
  }
  // chamando a funcao maxHeap para considerar a reorganizando dos filhos
  // pois se a prioridade do pai diminuir, o filho que tiver com a prioridade maior será o novo pai
  maxHeap(f, tamanho(f), posicao);
  return true;
}

PONT removerElemento(PFILA f){
  PONT res = NULL;
  if(f->elementosNoHeap == 0) return res;
  if(f->elementosNoHeap == 1){
      f->elementosNoHeap--;
      int id = f->heap[0]->id;
      f->arranjo[id] = NULL;
      PONT valor = f->heap[0];
      f->heap[0] = NULL;
      return valor;
  }
  int id = f->heap[0]->id;
  f->arranjo[id] = NULL;
  // pegamos o indice do ultimo id do heap e a partir dele trocamos com a raiz
  int indUlt = f->elementosNoHeap;
  PONT aux = f->heap[indUlt-1];
  PONT valor = f->heap[0];
  f->heap[indUlt-1] = NULL;
  f->heap[0] = aux;
  f->heap[0]->posicao = 0;
  // pegamos os filhos da esquerda e da direita da raiz para organiza-los mais a diante
  int esq = filhoEsq(0);
  int dir = filhoDir(0);
  int novaRaiz = 0;
  int max = 0;
  PONT temp;
  f->elementosNoHeap--;
  // a partir daqui será reorganizado a arvore heap, prevalendo aquele com maior prioridade na raiz
  // e cada pai tendo seus prioridades maiores do que dos seus filhos
  while (esq < f->elementosNoHeap){
    if(f->heap[esq]->prioridade > f->heap[novaRaiz]->prioridade || (dir < f->elementosNoHeap && f->heap[dir]->prioridade > f->heap[novaRaiz]->prioridade)){
        if( dir == f->elementosNoHeap || f->heap[esq]->prioridade > f->heap[dir]->prioridade)max = esq;
        else max = dir;
        maxHeap(f, tamanho(f), novaRaiz);
        f->heap[max]->posicao = max;
        f->heap[novaRaiz]->posicao = novaRaiz;
    }else break;
    // sera pego os novos filhos a esquerda e a direita, pois estamos navegando pela a arvore heap
    esq = filhoEsq(max);
    dir = filhoDir(max);
  }
  
  return valor;
}

bool consultarPrioridade(PFILA f, int id, float* resposta){
  bool res = false;
  if(id < 0 || id>=f->maxElementos || f->arranjo[id] == NULL) return res;
  // a prioridade é consultada pelo o indereço deixado por meio do id
  *resposta = f->arranjo[id]->prioridade;
  return true;
}

int main(){
  return 0;
}