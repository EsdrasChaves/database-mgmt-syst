#include "page.h"
#include <stdio.h>
#include <stdlib.h>


#define NUM_SLOTS 4

struct page{
    int id;
    int heap_id;
    Record** slots;
    int* bitmap_slots;
    int num_slots;
};

Record **get_record(Page *pg){
    return pg->slots;

}

Page *pageNew(int id, int heap_id) {
    Page *pg = (Page*) malloc(sizeof(Page));
    pg->slots = (Record**)malloc(NUM_SLOTS*sizeof(Record*));
    pg->bitmap_slots = (int*) calloc(NUM_SLOTS,sizeof(int));
    pg->num_slots = NUM_SLOTS;
    pg->id = id;
    pg->heap_id = heap_id;

    /// Teste

    int i;
    for(i=0; i < 4; i++)
        pg->slots[i] = NULL;

    return pg;
}


// Insere registro na página e atualiza bitmap
int inst_record(Page *pg,Record *record){
    int slot = 0;
    while(pg->bitmap_slots[slot] != 0){
        slot++;
    }
    pg->bitmap_slots[slot] = 1;
    pg->num_slots--;
    pg->slots[slot] = record;
    return 1;
}


// Remove registro na página e atualiza bitmap
int remove_record(Page *pg,int num_slot){
    pg->bitmap_slots[num_slot] = 0;
    pg->num_slots++;
    free(pg->slots[num_slot]);
    pg->slots[num_slot] = NULL;
}

void imprime_bitmap(Page *pg){
    int i;
    for(i=0;i<NUM_SLOTS;i++){
        if(pg->bitmap_slots[i] !=0){
            imprimeInfo(pg->slots[i]);
        }
    }

    for(i=0;i<NUM_SLOTS;i++)
        printf("%d ", pg->bitmap_slots[i]);
}

int* get_bitmap(Page *pg){
    return pg->bitmap_slots;
}

int get_size_page(){
    return sizeof(struct page);
}
int get_id(Page *page){
    return  page->id;
}

/// Testes do Esdras Gato

int get_heap_id(Page* page) {
    return page->heap_id;
}

int get_num_slots() {
    return NUM_SLOTS;
}

/// Fim dos testes

int num_slot_key(Page* pg, int chave) {
    int i;

    for(i=0; i < NUM_SLOTS; i++) {
        if(pg->slots[i] != NULL && record_get_id(pg->slots[i]) == chave)
            return i;
    }
    return -1;
}

void imprimeRecords(Page *page){
    Record* rec;
    int i;
    printf("Page ID = %d\n", page->id);
    for(i=0;i<4;i++){
        rec = page->slots[i];
        if(page->bitmap_slots[i] == 1)imprimeInfo(rec);
    }
    //imprime_bitmap(page);
}
Record** getSlots(Page *p){
    return p->slots;
}
void setSlots(Record **rec,Page *p){
    p->slots = rec;
}

Record* getSlot_by_Position(Page* p, int slot_position) {
    return p->slots[slot_position];
}
