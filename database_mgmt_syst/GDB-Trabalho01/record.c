#include <string.h>
#include <stdio.h>
#include "record.h"


#define RECORD_SIZE 16


/*
    Estrutura registro

    Tamanho fixo para cada Heap file
    Incialmente comporta um int e um char, posteriormente será um conjunto de bytes
*/

struct record{
    int id;
    char name[10];
};


int get_size_record(){
    return sizeof(struct record);
}

Record *recordNew() {
    Record *rcd = (Record*) malloc(sizeof(Record));
    return rcd;
}
void insereInfo(Record *rcd, int id, char name[10]) {
    rcd->id = id;
    strcpy(rcd->name, name);
}
void imprimeInfo(Record *rcd) {
    printf("ID: %d\nNAME: %s\n", rcd->id, rcd->name);
}

int record_get_id(Record* rcd) {
    return rcd->id;
}
