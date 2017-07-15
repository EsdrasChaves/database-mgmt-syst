#include "record.h"


typedef struct page Page;


Page *pageNew(int id, int heap_id);

// Insere registro na página e atualiza bitmap
int inst_record(Page *pg,Record *record);


// Remove registro na página e atualiza bitmap
int remove_record(Page *pg,int num_slot);

void imprime_bitmap(Page *pg);

int num_slot_key(Page* pg, int chave);

int record_get_id(Record* rcd);

int get_size_page();
int get_id(Page* page);
void imprimeRecords(Page *page);
Record** getSlots(Page *p);
void setSlots(Record **rec,Page *p);

Record* getSlot_by_Position(Page* p, int slot_position);


/// Testes do Esdras Gato
int get_heap_id(Page* page);

int get_num_slots();

/// Fim dos testes
