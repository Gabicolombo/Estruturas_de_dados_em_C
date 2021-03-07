#include <stdlib.h>
#include <stdio.h>
#define true 1
#define false 0

typedef int bool;

typedef struct aux {
  int id;
  bool ehPreferencial;
  struct aux* prox;
} ELEMENTO, * PONT;

typedef struct {
  PONT inicioPref;
  PONT fimPref;
  PONT inicioGeral;
  PONT fimGeral;
} FILAPREFERENCIAL, * PFILA;
