#define WORD_SIZE 20
#define MAX_ERRORS 5

void drawBanner();
void guessLetter();
void drawHangman();
void pickWord();
int checkWin();
int checkLoss();
int checkPreviousGuesses(char letra);
void addWord();