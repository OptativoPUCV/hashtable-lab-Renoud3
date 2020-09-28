#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"

typedef struct Pair Pair;
typedef struct HashMap HashMap;
int enlarge_called=0;

struct Pair {
     char * key;
     void * value;
};

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  unsigned long long a = hash(key , map -> capacity);
  if (map -> buckets[a] != NULL) {
    do {
      a++;
      if (a >= map -> capacity) {
        a = 0;
      }
    } while (map -> buckets[a] != NULL);
  }
  map -> buckets[a] = createPair(key, value);
  map -> size++;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    long capacidad;
    long a;
    Pair ** auxiliar = map -> buckets;
    map -> capacity *= 2;
    map -> buckets = (Pair**) calloc (map -> capacity , sizeof(Pair*));
    capacidad = map -> capacity;
    for (a = 0 ; a < (capacidad / 2) ; a++) {
      if (auxiliar[a] != NULL) {
        insertMap(map , auxiliar[a] -> key , auxiliar[a] -> value);
      }
    }
    
}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap *) malloc (sizeof (HashMap));
  map -> buckets = (Pair**) calloc (capacity , sizeof(Pair*));
  map -> current = -1;
  map -> capacity = capacity;
  return map;
}

void eraseMap(HashMap * map,  char * key) {    


}

void * searchMap(HashMap * map,  char * key) {   
  unsigned long long a = hash(key , map -> capacity);
  do {
    if (map -> buckets[a] != NULL) {
      if (is_equal(map -> buckets[a] -> key , key)) {
        map -> current = a;
        return map -> buckets[a] -> value;
      }
    }
    a++;
  } while (a < map -> capacity);
  return NULL;
}

void * firstMap(HashMap * map) {
  int a;
  for (a = 0 ; a < map -> capacity ; a++) {
    if ((map -> buckets[a] != NULL) && (map -> buckets[a] -> key != NULL)) {
      map -> current = a;
      return map -> buckets[a] -> value;
    }
  }
  return NULL;
}

void * nextMap(HashMap * map) {
  int a;
  for (a = map -> current + 1 ; a < map -> capacity ; a++) {
    if ((map -> buckets[a] != NULL) && (map -> buckets[a] -> key != NULL)) {
      map -> current = a;
      return map -> buckets[a] -> value;
    }
  }
  return NULL;
}
