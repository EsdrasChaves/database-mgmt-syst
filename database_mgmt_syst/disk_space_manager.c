#include <stdio.h>
#include <stdlib.h>
#include "disk_space_manager.h"


#define DISK_SPACE 100
#define ARQUIVO_INFO "DiskSpaceManager.txt"
#define ARQUIVO_DADOS "DSM-Dados.bin"

 struct blocks_info{
    int is_free;
    //int page_id;
};

struct disk_space_manager{
    Blocks_info* blocks_info;
};

void newDiskSpaceManager(){
    diskSpaceManager = malloc(sizeof(Disk_space_manager*));
    dsm_init(&diskSpaceManager,DISK_SPACE);
    //dsm_load();

}

int dsm_load(){
    int i,j;
    FILE *arquivo = fopen(ARQUIVO_INFO, "r");
    for(i=0;i<DISK_SPACE;i++){
        fscanf(arquivo,"%1d",&(diskSpaceManager->blocks_info[i]).is_free);
    }
    fclose(arquivo);
}

int dsm_save(){
    int i;
    FILE *arquivo = fopen(ARQUIVO_INFO, "w");
    for(i=0;i<DISK_SPACE;i++){
        fprintf(arquivo,"%d",diskSpaceManager->blocks_info[i].is_free);
    }
    fclose(arquivo);
}

int dsm_init(Disk_space_manager **disk,int num_blocks){
    int i;

    Blocks_info* novo = (Blocks_info*)malloc(num_blocks*sizeof(Blocks_info));
    (*disk)->blocks_info = novo;

    dsm_load();

    return 1;
}

int cr8_block(){
    int i;
    for(i=0;i<DISK_SPACE;i++){
        if(diskSpaceManager->blocks_info[i].is_free == 1){
            diskSpaceManager->blocks_info[i].is_free = 0;
            return i;
        }
    }
    return -1;
}

Page* read_block(int disk_block){
    FILE *f;
    Page* page;
    page = pageNew();
    f = fopen(ARQUIVO_DADOS,"rb");
    fseek(f,get_size_page()*disk_block,SEEK_SET);
    fread(page,get_size_page(),1,f);
    fclose(f);

    return page;
}

int write_block(Page* page, int disk_block){
    FILE *f;
    f = fopen(ARQUIVO_DADOS, "ab");
    fseek(f,get_size_page()*disk_block,SEEK_SET);
    fwrite(page, get_size_page(),1,f);
    fclose(f);
    return 1;
}
int free_block(int block_address){
    diskSpaceManager->blocks_info[block_address].is_free = 1;
}
