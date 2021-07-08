//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include <stdio.h>
#include "LOOP_ANINHADO.h"
#include "LOOP_UNICO.h"
#include "ORDENACAO.h"
#include "ORDENACAO_INTERCALACAO.c"

int main() {

    int c;
    scanf("%d", &c);

    if(c == 15) LOOP_ANINHADO(); 
    else if(c == 16) LOOP_UNICO();
    else if(c == 17) ORDENACAO();
    //else if(c == 18) ORDENACAO_INTERCALACAO();

    return 0;
}