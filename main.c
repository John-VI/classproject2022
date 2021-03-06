#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "monster.h"

#define INITBUFFSIZE 256

#define getline(lineptr, n, stream) getdelim(lineptr, n, '\n', stream)

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

int main(int argc, char *argv[]) {
  puts("Welcome to MythicQuest.");
  printf("Your name is...\n> ");
  char *name = NULL;
  size_t namelen = 0;
  getline(&name, &namelen, stdin);
  printf("a - Attack\nh - Heal\nq - Quit\n");
  char *pstages[1] = { "Go!\n" };
  char *estages[3] = { "We will carry out the new creation of destruction through the power of righteousness.\n", "We will remove the unnecessary borders from the earth.\n", "The world will change.\n" };
  monster *player = monster_New(400, 5, NULL, chomp(name),
				"Dead FOREVER\n", 1, pstages);
  monster *enemy = NULL;
  int quit = 0;
  char input;
  int failure = 0;
  int enemydelta = 0;
  do {
    monster_Delete(enemy);
    enemydelta++;
    enemy = monster_New(50 + (2 << enemydelta), 2 + (2 << enemydelta), NULL, "Enemy",
			"This isn't over!", 3, estages);
    printf("Enemy power plus 2^%d!\n", enemydelta);
    do {
      printf("[ahq]> ");
      do {
	input = getc(stdin);
	switch (input) {
	case 'a':
	  damage(player, enemy);
	  printf("You attacked the beast!\n");
	  goto getout;
	case 'h':
	  player->hp += 5;
	  printf("Healed a bit.\n");
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
      if (quit) {
	failure = 1;
	break;
      }
      damage(enemy, player);
      printf("%s: %d HP\n", player->name, player->hp);
      printf("%s: %d HP\n", enemy->name, enemy->hp);
      if (player->hp <= 0) {
	failure = 1;
	break;
      } else if (enemy->hp <= 0)
	break;
    } while (player->hp);
  } while (!failure);
  printf("Better luck next time, %s.\n", player->name);
  monster_Delete(player);
  monster_Delete(enemy);
  return 0;
}
