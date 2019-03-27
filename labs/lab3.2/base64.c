#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <inttypes.h>
#include <signal.h>

#define BUF_SIZE   0xFFFFFF
#define SIGINFO 29

//Code retrieved from: https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};

float status = 0;

void sig_handler(int sig) {
    printf("%d%% of file processed\n", (int) status);
}

void build_decoding_table() {

    decoding_table = malloc(256);
    int i;
    for (i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length) {
    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(*output_length);
    if (encoded_data == NULL) return NULL;
    int i, j;
    for (i = 0, j = 0; i < input_length;) {
        status = (float) i / (float) input_length * 100;
        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {
    if (decoding_table == NULL) build_decoding_table();
    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;
    int i, j;
    for (i = 0, j = 0; i < input_length;) {
        status = (float) i / (float) input_length * 100;
        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
    return decoded_data;
}

void base64_cleanup() {
    free(decoding_table);
}
/*
Here starts my code
*/
int main(int argc, char *argv[]) {
    int encode = -1; int decode = -1;
    char *tipo = calloc(1024, sizeof(char));
    char *filename = calloc(1024, sizeof(char));
    char *output_file = calloc(1024, sizeof(char));

     if(argc>=2) 
    { 
        strcpy(tipo, argv[1]);
        strcpy(filename, argv[2]);
        if(strcmp(tipo, "--encode")==0){
            strcpy(output_file,"encoded.txt");
            encode=0;
        }else if(strcmp(tipo, "--decode")==0){
            decode=0;
            strcpy(output_file,"decoded.txt");            
        }
    }else{
        printf("Error en los parametros");
        return -1;
    }

    signal(SIGINT, sig_handler); 
    signal(SIGINFO, sig_handler);

    long size;
    char *buffer;
    long read_size = 0;

    FILE *file;
    file = fopen(filename,"r");
    if (file != NULL) {
        fseek(file, 0L, SEEK_END);
        size = ftell(file);
        fseek(file, 0L, SEEK_SET);
        buffer = malloc(size);
        read_size = fread(buffer, 1, size, file);
        fclose(file);
    } else {
        return -1;
    }

    size_t output_size = 0;

    char *result;
    
    if (encode == 0) {
        infof("Encoding file\n");
        result = base64_encode(buffer, strlen(buffer), &output_size);
    } else if (decode == 0) {
        infof("Decoding file\n");
        result = (char *) base64_decode(buffer, strlen(buffer), &output_size);
    } else {
        return -1;
    }    
    base64_cleanup();

    FILE *output;
    output= fopen(output_file,"w");
    if (output != NULL) {
        fwrite(result, output_size, 1, output);
        infof("Creando archivo '%s'\n", output_file);
    } else {
        panicf("Error creando archivo '%s'\n", output_file);
        return -1;
    }
    fclose(output);
    return 0;
}