#include <curses.h>
#include "ulsnake.h"
#include "levels.h"

# define BRICK '#'
# define WCOLOR 3

// Generic function to draw walls. Used with 'for' loops to define size,
// location and direction of the wall. The matrix 'mat' is updated to 1 in
// locations where a wall is placed, so that the snake crashes against them.
void gwall(int row, int col)
{
   tcolor(WCOLOR);
   move(row, col);
   addch(BRICK);
   mat[row][col]=1;
}


// This funcion draws horizontal walls of a specified length.
void hwall(int row, int col, int length)
{
   int h;
   tcolor(WCOLOR);
   move(row,col);
   for (h=0;h<length;h++) {
       addch(BRICK);
       mat[row][col+h]=1;
   }
}

// This funcion draws vertical walls of a specified length.
void vwall(int row, int col, int length)
{
   int v;
   tcolor(WCOLOR);
   for (v=0;v<length;v++) {
      move(row+v,col);
      addch(BRICK);
      mat[row+v][col]=1;
   }
}

// This funcion draws horizontal walls using dashed lines.
void dhwall(int row, int col, int length, int dash)
{
   int h;
   tcolor(WCOLOR);
   for (h=0;h<length;h+=(dash+1)) {
      move(row,col+h);addch(BRICK);
      mat[row][col+h]=1;
   }
}

// This funcion draws horizontal walls using dashed lines.
void dvwall(int row, int col, int length,int dash)
{
   int v;
   tcolor(WCOLOR);
   for (v=0;v<length;v+=(dash+1)) {
      move(row+v,col);
      addch(BRICK);
      mat[row+v][col]=1;
   }
}

// Creates the different screens (levels) of the game.
void levels(int levelnum, int *x, int *y, int *dir)
{
    int b;

    clear();

    // Draws the border of the screen.
    hwall(0,1,79);
    hwall(38,1,79);
    vwall(1,1,37);
    vwall(1,79,37);
    
    move(39,40); printw("^");
    move(19,0); printw(">");

    // Draws the inner walls for each level and sets the
    // initial position and direction of the snake.
    switch (levelnum) {
        case 1:
        hwall(19,25,31);
        vwall(12,40,15);
        *x=40;*y=32;*dir=4;
        break;

        case 2:
        vwall(1,22,18);
        vwall(20,58,18);
        hwall(28,2,38);
        hwall(10,41,38);
        *x=58;*y=5;*dir=3;
        break;

        case 3:
        for (b=10;b<=70;b+=10) vwall(9,b,21);
        *x=10;*y=33;*dir=4;
        break;

        case 4:
        hwall(8,11,59);
        hwall(30,11,59);
        vwall(9,11,7);
        vwall(23,11,7);
        vwall(9,69,7);
        vwall(23,69,7);
        hwall(16,11,10);
        hwall(22,11,10);
        hwall(16,60,10);
        hwall(22,60,10);
        hwall(16,27,27);
        hwall(22,27,27);
        *x=55;*y=12;*dir=3;
        break;
        
        case 5:
        vwall(11,12,17);
        vwall(11,68,17);
        hwall(9,14,53);
        hwall(29,14,53);
        *x=40;*y=19;*dir=4;
        break;

        case 6:
        hwall(13,2,26);
        hwall(25,2,26); 
        hwall(13,53,26);
        hwall(25,53,26);
        vwall(1,29,11);
        vwall(1,51,11); 
        vwall(27,29,11);
        vwall(27,51,11);    
        *x=40;*y=19;*dir=2;
        break;

        case 7:
        for(b=0;b<13;b+=2){ 
           gwall(7+b,4+b);
           gwall(7+b,16+b);
           gwall(7+b,28+b);
           gwall(7+b,52-b);
           gwall(7+b,64-b);
           gwall(7+b,76-b);
           gwall(19+b,16-b);
           gwall(19+b,28-b);
           gwall(19+b,40-b);
           gwall(19+b,40+b);
           gwall(19+b,52+b);
           gwall(19+b,64+b);
	    }
        *x=40;*y=34;*dir=3;
        break;

        case 8:
        vwall(1,40,18);
        vwall(20,40,18);
        vwall(1,26,14);
        vwall(16,26,7);
        vwall(24,26,14);
        vwall(1,54,14);
        vwall(16,54,7);
        vwall(24,54,14);
        vwall(1,13,12);
        vwall(14,13,11);
        vwall(26,13,12);
        vwall(1,67,12);
        vwall(14,67,11);
        vwall(26,67,12);      
        *x=7;*y=30;*dir=1;
        break;
   
        case 9:
        dhwall(12,3,75,1);
        dhwall(26,3,75,1);
        dvwall(2,20,35,1);
        dvwall(2,40,35,1);
        dvwall(2,60,35,1);
        *x=50;*y=34;*dir=1;
        break;
        
        case 10:
        hwall(4,12,17);
        hwall(4,32,17);
        hwall(4,52,17);
        hwall(4,2,7);
        hwall(4,72,7);
        hwall(14,12,17);
        hwall(14,32,17);
        hwall(14,52,17);
        hwall(14,2,7);
        hwall(14,72,7);
        hwall(24,12,17);
        hwall(24,32,17);
        hwall(24,52,17);
        hwall(24,2,7);
        hwall(24,72,7);
        hwall(34,12,17);
        hwall(34,32,17);
        hwall(34,52,17);
        hwall(34,2,7);
        hwall(34,72,7);                          
        hwall(9,2,17);
        hwall(9,22,17);
        hwall(9,42,17);
        hwall(9,62,17);
        hwall(19,2,17);
        hwall(19,22,17);
        hwall(19,42,17);
        hwall(19,62,17);    
        hwall(29,2,17);
        hwall(29,22,17);
        hwall(29,42,17);
        hwall(29,62,17);
        *x=10;*y=16;*dir=4;
        break;

        case 11:
        for(b=0;b<13;b++){ 
           gwall(7+b,41+b);
           gwall(19+b,53-b);
           gwall(7+b,53+b);
           gwall(19+b,65-b);
           gwall(7+b,65+b);
           gwall(19+b,77-b);
           gwall(19+b,3+b);
           gwall(7+b,15-b);
           gwall(19+b,15+b);
           gwall(7+b,27-b);
           gwall(19+b,27+b);
           gwall(7+b,39-b);         
	    }      
        *x=40;*y=4;*dir=2;
        break;  
 
        default:              // From level 12 on, all levels will have the same format.
        for(b=2;b<35;b+=4){ 
           dhwall(b,4,75,5);
           dhwall(b+2,7,75,5);
        }
        *x=40;*y=19;*dir=3;
        break;
   }
}
/**********************************************************/

