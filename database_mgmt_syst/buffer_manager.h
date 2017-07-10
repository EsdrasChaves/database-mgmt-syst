#include <stdio.h>
#include "disk_space_manager.h"
#include "page.h"
#include "file_manager.h"

typedef struct buffer_manager Buffer_manager;


Buffer_manager *bufferpool_manager;

// Aloca os espaços para o buffer manager
int  bf_init(/*int num_frames*/);


// Inserção de um novo registro na página
int cr8_record(Record* rcd, int heap_id);




// Adiciona página na memoria a partir do seu endereço. Atraves de métodos do dsm


// Salva página no disco (métodos do dsm) e atualiza dados do frame_info



// Remove registro (é necessário busca-lo antes no req_record) e chama atualizaçao do file manager
int rmv_record(int heap_file_id, int page_id, int chave);



int read_page(int heap_file_id, int page_id);

// Atualiza registro é necessário buscar para ver a diferença
int update_record(int heap_file_id, int page_id, int chave, Record* record);


// Retorna o frame que possui a mesma página de um bloco
int num_frame(int disk_block);


// Atualiza dirt bit
void update_dirty_bit(int num_frame, int status);


// Atualiza pin count
void update_pin_count(int num_frame, int num);


// Retorna numero do slot do registro buscado sabendo a página
int req_record_page(int heap_file_id, int page_id, int chave);


// Retorna numero do slot do registro buscado com a página desconhecida
int req_record(Page* page, int chave);

void bufferpool_persist ();
