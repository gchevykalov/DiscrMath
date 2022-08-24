#include "func.h"

//initialize the list of sets
set_t * InitList(void) {
  return NULL;
}
//find the set in the list of sets, returns NULL if the set is not in the list
set_t * SetFind(set_t *list, const char *name) {
  if (name == NULL)
    return NULL;

  while (list != NULL) {
    if (strcmp(list->name, name) == 0) {
      return list;
    }
    list = list->next;
  }

  return NULL;
}
//add the set
int SetAdd(set_t **list, const char *name) {
  if (list != NULL && name != NULL) {
    set_t *newSet = (set_t *)malloc(sizeof(set_t)), *tmp = *list, *tmpprev = NULL;
    int flag;

    if (newSet == NULL) {//not enough memory
      free(newSet);
      return MEM_ERR;
    }

    strcpy(newSet->name, name);
    newSet->card = 0;
    newSet->head = NULL;
    if (tmp != NULL) {
      while (tmp != NULL) {
        flag = strcmp(name, tmp->name);
        if (flag < 0)
          break;
        else if (flag == 0) {//set with the same name already exists
          free(newSet);
          return NAME_ERR;
        }
        else {
          tmpprev = tmp;
          tmp = tmp->next;
        }
      }
      newSet->next = tmp;
      if (tmpprev == NULL) {//adding to the beginning
        *list = newSet;
      }
      else
        tmpprev->next = newSet;
    }
    else {//add in empty list
      *list = newSet;
      newSet->next = NULL;
    }

    return TRUE;
  }
  else
    return ERR;
}
//clear elements of the set
static void setClear(elem_t *set) {
  elem_t *cur = set, *next = NULL;

  if (set != NULL) {
    next = cur->next;
    while (next != NULL) {
      free(cur);
      cur = next;
      next = cur->next;
    }
    free(cur);
  }
}
//delete the set
int SetDel(set_t **list, const char *name) {
  if (name != NULL && list != NULL) {
    set_t *setToDel = *list, *prev = NULL;

    while (setToDel != NULL) {//find set
      if (!strcmp(setToDel->name, name))
        break;
      else {
        prev = setToDel;
        setToDel = setToDel->next;
      }
    }

    if (setToDel == NULL)//set not found
      return FALSE;

    if (prev == NULL) {//removal from the beginning
      *list = setToDel->next;
    }
    else {
      prev->next = setToDel->next;
    }

    setClear(setToDel->head);//clear elements of the set
    free(setToDel);
    return TRUE;
  }
  else
    return ERR;
}
//clear the list of sets
void ListClear(set_t **list) {
  if (list != NULL) {
    set_t *cur = *list, *next = NULL;

    if (*list != NULL) {
      next = cur->next;
      while (next != NULL) {
        setClear(cur->head);
        free(cur);
        cur = next;
        next = cur->next;
      }
      setClear(cur->head);
      free(cur);
    }
    *list = NULL;
  }
}
//delete the element from the set
int ElementDel(set_t *set, const char *data) {
  if (data != NULL && set != NULL) {
    elem_t *elemToDel = set->head, *prev = NULL;

    while (elemToDel != NULL) {//find element
      if (!strcmp(elemToDel->data, data))
        break;
      else {
        prev = elemToDel;
        elemToDel = elemToDel->next;
      }
    }

    if (elemToDel == NULL)//element not found
      return FALSE;

    if (prev == NULL) {//removal from the beginning
      set->head = elemToDel->next;
    }
    else {
      prev->next = elemToDel->next;
    }
    set->card--;

    free(elemToDel);
    return TRUE;
  }
  else
    return ERR;
}
//add the element to the set
int ElementAdd(set_t *set, const char *data) {
  if (data != NULL && set != NULL) {
    elem_t *newEl = (elem_t *)malloc(sizeof(elem_t)), *tmp = set->head, *tmpprev = NULL;
    int flag;

    if (newEl == NULL) {//not enough memory
      free(newEl);
      return MEM_ERR;
    }

    strcpy(newEl->data, data);
    if (tmp != NULL) {
      while (tmp != NULL) {//looking for a suitable place
        flag = strcmp(data, tmp->data);
        if (flag < 0)
          break;
        else if (flag == 0) {//element already exists
          free(newEl);
          return FALSE;
        }
        else {
          tmpprev = tmp;
          tmp = tmp->next;
        }
      }
      newEl->next = tmp;
      if (tmpprev == NULL) {//adding to the beginning
        set->head = newEl;
      }
      else
        tmpprev->next = newEl;
    }
    else {//set is empty
      set->head = newEl;
      newEl->next = NULL;
    }
    set->card++;

    return TRUE;
  }
  else
    return ERR;
}
//returns the cardinality of the set
int SetCard(set_t **list, const char *name) {
  set_t *set = NULL;

  if (list == NULL || name == NULL || *list == NULL)
    return ERR;

  set = SetFind(*list, name);
  if (set == NULL)//set not found
    return ERR;
  else {
    return set->card;
  }
}
//returns true if the element is contained in the set
int ElementCheck(set_t *set, const char *data) {
  if (set != NULL && data != NULL) {
    elem_t *tmp = set->head;

    while (tmp != NULL) {
      if (strcmp(data, tmp->data) == 0)
        return TRUE;
      tmp = tmp->next;
    }

    return FALSE;
  }
  else
    return ERR;
}
//display all elements of the set, does nothing for an empty set
int PrintElements(set_t **set, const char *name) {
  elem_t *tmp;
  set_t *settmp;
  
  if (set == NULL || name == NULL || *set == NULL)
    return ERR;

  settmp = SetFind(*set, name);
  if (settmp == NULL)//set not found
    return ERR;

  tmp = settmp->head;
  while (tmp != NULL) {
    printf("\t\"%s\"\n", tmp->data);
    tmp = tmp->next;
  }

  return TRUE;
}
//display the list of sets
int PrintSets(set_t *list) {
  if (list == NULL) {
    fprintf(stderr, "\tList of sets is empty.\n");
    return ERR;
  }

  while (list != NULL) {
    printf("\t\"%s\"\n", list->name);
    list = list->next;
  }

  return TRUE;
}
//add element "elem" in to the end of the list "set"
static int add(set_t *set, elem_t **end, elem_t *elem) {
  elem_t *newEl = malloc(sizeof(elem_t));
  
  if (newEl == NULL)//not enough memory
    return MEM_ERR;

  strcpy(newEl->data, elem->data);
  newEl->next = NULL;
  if (*end == NULL) {
    set->head = newEl;
  }
  else {
    (*end)->next = newEl;
  }
  *end = newEl;
  set->card++;

  return TRUE;
}
//calculate the union, the result is added to the list of sets
int SetUnion(set_t **list, const char *name1, const char *name2, const char *nameRes) {
  set_t *set1, *set2, *setRes;
  elem_t *elem1, *elem2, *end = NULL, *elemToAdd = NULL;
  int flag;

  if (list == NULL || name1 == NULL || name2 == NULL || nameRes == NULL)
    return ERR;

  set1 = SetFind(*list, name1);
  if (set1 == NULL)//set not found
    return ERR;

  set2 = SetFind(*list, name2);
  if (set2 == NULL)//set not found
    return ERR;

  flag = SetAdd(list, nameRes);
  if (flag == MEM_ERR)//not enough memory
    return MEM_ERR;
  if (flag == NAME_ERR)//set with the same name already exists
    return NAME_ERR;

  elem1 = set1->head;
  elem2 = set2->head;
  setRes = SetFind(*list, nameRes);
  while (elem1 != NULL && elem2 != NULL) {
    flag = strcmp(elem1->data, elem2->data);
    if (flag < 0) {
      elemToAdd = elem1;
      elem1 = elem1->next;
    }
    else if (flag > 0) {
      elemToAdd = elem2;
      elem2 = elem2->next;
    }
    else {//the elements are equal so we can add any of them
      elemToAdd = elem1;
      elem1 = elem1->next;
      elem2 = elem2->next;
    }
    if (add(setRes, &end, elemToAdd) == MEM_ERR) {//not enough memory
      SetDel(list, nameRes);
      return MEM_ERR;
    }
  }
  //add remaining elements
  elemToAdd = NULL;
  if (elem1 != NULL)
    elemToAdd = elem1;
  if (elem2 != NULL)
    elemToAdd = elem2;
  while (elemToAdd != NULL) {
    if (add(setRes, &end, elemToAdd) == MEM_ERR) {//not enough memory
      SetDel(list, nameRes);
      return MEM_ERR;
    }
    elemToAdd = elemToAdd->next;
  }

  return TRUE;
}
//calculate the intersection, the result is added to the list of sets
int SetInter(set_t **list, const char *name1, const char *name2, const char *nameRes) {
  set_t *set1, *set2, *setRes;
  elem_t *elem1, *elem2, *end = NULL;
  int flag;

  if (list == NULL || name1 == NULL || name2 == NULL || nameRes == NULL)
    return ERR;

  set1 = SetFind(*list, name1);
  if (set1 == NULL)//set not found
    return ERR;

  set2 = SetFind(*list, name2);
  if (set2 == NULL)//set not found
    return ERR;

  flag = SetAdd(list, nameRes);
  if (flag == MEM_ERR)//not enough memory
    return MEM_ERR;
  if (flag == NAME_ERR)//set with the same name already exists
    return NAME_ERR;

  elem1 = set1->head;
  elem2 = set2->head;
  setRes = SetFind(*list, nameRes);
  while (elem1 != NULL && elem2 != NULL) {
    flag = strcmp(elem1->data, elem2->data);
    if (flag < 0) {
      elem1 = elem1->next;
    }
    else if (flag > 0) {
      elem2 = elem2->next;
    }
    else {
      if (add(setRes, &end, elem1) == MEM_ERR) {//not enough memory
        SetDel(list, nameRes);
        return MEM_ERR;
      }
      elem1 = elem1->next;
      elem2 = elem2->next;
    }
  }

  return TRUE;
}
//calculate the set difference, the result is added to the list of sets
int SetDif(set_t **list, const char *name1, const char *name2, const char *nameRes) {
  set_t *set1, *set2, *setRes;
  elem_t *elem1, *elem2, *end = NULL;
  int flag;

  if (list == NULL || name1 == NULL || name2 == NULL || nameRes == NULL)
    return ERR;

  set1 = SetFind(*list, name1);
  if (set1 == NULL)//set not found
    return ERR;

  set2 = SetFind(*list, name2);
  if (set2 == NULL)//set not found
    return ERR;

  flag = SetAdd(list, nameRes);
  if (flag == MEM_ERR)//not enough memory
    return MEM_ERR;
  if (flag == NAME_ERR)//set with the same name already exists
    return NAME_ERR;

  elem1 = set1->head;
  elem2 = set2->head;
  setRes = SetFind(*list, nameRes);
  while (elem1 != NULL && elem2 != NULL) {
    flag = strcmp(elem1->data, elem2->data);
    if (flag < 0) {
      if (add(setRes, &end, elem1) == MEM_ERR) {//not enough memory
        SetDel(list, nameRes);
        return MEM_ERR;
      }
      elem1 = elem1->next;
    }
    else if (flag > 0) {
      elem2 = elem2->next;
    }
    else {
      elem1 = elem1->next;
      elem2 = elem2->next;
    }
  }
  //add remaining elements
  while (elem1 != NULL) {
    if (add(setRes, &end, elem1) == MEM_ERR) {//not enough memory
      SetDel(list, nameRes);
      return MEM_ERR;
    }
    elem1 = elem1->next;
  }

  return TRUE;
}
//calculate the symmetric set difference, the result is added to the list of sets
int SetSymDif(set_t **list, const char *name1, const char *name2, const char *nameRes) {
  set_t *set1, *set2, *setRes;
  elem_t *elem1, *elem2, *end = NULL, *elemToAdd = NULL;
  int flag;

  if (list == NULL || name1 == NULL || name2 == NULL || nameRes == NULL)
    return ERR;

  set1 = SetFind(*list, name1);
  if (set1 == NULL)//set not found
    return ERR;

  set2 = SetFind(*list, name2);
  if (set2 == NULL)//set not found
    return ERR;

  flag = SetAdd(list, nameRes);
  if (flag == MEM_ERR)//not enough memory
    return MEM_ERR;
  if (flag == NAME_ERR)//set with the same name already exists
    return NAME_ERR;

  elem1 = set1->head;
  elem2 = set2->head;
  setRes = SetFind(*list, nameRes);
  while (elem1 != NULL && elem2 != NULL) {
    flag = strcmp(elem1->data, elem2->data);
    if (flag < 0) {
      if (add(setRes, &end, elem1) == MEM_ERR) {//not enough memory
        SetDel(list, nameRes);
        return MEM_ERR;
      }
      elem1 = elem1->next;
    }
    else if (flag > 0) {
      if (add(setRes, &end, elem2) == MEM_ERR) {//not enough memory
        SetDel(list, nameRes);
        return MEM_ERR;
      }
      elem2 = elem2->next;
    }
    else {
      elem1 = elem1->next;
      elem2 = elem2->next;
    }
  }
  //add remaining elements
  if (elem1 != NULL)
    elemToAdd = elem1;
  if (elem2 != NULL)
    elemToAdd = elem2;
  while (elemToAdd != NULL) {
    if (add(setRes, &end, elemToAdd) == MEM_ERR) {//not enough memory
      SetDel(list, nameRes);
      return MEM_ERR;
    }
    elemToAdd = elemToAdd->next;
  }

  return TRUE;
}
//check the first set is included in the second
int SetEntry(set_t **list, const char *name1, const char *name2, const char *nameRes) {
  set_t *set1, *set2;
  elem_t *elem1, *elem2;
  int flag;

  nameRes;
  if (list == NULL || name1 == NULL || name2 == NULL)
    return ERR;

  set1 = SetFind(*list, name1);
  if (set1 == NULL)//set not found
    return ERR;

  set2 = SetFind(*list, name2);
  if (set2 == NULL)//set not found
    return ERR;

  elem1 = set1->head;
  elem2 = set2->head;
  while (elem1 != NULL && elem2 != NULL) {
    flag = strcmp(elem1->data, elem2->data);
    if (flag < 0) {
      return FALSE;
    }
    else if (flag > 0) {
      elem2 = elem2->next;
    }
    else {
      elem1 = elem1->next;
      elem2 = elem2->next;
    }
  }

  if (elem1 == NULL)
    return TRUE;
  else//part of the first set is not included in the second
    return FALSE;
}
//allows to get a pointer to a function with a given alias
int FindFunction(const char *name, function_t *res) {
  static const elem_function_t elemF[] = {
    {"elemAdd", ElementAdd, FALSE},
    {"elemDel", ElementDel, FALSE},
    {"elemCheck", ElementCheck, TRUE}
  };
  static const set_function_t setF[] = {
    {"setAdd", SetAdd, FALSE},
    {"setDel", SetDel, FALSE},
    {"setCard", SetCard, TRUE},
    {"setPrint",PrintElements,FALSE}
  };
  static const op_function_t opF[] = {
    {"union", SetUnion, FALSE},
    {"inter", SetInter, FALSE},
    {"dif", SetDif, FALSE},
    {"symDif", SetSymDif, FALSE},
    {"entry", SetEntry, TRUE},
  };
  int counter;

  if (name == NULL || res == NULL)
    return ERR;

  // Finding function in tables
  for (counter = 0; (unsigned)counter < sizeof(elemF) / sizeof(elemF[0]); counter++) {
    if ((_Bool)(strcmp(name, elemF[counter].name) == 0)) {
      res->type = FUNC_TYPE_ELEM;
      res->value.elem = &elemF[counter];
      return TRUE;
    }
  }
  for (counter = 0; (unsigned)counter < sizeof(setF) / sizeof(setF[0]); counter++) {
    if ((_Bool)(strcmp(name, setF[counter].name) == 0)) {
      res->type = FUNC_TYPE_SET;
      res->value.set = &setF[counter];
      return TRUE;
    }
  }
  for (counter = 0; (unsigned)counter < sizeof(opF) / sizeof(opF[0]); counter++) {
    if ((_Bool)(strcmp(name, opF[counter].name) == 0)) {
      res->type = FUNC_TYPE_OP;
      res->value.op = &opF[counter];
      return TRUE;
    }
  }

  res->type = FUNC_TYPE_UNKNOWN;
  return FALSE;
}