#include "caracteres.h"
#include <string.h>
#include <stdio.h>
#include "tabdb.h"

int get_len_file(FILE *fp) // Devuelve la cantidad de caracteres que contiene el archivo
{
        size_t i = 0;
        while (fgetc(fp) != EOF)
                i++;
        return i;
}

void move_to_line_file(FILE *fp, int row) // Mueve el puntero a una fila en especifico
{
        fseek(fp, 0, SEEK_SET);
        int line = 0;
        char caracter;
        while (line < row)
        {
                if ((caracter = fgetc(fp)) == '\n')
                {
                        line++;
                }
                if (caracter == EOF)
                        break;
        }
}

void move_to_col_file(FILE *fp, int col) // Mueve el puntero a la columna que se le indique. esta columna pertenecera a la
{                                        // fila donde este actualmente el puntero. En caso de que el puntero este en una columna diferente a la 0 dara error,
                                         // el puntero siempre debe de estar en la columna 0 para funcionar correctamente
        int tabs = 0;
        char caracter;
        int i = 0;

        while (tabs < col)
        {
                if ((caracter = fgetc(fp)) == '\t')
                        tabs++;
                if (caracter == '\n' || caracter == EOF)
                        break;
        }
}

int get_len_line_file(FILE *fp, int row) // Devuelve la longitud (cantidad de caracteres) de la fila especificada
{
        move_to_line_file(fp, row);
        size_t i = 0;
        char caracter;
        while ((caracter = fgetc(fp)) && caracter != '\n' && caracter != EOF)
                i++;
        return i;
}

int get_len_col_file(FILE *fp, int row, int col) // Devuelve la longitud (cantidad de caracteres) de la columna x de la fila especificada
{
        move_to_line_file(fp, row);
        size_t i = 0;
        int tabs = 0;
        char caracter;
        while ((caracter = fgetc(fp)) && caracter != '\n' && caracter != EOF)
        {
                if (caracter == '\t')
                        tabs++;
                if (tabs == col)
                        i++;
        }
        return i;
}

int gets_lines_file(FILE *fp) // Deuvelve la cantidad de saltos de linea que contiene el archivo (no cuenta los ultimos 2)
{
        int jumplines = 0;
        char caracter;
        while ((caracter = fgetc(fp)) && caracter != EOF)
        {
                if (caracter == '\n')
                        jumplines++;
        }
        return jumplines - 1;
}

int gets_jumplines_file(FILE *fp) // Devuelve la cantidad de lineas que no estan en blanco
{
        int jumplines = 0;
        char caracter = fgetc(fp);
        char last_char = NULL;
        char first_char_line = caracter;
        while (caracter != EOF)
        {
                caracter = fgetc(fp);
                if (caracter == '\n' && first_char_line != ' ')
                        jumplines++;
                if (last_char == '\n')
                        first_char_line = caracter;
                last_char = caracter;
        }
        return jumplines - 1;
}

void read_line_file(FILE *fp, int row, char *str) // Inserta en la variable str el contenido de la linea especificada
{
        int i = 0;
        char temp[get_len_line_file(fp, row) + 1];
        char caracter;

        move_to_line_file(fp, row);

        while ((caracter = fgetc(fp)) != '\n')
        {
                if (caracter == EOF)
                {
                        strcpy(str, "UNDEFINED");
                        return;
                }
                temp[i] = caracter;
                i++;
        }
        temp[i] = '\0';
        strcpy(str, temp);
}

void read_col_file(FILE *fp, int row, int col, char *str) // Inserta en la varible str el contenido de la columna n de la linea m
{
        int i = 0;
        char temp[get_len_col_file(fp, row, col) + 1];
        char caracter;

        move_to_line_file(fp, row);
        move_to_col_file(fp, col);

        while ((caracter = fgetc(fp)) != '\t')
        {
                if (caracter == '\n' || caracter == EOF)
                {
                        break;
                }
                temp[i] = caracter;
                i++;
        }
        temp[i] = '\0';
        strcpy(str, temp);
}

int search_data_file(FILE *fp, int col, char *str) // Busca en todas las columnas n el dato que se paso en el parametro str
{                                                  // Si lo encuentra devuelve la linea donde se encuentra, en caso de que en esa columna
                                                   // esten dos datos iguales se devolvera el primer que se encuentre mas cerca de la linea 0
        int row = 0;
        fseek(fp, 0, SEEK_SET);

        char string[320];

        while (strcmp(string, "UNDEFINED"))
        {
                int from = 0;
                int to = 0;
                int tabs = 0;
                int i = 0;

                read_line_file(fp, row, string);
                while (string[i] != '\0')
                {
                        if (string[i] == '\t')
                        {
                                tabs++;
                                if (tabs == col)
                                        from = i + 1;
                        }
                        if (tabs == col)
                        {
                                to++;
                        }
                        i++;
                }
                char temp[320];
                substr(temp, string, from, (col == 0) ? from + to : from + to - 1);
                if (!strcmp(temp, str))
                        return row;

                row++;
        }
        return -1;
}

void delete_line_file(FILE *fp, int row) // A pesar de que diga que elimina la linea especificada este no la elimina, esta funcion sustituye
{                                        // Todos los caracteres de la linea por espacios en blanco, convirtiendola en lo que nosotros llamamos
                                         // Linea en blanco
        move_to_line_file(fp, row);
        fseek(fp, 0, SEEK_CUR);

        int len = get_len_line_file(fp, row);

        move_to_line_file(fp, row);
        fseek(fp, 0, SEEK_CUR);

        for (int i = 0; i < len; i++)
        {
                fputc(' ', fp);
        }
}

void add_line_file(FILE *fp, char *str) // Añade el contenido del parametro str al archivo en la ultima linea
{
        fseek(fp, -2, SEEK_END);
        int i = 0;
        while (str[i] != '\0')
        {
                fputc(str[i], fp);
                i++;
        }

        fprintf(fp, "\n\n");
}

void modify_col_file(FILE *fp, int row, int col, char *str) // Se cambia el contenido de la columna n fila m por el contenido del parametro str
{
        char line[get_len_line_file(fp, row) + 1];
        read_line_file(fp, row, line);

        int tabs = 0;
        int len = 0;
        int from = 0;
        int i = 0;
        while (line[i] != '\0')
        {
                if (tabs == col)
                        len++;
                if (line[i] == '\t')
                {
                        tabs++;
                        if (tabs == col)
                                from = i + 1;
                }
                i++;
        }

        int lenStr = lenghtStr(str);

        if (col == 0)
        {
                char final[i - from + len + lenStr + 5];
                strcpy(final, str);

                char part1[i - from + len + 1];
                substr(part1, line, from + len, i);

                strcat(final, "\t");
                strcat(final, part1);

                add_line_file(fp, final);
                delete_line_file(fp, row);
                return;
        }

        if (col == tabs)
        {
                char part1[from + 1];

                char final[i - from + len + lenStr + 5];
                substr(part1, line, 0, from);

                strcpy(final, part1);
                strcat(final, str);

                add_line_file(fp, final);
                delete_line_file(fp, row);
                return;
        }

        char part1[300];
        char part2[300];
        char final[300];

        substr(part1, line, 0, from);
        substr(part2, line, from + len, i);

        strcpy(final, part1);
        strcat(final, str);
        strcat(final, "\t");
        strcat(final, part2);

        add_line_file(fp, final);
        delete_line_file(fp, row);
}

int is_blank(FILE *fp, int row) // Revisa si es una linea en blanco o no, devuelve 1 si lo es, de lo contrario devuelve 0
{
        move_to_line_file(fp, row);

        char caracter = fgetc(fp);

        if (caracter == ' ')
                return 1;
        return 0;
}