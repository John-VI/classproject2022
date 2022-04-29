#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "monster.h"

item *item_New(int hpdelta, int damagedelta, const char name[]) {
  item *output = malloc(sizeof(item));
  output->hpdelta = hpdelta;
  output->damagedelta = damagedelta;
  output->name = malloc((sizeof(char)) * (strlen(name) + 1));
  output->next = NULL;
  return output;
}

void item_Destroy(item *item) {
  free(item->name);
  free(item);
}

int item_Append(item **list, item *new) {
  if (!list) {
    *list = new;
    return 0;
  }
  int i = 0;
  item *inter = *list;
  while (inter->next) {
    inter = inter->next;
    i++;
  }
  inter->next = new;
  return i;
}

void item_Remove(item **list, int entry) {
  if (entry == 0) {
    item *inter = *list;
    *list = (*list)->next;
    item_Destroy(inter);
    return;
  }
  item *inter = *list;
  for (int i = 0; i < entry - 1; i++)
    inter = inter->next;

  item *removal = inter->next;
  inter->next = inter->next->next;
  item_Destroy(removal);
}

void item_ChainDestroy(item *list) {
  do {
    item *inter = list->next;
    item_Destroy(list);
    list = inter;
  } while (list);
}

void printitems(item *inv) {
  int i = 0;
  if (!inv) {
    puts("No items.");
    return;
  } else
    do {
      i++;
      printf("%s\n", inv->name);
      inv = inv->next;
    } while (inv);
}

item *item_Nth(item *list, int num) {
  int i;
  for (i = 0; i < num; i++)
    if ((list = list->next) == NULL)
      return NULL;
  return list;
}

monster *monster_New(int hp, int damage,
		     item *inventory, char *name,
		     char *death, int stages, char **stagetext) {
  monster *output = (monster *)malloc(sizeof(monster));
  output->hp = hp;
  output->damage = damage;
  output->inventory = inventory;
  output->name = name;
  output->death = death;
  output->stages = stages;
  output->stagetext = stagetext;
  output->stage = 0;

  return output;
}

void monster_Delete(monster *mon) {
  //item_ChainDestroy(mon->inventory);
  free(mon);
}

int damage(monster *from, monster *to) {
  to->hp -= from->damage;
  if (to->stage < to->stages) {
    printf("%s", to->stagetext[to->stage++]);
  }
  if (to->hp <= 0)
    printf("%s is dead.\n %s", to->name, to->death);
  return to->hp;
}

void useitem(monster *to, item *item) {
  to->hp += item->hpdelta;
  to->damage += item->damagedelta;
  item_Destroy(item);
}
