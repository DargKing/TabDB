#include <stdio.h>
#include <string.h>
#include "tabdb.h"

typedef struct tab{
        char juan[20];
        char pedro[20];
};

int main()
{
        FILE *fp;
        fp = fopen("prueba.txt", "r+");

        if (fp == NULL)
                return -1;

 

        fclose(fp);
        return 0;
}