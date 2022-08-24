#define _CRTDBG_MAP_ALLOC
#include "func.h"
#include <crtdbg.h>

#define WRONG_FORMAT printf("\tWrong format, use help to learn more\n")
#define SKIP(sym) while (sym != '\n') { sym = (char)getchar(); } //skip the rest of the line
#define CHECK_FORMAT(sym, sign, i) if (sym != sign || i <= 0) { SKIP(sym) WRONG_FORMAT; return; }
#define MEMORY printf("\tNot enough memory\n")
#define NAME printf("\tUnknown name\n")
#define SET_EXISTS printf("\tSet with the same name exists\n");

//read a set name or element
int ReadOperand(char str[], char *last, int maxSize) {
  char ch, sign = ' ';
  int size = 0;
  
  if ((ch = (char)getchar()) == '"') {//if the operand is compound
    sign = '"';
    ch = (char)getchar();
  }
  //read until it's a sign of the end
  while (size < (maxSize - 1) && ch != sign && ch != '"' && ch != '\n') {
    str[size] = ch;
    size++;
    ch = (char)getchar();
  }

  str[size] = '\0';
  //check format
  if (ch == '"' && sign == '"')
    ch = (char)getchar();
  else if (sign == '"')
    size = 0;
  else if (ch == '"')
    size = 0;
  *last = ch;

  return size;
}
//handle the case of working with a set element
void ElemFuncHandler(const elem_function_t *func, set_t *list) {
  char name[MAX_NAME_SIZE], data[MAX_ELEM_SIZE], ch;
  int i, flag;
  set_t *set;

  i = ReadOperand(name, &ch, MAX_NAME_SIZE);
  CHECK_FORMAT(ch, ' ', i)
  i = ReadOperand(data, &ch, MAX_ELEM_SIZE);
  CHECK_FORMAT(ch, '\n', i)
  set = SetFind(list, name);
  //try to execute command
  if (set != NULL) {
    flag = func->function(set, data);
    if (func->output) {//if ElementCheck was called
      if (flag)
        printf("\tTrue\n");
      else
        printf("\tFalse\n");
    }
    else {
      if (flag == MEM_ERR)//not enough memory
        MEMORY;
    }
  }
  else {//set not found
    NAME;
  }
}
//handle the case of working with a set
void SetFuncHandler(const set_function_t *func, set_t **list) {
  char name[MAX_NAME_SIZE], ch;
  int i, flag;

  i = ReadOperand(name, &ch, MAX_NAME_SIZE);
  CHECK_FORMAT(ch, '\n', i)
  //try to execute command
  flag = func->function(list, name);
  if (flag == ERR) {//set not found
    NAME;
    return;
  }
  if (func->output) {//if SetCard was called
    printf("\t%d\n", flag);
  }
  else {
    if (flag == MEM_ERR) {//not enough memory
      MEMORY;
      return;
    }
    if (flag == NAME_ERR)//set with the same name already exists
      SET_EXISTS;
  }
}
//handle the case of working with set operations
void OpFuncHandler(const op_function_t *func, set_t **list) {
  char name1[MAX_NAME_SIZE], name2[MAX_NAME_SIZE], nameRes[MAX_NAME_SIZE], ch;
  int i, flag;

  i = ReadOperand(name1, &ch, MAX_NAME_SIZE);
  CHECK_FORMAT(ch, ' ', i)
  i = ReadOperand(name2, &ch, MAX_NAME_SIZE);
  if (func->output) {//if SetEntry was called
    CHECK_FORMAT(ch, '\n', i)
    //try to execute command
    flag = func->function(list, name1, name2, NULL);
    if (flag)
      printf("\tTrue\n");
    else
      printf("\tFalse\n");
  }
  else {
    CHECK_FORMAT(ch, ' ', i)
    i = ReadOperand(nameRes, &ch, MAX_NAME_SIZE);
    CHECK_FORMAT(ch, '\n', i)
    //try to execute command
    flag = func->function(list, name1, name2, nameRes);
    if (flag == MEM_ERR) {//not enough memory
      MEMORY;
      return;
    }
    if (flag == ERR) {//set not found
      NAME;
      return;
    }
    if (flag == NAME_ERR)//set with the same name already exists
      SET_EXISTS;
  }
}
//display help
void PrintHelp(void) {
  printf("Help:\n");
  printf("\tThe maximum name size must not exceed 100 characters\n");
  printf("\tThe maximum element size must not exceed 80 characters\n");
  printf("\tNames and elements must not contain the \"\n");
  printf("\tAll names and elements containing more than one word must be enclosed in \"\n");
  printf("Possible commands:\n");
  printf("\thelp - to display help\n");
  printf("\texit - to end the session\n");
  printf("\tprint - to display the list of sets\n");
  printf("\tsetAdd <set name> - to add the set\n");
  printf("\tsetDel <set name> - to delete the set\n");
  printf("\tsetCard <set name> - to output the cardinality of the set\n");
  printf("\tsetPrint <set name> - to display all elements of the set, does nothing for an empty set\n");
  printf("\telemAdd <set name> <element> - to add the element to the set\n");
  printf("\telemDel <set name> <element> - to delete the element from the set\n");
  printf("\telemCheck <set name> <element> - outputs true if the element is contained in the set\n");
  printf("\tunion <first set name> <second set name> <result name> - to calculate the union, the result is added to the list of sets\n");
  printf("\tinter <first set name> <second set name> <result name> - to calculate the intersection, the result is added to the list of sets\n");
  printf("\tdif <first set name> <second set name> <result name> - to calculate the set difference, the result is added to the list of sets\n");
  printf("\tsymDif <first set name> <second set name> <result name> - to calculate the symmetric set difference, the result is added to the list of sets\n");
  printf("\tentry <first set name> <second set name> - outputs true if the first set is included in the second\n");
}
//read a comand
int ReadComand(char comand[], char *last) {
  char ch;
  int size = 0;

  ch = (char)getchar();
  //read until it's a sign of the end
  while (size < (MAX_FUNC_NAME_SIZE - 1) && ch != ' ' && ch != '\n') {
    comand[size] = ch;
    size++;
    ch = (char)getchar();
  }

  comand[size] = '\0';
  *last = ch;

  return size;
}

int main(void) {
  char comand[MAX_FUNC_NAME_SIZE], ch;
  function_t func;
  int i;
  set_t *list;

  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);

  list = InitList();
  while (TRUE) {
    i = ReadComand(comand, &ch);
    //check comand format
    if (ch != ' ' || i <= 0) {
      if (!strcmp(comand, "help")) {//display help
        PrintHelp();
      }
      else if (!strcmp(comand, "exit")) {//end the session
        ListClear(&list);
        break;
      }
      else if (!strcmp(comand, "print")) {//display the list of sets
        PrintSets(list);
      }
      else {//report wrong format
        WRONG_FORMAT;
        SKIP(ch)
      }
    }
    else {//try to execute command
      FindFunction(comand, &func);
      switch (func.type) {
        case FUNC_TYPE_ELEM:
          ElemFuncHandler(func.value.elem, list);
          break;
        case FUNC_TYPE_SET:
          SetFuncHandler(func.value.set, &list);
          break;
        case FUNC_TYPE_OP:
          OpFuncHandler(func.value.op, &list);
          break;
        default://report unknown command
          printf("\tUnknown comand, use help to learn more\n");
          SKIP(ch)
          break;
      }
    }
  }

  return 0;
}