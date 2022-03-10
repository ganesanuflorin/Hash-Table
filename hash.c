#include<stdio.h>
#include<stdlib.h>
#include<string.h>



struct info {
    void *freq;
    void *word;
};

struct Hashtable {
    struct info *buckets;
    int size; 
    int hmax; 
    unsigned int (*hash_function_string)(void*);
};

void init_ht(struct Hashtable *ht, int hmax, unsigned int (*hash_function_string)(void*));

unsigned int hash_function_string(void *a);

int compare_function_strings(void *a, void *b);


void free_ht(struct Hashtable *ht);

void put(struct Hashtable *ht, int word_size_bytes, int freq_size_bytes, void *word, void *freq);

// returneaza valorea
void* get(struct Hashtable *ht, void *word);

int main() {
 	
 	struct Hashtable *ht = malloc(sizeof(struct Hashtable));
 	int i, hmax = 30;
 	char s[100];
 	int  prev_freq;
 	void *aux_point;
 	memset(s, 0, 100);
 	init_ht(ht, hmax, hash_function_string);

 	while (fgets(s, 99, stdin)) {
 		aux_point = get(ht, (void*)s);
 		if (aux_point == NULL){
 			prev_freq = 0;
 		} else {
			prev_freq = *(int*)aux_point;
 		}
 		prev_freq++;
 		put(ht, (strlen(s) + 1)*sizeof(char), sizeof(int), (void*)s, &prev_freq);
 		memset(s, 0, 100);
 	}
 	for(i = 0; i < hmax; i++) {
 		char *word = (char*)ht->buckets[i].word;
 		if (ht->buckets[i].word != NULL) {
 			if(word[strlen(word) - 1] == '\n') {
 				word[strlen(word) - 1] = '\0';
 			}
 			printf("%s %d\n", (char*)(ht->buckets[i].word), *(int*)(ht->buckets[i].freq));
 		}
	}
	free_ht(ht);
	return 0;
}

void* get(struct Hashtable *ht, void *word) {
   unsigned int hs;
   hs = hash_function_string(word) % ht->hmax;

   while(ht->buckets[hs].word != NULL) {
   		if (compare_function_strings(ht->buckets[hs].word, word) == 0) {
   			return ht->buckets[hs].freq;
   		}
   		hs = (hs + 1) % ht->hmax;
   }
   return NULL;
}

void put(struct Hashtable *ht, int word_size_bytes, int freq_size_bytes, void *word, void *freq) {
   unsigned int hs;
   hs = hash_function_string(word) % ht->hmax;
   
   while(ht->buckets[hs].word != NULL) {
   		if (compare_function_strings(ht->buckets[hs].word, word)) {
   			free(ht->buckets[hs].freq);
   			ht->buckets[hs].freq = malloc(freq_size_bytes);
   			memcpy(ht->buckets[hs].freq, freq, freq_size_bytes);
   			return;
   		}

   		hs = (hs + 1) % ht->hmax;
   }

   ht->buckets[hs].word = malloc(word_size_bytes);
   memcpy(ht->buckets[hs].word, word, word_size_bytes);
   ht->buckets[hs].freq = malloc(freq_size_bytes);
   memcpy(ht->buckets[hs].freq, freq, freq_size_bytes);
}

void init_ht(struct Hashtable *ht, int hmax, unsigned int (*hash_function_string)(void*)) {
    ht->size = 0;
    ht->hmax = hmax;
    ht->hash_function_string = hash_function_string;
    ht->buckets = calloc(hmax, sizeof(struct info));
    if(ht->buckets == NULL) {
        perror("Nu a fost alocata memoria\n");
        exit(1);
    }
}

unsigned int hash_function_string(void *a) {

    unsigned char *puchar_a = (unsigned char*) a;
    unsigned long hash = 5381;
    int c;

    while ((c = *puchar_a++))
        hash = ((hash << 5u) + hash) + c; /* hash * 33 + c */

    return hash;
}

int compare_function_strings(void *a, void *b) {
    char *str_a = (char *)a;
    char *str_b = (char *)b;

    return strcmp(str_a, str_b);
}


void free_ht(struct Hashtable *ht) {

   for(int i = 0; i < ht->hmax; i++) {
   		free(ht->buckets[i].word);
   		free(ht->buckets[i].freq);
   	}	
   free(ht->buckets);
   free(ht);
}