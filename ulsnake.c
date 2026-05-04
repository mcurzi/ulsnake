// Headers of integrated functions.
#include <curses.h>   // Library that includes functions for screen positioning and format printing.
#include <time.h>     // Necessary for 'time()'function.
#include <ctype.h>    // Necessary for 'tolower()' function.
#include <stdlib.h>   // Necessary for 'exit() function.
#include <string.h>   // Necessary for 'strlen() function.
#include "ulsnake.h"
#include "levels.h"

#define VERSION "0.1"

// Enumerates the names that will be assigned to the color pairs;
enum{White=1, Red, Blue, Cyan, Magenta, Yellow, Green, Black};

int endgame;     // Global variable.
int mat[40][80];  // Matrix to save the state of screen coordinates.

int main()
{
    int a, b=0;

    initscr();
    cbreak(); 
    noecho();
	keypad (stdscr, TRUE );     // Extended keyboard support.
    nodelay(stdscr, TRUE );     // Don't wait for a key to continue.

    CheckScreenSize();          // Checks the number of lines and columns of the terminal.  
    
     // Assigns fg and bg colors to the names declared earlier.
     start_color();
     init_pair(White, COLOR_WHITE, COLOR_BLACK);
     init_pair(Red, COLOR_RED, COLOR_BLACK);
     init_pair(Blue, COLOR_BLUE, COLOR_BLACK);
     init_pair(Cyan, COLOR_CYAN, COLOR_BLACK);
     init_pair(Magenta, COLOR_MAGENTA, COLOR_BLACK);
     init_pair(Yellow, COLOR_YELLOW, COLOR_BLACK);
     init_pair(Green, COLOR_GREEN, COLOR_BLACK);
     init_pair(Black, COLOR_BLACK, COLOR_BLACK);
    
     while(b!=1){
        a=game();
        if (a==0) b=gameover();
        else if (endgame==3) b=1;
    }
    endwin();
    return 0;
}

/**********************************************************/

// This function shows the initial screen with the game options.
void options(int *speed, int *incspeed, int *cheats)
{
	int i, input=0, option=0;

	int numspeed[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

	clear();

	while (!option)
	{
		input=tolower(getch());
		
			switch (input)
	{
	case '\n':
		option=1;
		break;

	case 'q':
        endgame=3; option=1;
		break; 

	case '@': 
	    *cheats=1;
	    break;
	
	case KEY_UP: case 'w':
        if(*incspeed==1) *incspeed=0;
		break;

	case KEY_DOWN: case 's':
        if(*incspeed==0) *incspeed=1;
		break;

	case KEY_LEFT: case 'a':
		if (*speed > 1)
			(*speed)--;
		break;

	case KEY_RIGHT: case 'd':
		if (*speed < 9)
			(*speed)++;
		break;

	case '1':
		*speed = 1;
		break;
	case '2':
		*speed = 2;
		break;
	case '3':
		*speed = 3;
		break;
	case '4':
		*speed = 4;
		break;
	case '5':
		*speed = 5;
		break;
	case '6':
		*speed = 6;
		break;
	case '7':
		*speed = 7;
		break;
	case '8':
		*speed = 8;
		break;
	case '9':
		*speed = 9;
		break;

	default:
		break;
	}
		
		tcolor(1);
		mvprintw (6, 14, " ___________________________________________________ ");
		for(i=7;i<22;i++)
		   mvprintw (i, 14, "|                                                   |");
		mvprintw (22, 14, "|___________________________________________________|");
		tcolor(6); mvprintw( 8,20, "A simple Snake game for Unix-like systems");
		tcolor(6); mvaddch (9, 38, 'v'); mvprintw (9, 39, VERSION);
		tcolor(7);
		mvprintw (12, 20, "Use arrow keys to select speed options.");
		mvprintw (13, 20, "Press <Enter> to start game.");
		mvprintw (14, 20, "Press <q> to quit game.");

		// Select snake speed.
		tcolor(2);
		mvprintw (16, 20, "Starting speed for each level:");

		// Draw the speed levels with the right colors.
		int j;
		for (i = 0, j = 0; i < 9; i++)
		{
			if (i == (*speed-1))
				tcolor(7);
			else
				tcolor(3);

			mvprintw (17, 20+j, "%d", numspeed[i]);
			j += 2;
		}

		// Increasing speed option.
		tcolor(2); mvprintw (19, 20, "Increase level speed while playing?");
		if (*incspeed == 1)
		{
			tcolor(3);
			mvprintw (19, 56, "No");

			tcolor(7);
			mvprintw (20, 56, "Yes");
		}
		else
		{
			tcolor(7);
			mvprintw (19, 56, "No");

			tcolor(3);
			mvprintw (20, 56, "Yes");
		}


		curs_set(0); 
	}

}

/**************************************************************/

int game()
{
    int a=0,b;         // Counters and auxiliary variables.
    int dir,x,y;       // Snake movement direction and screen coordinates.
    int bodylength;    
    int trailx[400];   // These two arrays save the coordinates trough which the
    int traily[400];   // snake has passed; they are used to draw the snake body.
    int stepcount;     // Counter to delete snake 'trail'.
    int num,nx,ny;     // Value and coordinates of the number that appears on the screen.
    int levelnum=1,lives=5,speed=1,gamedelay;
    int cheats=0,incspeed=0,score=0,scorecount=0;

    srand(time(0));  // Generates a random seed for rand().

    endgame=0;

  options(&speed,&incspeed,&cheats);
  
  // 'endgame' is 2 if there are no more lives or if the last level is completed.
  while (endgame<2){
    stepcount=0;
    bodylength=4;
    num=1;
    endgame=0;
    gamedelay=270-27*speed+speed*3;

    // The 'mat' array is a matrix that saves values for each coordinate of the screen.
    // if the screen is clear, mat[y][x] is 0.
    for(b=0;b<=39;b++)
        for(a=0;a<=79;a++)
            mat[b][a]=0;

    // Sets 'trailx[]' and 'traily[]' to 0.
    for(b=0;b<401;b++)
       {trailx[b]=0; traily[b]=0;}

    // Loads level, snake position and movement direction.
    levels(levelnum, &x, &y, &dir);
    
    curs_set(0); // Hides the cursor.
    
    // Prints score, credits and a message.
    tcolor(7); move(39,1); printw("Unix-like Snake v0.1");
    tcolor(7); move(39, 62); printw("by Matias J. Curzi");
    tcolor(7); move(0,21); printw("<Level %02d>   <Lives %02d>   <Score %05d>", levelnum, lives, score);
    
    message(" Press SPACE to start the level... ",32);

    // Prints a number in a random position of the screen in which mat[y][x] is 0.
    do{
        nx=rand()%77+2;
        ny=rand()%37+1;
    } while(mat[ny][nx]!=0);
    
    // Here starts the dynamic part of the game.
    while (!endgame){

        tcolor(6); move(ny,nx); printw("%d",num);  // Prints the number to collect.

        snake(y,x,1);         // Draws the snake head.
        napms(gamedelay);   // Regulates movement speed.       
       
        //Record places where the snake has passed (used to build the body).
        trailx[stepcount]=x; 
        traily[stepcount]=y;
        stepcount++;

        // This section analyzes input from the keyboard.
        a=-1; // Deactivates 'a'to delete the last input.
        a = tolower(getch());
        switch (a) {
            case KEY_UP: case 'w':
            if(dir!=2) dir=1; break;
            case KEY_DOWN: case 's':
            if(dir!=1) dir=2; break;
            case KEY_LEFT: case 'a':
            if(dir!=4) dir=3; break;
            case KEY_RIGHT: case 'd':
            if(dir!=3) dir=4; break;
            case 32: case 'p':
            message("Pause... Press SPACE to resume.",32);
            break;

            case 'c':                    // Key 'c' speeds up snake movement by 20%.
            if (cheats==1) gamedelay*=.8;
            break;
            case 'z':                    // Key 'z' slows down snake movement by 20%.
            if (cheats==1) gamedelay*=1.2;
            break;
            case 'n':                    // 'n' moves to the next level.
            if (cheats==1) {levelnum++; endgame=1; break;}
            case 'u':                    // 'u' adds one life.
            if (cheats==1) lives++;
            tcolor(7); move(0,21);
            printw("<Level %02d>   <Lives %02d>   <Score %05d>", levelnum, lives, score);
            break;
            case 'q':
            message("Quit game? (Y/N)",-1);
            break;
        }
         
        snake(y,x,0);         // Draws the snake body. 
        // With each iteration, snake coordinates change
        // depending value of 'dir', causing the snake to move.
        if (dir==1) y--;
        if (dir==2) y++;
        if (dir==3) x--;
        if (dir==4) x++;
    
        // This section activates when the snake picks up a number.
        if(x==nx && y==ny){
            score=score+num*10;
            scorecount=scorecount+num*10;
            if (scorecount>=2000) {lives++;scorecount=0;}       // Extra life every 2000 points.
            tcolor(7); move(0,21);
            printw("<Level %02d>   <Lives %02d>   <Score %05d>", levelnum, lives, score);
            num++;
            bodylength+=(4*num);
            if(num>9){
                snake(y,x,1);
                message("LEVEL COMPLETED",0);
                levelnum++;
                endgame=1;
		        continue;
            }
            else {
                do{
                    nx=rand()%77+2;
                    ny=rand()%37+1;
                } while(mat[ny][nx]!=0);
                tcolor(6);move(ny,nx);printw("%d",num);
                if(incspeed==1) gamedelay *=.85;  // Speed increases 10% with each number.
            }
        }
        
        // This section details what to do when the snake crashes.
        // If the snake reaches coordinates in which 'mat' is not 0, it dies.
        if (mat[y][x]!=0){
            lives--;
            tcolor(7); move(0,21);
            printw("<Level %02d>   <Lives %02d>   <Score %05d>", levelnum, lives, score);
            message("YOU DIED!",0);
            (lives)?(endgame=1):(endgame=2);
        }

        // Deletes snake body to match the length specified in 'bodylength'.
        if (stepcount>=bodylength){
            move(traily[stepcount-bodylength],trailx[stepcount-bodylength]);printw(" ");
            mat[traily[stepcount-bodylength]][trailx[stepcount-bodylength]]=0;
        }

        // If the snake moves a number of steps that duplicates the length, 
        // of the body, the loop copies the second half of 'trailx' and 
        // 'traily' arrays to the beginning, mantaining only those coordinates
        // in which there is part of the snake body. This is to save memory.
         if(stepcount==bodylength*2){
             for(b=0;b<=bodylength;b++){
                 trailx[b]=trailx[b+bodylength];
                 traily[b]=traily[b+bodylength];
             }
            stepcount=bodylength;
         }

    } // End of the dynamic part of the game.

    if (levelnum==13) {
        message("Congratulations, you finished the game!!",0);
        message("Press SPACE to return to the main menu.",32);
        endgame=2;
    }
  }
return lives;
}

/**********************************************************/


void snake(int row, int col, int head)
{
   // Draws the snake in the given coordinates.
   // It also updates the screen state matrix.
   move(row,col); 
   if(head) {tcolor(1);addch('0');} else {tcolor(2); addch('O');}
   mat[row][col]=2;
   // Updates the screen immediately (before x and y coordinates change).
   refresh();
}

/**********************************************************/

int gameover(void)
{
    int a, end=0;

    clear();
    tcolor(7);move(19,7);
    printw("You lost. Press ENTER to play again or or 'q' to exit.");
    while (!end){
        a = getch();
        if(a=='q') {end=1;}
        if(a==10) {end=2;}
    }
return end;
}

/**********************************************************/

void message(char text[81], int key)
{
    int a,b;

    tcolor(1);
    move(17,18), printw("#############################################");
    move(18,18); printw("#                                           #");
    move(19,18); printw("#                                           #");
    move(20,18); printw("#                                           #");
    move(21,18); printw("#############################################");
    // Centers the message text within the box.
    tcolor(7); move(19,40-strlen(text)/2); printw("%s", text);
    refresh();
    
    if (key>0)
        while((a=getch())!=key);
    else if (key<0)
        {while((a=getch())<0); if(tolower(a)=='y') endgame=3;}
    else
        napms(2000);

    // Restores what was behind the message.
    for (b=17;b<=21;b++) {
        for(a=18;a<=66;a++){
            move(b,a);
            if (mat[b][a]==0) printw(" ");  // If mat es 0 the place was clear.
            if (mat[b][a]==1) gwall(b,a);   // If mat is 1 there was a 'brick' of a wall.
            if (mat[b][a]==2) snake(b,a,0);   // If mat is 2 there was a part of the snake.
        }
    }
}

/**********************************************************/

void tcolor(int c)
{
	
	switch (c){
	case 0: attron(COLOR_PAIR(Black)); break;
	case 1: attron(COLOR_PAIR(Red)); break;
	case 2: attron(COLOR_PAIR(Green)); break;
    case 3: attron(COLOR_PAIR(Yellow)); break;
	case 4: attron(COLOR_PAIR(Blue)); break;
	case 5: attron(COLOR_PAIR(Magenta)); break;
	case 6: attron(COLOR_PAIR(Cyan)); break;
	case 7: attron(COLOR_PAIR(White)); break;
	default: attron(COLOR_PAIR(Black)); break;
     }
}

/**********************************************************/

void CheckScreenSize() {
        //Checks console screen dimensions.
        int h, w;
        getmaxyx(stdscr, h, w);
        if((h < 40) || (w < 80)) {
                endwin();
                fprintf(stderr, "\n    The console screen must have at least 40 lines and 80 columns.");
                fprintf(stderr, "\n    Please resize your window and restart the program.\n\n");
                exit(0);
        }
}

/**********************************************************/
