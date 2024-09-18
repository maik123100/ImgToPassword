#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#if defined (sha256depc)
#include <openssl/sha.h>
#elif defined (EVP)
#include <openssl/evp.h>
#endif

void customHash(const char *filePth, const char *charArr, size_t passwordLen){
    FILE *fd = fopen(filePth,"rb");
    if(!fd){
        fprintf(stderr,"Error opening file!");
        return;
    }

#if defined(sha256depc)

    SHA256_CTX sha256;
    SHA256_Init(&sha256);

    unsigned char buffer[1024];
    size_t bytesRead=0;
    while((bytesRead = fread(buffer, 1, sizeof(buffer), fd) !=0 )){
        SHA256_Update(&sha256, buffer, bytesRead);
    }
    fclose(fd);
    
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &sha256);
    
    size_t charCount=strlen(charArr);
    for (size_t i=0; i < passwordLen; i++) {
        unsigned char byte = hash[i% SHA256_DIGEST_LENGTH];
        printf("%c", charArr[byte %charCount]);
    }
    printf("\n");
#elif defined (EVP)
   EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    if(mdctx==NULL){
        fprintf(stderr, "\033[1;31mError: Failed init of EVP_MD_CTX!\033[0m\n");
        return;
    }
    
    if(!EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL )){
        fprintf(stderr, "\033[1;31mError: Failed init of SHA-256 Digest!\033[0m\n");
        return;
    }

    unsigned char byte;
    size_t curBytesRead;
    
    while (1) {
        curBytesRead=fread(&byte, sizeof(unsigned char), 1, fd);
        if(curBytesRead!=1){
            if(feof(fd)){
                break;
            }else{
                fprintf(stderr, "\033[1;31mError: Failed to read from File!\033[0m\n");
                EVP_MD_CTX_free(mdctx);
                return;
            }
        }
        if(EVP_DigestUpdate(mdctx, &byte, 1)!=1){
            fprintf(stderr, "\033[1;31mError: Failed to Update SHA-256 digest!\033[0m\n");
            EVP_MD_CTX_free(mdctx);
            return;
        }
    }
    unsigned int mdLen;
    unsigned char mdValue[EVP_MAX_MD_SIZE];
    if(!(EVP_DigestFinal_ex(mdctx, mdValue, &mdLen))){
        fprintf(stderr, "\033[1;31mError: Failed init of EVP_MD_CTX!\033[0m\n");
        EVP_MD_CTX_free(mdctx);
        return;
    }    
    size_t charCount=strlen(charArr);
    for (size_t i=0; i < passwordLen; i++) {
        byte = mdValue[i% mdLen];
        printf("%c", charArr[byte %charCount]);
    }
    printf("\n");
#elif defined (custom)
#else
    printf("Compiler Flag missing: either -DEVP,-Dcustom or -Dsha256depc");
#endif
}

void printUsg() {
    printf("Usage: -f <file> -c <char_array> -l <output_length>\n");
    printf("Options:\n");
    printf("  -f <file>         Path to password-file\n");
    printf("  -c <char_array>   chars that can be in the password (i.e. the password alphabet)\n");
    printf("  -l <length>       Desired password length\n");
    printf("  -h                Display this help message\n");
}

int main(int argc,char* argv[]){
    char* stdPswdChars="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz{}|–…·.,<>+~*#";
    char *filePth=NULL;
    char *charArr=NULL;
    int passwdLength=0;
    //CMD-Line opt parsing
    int opt;
    while ((opt = getopt(argc,argv, "f:c:l:h"))!=-1) {
        switch(opt){
            case 'f':
                filePth = optarg;
                break;
            case 'c':
                if(strcmp(optarg, "std") != 0){
                    charArr = optarg;
                }else{
                    charArr=stdPswdChars;
                }
                break;
            case 'l':
                passwdLength = atoi(optarg);
                break;
            case 'h':
                printUsg();
                return 0;
            default:
                printUsg();
                return 1;
        } 
    }
    
    if (!filePth || !charArr || passwdLength <= 0) {
        fprintf(stderr, "\033[1;31mError: Missing input parameters!\033[0m\n");
        printUsg();
        return 1;
    }

    customHash(filePth, charArr, passwdLength);

    return 0;
}
