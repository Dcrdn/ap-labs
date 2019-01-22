#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(){
	char c;
	FILE *file;
	FILE *newFile;
	char line[256];
	bool imprimir;
	bool comentarioLargo;
	bool comentario;

	char fileName[20];

	printf("Enter the name of the file of which you want to delete the comments:\n");
	scanf("%s",fileName);

	comentario=false;
	comentarioLargo=false;
	imprimir=true;
	file=fopen(fileName, "r");
	newFile=fopen("copy.c","w");
	
	if(file && newFile){
		while(fgets(line,sizeof(line),file)){
			int i;
			for(i=0;i<strlen(line);i++){
				if(line[i]=='"'){
					comentario=!comentario;
				}
				if(line[i]=='/' && line[i+1]=='/'){
					if(comentario){
						imprimir=true;	
					}
					else{
						imprimir=false;	
					}
				}
				else if(line[i]=='/' && line[i+1]=='*'){
					imprimir=false;
					comentarioLargo=true;
				}
				else if(line[i]=='/' && line[i-1]=='*'){
					imprimir=true;
					i++;
					comentarioLargo=false;
				}
				if(imprimir){
					fprintf(newFile,"%c", line[i]);
				}

			}
			if(!comentarioLargo){
				imprimir=true;				
			}
		}
		fclose(file);
		fclose(newFile);
		int status;
		status=remove(fileName);
		if(status==0){
			rename("copy.c",fileName);	
		}
		status=remove("copy.c");
		if(status){
			printf("Terminado\n");
		}
	}
	return 0;
}
