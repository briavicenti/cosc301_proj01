/*
 *
 * Bria Vicenti, 9/24/14
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "list.h"

char** tokenify(const char *s) {
    // Taken from lab 2, code by Bria Vicenti
    // Will return a char array with one NULL element if no tokens are present. 
    char *temp = strdup(s);
    char *temp2 = strdup(s);

    int count = 1;

    if (NULL != strtok(temp, "\n \t")) {
        while (NULL != strtok(NULL, "\n \t")) {
            count++;   
        }

        char** finished = malloc((count+1) * sizeof(char*)); // Account for final null slot.
        char *p = strtok(temp2, "\n \t");
        char *p2 = strdup(p);
        finished[0] = p2;

        for (int i = 1; i < count; i++) {
            p = strtok(NULL, "\n \t");
            p2 = strdup(p);
            finished[i] = p2;
        }

        finished[count] = NULL;
        free(temp);
        free(temp2);
        return finished;
    }
    else {
        char** finished = malloc(sizeof(char*));
        char *p = NULL;
        finished[0] = p;
        free(temp);
        free(temp2);
        return finished;
    }
}

void free_tokens(char **tokens) {
    // Taken from Lab 2, code by Professor Sommers.
    int i = 0;
    while (tokens[i] != NULL) {
        free(tokens[i]); // free each string
        i++;
    }
    free(tokens); // then free the array
}

bool check_valid(const char* str) {
    // Check validity of tokenized input strings.
    for (int i = 0; i < strlen(str); i++) {
        if (!isdigit(str[i]) && str[i] != '-') { // If there is anything but a digit or negative symbol
            return false;
        }
    }
    return true;
}

void process_data(FILE *input_file) {
    // !! your code should start here.  the input_file parameter
    // is an already-open file.  you can read data from it using
    // the fgets() C library function.  close it with the fclose()
    // built-in function

    struct node *list = NULL;
    char line[128] = {'\0'};
    char **split_line = NULL;
    while (fgets(line, 128, input_file) != NULL) {
        split_line = tokenify(line); 
        int i = 0;
        if (*split_line == NULL || split_line[0][0] == '#') { // Check with assignment.
            free_tokens(split_line);
            continue;
        }
        while (split_line[i] != NULL) {
            if (check_valid(split_line[i])) {
                int num = atoi(split_line[i]);
                int check = insert(num, &list);
                if (check != 0) {
                    return; // Insert failed.
                }
            }
            i++;
        }
        free_tokens(split_line);   
    }
    list_print(list);
    list_clear(list);
}


void usage(char *program) {
    fprintf(stderr, "usage: %s [<datafile>]\n", program);
    exit(1);
}

#ifndef AUTOTEST

int main(int argc, char **argv) {
    
    FILE *datafile = NULL;

    /* find out how we got invoked and deal with it */
    
    switch (argc) {
        case 1:
            /* only one program argument (the program name) */ 
            /* just equate stdin with our datafile */
            datafile = stdin;        
            break;

        case 2:
            /* two arguments: program name and input file */
            /* open the file, assign to datafile */
            datafile = fopen(argv[1], "r");
            if (datafile == NULL) {
                printf("Unable to open file %s: %s\n", argv[1], strerror(errno));
                exit(-1);
            }
            break;

        default:
            /* more than two arguments?  throw hands up in resignation */
            usage(argv[0]);
    }

    /* 
     * you should be able to just read from datafile regardless 
     * whether it's stdin or a "real" file.
     */
    process_data(datafile);
    fclose(datafile);

    struct timeval utime;
    struct timeval stime;
    struct rusage usage;

    getrusage(RUSAGE_SELF, &usage);

    utime = usage.ru_utime;
    stime = usage.ru_stime;

    double utime_total = utime.tv_sec + (utime.tv_usec/1000000.0); // Converts microseconds 
    double stime_total = stime.tv_sec + (stime.tv_usec/1000000.0); // to a fraction of seconds.

    printf("User time: %f\n", utime_total);
    printf("System time: %f\n", stime_total);

    return 0;

}
#endif
