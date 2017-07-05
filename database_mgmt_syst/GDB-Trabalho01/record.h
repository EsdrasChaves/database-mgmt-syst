

typedef struct record Record;


Record *recordNew();
void insereInfo(Record *rcd, int id, char name[10]);
void imprimeInfo(Record *rcd);
int get_size_record();
