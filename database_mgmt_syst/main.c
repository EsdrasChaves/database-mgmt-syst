#include <stdio.h>
#include <stdlib.h>
#include "file_manager.h"
#include "buffer_manager.h"
#include "disk_space_manager.h"

void init() {
    bf_init();
    initHeap();
    newDiskSpaceManager();
}

void close() {
    bufferpool_persist();
    dsm_save();
    ///fm_save();
}

int main()
{
   /// printf("INICIALIZANDO DB...\n\n");
   /// init();


  ///  int heap = cr8_heapfile();
  ///  printf("NEW HEAP ID: %d\n", heap);
///
  ///  heap = cr8_heapfile();
    //printf("NEW HEAP ID: %d\n", heap);
/*
    heap = cr8_heapfile();
    printf("NEW HEAP ID: %d\n", heap);

    heap = cr8_heapfile();
    printf("NEW HEAP ID: %d\n", heap);

    heap = cr8_heapfile();
    printf("NEW HEAP ID: %d\n", heap);

    heap = cr8_heapfile();
    printf("NEW HEAP ID: %d\n", heap);

    heap = cr8_heapfile();
    printf("NEW HEAP ID: %d\n", heap);

    heap = cr8_heapfile();
    printf("NEW HEAP ID: %d\n", heap);

    heap = cr8_heapfile();
    printf("NEW HEAP ID: %d\n", heap);

    heap = cr8_heapfile();
    printf("NEW HEAP ID: %d\n", heap);

    heap = cr8_heapfile();
    printf("NEW HEAP ID: %d\n", heap);

    heap = cr8_heapfile();
    printf("NEW HEAP ID: %d\n", heap);
*/

    ///Record *rcd, *rcd1;
   /// rcd = recordNew();
  ///  insereInfo(rcd, 1, "Joao");
   /// cr8_record(rcd, 0);

 ///   printMem();

 ///   rcd = recordNew();
   /// insereInfo(rcd, 2, "Pedro");
  ///  cr8_record(rcd, 1);

  ///  printMem();

    //printMem();
    //Page* page = read_block(0);
    //imprime_bitmap(page);

    //rcd = recordNew();
    //insereInfo(rcd, 2, "Joao");
    //cr8_record(rcd, 1);



    //printMem();

   /// dsm_save();
    //Page* page = read_block(0);
    //imprime_bitmap(page);

    /*rcd = recordNew();
    insereInfo(rcd, 2, "Marcio");
    cr8_record(rcd, 1);

    rcd1 = recordNew();
    insereInfo(rcd1, 3, "Esdras");
    cr8_record(rcd1, 2);

     rcd1 = recordNew();
    insereInfo(rcd1, 4, "Guto");
    cr8_record(rcd1, 3);

    rcd1 = recordNew();
    insereInfo(rcd1, 5, "Bruno");
    cr8_record(rcd1, 4);

    rcd1 = recordNew();
    insereInfo(rcd1, 6, "Joao");
    cr8_record(rcd1, 5);

    rcd1 = recordNew();
    insereInfo(rcd1, 7, "MAteus");
    cr8_record(rcd1, 6);

    rcd1 = recordNew();
    insereInfo(rcd1, 8, "Pedro");
    cr8_record(rcd1, 7);

    rcd1 = recordNew();
    insereInfo(rcd1, 9, "Lucas");
    cr8_record(rcd1, 8);

    rcd1 = recordNew();
    insereInfo(rcd1, 10, "Jonas");
    cr8_record(rcd1, 9);

    dsm_save();

    printMem();
    printFile();

    rcd1 = recordNew();
    insereInfo(rcd1, 11, "Bolsonaro");
    cr8_record(rcd1, 10);

    printMem();*/

 /*   Record *rcd, *rcd2;
    Page *pg;

    rcd = recordNew();
    rcd2 = recordNew();
    pg = pageNew();

    insereInfo(rcd, 15, "Hello");
    insereInfo(rcd2, 16, "MarcioBixa");

    inst_record(pg,rcd);
    inst_record(pg,rcd2);
    imprime_bitmap(pg);

    remove_record(pg,0);
    imprime_bitmap(pg);
*/


 /*   Record *rcd, *rcd2;
    Page *pg;

    rcd = recordNew();
    rcd2 = recordNew();
    pg = pageNew();

    insereInfo(rcd, 15, "Hello");
    insereInfo(rcd2, 16, "MarcioBixa");

    inst_record(pg,rcd);
    inst_record(pg,rcd2);
    imprime_bitmap(pg);

    remove_record(pg,0);
    imprime_bitmap(pg);
*/

/// close();

    return 0;
}
