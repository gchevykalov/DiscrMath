#ifndef __FUNC_H_INCLUDED
#define __FUNC_H_INCLUDED
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"

#define TRUE 1 //returned on success
#define FALSE 0 //returns on normal termination with no expected result
#define ERR -1 //returned when arguments are invalid or when set is not found
#define MEM_ERR -2 //returns when out of memory
#define NAME_ERR -3 //returns when a set with the same name already exists
#define MAX_FUNC_NAME_SIZE 10

//defining the type of function working with elements
typedef struct {
  char name[MAX_FUNC_NAME_SIZE];
  int (*function)(set_t *, const char *);
  int output;
} elem_function_t;
//defining the type of function working with sets
typedef struct {
  char name[MAX_FUNC_NAME_SIZE];
  int (*function)(set_t **, const char *);
  int output;
} set_function_t;
//defining the type of function performing operations on sets
typedef struct {
  char name[MAX_FUNC_NAME_SIZE];
  int (*function)(set_t **, const char *, const char *, const char *);
  int output;
} op_function_t;
//enumeration of possible types of functions
typedef enum {
  FUNC_TYPE_ELEM,
  FUNC_TYPE_SET,
  FUNC_TYPE_OP,
  FUNC_TYPE_UNKNOWN
} FUNC_TYPE;
//generic type for working with function pointers
typedef struct {
  FUNC_TYPE type;
  union {
    const elem_function_t *elem;
    const set_function_t *set;
    const op_function_t *op;
  } value;
} function_t;

int PrintSets(set_t *list);
set_t * InitList(void);
void ListClear(set_t **list);
set_t * SetFind(set_t *list, const char *name);
int FindFunction(const char *name, function_t *res);

int ElementAdd(set_t *set, const char *data);
int ElementDel(set_t *set, const char *data);
int ElementCheck(set_t *set, const char *data);

int SetAdd(set_t **list, const char *name);
int SetDel(set_t **list, const char *name);
int SetCard(set_t **list, const char *name);
int PrintElements(set_t **list, const char *name);

int SetUnion(set_t **list, const char *name1, const char *name2, const char *nameRes);
int SetInter(set_t **list, const char *name1, const char *name2, const char *nameRes);
int SetDif(set_t **list, const char *name1, const char *name2, const char *nameRes);
int SetSymDif(set_t **list, const char *name1, const char *name2, const char *nameRes);
//nameRes is fiction
int SetEntry(set_t **list, const char *name1, const char *name2, const char *nameRes);
#endif /* __FUNC_H_INCLUDED */