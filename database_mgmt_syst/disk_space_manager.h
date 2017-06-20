#include "page.h"

typedef struct blocks_info Blocks_info;
typedef struct disk_space_manager Disk_space_manager;

Disk_space_manager *diskSpaceManager;



//Instancia o Disk_space_manager
void newDiskSpaceManager();


// Carrega informaçoes do disk manager
int dsm_load();

// Salva informaçoes do disk manager
int dsm_save();

// Aloca os blocos do disk manager
int dsm_init();


// Retorna endereço do novo bloco e atualiza informações no blocks_info
int cr8_block();

// Realiza a leitura a partir de o endereço
Page* read_block(int disk_block);

// Realiza a persistência dos dados da página no bloco
int write_block(Page* page, int disk_block);

// Libera o espaço do block para outra página
int free_block(int block_address);

