#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    fm_save();
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

    printf("\n\n\n\tPressione enter tecla para continuar...\n");
}

void imprime_menu() {
    printf("Escolha uma opção:\n\n");
    printf("1 - Criar nova tabela\n");
    printf("2 - Inserir registro na tabela\n");
    printf("3 - Remover registro da tabela\n");
    printf("4 - Atualizar registro\n");
    printf("5 - Buscar registro pelo ID\n");
    printf("6 - Buscar registro pelo Nome\n");
    printf("7 - Buscar registro pelo RID\n");
    printf("8 - Imprimir paginas presentes na memoria\n");
    printf("9 - Scan Completo\n");
    printf("10 - Deletar tabela\n");
    printf("0 - Sair\n");
    printf("\n\nEscolha: ");

}

int main()
{


    int escolha = -1, heap, id, page_id, record_id, id_aux;
    char nome[10];
    Record* rcd;

    imprime_boasVindas();

    getchar();
    system("clear");

    init();

    while(escolha != 0) {
        system("clear");
        imprime_menu();
        scanf("%d", &escolha);
        system("clear");
        switch(escolha) {
            case 0:
                close();
                break;
            case 1:
                heap = cr8_heapfile();
                printf("Tabela Criada\nID = %d\n", heap);
                getchar();
                getchar();
                break;
            case 2:
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
                break;
            case 3:
                printf("Qual o id da tabela em que deseja remover? ");
                scanf("%d", &heap);
                printf("Digite a chave do registro: ");
                scanf("%d", &id);
                printf("Digite o id da pagina: ");
                scanf("%d", &page_id);
                rmv_record(heap, page_id, id);
                printf("Removido com sucesso...\n");
                getchar();
                getchar();
                break;
            case 4:
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
                printf("Atualizado com sucesso...\n");
                getchar();
                break;
            case 5:
                printf("Digite o Heap ID da tabela: ");
                scanf("%d", &heap);
                printf("Digite o ID da tupla: ");
                scanf("%d", &record_id);
                search_by_id(heap, record_id);
                getchar();
                getchar();
                break;
            case 6:
                printf("Digite o Heap ID da tabela: ");
                scanf("%d", &heap);
                getchar();
                printf("Digite o nome da tupla: ");
                fgets(nome, 10, stdin);
                search_by_name(heap, nome);
                getchar();
                break;
            case 7:
                printf("Digite o Heap ID da tabela: ");
                scanf("%d", &heap);
                printf("Digite o ID da pagina: ");
                scanf("%d", &page_id);
                printf("Digite o ID da tupla: ");
                scanf("%d", &record_id);
                search_by_rid(heap, page_id, record_id);
                getchar();
                getchar();
                break;
            case 8:
                printMem();
                getchar();
                getchar();
                break;
            case 9:
                printFile();
                getchar();
                getchar();
                break;
            case 10:
                printf("Digite o Heap ID da tabela: ");
                scanf("%d", &heap);
                delete_table(heap);
                printf("Chama\n");
                getchar();
            default:
                printf("Opcao invalida\n");
                getchar();
        }
    }




    return 0;
}
