#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    char *src = user_str;
    char *dst = buff;
    int user_str_len = 0;
    int space_added = 0;

    while (*src != '\0' && user_str_len < len) {
        if (*src != ' ' && *src != '\t') {
            *dst++ = *src;
            user_str_len++;
            space_added = 0;
        } else if (!space_added) {
            *dst++ = ' ';
            user_str_len++;
            space_added = 1;
        }
        src++;
    }

    if (*src != '\0') {
        return -1; // User supplied string is too large
    }

    while (user_str_len < len) {
        *dst++ = '.';
        user_str_len++;
    }

    return user_str_len;
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int word_count = 0;
    int in_word = 0;

    for (int i = 0; i < str_len; i++) {
        if (*(buff + i) != ' ') {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }

    return word_count;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        case 'r': {
            char *start = buff;
            char *end = buff + user_str_len - 1;
            while (start < end) {
                char temp = *start;
                *start++ = *end;
                *end-- = temp;
            }
            printf("Reversed String: ");
            for (int i = 0; i < user_str_len; i++) {
                putchar(*(buff + i));
            }
            putchar('\n');
            break;
        }

        case 'w': {
            printf("Word Print\n----------\n");
            int word_index = 1;
            int char_count = 0;
            char *start = buff;

            for (int i = 0; i < user_str_len; i++) {
                if (*(buff + i) != ' ') {
                    char_count++;
                } else if (char_count > 0) {
                    printf("%d. ", word_index++);
                    for (int j = 0; j < char_count; j++) {
                        putchar(*(start + j));
                    }
                    printf(" (%d)\n", char_count);
                    char_count = 0;
                    start = buff + i + 1;
                }
            }

            if (char_count > 0) {
                printf("%d. ", word_index++);
                for (int j = 0; j < char_count; j++) {
                    putchar(*(start + j));
                }
                printf(" (%d)\n", char_count);
            }
            break;
        }

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE