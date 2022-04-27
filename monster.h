#ifndef MONSTER_H
#define MONSTER_H

typedef struct item {
  int hpdelta;
  int damagedelta;
  struct item *next;
  char *name;
} item;

typedef struct monster {
  int hp;
  int damage;
  item *inventory;
  char *name;
  int stage;
  int stages;
  char **stagetext;
  char *death;
} monster;

item *item_New(int hpdelta, int damagedelta, const char name[]);
void item_Destroy(item *item);
int item_Append(item **list, item *new);
void item_Remove(item **list, int entry);
void item_ChainDestroy(item *list);
void printitems(item *inv);
item *item_Nth(item *list, int num);
monster *monster_New(int hp, int damage,
		     item *inventory, char *name,
		     char *death, int stages, char **stagetext);
void monster_Delete(monster *mon);
int damage(monster *from, monster *to);
void useitem(monster *to, item *item);

#endif
