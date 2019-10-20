#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define CHARSET_SIZE    27

struct trie{
    struct trie *children[CHARSET_SIZE];
    bool eow;
    char c;
};

//init a trie node
//mallocs all children
//returns pointer to initialised trie node
struct trie* init_trie(){
    //printf("\n init \n");
    struct trie *t = malloc(sizeof(struct trie));
    t->eow = false;
    t->c = NULL;
    for(int i = 0; i<CHARSET_SIZE; i++){
        t->children[i] = malloc(sizeof(struct trie));
    }
    return t;
}

//insert word as char array into trie
//return 0 if successful
//assuming trie passed is the root of trie
int insert(char *word, struct trie *trie){

    int len = strlen(word)-1;

    for(int i = 0; i<len; i++){
        bool found = false;
   	    //scan for letter in children
        for(int j = 0; j<CHARSET_SIZE; j++){
            if(trie->children[j]->c==word[i]){//char already in child array
		        if(i==len-1){ //handle last char of word
                    trie->children[j]->eow=true;
                    return 0;
		        }else{  //continue with next char
                    trie = trie->children[j];
                    found = true;
                    break;
                }
            }
        }
        if(!found){//char not found - insert new char
            bool space = false;
            //traverse until an empty space is found for new letter
            for(int j = 0; j<CHARSET_SIZE; j++){
                if(trie->children[j]->c==NULL){
                    trie->children[j] = init_trie();
                    trie->children[j]->c = word[i];
                    trie = trie->children[j];
                    if(i==len-1){ //handle last char of word
                        trie->eow=true;
                    }
                    space = true;
                    break;
                }
            }
            if(!space){
                printf("\n%s\n", " ERROR: no space for new character in child array\n Terminating");
                exit(-1);
            }
        }
    }
}//insert

//searches for word in trie
//assumes param t is root node of trie
//return 0 if not found, 1 if found, -1 on error
int search(char *word, struct trie *t){
    int len = strlen(word);
    for(int i = 0; i<len; i++){
        bool found = false;
        //test t's children for i
            //if true
                //t = t.child
                //inc i
                //loop
            //else return false
        for(int j = 0; j<CHARSET_SIZE; j++){
            if(t->children[j]->c==word[i]){
                found = true;
                t = t->children[j];
                if(i==len-1){
                    if(t->eow==1){
                        printf("word found \n");
                        return 1;
                    }else{
                        printf("word not found \n");
                        return 0;
                    }
                }
                break;
            }
        }
        if(!found){
            printf("\nWord not found.\n");
            return 0;
        }
    }
}//search

int test1(struct trie *t){
    char *word = "word";

    printf("%c, eow: %s\n", t->children[0]->c, (t->children[0]->eow ? "true" : "false"));
    printf("%c, eow: %s\n", t->children[0]->children[0]->c, (t->children[0]->children[0]->eow ? "true" : "false"));
    printf("%c, eow: %s\n", t->children[0]->children[0]->children[0]->c, (t->children[0]->children[0]->children[0]->eow ? "true" : "false"));
    printf("%c, eow: %s\n", t->children[0]->children[0]->children[0]->children[0]->c, (t->children[0]->children[0]->children[0]->children[0]->eow ? "true" : "false"));

    printf("\n ************************ \n");

    printf("%c, eow: %s\n", t->children[0]->c, (t->children[0]->eow ? "true" : "false"));
    printf("%c, eow: %s\n", t->children[0]->children[0]->c, (t->children[0]->children[0]->eow ? "true" : "false"));
    printf("%c, eow: %s\n", t->children[0]->children[0]->children[0]->c, (t->children[0]->children[0]->children[0]->eow ? "true" : "false"));
    printf("%c, eow: %s\n", t->children[0]->children[0]->children[0]->children[0]->c, (t->children[0]->children[0]->children[0]->children[0]->eow ? "true" : "false"));

}


int main(int argc, int argv[]){

    struct trie *t = init_trie();

    //insert all words from dictionary file into trie
    char word[1000];
    FILE *fp;
    fp = fopen("./dict.txt","r");
    if(fp==NULL){
        printf("\nbad file path\n");
        return -1;
    }

    while(fgets(word, 1000, fp)!=NULL){
        //printf("%s      %d\n", word, strlen(word));
        insert(word, t);
    }
    fclose(fp);

    //search for "word" in dictionary
    printf("%d\n", search("word", t));
}//main
