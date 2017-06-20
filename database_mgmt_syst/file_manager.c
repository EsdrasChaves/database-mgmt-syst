#include "file_manager.h"
#include "disk_space_manager.h"

struct directory_pages{
    int page_id;
    int disk_block;             /// Qual bloco do disco a página se encontra
    int free_space;             /// Quantidade de espaço livre que a página contém
    Directory_pages* next;      /// Próxima página do diretório de página
};

/// Nome do Heap file e seu primeiro endereço
struct heap_dictionary{
    int heap_id;                    /// Id do HeapFile (Tabela)
    Directory_pages* header_page;   /// Endereço da primeira página no diretório
    Heap_dictionary* next;          /// Próxima informações de outra tabela
};


void printFile() {
    Heap_dictionary* hp_aux = HEAP;
    Directory_pages* dp_aux = NULL;
    int i = 0;
    while(hp_aux != NULL) {
        dp_aux = hp_aux->header_page;
        printf("HEAP: %d\n", i++);
        while(dp_aux != NULL) {
            printf("Pagina %d\n", dp_aux->page_id);
            dp_aux = dp_aux->next;
        }
        hp_aux = hp_aux->next;
    }
}



int update_free_space(int heap_id, int page_id, int num_update) {
    Heap_dictionary* hp_aux = HEAP;
    Directory_pages* dp_aux = NULL;

    while(hp_aux != NULL) {
        if(hp_aux->heap_id == heap_id) {
            dp_aux = hp_aux->header_page;

            while(dp_aux != NULL) {
                if(dp_aux->page_id == page_id) {
                    dp_aux->free_space += num_update;
                    return 1;
                }
                dp_aux = dp_aux->next;
            }
        }
        hp_aux = hp_aux->next;
    }

    return 0;
}

int cr8_heapfile() {
    int i, heap_id;
    Heap_dictionary* hp_aux = HEAP;
    Heap_dictionary* new_heap = (Heap_dictionary*)malloc(sizeof(Heap_dictionary));

    for(i=0; i<MAX_HEAP_ID_SIZE; i++) {
        if(!used_heaps_ids[i]) {
            heap_id = i;
            used_heaps_ids[i] = 1;
            break;
        }
    }

    new_heap->heap_id = heap_id;
    new_heap->header_page = NULL;
    new_heap->next = NULL;

    if(hp_aux == NULL) {
        HEAP = new_heap;
        return i;
    } else {
        while(hp_aux->next != NULL) {
            hp_aux = hp_aux->next;
        }

        hp_aux->next = new_heap;
        return i;
    }

    return -1;
}

int cr8_page_directory(int heap_id, int page_id, int disk_block) {
    Heap_dictionary* aux = HEAP;
    Directory_pages* new_page = (Directory_pages*)malloc(sizeof(Directory_pages));
    Directory_pages* pg_aux = NULL;

    while(aux != NULL) {
        if(heap_id == aux->heap_id) {
            new_page->page_id = page_id;
            new_page->disk_block = disk_block;
            new_page->free_space = 4;
            new_page->next = NULL;

            pg_aux = aux->header_page;
            if(pg_aux != NULL) {
                while(pg_aux->next != NULL) {
                    pg_aux = pg_aux->next;
                }
            } else {
                aux->header_page = new_page;
                return 1;
            }
            pg_aux->next = new_page;

            return page_id;
        }
        aux = aux->next;
    }

    return -1;
}

int initHeap() {
    int i;

    /*HEAP = (Heap_dictionary*)malloc(sizeof(Heap_dictionary));
    HEAP->heap_id = 0;
    HEAP->header_page = NULL;
    HEAP->next = NULL;
    */

    used_pages_ids = (int*)malloc(MAX_PAGE_ID_SIZE * sizeof(int));
    used_heaps_ids = (int*)malloc(MAX_HEAP_ID_SIZE * sizeof(int));

    for(i=0; i<MAX_PAGE_ID_SIZE; i++)
        used_pages_ids[i] = 0;

    for(i=0; i<MAX_HEAP_ID_SIZE; i++)
        used_heaps_ids[i] = 0;

}

int fm_load(void) {
}

int get_free_page(int heap_id, int *disk_block) {
    Heap_dictionary* hp_aux = HEAP;
    Directory_pages* pg_aux = NULL;
    int page_id_aux;

    while(hp_aux != NULL) {
        if(heap_id == hp_aux->heap_id) {
            pg_aux = hp_aux->header_page;

            while(pg_aux != NULL) {
                if(pg_aux->free_space > 0) {
                    *disk_block = pg_aux->disk_block;
                    return pg_aux->page_id;
                }

                pg_aux = pg_aux->next;
            }

            page_id_aux = cr8_page(heap_id, disk_block);
            if(page_id_aux != -1)
                return page_id_aux;

        }

        hp_aux = hp_aux->next;
    }


    return -1;
}

int fetch_page(int heap_id, int page_id) {
    Heap_dictionary* hp_aux = HEAP;
    Directory_pages* pg_aux = NULL;

    while(hp_aux != NULL) {
        if(hp_aux->heap_id == heap_id) {
            pg_aux = hp_aux->next;

            while(pg_aux != NULL) {
                if(pg_aux->page_id == page_id) {
                    return pg_aux->disk_block;
                }
                pg_aux = pg_aux->next;
            }
        }
        hp_aux = hp_aux->next;
    }

    return -1;
}

int cr8_page(int heap_id, int *disk_block) {
    int i;
    int new_block = cr8_block();

    if(new_block != -1) {
        for(i=0; i<MAX_PAGE_ID_SIZE; i++) {
            if(!used_pages_ids[i]) {
                *disk_block = new_block;
                used_pages_ids[i] = 1;
                if(cr8_page_directory(heap_id, i, new_block))
                    return i;
            }
        }
    }
    return -1;
}

