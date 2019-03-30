#include <stdio.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define INFO_LENGTH 20

char *header = "|%-7s | %-6s | %-42s | %-13s | %-9s | %-5s | %-11s|\n",
	 *headDiv =	"|--------|--------|--------------------------------------------|-------------|-------------|------------|------------|\n",
	 *tableFormat = "| %-5s | %-6s | %-40s | %-10s | %-9.1fM | %-9s | %-10d|\n";

void clear();

struct process {
	char name[INFO_LENGTH],
	state[INFO_LENGTH],
	pid[INFO_LENGTH],
	ppid[INFO_LENGTH],
	memory[INFO_LENGTH],
	threads[INFO_LENGTH];
	int openFiles;
};

struct process processes[100000];


int printProcesses() {
	int pos = 0;
	printf(header, " PID", "Parent", "Name", "State", "Memory", "# Threads", "Open Files");
	printf("%s\n", headDiv);

	while(processes[pos].name[0] != '\0') {
		printf(tableFormat, processes[pos].pid, processes[pos].ppid, processes[pos].name, processes[pos].state, atof(processes[pos].memory) / 1000, processes[pos].threads, processes[pos].openFiles);
		pos++;
	}
	return 0;
}


int numberOfPs(){
  int num=0;

    DIR *d = opendir("/proc/");
    struct dirent *ep;  
    if (d != NULL)
      {
        while (ep = readdir (d)){

          if(isdigit(ep->d_name[0])){
            num+=1;
          }
        }
        (void) closedir (d);
      }
      else{
        printf("error\n");
        num-=1;
      }
  return num;
}

struct pid{
  int value;
};


void getInfo(){
  int size=numberOfPs();
  struct pid pids[size]; 
  int pos=0;
  int k=0;
  int files=0;

  DIR *d = opendir("/proc/");
	char path[30], fdpath[30];
       	strcpy(path, "/proc/");
         strcpy(fdpath, "/proc/");
FILE *fp;
    struct dirent *ep;  
    if (d != NULL)
      {

        while (ep = readdir (d)){
          if(isdigit(ep->d_name[0])){
            files = 0;

            pids[pos].value=atoi(ep->d_name);
            pos+=1;

            strcat(path, ep->d_name);
            strcat(path, "/status");

            strcat(fdpath, ep->d_name);
		      	strcat(fdpath, "/fd");

            fp = fopen(path, "r");
            char buffer[255];
//Name, State, Pid, PPid, memory (VmRSS), Threads

            while(fgets(buffer, 255, (FILE*) fp)) {
                int i;
                if(buffer[0]=='N'){
                  if(buffer[1]=='a' && buffer[2]=='m' && buffer[3]=='e'){
                    int i;
                    char nombre[20];
                    int posN=0; 
                    int terminar=0;   
                    for(i=5;i<255;i++){
                      if(buffer[i]=='\n'){
                        break;
                      }
                      if(buffer[i]==' '){
                        if(terminar==1){
                          nombre[posN]='\0';
                          break;
                        }
                        continue;
                      }

                      nombre[posN]=buffer[i];
                      posN+=1;
                      terminar=1;
                    }
                    strcpy(processes[k].name,nombre);
                    memset(nombre,0,20);
                  }
                }else if(buffer[0]=='S'){
                    if(buffer[1]=='t' && buffer[2]=='a' && buffer[3]=='t' && buffer[4]=='e'){
                      int i;
                      char estado[20];
                      int posN=0;
                      int terminar=0;

                      for(i=6;i<255;i++){
                        if(buffer[i]=='\n'){
                        //nombre[posN]=buffer[i];                        
                          break;
                        }
                        if(buffer[i]==' '){
                          if(terminar==1){
                          estado[posN]='\0';
                          break;
                        }
                          continue;
                        }
                        estado[posN]=buffer[i];
                        posN+=1;
                        terminar=1;
                      }
                      strcpy(processes[k].state,estado);
                      memset(estado,0,20);

                    }
                }else if(buffer[0]=='P'){
                    if(buffer[1]=='i' && buffer[2]=='d'){
                      int i;
                      char dataPID[20];
                      int posN=0;
                      int terminar=0;
                      for(i=4;i<255;i++){
                        if(buffer[i]=='\n'){
                        //nombre[posN]=buffer[i];                        
                          break;
                        }
                        if(buffer[i]==' '){
                          if(terminar==1){
                            dataPID[posN]='\0';
                            break;
                          }
                          continue;
                        }
                        dataPID[posN]=buffer[i];
                        posN+=1;
                        terminar=1;
                      }
                      strcpy(processes[k].pid,dataPID);
                      memset(dataPID,0,20);

                    }else if(buffer[1]=='P' && buffer[2]=='i' && buffer[3]=='d'){
                      int i;
                      char dataPPID[20];
                      int posN=0;
                      int terminar=0;
                      for(i=5;i<255;i++){
                        if(buffer[i]=='\n'){
                        //nombre[posN]=buffer[i];                        
                          break;
                        }
                        if(buffer[i]==' '){
                          if(terminar==1){
                            dataPPID[posN]='\0';
                            break;
                          }
                          continue;
                        }
                        dataPPID[posN]=buffer[i];
                        posN+=1;
                        terminar=1;
                      }
                      strcpy(processes[k].ppid,dataPPID);
                      memset(dataPPID,0,20);

                    }
                }else if(buffer[0]=='V'){
                  //VmRSS
                  if(buffer[1]=='m' && buffer[2]=='R' && buffer[3]=='S' && buffer[4]=='S'){

                      int i;
                      char memoria[20];
                      int posN=0;
                      int terminar=0;
                      for(i=6;i<255;i++){
                        if(buffer[i]=='\n'){
                          break;
                        }
                        if(buffer[i]==' '){
                          
                          continue;
                        }

                        memoria[posN]=buffer[i];
                        posN+=1;
                        terminar=1;
                      }
                      if(memoria[0] == '\0'){
                          strcpy(processes[k].memory," ");
                      }else{
                          strcpy(processes[k].memory,memoria);
                          memset(memoria,0,20);
                      }
          

                    }
                }else if(buffer[0]=='T'){
                  //Threads
                    if(buffer[1]=='h' && buffer[2]=='r' && buffer[3]=='e' && buffer[4]=='a' && buffer[5]=='d' && buffer[6]=='s'){
                      int i;
                      char threads[20];
                      int posN=0;   
                      int terminar=0;                 
                      for(i=8;i<255;i++){
                        if(buffer[i]=='\n'){
                          break;
                        }
                        if(buffer[i]==' '){
                          if(terminar==1){
                              threads[posN]='\0';
                              break;
                            }
                          continue;
                        }

                        threads[posN]=buffer[i];
                        posN+=1;
                        terminar=1;
                      }
                      if(threads[0] == '\0'){
                          strcpy(processes[k].memory," ");
                      }else{
                          strcpy(processes[k].threads,threads);
                          memset(threads,0,20);
                      }
                    }
                }

            }

             fclose(fp);

            DIR *d2 = opendir(fdpath);
            int contador=0;
            FILE *fp2;
            struct dirent *ep2;  

            if (d != NULL){
              while (ep2 = readdir (d2)){
                contador+=1;
              }
            }
            (void) closedir (d2);
            processes[k].openFiles = contador - 2;
            k++;
	      		strcpy(path, "/proc/");
            strcpy(fdpath, "/proc/");
          }
        }
        (void) closedir (d);
      }
      else{
        printf("error\n");
      }
}

static void saveProcesses(int signo) {
	  FILE *fp;
    char filename[150];
    strcpy(filename, "mytop-status-");
    
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strcat(filename, asctime(timeinfo));
    strcat(filename, ".txt");

	fp = fopen(filename, "w");
	fprintf(fp, header, " PID", "Parent", "Name", "State", "Memory", "# Threads", "Open Files");
	fprintf(fp, "%s\n", headDiv);
  int i;
	for(i = 0; processes[i].name[0] != '\0'; i++) {
		fprintf(fp, tableFormat, processes[i].pid, processes[i].ppid, processes[i].name, processes[i].state, atof(processes[i].memory) / 1000, processes[i].threads, processes[i].openFiles);
	}
	printf("\n Top saved as:  %s\n", filename);
	fclose(fp);
	sleep(4);
}


int main(){

      if(signal(SIGINT, saveProcesses) == SIG_ERR) {
        printf("Error\n");
      }

      while(1) {
        getInfo();
        printProcesses();
        sleep(3);
        clear();
      }
    return 0;
}

void clear() {
  printf("\e[1;1H\e[2J"); 
}
