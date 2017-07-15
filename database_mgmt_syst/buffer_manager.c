#include "buffer_manager.h"
#include <string.h>
//Array da memória
Page** primary_memory;

// Numero de paginas na memoria
#define MEM_SIZE 2
#include <stdlib.h>


int current_frame = 0;


typedef struct {
    int pin_count;
    int dirt_bit;
    Page** frame;       // Referencia para o frame das quais as informações se referem
    int disk_block;
    int referenced_bit;
} Frame_info;

struct buffer_manager{
    Frame_info* frame_info;       // Biblioteca de dados do Buffer Manager
    Page** memory_ref;       	  // Referencia ao primeiro endereço da memória
    //Frame_info* current_frame;    // Referência ao frame corrente (para política de clock)
};


// Aloca os espaços para o buffer manager
int  bf_init(/*int num_frames*/) {
    int i;

    primary_memory = (Page**) malloc(MEM_SIZE * sizeof(Page*));
    for(i=0; i < MEM_SIZE; i++)
        primary_memory[i] = NULL;

    bufferpool_manager = (Buffer_manager*) malloc(sizeof(Buffer_manager));
    bufferpool_manager->frame_info = (Frame_info*) malloc(MEM_SIZE*sizeof(Frame_info));

    bufferpool_manager->memory_ref = primary_memory;
    //bufferpool_manager->current_frame = bufferpool_manager->frame_info;

    for(i=0; i < MEM_SIZE; i++){
        (bufferpool_manager->frame_info)[i].pin_count = 0;
        (bufferpool_manager->frame_info)[i].dirt_bit = 0;
        (bufferpool_manager->frame_info)[i].referenced_bit = 0;
        (bufferpool_manager->frame_info)[i].frame = &(bufferpool_manager->memory_ref[i]);
        (bufferpool_manager->frame_info)[i].disk_block = -1;
    }

    return 1;
}

void free_bpm() {
    free(primary_memory);
    free(bufferpool_manager->frame_info);
    free(bufferpool_manager);
}

void printMem() {
    int i;
    for(i=0; i<MEM_SIZE; i++) {
        if(*(bufferpool_manager->frame_info[i]).frame != NULL) {
            printf("FRAME %d:\n", i);
            imprime_bitmap(*(bufferpool_manager->frame_info[i].frame));
        }
        printf("\n");
    //imprime_bitmap(primary_memory[0]);
    //imprime_bitmap(primary_memory[1]);
    }
}

// Política de substituição
Frame_info* clock_policies() {
    int currentAux;

    currentAux = current_frame;

    do {
        if(current_frame == MEM_SIZE)
            current_frame = 0;
        if(bufferpool_manager->frame_info[current_frame].pin_count == 0 && bufferpool_manager->frame_info[current_frame].referenced_bit == 0) {
            if(bufferpool_manager->frame_info[current_frame].dirt_bit != 0)
                save_page(&(bufferpool_manager->frame_info[current_frame]));
            return &(bufferpool_manager->frame_info[current_frame]);
        } else if(bufferpool_manager->frame_info[current_frame].referenced_bit == 1) {
            bufferpool_manager->frame_info[current_frame].referenced_bit = 0;
        }
        current_frame++;


    }while(currentAux != current_frame);

    return NULL;
}


// Verifica se há espaço na memoria e chama a política de substituição se necessário
Frame_info* check_memory_space() {
    int i;
    Frame_info* frame = NULL;
    for(i=0; i < MEM_SIZE; i++) {
        if(*((bufferpool_manager->frame_info)[i].frame) == NULL){
            return &(bufferpool_manager->frame_info[i]);
        }
    }
    frame = clock_policies();
    if(frame ==  NULL){
        printf("Não é possível adicionar mais páginas");
        return NULL;
    } else {
        return frame;
    }
}


// Verifica se a página ja está na memória, senão ela faz o fetch (Página retornada pelo free_page())
Frame_info* check_page(int heap_id, int page_id) {
    int i, page_id_gtn, page_disk_block, index_page_free = -1, page_new = 1;
    Frame_info* frameAux;
    Page* page = NULL;


    if(page_id == -1){
        page_id = get_free_page(heap_id, &page_disk_block, &page_new);
        if(page_new == 1)
            page = pageNew(page_id, heap_id);
    }else
        page_disk_block = fetch_page(heap_id, page_id);


    for(i=0; i < MEM_SIZE; i++) {
        if(*(bufferpool_manager->frame_info[i]).frame != NULL) {
            page_id_gtn = get_id((*(bufferpool_manager->frame_info[i].frame)));
            if(page_id_gtn == page_id) {
                return &(bufferpool_manager->frame_info[i]);
            }
        } /*else {
            index_page_free = i;
        }*/
    }

    if(index_page_free == -1)
        frameAux = check_memory_space();
    else
        frameAux = &(bufferpool_manager->frame_info[index_page_free]);
    if(frameAux != NULL) {
        add_page_memory(frameAux, page_disk_block, page);
        return frameAux;
    } else {
        return NULL;
    }
}


// Inserção de um novo registro na página
int cr8_record(Record* rcd, int heap_id) {
    Frame_info* frameAux;

    frameAux = check_page(heap_id, -1);

    if(frameAux != NULL) {
        inst_record(*(frameAux->frame), rcd);
        update_free_space(heap_id, get_id(*(frameAux->frame)), -1);                     // AKI PODE TER UM ERRO
    }
    else
        return 0;

    frameAux->dirt_bit = 1;
    frameAux->referenced_bit = 0;
    frameAux->pin_count = 0;



    return 1;
}

// Adiciona página na memoria a partir do seu endereço. Atraves de métodos do dsm
int add_page_memory(Frame_info* frameAux, int disk_block, Page* page) {
    // Método de buscar na memória
    if(page == NULL)
        page = read_block(disk_block);

    if(frameAux != NULL){
        *(frameAux->frame) = page;
        frameAux->disk_block = disk_block;
        return 1;
    }else {
        return 0;
    }
}


// Salva página no disco (métodos do dsm) e atualiza dados do frame_info
int save_page(Frame_info* frameAux) {
    if(*(frameAux->frame) == NULL)
        return 0;

    write_block((*(frameAux->frame)), (frameAux->disk_block));

    frameAux->dirt_bit = 0;
    frameAux->disk_block = -1;
    frameAux->pin_count = 0;


    free(*(frameAux->frame));
    *(frameAux->frame) = NULL;

    return 1;
}


// Remove registro (é necessário busca-lo antes no req_record) e chama atualizaçao do file manager
int rmv_record(int heap_file_id, int page_id, int chave) {
    Frame_info* frameAux;
    int num_slot;

    frameAux = check_page(heap_file_id, page_id);

    if(frameAux != NULL) {
        num_slot = req_record(*(frameAux->frame), chave);
        if(num_slot != -1) {
            remove_record(*(frameAux->frame), num_slot);
            frameAux->dirt_bit = 1;
            update_free_space(heap_file_id, page_id, 1);
            return 1;
        } else
            return 0;
    }
    return 0;
}

int read_page(int heap_file_id, int page_id) {
    Frame_info* frameAux;

    frameAux = check_page(heap_file_id, page_id);

    if(frameAux != NULL) {
        imprimeRecords(*(frameAux->frame));
        return 1;
    }

    imprime_bitmap(frameAux->frame);
    return 0;
}


Page* read_page_record(int heap_id, int page_id) {
    Frame_info* frameAux;

    frameAux = check_page(heap_id, page_id);

    if(frameAux != NULL) {
        return *(frameAux->frame);
    }

    return NULL;
}

int update_record(int heap_file_id, int page_id, int chave, Record* record) {
    Frame_info* frameAux;
    int num_slot;

    frameAux = check_page(heap_file_id, page_id);

    if(frameAux != NULL) {
        num_slot = req_record(*(frameAux->frame), chave);
        if(num_slot != -1) {
            remove_record(*(frameAux->frame), num_slot);
            frameAux->dirt_bit = 1;
            inst_record(*(frameAux->frame), record);
            return 1;
        } else
            return 0;
    }
    return 0;
}


// Atualiza registro é necessário buscar para ver a diferença
//int update_record(int heap_file_id, int page_id, int num_slot, Record record);


// Retorna o frame que possui a mesma página de um bloco
int num_frame(int disk_block) {
    int i;

    for(i=0; i < MEM_SIZE; i++) {
        if(bufferpool_manager->frame_info[i].disk_block == disk_block)
            return i;
    }

    return -1;
}


// Atualiza dirt bit



// Atualiza pin count
void update_pin_count(int num_frame, int num) {
    bufferpool_manager->frame_info[num_frame].pin_count += num;
    if(bufferpool_manager->frame_info[num_frame].pin_count == 0) {
        bufferpool_manager->frame_info[num_frame].referenced_bit = 1;
    }
}


// Retorna numero do slot do registro buscado sabendo a página
int req_record(Page* page, int chave) {
    return num_slot_key(page, chave);
}


void bufferpool_persist () {
    int i;
    for(i=0 ; i < MEM_SIZE; i++)
        save_page(&(bufferpool_manager->frame_info[i]));
    free_bpm();
}

// Função de retornar registro;

int delete_page(int page_id) {
    int i;

    for(i=0; i < MEM_SIZE; i++) {
        if(page_id == get_id((*(bufferpool_manager->frame_info[i].frame)))) {
            free(&(bufferpool_manager->frame_info[i]));
            *((bufferpool_manager->frame_info)[i].frame) = NULL;
            return 1;
        }
    }
    return 0;
}
