#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "hangman.h"

char g_word[WORD_SIZE];
char g_guesses[26];
int g_guessesCount = 0;
char *g_path;

void initialize(int argc, char **argv)
{
   if (argc < 2)
   {
      puts("No words input! Exitting...");
      exit(1);
   }

   srand(time(0));

   drawBanner();

   g_path = argv[1];
   pickWord();
}

bool checkGuess(char letter)
{
   for (int j = 0; j < strlen(g_word); j++)
      if (letter == g_word[j])
         return true;

   return false;
}

int countWrongGuesses()
{
   int errors = 0;

   for (int i = 0; i < g_guessesCount; i++)
      if (!checkGuess(g_guesses[i]))
         errors++;

   return errors;
}

int checkLoss()
{
   return countWrongGuesses() >= MAX_ERRORS;
}

int checkWin()
{
   for (int i = 0; i < strlen(g_word); i++)
      if (!checkPreviousGuesses(g_word[i]))
         return false;

   return true;
}

void drawBanner()
{
   printf("/****************/\n");
   printf("/  Hangman Game  /\n");
   printf("/****************/\n\n");
}

void guessLetter()
{
   char guess;
   printf("Pick a letter: ");
   scanf(" %c", &guess);
   guess = toupper(guess);

   if (checkGuess(guess))
   {
      printf("Good guess! The word contains the letter %c\n\n", guess);
   }
   else
   {
      printf("\nOops! The word does not have the letter %c\n\n", guess);
   }

   g_guesses[g_guessesCount] = guess;
   g_guessesCount++;
}

int checkPreviousGuesses(char letter)
{
   for (int j = 0; j < g_guessesCount; j++)
      if (g_guesses[j] == letter)
         return true;

   return false;
}

void drawHangman()
{
   printf("\e[1;1H\e[2J");

   int errors = countWrongGuesses();

   printf("  _______       \n");
   printf(" |/      |      \n");
   printf(" |      %c%c%c  \n", (errors >= 1 ? '(' : ' '), (errors >= 1 ? '_' : ' '), (errors >= 1 ? ')' : ' '));
   printf(" |      %c%c%c  \n", (errors >= 3 ? '\\' : ' '), (errors >= 2 ? '|' : ' '), (errors >= 3 ? '/' : ' '));
   printf(" |       %c     \n", (errors >= 2 ? '|' : ' '));
   printf(" |      %c %c   \n", (errors >= 4 ? '/' : ' '), (errors >= 4 ? '\\' : ' '));
   printf(" |              \n");
   printf("_|___           \n");
   printf("\n\n");

   for (int i = 0; i < strlen(g_word); i++)
      if (checkPreviousGuesses(g_word[i]))
         printf("%c ", g_word[i]);
      else
         printf("_ ");

   printf("\n\n");
}

void pickWord()
{
   FILE *f = fopen(g_path, "r");
   if (f == NULL)
   {
      printf("Words file not found! Exitting...\n\n");
      exit(1);
   }

   int wordAmount;
   fscanf(f, "%d", &wordAmount);

   int randomIndex = rand() % wordAmount;

   for (int i = 0; i <= randomIndex; i++)
      fscanf(f, "%s", g_word);

   fclose(f);
}

void addWord()
{
   char option;

   printf("Do you want to add a new word (S/N)? ");
   scanf(" %c", &option);
   option = toupper(option);

   if (option == 'S')
   {
      char newWord[WORD_SIZE];

      printf("Enter the new word: ");
      scanf("%s", newWord);

      int idx = 0;
      while (newWord[idx])
      {
         char upperChar = toupper(newWord[idx]);
         newWord[idx++] = upperChar;
      }

      FILE *f = NULL;

      f = fopen(g_path, "r+");
      if (f == NULL)
      {
         printf("Words file not found! Exitting...\n\n");
         exit(1);
      }

      int wordCount;
      fscanf(f, "%d", &wordCount);
      wordCount++;
      fseek(f, 0, SEEK_SET);
      fprintf(f, "%d", wordCount);

      fseek(f, 0, SEEK_END);
      fprintf(f, "\n%s", newWord);

      fclose(f);
   }
}

int main(int argc, char **argv)
{
   initialize(argc, argv);

   do
   {
      drawHangman();
      guessLetter();
   } while (!checkWin() && !checkLoss());

   printf("\e[1;1H\e[2J");

   if (checkWin())
   {
      printf("\nCongratulations, you won!\n\n");
      printf("The word was: %s\n\n", g_word);

      printf("       ___________      \n");
      printf("      '._==_==_=_.'     \n");
      printf("      .-\\:      /-.    \n");
      printf("     | (|:.     |) |    \n");
      printf("      '-|:.     |-'     \n");
      printf("        \\::.    /      \n");
      printf("         '::. .'        \n");
      printf("           ) (          \n");
      printf("         _.' '._        \n");
      printf("        '-------'       \n\n");

      addWord();
   }
   else
   {
      printf("\nTO THE HANG!\n");
      printf("The word was: %s\n\n", g_word);

      printf("    _______________       \n");
      printf("   /               \\     \n");
      printf("  /                 \\    \n");
      printf("//                   \\/\\\n");
      printf("\\|   XXXX     XXXX   | / \n");
      printf(" |   XXXX     XXXX   |/   \n");
      printf(" |   XXX       XXX   |    \n");
      printf(" |                   |    \n");
      printf(" \\__      XXX      __/   \n");
      printf("   |\\     XXX     /|     \n");
      printf("   | |           | |      \n");
      printf("   | I I I I I I I |      \n");
      printf("   |  I I I I I I  |      \n");
      printf("   \\_             _/     \n");
      printf("     \\_         _/       \n");
      printf("       \\_______/         \n");
   }
}