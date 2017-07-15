#include "file_manager.h"
#include "disk_space_manager.h"

#define ARQUIVO_HEAP "heap_file.txt"
#define ARQUIVO_DIRECTORY "directory_file.txt"

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
            read_page(hp_aux->heap_id, dp_aux->page_id);
            printf("\n");
            dp_aux = dp_aux->next;
        }
        printf("\n\n");
        hp_aux = hp_aux->next;
    }
}

void search_by_id(int heap_id, int record_id) {
    Heap_dictionary* hp_aux = HEAP;
    Directory_pages* dp_aux = NULL;
    Page* page;
    int i = 0, *bitmap, flag = 0;

    bitmap = (int*) malloc(get_num_slots() * sizeof(int));

    while(hp_aux != NULL && hp_aux->heap_id != heap_id) {
        hp_aux = hp_aux->next;
    }
    if(hp_aux != NULL) {
        dp_aux = hp_aux->header_page;
        while(dp_aux != NULL) {
            page = read_page_record(hp_aux->heap_id, dp_aux->page_id);
            if(page != NULL) {
                bitmap = get_bitmap(page);
                for(i=0; i < get_num_slots(); i++) {
                    if(bitmap[i] == 1 && record_get_id(getSlot_by_Position(page, i)) == record_id) {
                        imprimeInfo(getSlot_by_Position(page, i));
                        flag = 1;
                    }
                }
            }
            dp_aux = dp_aux->next;
        }
    }

    if(flag == 0)printf("Registro não encontrad0!\n");
}

void search_by_name(int heap_id, char *name) {
    Heap_dictionary* hp_aux = HEAP;
    Directory_pages* dp_aux = NULL;
    Page* page;
    int i = 0, *bitmap, flag = 0;

    bitmap = (int*) malloc(get_num_slots() * sizeof(int));

    while(hp_aux != NULL && hp_aux->heap_id != heap_id) {
        hp_aux = hp_aux->next;
    }
    if(hp_aux != NULL) {
        dp_aux = hp_aux->header_page;
        while(dp_aux != NULL) {
            page = read_page_record(hp_aux->heap_id, dp_aux->page_id);
            if(page != NULL) {
                bitmap = get_bitmap(page);
                for(i=0; i < get_num_slots(); i++) {
                    if(bitmap[i] == 1 && !strcmp(record_get_name(getSlot_by_Position(page, i)), name)) {
                        imprimeInfo(getSlot_by_Position(page, i));
                        flag = 1;
                    }
                }
            }
            dp_aux = dp_aux->next;
        }
    }

    if(flag == 0)printf("Registro não encontrad0!\n");
}

void search_by_rid(int heap_id, int page_id, int record_id) {
    Heap_dictionary* hp_aux = HEAP;
    Directory_pages* dp_aux = NULL;
    Page* page;
    int i = 0, *bitmap, flag = 0;

    bitmap = (int*) malloc(get_num_slots() * sizeof(int));

    while(hp_aux != NULL && hp_aux->heap_id != heap_id) {
        hp_aux = hp_aux->next;
    }
    if(hp_aux != NULL) {
        dp_aux = hp_aux->header_page;
        while(dp_aux != NULL) {
            page = read_page_record(hp_aux->heap_id, dp_aux->page_id);
            if(page != NULL && get_id(page) == page_id) {
                bitmap = get_bitmap(page);
                if(bitmap[record_id] == 1) {
                    imprimeInfo(getSlot_by_Position(page, record_id));
                    flag = 1;
                }

            }
            dp_aux = dp_aux->next;
        }
    }

    if(flag == 0)printf("Registro não encontrad0!\n");
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

int cr8_page_directory_load(int heap_id, int page_id, int disk_block,int free_space) {
    Heap_dictionary* aux = HEAP;
    Directory_pages* new_page = (Directory_pages*)malloc(sizeof(Directory_pages));
    Directory_pages* pg_aux = NULL;

    while(aux != NULL) {
        if(heap_id == aux->heap_id) {
            new_page->page_id = page_id;
            new_page->disk_block = disk_block;
            new_page->free_space = free_space;
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

    fm_load();

}

int fm_save()
{
    int count,aux;

    FILE *arq_heap = fopen(ARQUIVO_HEAP,"w");
    FILE *arq_directory = fopen(ARQUIVO_DIRECTORY,"w");

    Directory_pages *directory_aux;

    Heap_dictionary *heap_aux = HEAP;
    while(heap_aux!=NULL)
    {
        count = 0;

        directory_aux = heap_aux->header_page;
        while( directory_aux != NULL )
        {

            //printf("he");
            //printf("%d",directory_aux->disk_block);

            fprintf(arq_directory,"%d\n",directory_aux->page_id);
            fprintf(arq_directory,"%d\n",directory_aux->disk_block);
            fprintf(arq_directory,"%d\n",directory_aux->free_space);

            count++;
            directory_aux = directory_aux->next;
        }
        aux = heap_aux->heap_id;
        fprintf(arq_heap,"%d\n",aux);
        fprintf(arq_heap,"%d\n",count);

        heap_aux = heap_aux->next ;
    }

    fclose(arq_directory);
    fclose(arq_heap);
    return 1;
}

int cr8_heapfile_load(int hp_id) {

    Heap_dictionary* hp_aux = HEAP;
    Heap_dictionary* new_heap = (Heap_dictionary*)malloc(sizeof(Heap_dictionary));

    if(hp_id<MAX_HEAP_ID_SIZE && used_heaps_ids[hp_id]!=1)
    {
        new_heap->heap_id = hp_id;
        new_heap->header_page = NULL;
        new_heap->next = NULL;
    }
    else
        return -1;

    if(hp_aux == NULL) {
        HEAP = new_heap;
        return hp_id;
    } else {
        while(hp_aux->next != NULL) {
            hp_aux = hp_aux->next;
        }

        hp_aux->next = new_heap;
        return hp_id;
    }

    return -1;
}


int fm_load() {

    int count, i, aux,hp_id = 0,a;
    int pg_id,disk_block,free_space;

    FILE *arq_heap = fopen(ARQUIVO_HEAP,"r");
    FILE *arq_directory = fopen(ARQUIVO_DIRECTORY,"r");

    Directory_pages *directory_aux;

    fscanf(arq_heap,"%d",&hp_id);

    while(!feof(arq_heap))
    {
        fscanf(arq_heap,"%d",&count);
        cr8_heapfile_load(hp_id);
        for(i=0;i<count;i++)
        {
            fscanf(arq_directory,"%d",&pg_id);
            fscanf(arq_directory,"%d",&disk_block);
            fscanf(arq_directory,"%d",&free_space);

            printf("%d",pg_id);
            scanf("%d",&a);

            cr8_page_directory_load(hp_id,pg_id,disk_block,free_space);

        }

        hp_id++;
        fscanf(arq_heap,"%d",&hp_id);
    }
    fclose(arq_directory);
    fclose(arq_heap);

    return 1;
}

int get_free_page(int heap_id, int *disk_block, int *page_new) {
    Heap_dictionary* hp_aux = HEAP;
    Directory_pages* pg_aux = NULL;
    int page_id_aux;

    while(hp_aux != NULL) {
        if(heap_id == hp_aux->heap_id) {
            pg_aux = hp_aux->header_page;

            while(pg_aux != NULL) {
                if(pg_aux->free_space > 0) {
                    *disk_block = pg_aux->disk_block;
                    *page_new = 0;
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
            pg_aux = hp_aux->header_page;

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


