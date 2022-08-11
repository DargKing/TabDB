#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tabdb.h"

int login(char *email, char *password)
{
        FILE *fp;

        fp = fopen("users.txt", "r+");
        if (fp == NULL)
                return -1;

        int fila = search_data_file(fp, 1, email);

        if (fila == -1)
        {
                fclose(fp);
                return -2;
        }

        char temp_password[100];
        read_col_file(fp, fila, 2, temp_password);

        if (strcmp(temp_password, password))
        {
                fclose(fp);
                return -2;
        }

        fclose(fp);

        return fila;
}

int create_new_user(char *email, char *password, char *username, char* range)
{
        FILE *fp;

        fp = fopen("users.txt", "r+");
        if (fp == NULL)
                return -1;

        if (search_data_file(fp, 1, email) != -1)
                return 0;

        time_t rawtime;
        struct tm *info;
        char ID[20];

        time(&rawtime);

        info = localtime(&rawtime);

        strftime(ID, 20, "%y%m%H%I%M%S%p", info);

        char line[320];

        strcpy(line, ID);
        strcat(line, "\t");
        strcat(line, email);
        strcat(line, "\t");
        strcat(line, password);
        strcat(line, "\t");
        strcat(line, username);
        strcat(line, "\t");
        strcat(line, "USER");

        add_line_file(fp, line);

        fclose(fp);

        return 1;
}

int search_user(char *email)
{
        FILE *fp;

        fp = fopen("users.txt", "r+");
        if (fp == NULL)
                return -1;

        int resul = search_data_file(fp, 1, email);

        if (resul == -1)
                return -2;

        fclose(fp);
        return resul;
}

int get_data_user(int row, char *ID, char *email, char *password, char *username, char *range)
{
        FILE *fp;

        fp = fopen("users.txt", "r+");
        if (fp == NULL)
                return -1;

        read_col_file(fp, row, 0, ID);
        read_col_file(fp, row, 1, email);
        read_col_file(fp, row, 2, password);
        read_col_file(fp, row, 3, username);
        read_col_file(fp, row, 4, range);

        fclose(fp);
}

int change_password(char *ID, char *newPassword)
{
        FILE *fp;
        fp = fopen("users.txt", "r+");
        if (fp == NULL)
                return -1;

        int search = search_data_file(fp, 0, ID);

        if (search == -1)
        {
                fclose(fp);
                return 0;
        }

        char temp[100];
        read_col_file(fp, search, 2, temp);

        if (!strcmp(temp, newPassword))
        {
                fclose(fp);
                return 2;
        }

        modify_col_file(fp, search, 2, newPassword);

        fclose(fp);
        return 1;
}

int change_username(char *ID, char *newUsername)
{
        FILE *fp;
        fp = fopen("users.txt", "r+");
        if (fp == NULL)
                return -1;

        int search = search_data_file(fp, 0, ID);

        if (search == -1)
        {
                fclose(fp);
                return 0;
        }

        char temp[100];
        read_col_file(fp, search, 2, temp);

        if (!strcmp(temp, newUsername))
        {
                fclose(fp);
                return 2;
        }

        modify_col_file(fp, search, 3, newUsername);

        fclose(fp);
        return 1;
}

int delete_user(char *ID)
{
        FILE *fp;
        fp = fopen("users.txt", "r+");
        if (fp == NULL)
                return -1;

        int search = search_data_file(fp, 0, ID);

        if (search == -1)
        {
                fclose(fp);
                return 0;
        }

        delete_line_file(fp, search);
        fclose(fp);

        return 1;
}

int set_admin(char *ID)
{
        FILE *fp;
        fp = fopen("users.txt", "r+");
        if (fp == NULL)
                return -1;

        int row = search_data_file(fp, 0, ID);

        if (row == -1)
        {
                fclose(fp);
                return -1;
        }

        modify_col_file(fp, row, 4, "ADMIN");

        fclose(fp);
        return 1;
}

int set_user(char *ID)
{
        FILE *fp;
        fp = fopen("users.txt", "r+");
        if (fp == NULL)
                return -1;

        int row = search_data_file(fp, 0, ID);

        if (row == -1)
        {
                fclose(fp);
                return -1;
        }

        modify_col_file(fp, row, 4, "USER");

        fclose(fp);
        return 1;
}