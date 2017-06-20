#include "record.h"


typedef struct page Page;


Page *pageNew();

// Insere registro na página e atualiza bitmap
int inst_record(Page *pg,Record *record);


// Remove registro na página e atualiza bitmap
int remove_record(Page *pg,int num_slot);

void imprime_bitmap(Page *pg);

int num_slot_key(Page* pg, int chave);

int record_get_id(Record* rcd);

int get_size_page();
int get_id(Page* page);
