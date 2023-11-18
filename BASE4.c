#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>


int main(unsigned int argc,unsigned char **argv){
    if(argc == 4){
        unsigned char *in_memory_block = 0;
        unsigned char *out_memory_block = 0;
        unsigned char e_d = argv[1][0];
        unsigned long file_size = 0;
        unsigned short unfilled_block_size = 0;
        FILE *in_file = 0;
        FILE *out_file = 0;
        in_file = fopen(argv[2],"rb");
        if(in_file == NULL){
            puts("IN file not exist.");
            return -1;
        }
        out_file = fopen(argv[3],"wb");
        setvbuf(in_file,NULL,_IONBF,0);
        setvbuf(out_file,NULL,_IONBF,0);
        fseek(in_file,0,2);
        file_size = ftell(in_file);
        rewind(in_file);
        switch (e_d)
        {
        case 'E':
            puts("Encrypting.");
            in_memory_block = malloc(1024);
            out_memory_block = malloc(4096);
            fread(in_memory_block,sizeof(unsigned char),1024,in_file);
            for(unsigned long arr_index = 0; arr_index<file_size*4; arr_index+=4){
                if((arr_index % 4096 == 0) && (arr_index != 0)){
                    fread(in_memory_block,sizeof(unsigned char),1024,in_file);
                    fwrite(out_memory_block,sizeof(unsigned char),4096,out_file);
                    unfilled_block_size = 0;
                }
                out_memory_block[arr_index % 4096] = (in_memory_block[(arr_index/4) % 1024] & 192) == 0 ? 'A' : out_memory_block[arr_index % 4096];
                out_memory_block[arr_index % 4096] = (in_memory_block[(arr_index/4) % 1024] & 192) == 64 ? 'B' : out_memory_block[arr_index % 4096];
                out_memory_block[arr_index % 4096] = (in_memory_block[(arr_index/4) % 1024] & 192) == 128 ? 'C' : out_memory_block[arr_index % 4096];
                out_memory_block[arr_index % 4096] = (in_memory_block[(arr_index/4) % 1024] & 192) == 192 ? 'D' : out_memory_block[arr_index % 4096];
                out_memory_block[(arr_index+1) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 48) == 0 ? 'A' : out_memory_block[(arr_index+1) % 4096];
                out_memory_block[(arr_index+1) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 48) == 16 ? 'B' : out_memory_block[(arr_index+1) % 4096];
                out_memory_block[(arr_index+1) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 48) == 32 ? 'C' : out_memory_block[(arr_index+1) % 4096];
                out_memory_block[(arr_index+1) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 48) == 48 ? 'D' : out_memory_block[(arr_index+1) % 4096];
                out_memory_block[(arr_index+2) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 12) == 0 ? 'A' : out_memory_block[(arr_index+2) % 4096];
                out_memory_block[(arr_index+2) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 12) == 4 ? 'B' : out_memory_block[(arr_index+2) % 4096];
                out_memory_block[(arr_index+2) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 12) == 8 ? 'C' : out_memory_block[(arr_index+2) % 4096];
                out_memory_block[(arr_index+2) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 12) == 12 ? 'D' : out_memory_block[(arr_index+2) % 4096];
                out_memory_block[(arr_index+3) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 3) == 0 ? 'A' : out_memory_block[(arr_index+3) % 4096];
                out_memory_block[(arr_index+3) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 3) == 1 ? 'B' : out_memory_block[(arr_index+3) % 4096];
                out_memory_block[(arr_index+3) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 3) == 2 ? 'C' : out_memory_block[(arr_index+3) % 4096];
                out_memory_block[(arr_index+3) % 4096] = (in_memory_block[(arr_index/4) % 1024] & 3) == 3 ? 'D' : out_memory_block[(arr_index+3) % 4096];
                unfilled_block_size+=4;
            }
            break;
        case 'D':
            puts("Decrypting.");
            in_memory_block = malloc(4096);
            out_memory_block = malloc(1024);
            fread(in_memory_block,sizeof(unsigned char),4096,in_file);
            for(unsigned long arr_index = 0; arr_index<=file_size-1; arr_index+=4){
                if((arr_index % 4096 == 0) && (arr_index != 0)){
                    fread(in_memory_block,sizeof(unsigned char),4096,in_file);
                    fwrite(out_memory_block,sizeof(unsigned char),1024,out_file);
                    unfilled_block_size = 0;
                }
                out_memory_block[(arr_index/4) % 1024] = (in_memory_block[arr_index % 4096]-65<<6) | (in_memory_block[(arr_index+1) % 4096]-65<<4) | (in_memory_block[(arr_index+2) % 4096]-65<<2) | (in_memory_block[(arr_index+3) % 4096]-65);
                ++unfilled_block_size;
            }
            break;
        default:
            puts("E/D parameter was passed incorrectly.");
            return -1;
            break;
        }
        if(unfilled_block_size != 0)
            fwrite(out_memory_block,sizeof(unsigned char),unfilled_block_size,out_file);
        free(in_memory_block);
        free(out_memory_block);
        puts("Success.");
    }
    else{
        puts("Use: base4 <E(NCRYPT)/D(ECRYPT)> <INPUT FILE LOCATION> <OUTPUT FILE LOCATION>");
    }
}