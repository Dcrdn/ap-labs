#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define REPORT_FILE "report.txt"

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./dmesg-analizer.o logfile.txt\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

int countLines(char *fileName){
    FILE *file;
    file=fopen(fileName, "r");
    char line[256];
    int lineas=0;
    if(file){
        while(fgets(line, sizeof(line),file)){
            lineas++;
        }
    }
    fclose(file);
    printf("\n %d \n", lineas);
    return lineas;
}

struct logs{
    char type[30];
    char content[256];
    int visto;
};

struct logs analizarLinea(char *line, int size){
    int i;
    char type[30];

    int startPos=0;
    int finalPos=0;
    
    for(i=1;i<size-1;i++){
        if(line[i]==' ' && line[i-1]==']'){
            startPos=i+1;
        }
        else if(line[i]==':' && line[i+1]==' '){
            finalPos=i;
            break;
        }
    }
    if(finalPos==0){
        struct logs dato;
        char name[]="General:";
        strcpy(dato.type, name);
        strcpy(dato.content, line);
        dato.visto=0;
        return dato;
    }else{
        int sizeCategoria=finalPos-startPos;
        char categoria[sizeCategoria];
        int sizeDescripcion=size-sizeCategoria;
        char descripcion[256];
        int j=0;
        for(i=startPos;i<finalPos+1;i++){
            categoria[j]=line[i];
            j++;
        }
        //categoria[j]='\0';
        int k=0;
        for(i=0;i<startPos;i++){
            descripcion[k]=line[i];
            k++;
        }
        for(j=finalPos+1;j<size;j++){
            descripcion[k]=line[j];
            k++;
        }    
        descripcion[k]='\0';
        struct logs dato;
        char name[]="cosa sucia";
        strcpy(dato.type, categoria);
        strcpy(dato.content, descripcion);
        dato.visto=0;
        return dato;
    }

}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int totalLines=countLines(logFile);
    struct logs arr_logs[totalLines];
    FILE *file;
    file=fopen(logFile, "r");
    char line[256];
    int pos=0;
    if(file){
        while(fgets(line, sizeof(line),file)){
            struct logs temp;
            temp = analizarLinea(line, strlen(line));
            arr_logs[pos]=temp;
            pos++;
        }
    }
    fclose(file);
    int i;
    int j;
    FILE *fp;
    fp=fopen(report, "w+");

    for(i=0;i<totalLines;i++){
        if(arr_logs[i].visto==0){            
            printf("%s\n", arr_logs[i].type);
            fprintf(fp, "%s\n", arr_logs[i].type);

            for(j=i;j<totalLines;j++){
                if(strcmp(arr_logs[i].type, arr_logs[j].type)==0){
                    printf("        %s\n", arr_logs[j].content);
                    fprintf(fp, "          %s\n", arr_logs[j].content);

                    arr_logs[j].visto=1;
                }
            }
        }
    }
    fclose(fp);
    printf("Report is generated at: [%s]\n", report);
}
