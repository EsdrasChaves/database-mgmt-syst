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
    dsm_load();

}

void free_dsm() {
    free(diskSpaceManager->blocks_info);
    free(diskSpaceManager);
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
    free_dsm();
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


int write_block(Page* page, int disk_block){
    FILE *f;
    f = fopen(ARQUIVO_DADOS, "r+b");
    fseek(f,(6*sizeof(int)+4*get_size_record())*disk_block,0);

    int page_id, heap_id, *bitmap;
    page_id = get_id(page);
    heap_id = get_heap_id(page);
    bitmap = get_bitmap(page);

    fwrite(&page_id, sizeof(int),1,f);
    fwrite(&heap_id, sizeof(int),1,f);
    fwrite(bitmap, sizeof(int), 4, f);

    int i;
    struct record **aux_rc;
    aux_rc = get_record(page);
    for(i=0;i<4;i++){
        if(aux_rc[i] != NULL){
            fwrite(aux_rc[i],get_size_record(),1,f);
        }
        else{
            fseek(f,get_size_record(),SEEK_CUR);
        }
    }
    fclose(f);
    return 1;
}

Page* read_block(int disk_block){
    FILE *f;
    Page* page;
    Record* rc;
    Record** rec;
    f = fopen(ARQUIVO_DADOS,"rb");

    int page_id, heap_id, *bitmap, i;
    bitmap = (int*) malloc(4*sizeof(int));

    //if(diskSpaceManager->blocks_info[disk_block].is_free == 0) {
        fseek(f,(6*sizeof(int)+4*get_size_record())*disk_block,SEEK_SET);

        fread(&page_id, sizeof(int), 1, f);
        fread(&heap_id, sizeof(int), 1, f);
        fread(bitmap, sizeof(int), 4, f);
        page = pageNew(page_id, heap_id);

     /*else {
        page = pageNew();
        bitmap = get_bitmap(page);
        diskSpaceManager->blocks_info[disk_block].is_free = 0;
        return page;
    }*/
    for(i=0;i<4;i++){
        if(bitmap[i]==1){
            bitmap[i]=0;
            rc = recordNew();
            fread(rc,get_size_record(),1,f);
            inst_record(page,rc);
        }
        else{
            fseek(f,get_size_record(),SEEK_CUR);
        }
    }

    fclose(f);

    return page;
}

int free_block(int block_address){
    diskSpaceManager->blocks_info[block_address].is_free = 1;
}
