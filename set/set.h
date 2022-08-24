#ifndef __SET_H_INCLUDED
#define __SET_H_INCLUDED
#pragma once

#define MAX_NAME_SIZE 101
#define MAX_ELEM_SIZE 81

typedef struct elem_t {
  char data[MAX_ELEM_SIZE];
  struct elem_t *next;
} elem_t;

typedef struct set_t {
  char name[MAX_NAME_SIZE];
  elem_t *head;
  unsigned int card;
  struct set_t *next;
} set_t;

#endif /* __SET_H_INCLUDED */