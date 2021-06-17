//Laís Saloum Deghaide, nUSP: 11369767
//Thiago Henrique dos Santos Cardoso, nUSP: 11796594

#include <stdio.h>
#include "CREATE_TABLE.h"
#include "SELECT.h"
#include "INSERT_INTO.h"
#include "CREATE_INDEX.h"

int main() {

    char c;
    scanf("%c", &c);

    if(c == '1') CREATE_TABLE(c); 
    else if(c == '2') CREATE_TABLE(c);

    else if(c == '3') SELECT_FROM(c);
    else if(c == '4') SELECT_FROM(c);

    else if(c == '5') SELECT_WHERE(c);
    else if(c == '6') SELECT_WHERE(c);

    else if(c == '7') INSERT_INTO(c);
    else if(c == '8') INSERT_INTO(c);
    
    else if(c == '9') CREATE_INDEX(c); 
    else if(c == '10') CREATE_INDEX(c);
    
    else if(c == '11') PREFIXO(c);
    else if(c == '12') RECUPERA_CODLINHA(c);
    
    else if(c == '13') INSERT(c);
    else if(c == '14') INSERT(c);

    return 0;
}