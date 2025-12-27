#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Task
{
  char *task;
  int completed; // 1 for completed, 0 for not completed
};

struct Task *tasks = NULL;
size_t task_count = 0;

int main()
{
  return 0;
}