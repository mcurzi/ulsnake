// Core functions:
int main(void);
void options(int *speed, int *incspeed, int *cheats);
int game(void);
void snake(int row, int col, int head);
int gameover(void);
void message(char text[81], int key);
void tcolor(int c);  
void CheckScreenSize();

extern int mat[40][80];  // Matrix to save the state of screen coordinates.
