#include "filapreferencial.h"

PFILA criarFila(){
  PFILA res = (PFILA) malloc(sizeof(FILAPREFERENCIAL));
  res->inicioPref = NULL;
  res->fimPref = NULL;
  res->inicioGeral = NULL;
  res->fimGeral = NULL;
  return res;
}

int tamanho(PFILA f){
  PONT atual = f->inicioGeral;
  int tam = 0;
  while (atual) {
    atual = atual->prox;
    tam++;
  }
  return tam;
}

int tamanhoFilaPreferencial(PFILA f){
  PONT atual = f->inicioPref;
  int tam = 0;
  while (atual) {
    atual = atual->prox;
    tam++;
  }
  return tam;
}

PONT buscarID(PFILA f, int id){
  PONT atual = f->inicioGeral;
   while (atual) {
    if (atual->id == id) return atual;
    atual = atual->prox;
  }
  return NULL;
}

void exibirLog(PFILA f){
  int numElementos = tamanho(f);
  printf("\nLog fila geral [elementos: %i] - Inicio:", numElementos);
  PONT atual = f->inicioGeral;
  while (atual){
    printf(" [%i;%i]", atual->id, atual->ehPreferencial);
    atual = atual->prox;
  }
  printf("\n");
  numElementos = tamanhoFilaPreferencial(f);
  printf("\nLog fila preferencial [elementos: %i] - Inicio:", numElementos);
  atual = f->inicioPref;
  while (atual){
    printf(" [%i;%i]", atual->id, atual->ehPreferencial);
    atual = atual->prox;
  }
  printf("\n\n");
}

bool inserirPessoaNaFila(PFILA f, int id, int ehPreferencial){
  bool resposta = false;
  if(id < 0 || buscarID(f, id ) != NULL) return resposta;
  PONT novo = (PONT) malloc(sizeof(ELEMENTO));
  novo ->id = id;
  novo ->ehPreferencial = ehPreferencial;
  if(f->inicioGeral == NULL && f->fimGeral == NULL) {
      novo->prox = NULL;
      f->inicioGeral = novo;
      f->fimGeral = novo;
  }else{
      f->fimGeral->prox = novo;
      f->fimGeral = novo;
      novo->prox = NULL;
  }
  // Verificando se é preferencial para arrumar os ponteiros
  if(novo->ehPreferencial){
      PONT novoP = (PONT) malloc(sizeof(FILAPREFERENCIAL));
      novoP->id = id;
      novoP->ehPreferencial = ehPreferencial;
      if(f->inicioPref == NULL && f->fimPref == NULL){
          f->inicioPref = novoP;
          novoP->prox = NULL;
          f->fimPref = novoP;
      }else{
          // Arrumando os ponteiros quando não é o primeiro para entrar na fila
          novoP->prox = NULL;
          f->fimPref->prox = novoP;
          f->fimPref = novoP;
      }
  }
  return true;
}

bool atenderPrimeiraDaFilaPreferencial(PFILA f, int* id){
  bool resposta = false;
  PONT aux2 = f->inicioGeral;
  PONT antP = NULL;
  if(f->inicioGeral == NULL){ 
      f->fimGeral = NULL;
      f->inicioPref = NULL;
      f->fimPref = NULL;
      return resposta;
  }
  // verificando se a fila preferencial está vazia
  if(tamanhoFilaPreferencial(f) != 0){
      *id = f->inicioPref->id;
      PONT apagarPref = f->inicioPref;
      if(f->inicioPref->id == f->inicioGeral->id){
          PONT apagarGeral = f->inicioGeral;
          f->inicioPref = f->inicioPref->prox;
          f->inicioGeral = f->inicioGeral->prox;
          free(apagarPref);
          free(apagarGeral);
      }else{
        while(aux2->id != f->inicioPref->id){
            antP = aux2;
            aux2 =  aux2->prox;
        } 
        antP->prox = aux2->prox; 
        f->inicioPref = f->inicioPref->prox;
        free(apagarPref);
        free(aux2);
      }
  }else if(tamanhoFilaPreferencial(f) == 0 && tamanho(f) != 0){
      *id = f->inicioGeral->id;
      PONT apagar2 = f->inicioGeral;
      f->inicioGeral = f->inicioGeral->prox;
      free(apagar2);
  }else return resposta;
  return true;
}

bool atenderPrimeiraDaFilaGeral(PFILA f, int* id){
  bool resposta = false;
  PONT ant= NULL;     
  PONT aux = f->inicioPref;
  PONT apagar = f->inicioGeral;
  // Condição necessária para se não estiver ninguem na fila
  // consiga inserir novamente
  if(f->inicioGeral == NULL) {
      f->inicioPref = NULL;
      f->fimGeral = NULL;
      f->fimPref = NULL;
      return resposta;
  }
  // Vendo se é preferencial
  if(f->inicioGeral->ehPreferencial){
      *id = f->inicioGeral->id;
      while(aux->id!= f->inicioGeral->id){
          ant = aux;
          aux = aux->prox;
      }
      if(f->inicioPref->id == f->inicioGeral->id){
        f->inicioPref = f->inicioPref->prox;
        f->inicioGeral = f->inicioGeral->prox;
      }
      else ant->prox = aux->prox;
      free(apagar);
      free(aux);
  }else{
      *id = f->inicioGeral->id;
      f->inicioGeral = f->inicioGeral->prox;
      free(apagar);
  }
  return true;
}
// Funcao para poder buscar a posicao do elemento com aquele determinado ID
PONT buscarIDPref(PFILA f, int id){
  PONT atual = f->inicioPref;
   while (atual) {
    if (atual->id == id) return atual;
    atual = atual->prox;
  }
  return NULL;
}

bool desistirDaFila(PFILA f, int id){
  bool resposta = false;
  if(buscarID(f, id) == NULL) return resposta;
  PONT pref = buscarIDPref(f, id);
  PONT nav = f->inicioPref;
  PONT proxP = f->inicioPref;
  PONT navG = buscarID(f, id);
  PONT navGeral = f->inicioGeral;
  PONT proxG = f->inicioGeral;
  // Buscando o proximo elemento do id para poder
  // arrumar os ponteiros
  if(pref != NULL){
      if(f->inicioPref->id == navG->id){
          f->inicioPref = f->inicioPref->prox;
          free(pref);
      }
      else{
          while(nav != pref){
              proxP = nav;
              nav = nav->prox;
          }
          proxP->prox = nav->prox;
          free(nav);
      }
  }
  // verificando a posicao e o proximo para poder
  // arrumar os ponteiros na fila geral
  if(f->inicioGeral == navG){
      f->inicioGeral = f->inicioGeral->prox;
      free(navG);
  }else{
      if(f->fimGeral->id == navG->id) {
          f->fimGeral = proxG;
      }
      while(navGeral != navG){
          proxG = navGeral;
          navGeral = navGeral->prox;  
      }
      proxG->prox = navGeral->prox;
      free(navGeral);
  }
  if(tamanho(f) == 0){
      f->inicioGeral = NULL;
      f->fimGeral = NULL;
  }else if(tamanhoFilaPreferencial(f) == 0){
      f->inicioPref = NULL;
      f->fimPref = NULL;
               
  }
  return true;
}

int main(){
  return 0;
}