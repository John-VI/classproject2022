#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITBUFFSIZE 256

#define getline(lineptr, n, stream) getdelim(lineptr, n, '\n', stream)

typedef struct item {
  int hpdelta;
  int damagedelta;
  struct item *next;
  char *name;
} item;

typedef struct monster {
  int maxhp;
  int hp;
  int damage;
  item *inventory;
  char *name;
} monster;

ssize_t getdelim (char **lineptr, size_t *n, int delimiter, FILE *stream) {
  if (stream == NULL)
    return -1;
  /* if (*n < 0 || n == NULL) */
  /*   return -1; */
  if (lineptr == NULL)
    return -1;
  
  if (*n == 0) {
    *lineptr = malloc(INITBUFFSIZE * sizeof(char));
    if (!*lineptr)
      return -1;
    else
      *n = INITBUFFSIZE;
  }

  char c;
  ssize_t i = 0;

  if ((c = fgetc(stream)) == EOF)
    return -1;

  do {
    if (i == INITBUFFSIZE - 2)
      *lineptr = realloc(*lineptr, *n = (*n + INITBUFFSIZE) * sizeof(char));
    (*lineptr)[i] = c;
    i++;
  } while ((c = fgetc(stream)) != delimiter && c != EOF);
  
  (*lineptr)[i] = delimiter;
  (*lineptr)[++i] = '\0';
  return i;
}

char *chomp(char *str) {
  int i = 0;
  while (str[i] != '\0')
    i++;
  if (str[i - 1] == '\n') {
    str[i - 1] = '\0'; // This leaves an extra trailing null byte
  }                    // but I really don't think it matters for
  return str;          // this, I can live without thirty bytes.
}                      

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
  if (!inv) {
    puts("No items.");
    return;
  } else
    do {
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

monster *monster_New(int maxhp, int hp, int damage,
		     item *inventory, char *name) {
  monster *output = (monster *)malloc(sizeof(monster));
  output->maxhp = maxhp;
  output->hp = hp;
  output->damage = damage;
  output->inventory = inventory;
  output->name = name;

  return output;
}

void monster_Delete(monster *mon) {
  item_ChainDestroy(mon->inventory);
  free(mon);
}

int damage(monster *from, monster *to) {
  to->hp -= from->damage;
  if (to->hp <= 0)
    printf("%s is dead.\n", to->name);
  return to->hp;
}

int main(int argc, char *argv[]) {
  puts("Welcome to MythicQuest.");
  printf("Your name is...\n> ");
  char *name = NULL;
  size_t namelen = 0;
  getline(&name, &namelen, stdin);
  printf("a - Attack\ni - Item\nq - Quit\n");
  monster *player = monster_New(100, 100, 5, NULL, chomp(name));
  monster *enemy = monster_New(100, 100, 2, NULL, "Enemy");
  int quit = 0;
  char input;
  do {
    printf("[aiq]> ");
    do {
      input = getc(stdin);
      switch (input) {
      case 'a':
	damage(player, enemy);
	goto getout;
      case 'i':
	printitems(player->inventory);
	goto getout;
      case 'q':
	quit = 1;
	goto getout;
      case '\n':
	break;
      default:
	printf("\n[aiq]> ");
      } 
    } while (input != EOF);
  getout:
    if (quit)
      break;
    damage(enemy, player);
    printf("%s: %d HP\n", player->name, player->hp);
    printf("%s: %d HP\n", enemy->name, enemy->hp);
  } while (player->hp);
  return 0;
}
