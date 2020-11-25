#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "listset.h"

// main function to test the listset ADT
int main(int argc, char ** argv) {
  struct listset * myset;
  struct listset* myset1;
  struct listset* myset2;
  struct listset* dst;

  if ( argc < 2 ) {
    fprintf(stderr, "Usage: ./listset <command> [<string>]\n");
    fprintf(stderr, "       where <command> is + (add following string)");
    fprintf(stderr, "                          - (remove following string)");
    fprintf(stderr, "                          = (print set)");
    exit(1);
  }

  // create a set and follow the commands
  if(argv[1][0] == '+' || argv[1][0] == '-')
  {
      myset = listset_new();
      int i = 1;
      while (i < argc)
      {
          if (argv[i][0] == '+') {
              assert(i + 1 < argc);
              listset_add(myset, argv[i + 1]);
              i = i + 2;
          } else if (argv[i][0] == '-') {
              assert(i + 1 < argc);
              listset_remove(myset, argv[i + 1]);
              i = i + 2;
          } else if (argv[i][0] == '=') {
              listset_print(myset);
              i++;
          } else {
              fprintf(stderr, "Error 01: Unknown command %s\n", argv[i]);
              exit(1);
          }
      }

  }
  else if(argv[1][0] == 'o' && argv[1][1] == 'p' && argv[1][2] == 's')
  {
      myset1 = listset_new();
      myset2 = listset_new();
      int i = 2;

      while(i < argc)
      {
          if (argv[i][0] == '+') {
              assert(i + 1 < argc);
              listset_add(myset1, argv[i + 1]);
              i = i + 2;
          }
          else if (argv[i][0] == '-') {
              assert(i + 1 < argc);
              listset_remove(myset1, argv[i + 1]);
              i = i + 2;
          }
          else if(argv[i][0] == 'X')
          {
              i++;
              break;
          }
          else {
              fprintf(stderr, "Error 02: Unknown command %s\n", argv[i]);
              exit(1);
          }
      }

      printf("Set 1 length = %d\n", listset_cardinality(myset1));

      listset_print(myset1);

      while(i < argc)
      {
          if (argv[i][0] == '+') {
              assert(i + 1 < argc);
              listset_add(myset2, argv[i + 1]);
              i = i + 2;
          }
          else if (argv[i][0] == '-') {
              assert(i + 1 < argc);
              listset_remove(myset2, argv[i + 1]);
              i = i + 2;
          }
          else {
              fprintf(stderr, "Error 03: Unknown command %s\n", argv[i]);
              exit(1);
          }
      }

      printf("Set 2, length = %d\n", listset_cardinality(myset2));

      listset_print(myset2);

      printf("Union\n");

      dst = listset_new();
      listset_union(dst,myset1,myset2);

      listset_print(dst);

      printf("Intersection\n");

      dst = listset_new();
      listset_intersect(dst,myset1,myset2);

      listset_print(dst);

  }
  else
  {
      fprintf(stderr, "Error 04: Unknown command %s\n", argv[1]);
      exit(1);
  }

    
  return 0;
}
