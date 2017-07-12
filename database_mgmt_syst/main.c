#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_manager.h"
#include "buffer_manager.h"
#include "disk_space_manager.h"

void init() {
    initHeap();
    bf_init();
    newDiskSpaceManager();
}


void imprime_boasVindas() {
    printf(" ---------------------------------------------------------\n");
    printf("|\tSistema de gerenciamento de banco de dados        |\n");
    printf("|\t\t\t\t\t\t\t  |\n");
    printf("|\t\t\t\t\t\t\t  |\n");
    printf("|\tDesenvolvido por:\t\t\t\t  |\n");
    printf("|\t\t\t\t\t\t\t  |\n");
    printf("|\tEsdras de Lima Chaves: 11511BCC015                |\n");
    printf("|\tJose Augusto Bolina: 11511BCC048                  |\n");
    printf("|\tMarcio Antonio de Freitas Junior: 11511BCC026     |\n");
    printf("|\tTiago Pereira de Faria: 11511BCC001               |\n");
    printf("|\t\t\t\t\t\t\t  |\n");
    printf(" ---------------------------------------------------------\n");

    printf("\nOBS: O sistema esta com um problema para recuperar alguns dados dos arquivos\n");
    printf("\nPortanto, antes de iniciar a aplicação novamente deve-se apagar o conteúdo\ndo arquivo \"DSM-DADOS.bin\" E substituir os valores \'0\' para \'1\' no arquivo \n\"DiskSpaceManager.txt\" ");

    printf("\n\n\n\tPressione enter tecla para continuar...\n");
}

void imprime_menu() {
    printf("Escolha uma opção:\n\n");
    printf("1 - Criar nova tabela\n");
    printf("2 - Inserir registro na tabela\n");
    printf("3 - Remover registro da tabela\n");
    printf("4 - Atualizar registro\n");
    ///printf("5 - Buscar registro pelo \n");
    printf("5 - Imprimir paginas presentes na memoria\n");
    printf("0 - Sair\n");
    printf("\n\nEscolha: ");

}

int main(){


    int escolha = -1, heap, id, page_id, id_aux;
    char nome[10];
    Record* rcd;

    imprime_boasVindas();

    getchar();
    system("clear");

    init();

    while(escolha != 0) {
        imprime_menu();
        scanf("%d", &escolha);

        switch(escolha) {
            case 0:
                system("clear");
                printFile();
                printf("Pressione qualquer enter para sair\n");
                getchar();
                printf("Zegzo");
                bufferpool_persist();
                dsm_save();
                printf("ola");
                fm_save();
                break;
            case 1:
                heap = cr8_heapfile();
                system("clear");
                printf("Tabela Criada\nID = %d\n", heap);
                getchar();
                getchar();
                system("clear");
                break;
            case 2:
                system("clear");
                printf("Qual o id da tabela em que deseja inserir? ");
                scanf("%d", &heap);
                printf("Digite o id do registro: ");
                scanf("%d", &id);
                printf("Digite um nome de ate 10 caracteres: ");
                getchar();
                fflush(stdin);
                fgets(nome, 10, stdin);
                rcd = recordNew();
                insereInfo(rcd, id, nome);
                cr8_record(rcd, heap);
                printf("Inserido com sucesso...\n");
                getchar();
                system("clear");
                break;
            case 3:
                system("clear");
                printf("Qual o id da tabela em que deseja remover? ");
                scanf("%d", &heap);
                printf("Digite a chave do registro: ");
                scanf("%d", &id);
                printf("Digite o id da pagina: ");
                scanf("%d", &page_id);
                rmv_record(heap, page_id, id);
                printf("Removido com sucesso...\n");
                getchar();
                system("clear");
                break;
            case 4:
                system("clear");
                printf("Qual o id da tabela em que deseja atualizar? ");
                scanf("%d", &heap);
                printf("Digite a chave do registro: ");
                scanf("%d", &id_aux);
                printf("Digite o id da pagina: ");
                scanf("%d", &page_id);
                printf("Digite o id do registro: ");
                scanf("%d", &id);
                printf("Digite um nome de ate 10 caracteres: ");
                getchar();
                fflush(stdin);
                fgets(nome, 10, stdin);
                rcd = recordNew();
                insereInfo(rcd, id, nome);
                update_record(heap, page_id, id_aux, rcd);
                printf("Removido com sucesso...\n");
                getchar();
                system("clear");
                break;
            case 5:
                system("clear");
                printMem();
                getchar();
                getchar();
                break;
            default:
                printf("Opcao invalida\n");
        }
    }




    return 0;
}
