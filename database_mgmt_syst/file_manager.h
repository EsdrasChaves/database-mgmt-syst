#include <stdio.h>
#include <stdlib.h>

#define MAX_PAGE_ID_SIZE    100
#define MAX_HEAP_ID_SIZE    100

typedef struct directory_pages Directory_pages;
typedef struct heap_dictionary Heap_dictionary;

int* used_pages_ids;
int* used_heaps_ids;
Heap_dictionary* HEAP;

int initHeap();

/// Atualiza numero de slots livres
int update_free_space(int heap_id, int page_id, int num_update);

/// Criar nova tabela
int cr8_heapfile();

/// Carrega informações do file manager
int fm_load(void);

/* Retorna o id da página com espaço livre ou avisa que nao existe.
 * Além de atualizar free_space etc. Considera que ela é chamada somente
 * quando for escrever
 */
int get_free_page(int heap_id, int *disk_block);


/* Pede para o dsm alocar espaço e retorna o id para a função freepage() chama
 * função de adicionar página no heapfile e chama cr8 page directory
 */
int cr8_page(int heap_id, int *disk_block);


/// Coloca a nova página no heapfile
int cr8_page_directory(int heap_id, int page_id, int disk_block);

/// Retorna o endereço da página buscada
int fetch_page(int heap_id, int page_id);

/*
1 - Implementar o update la
2 - o cr8_page_directory q tem que gerar os ids das páginas pra
cada heap (entao nao tem como passa o page_id como parametro na função la)
3 - Implementar a persistencia
*/
