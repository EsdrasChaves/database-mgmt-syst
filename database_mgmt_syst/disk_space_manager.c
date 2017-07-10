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

/*Page* read_block(int disk_block){
    FILE *f;
    Page* page;
    page = pageNew();
    f = fopen(ARQUIVO_DADOS,"rb");
    fseek(f,get_size_page()*disk_block,SEEK_SET);
    fread(page,get_size_page(),1,f);
    fclose(f);

    return page;
}*/

Page* read_block(int disk_block){
    FILE *f;
    Page* page;
    Record* rc;
    Record** rec;
    page = pageNew();
    f = fopen(ARQUIVO_DADOS,"rb");

    fseek(f,(get_size_page()+4*get_size_record())*disk_block,SEEK_SET);
    rec = getSlots(page);
    fread(page,get_size_page(),1,f);
    setSlots(rec,page);
    int i,*bitmap;
    bitmap = get_bitmap(page);
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

/*int write_block(Page* page, int disk_block){
    FILE *f;
    f = fopen(ARQUIVO_DADOS, "ab");
    fseek(f,get_size_page()*disk_block,SEEK_SET);
    fwrite(page, get_size_page(),1,f);
    fclose(f);
    return 1;

}*/

int write_block(Page* page, int disk_block){
    FILE *f;
    f = fopen(ARQUIVO_DADOS, "r+b");
    fseek(f,(get_size_page()+4*get_size_record())*disk_block,0);
    fwrite(page, get_size_page(),1,f);
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

/*int write_block(Page* page, int disk_block){
    FILE *f, *f1;
    Page* aux;

    f = fopen(ARQUIVO_DADOS, "rb");
    f1 = fopen("aux.bin", "wb");
    int j = 0, i;
    struct record **aux_rc;
    char c =
    while(!EOF) {
        if(j == disk_block) {
            fwrite(page, get_size_page(),1,f1);
            aux_rc = get_record(page);
            fseek(f,(get_size_page()+4*get_size_record())*(disk_block+1),0);
        } else {
            aux = read_block(j);
            fwrite(aux, get_size_page(),1,f1);
            aux_rc = get_record(aux);
        }
        for(i=0;i<4;i++){
            if(aux_rc[i] != NULL){
                fwrite(aux_rc[i],get_size_record(),1,f1);
            }
            else{
                fseek(f1,get_size_record(),SEEK_CUR);
            }
        }
        j++;
    }

    fclose(f);
    fclose(f1);

    remove("DSM-Dados.bin");
    rename("aux.bin", "DSM-Dados.bin");
    return 1;
}*/
int free_block(int block_address){
    diskSpaceManager->blocks_info[block_address].is_free = 1;
}
