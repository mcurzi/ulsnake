// Encabezados de las funciones integradas.
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

// Declaracion de funciones.
void main(void);
int juego(void);
void niveles(int nivel, int *x, int *y, int *dir);
void pared(int col, int fil);
void gusano(int col, int fil);
int final(void);
void mensaje(char texto[81], int tecla);

int mat[81][51];  // Matriz que guarda el estado de cada coordenada x,y.
int salida;     // Variable global.

void main()
{
    int a, b=0;

    textmode(64); // Modo texto con 80 columnas y 50 filas.
    while(b!=1){
        a=juego();
        if (a==0) b=final();
        else b=1;
    }
    textmode(C80);  // Vuelve al modo 80x25.
    clrscr();
}
/**********************************************************/

int juego()
{
    int a=0,b,c;      // Contadores y variables auxiliares.
    int dir,x,y;      // Coordenadas y direccion del movimiento del gusano.
    int largo;        // Tamaño de la cola del gusano.
    int pasox[401];   // Guardan las coordenadas de los lugares por donde paso
    int pasoy[401];   // el gusano. Se usan para armar la cola del mismo.
    int pcont;        // Contador para borrar el rastro del gusano.
    int num,nx,ny;    // Coordenadas y valor del numero.
    int nivel=1,vidas=5,velocidad=0,retardo;
    int trucos=0,dinamica=2,puntaje=0,regalo=0;
    char entrada[10];

    salida=0;

    // Permite que el jugador elija la velocidad del juego.
    while (velocidad<1||velocidad>10) {
        clrscr();
        gotoxy(10,15);printf("Ingrese la velocidad inicial del gusano (1 a 10): ");
        gets(entrada);
        velocidad=atoi(entrada);
        if (velocidad==1357)
            {velocidad=3; trucos=2; break;} // Es el primer numero para los trucos.
    }

    while (dinamica==2) {
        gotoxy(47,20);printf("                                  ");
        gotoxy(10,20);printf("Velocidad estatica o dinamica? (E/D): ");
        gets(entrada);
        for(b=15;b<=65;b++) pared(b,49);
        if (entrada[0]=='D'||entrada[0]=='d') {dinamica=1;}
        if (entrada[0]=='E'||entrada[0]=='e') {dinamica=0;}
        if (entrada[1]!='\0') dinamica=2;
        if((atoi(entrada))==2468) {dinamica=1; break;}
    }

        if (trucos==2&&(atoi(entrada))==2468) // Activa los trucos.
            trucos=1;

  // Esta seccion se reinicia cuando se pasa de nivel
  // o se pierde una vida.
  while (salida!=2){
    pcont=0;
    largo=4;
    num=1;
    salida=0;
    retardo=500/velocidad;

    // El arreglo 'mat' es una matriz con las dimensiones de la pantalla,
    // vale 1 en aquellos lugares donde hay pared o gusano (sino vale 0).
    for(b=1;b<=50;b++)
        for(a=1;a<=80;a++)
            mat[a][b]=0;

    // Inicializa los arreglos 'pasox' y 'pasoy'
    for(b=0;b<401;b++)
       {pasox[b]=0; pasoy[b]=0;}

    // Carga el nivel, la posicion y la direccion inicial del gusano.
    niveles(nivel, &x, &y, &dir);

    // Mensaje de inicio de cada nivel.
    textcolor(11); gotoxy(2,50); cprintf("MJC Nibbles");
    textcolor(11); gotoxy(60,50); cprintf("(de Matias J. Curzi)");
    textcolor(15); gotoxy(20,1);
    cprintf("<Nivel %2d>   <Vidas %2d>   <Puntos %5d>", nivel, vidas, puntaje);
    mensaje(" Pulsa ESPACIO para iniciar el nivel... ",32);

    // Muestra el primer numero en pantalla, el mismo se ubica en
    // una posición aleatoria donde no haya ni pared ni gusano.
    randomize();
    do{
        nx=random(78)+2;
        ny=random(48)+2;
    } while(mat[nx][ny]!=0);
    textcolor(11);gotoxy(nx,ny);cprintf("%d",num);
    gotoxy (1,2);

    // Aca empieza la parte dinamica del juego.
    while (!salida){

        gusano(x,y);   // Muestra el gusano en la pantalla.
        pasox[pcont]=x;
        pasoy[pcont]=y;
        pcont++;

        // Esto pasa si se agarra un numero.
        if(x==nx && y==ny){
            puntaje=puntaje+num*10;
            regalo=regalo+num*10;
            if (regalo>=2000) {vidas++;regalo=0;}
            textcolor(15); gotoxy(20,1);
            cprintf("<Nivel %2d>   <Vidas %2d>   <Puntos %5d>", nivel, vidas, puntaje);
            num++;
            largo+=(4*num);
            if(num>9){
                mensaje("            NIVEL COMPLETADO            ",0);
                nivel++;
                salida=1;
            }
            else {
                do{
                    nx=random(78)+2;
                    ny=random(48)+2;
                } while(mat[nx][ny]!=0);
                textcolor(11);gotoxy(nx,ny);cprintf("%d",num);
                if(dinamica==1) retardo-=retardo/7;
            }
        }
        if(salida==1) continue;

        // Retraso para regular la velocidad del movimiento.
        delay(retardo);

        // Esto activa las teclas que se usan durante el juego.
        a=0; // Para que no se repita la orden en cada bucle.
        while (kbhit())
            a = getch();
        switch (a) {
            case 72:                   // 72: Flecha arriba.
            if(dir!=2) dir=1; break;
            case 80:                   // 80: Flecha abajo.
            if(dir!=1) dir=2; break;
            case 75:                   // 75: Flecha izquierda.
            if(dir!=4) dir=3; break;
            case 77:                   // 77: Flecha derecha.
            if(dir!=3) dir=4; break;
            case 32:
            mensaje("  PAUSA - Pulsa ESPACIO para seguir...  ",32);
            textcolor(11);gotoxy(nx,ny);cprintf("%d",num);
            break;

            case 97: case 65:           // Con 'a' o 'A' se acelera el juego.
            if (trucos==1) retardo-=retardo/5;
            break;
            case 100: case 68:          // Con 'd' o 'D' se baja la velocidad.
            if (trucos==1) {if (retardo<2000) retardo+=retardo/5;}
            break;
            case 62:                    // Con F4 se pasa de nivel.
            if (trucos==1) {nivel++; salida=1; break;}
            case 64:                    // Con F6 se agregan vidas.
            if (trucos==1) vidas++;
            textcolor(15); gotoxy(20,1);
            cprintf("<Nivel %2d>   <Vidas %2d>   <Puntos %5d>", nivel, vidas, puntaje);
            break;
            case 27:
            mensaje("    Queres abandonar el juego? (S/N)    ",-1);
            textcolor(11);gotoxy(nx,ny);cprintf("%d",num);
            break;
        }

        // Esto cambia las coordenadas del gusano en cada
        // iteraccion segun el estado de la variable 'dir'.
        if (dir==1) y--;
        if (dir==2) y++;
        if (dir==3) x--;
        if (dir==4) x++;

        // Esto pasa si se choca, es decir, si pasa por aquellos lugares de
        // la matriz que tienen valor 1 (las paredes y el gusano mismo).
        if (mat[x][y]!=0){
            vidas--;
            textcolor(15); gotoxy(16,1);
            // cprintf("<Nivel: %2d> <Vidas: %2d> <Puntaje: %5d>", nivel, vidas, puntaje);
            mensaje("              CHOCASTE!!!               ",0);
            (vidas)?(salida=1):(salida=2);
        }

        // Va borrando la cola para que mida lo que indica 'largo'.
        if (pcont>=largo){
            gotoxy(pasox[pcont-largo],pasoy[pcont-largo]);printf(" ");
            mat[pasox[pcont-largo]][pasoy[pcont-largo]]=0;
            textcolor(11);gotoxy(nx,ny);cprintf("%d",num);} // Por las dudas.

        // Copia los ultimos valores de los arreglos 'pasox' y 'pasoy' al
        // princico de los mismos, y retrocede el contador. De esa forma
        // se ahorra mucha memoria ya que los arreglos no necesitan tantos
        // elementos (sino habría un elemento por cada paso del gusano).
        if(pcont>=(largo*2)){
            for(b=0;b<=largo;b++){
                pasox[b]=pasox[b+largo];
                pasoy[b]=pasoy[b+largo];
            }
            pcont=largo;
        }

    } // Fin de la parte dinamica del juego.

    if (nivel==16) {
        mensaje("Terminaste el Juego!!!  Pulsa ESCAPE...",27);
        vidas=0;
        salida=2;
    }
  }
return vidas;
}
/**********************************************************/

void pared(int col, int fil)
{
   // Dibuja paredes en los lugares que se le indica,
   // tambien actualiza la matriz 'mat' para que el
   // gusano se choque con las paredes dibujadas.
   textcolor(14);
   gotoxy(col, fil);
   cprintf("%c",-37);
   mat[col][fil]=1;
}
/**********************************************************/

void gusano(int col, int fil)
{
   // Dibuja el gusano en las coordenadas indicadas.
   // Tambien actualiza la matriz.
   textcolor(12);
   gotoxy(col, fil); cprintf("%c",2);
   gotoxy(1,2);
   mat[col][fil]=2;
}
/**********************************************************/

void niveles(int nivel, int *x, int *y, int *dir)
{
    int b;

    textbackground(1);
    clrscr();

    // Crea el borde de la pantalla.
    for(b=1;b<=80;b++)
        {pared(b,2); pared(b,49);}
    for(b=2;b<=49;b++)
        {pared(1,b); pared(80,b);}

    // Crea las paredes internas para cada nivel, tambien señala
    // la posicion y la direccion inicial del gusano.
    switch (nivel) {

        case 1:
        for(b=25;b<=55;b++)
            pared(b,25);
        for(b=13;b<=38;b++)
            pared(40,b);
        *x=50;*y=30;*dir=2;
        break;

        case 2:
        for(b=2;b<=28;b++)
            {pared(20,b); pared(60,51-b);}
        for(b=2;b<=40;b++)
            {pared(b,36); pared(81-b,15);}
        *x=60;*y=7;*dir=3;
        break;

        case 3:
        for(b=10;b<=70;b++)
            pared(b, 25);
        for(b=10;b<=41;b++) {
            pared(10,b);
            pared(20,b);
            pared(30,b);
            pared(40,b);
            pared(50,b);
            pared(60,b);
            pared(70,b);
        }
        *x=70;*y=47;*dir=3;
        break;

        case 4:
        for(b=12;b<=39;b++)
            {pared(21,b); pared(59,b);}
        for(b=23;b<=57;b++)
            {pared(b,10); pared(b,41);}
        *x=50;*y=25;*dir=1;
        break;

        case 5:
        for(b=10;b<=20;b++)
            {pared(b,22); pared(b,29);}
        for(b=30;b<=50;b++)
            {pared(b,22); pared(b,29);}
        for(b=60;b<=70;b++)
            {pared(b,22); pared(b,29);}
        for(b=10;b<=70;b++)
            {pared(b,12); pared(b,39);}
        for(b=12;b<=22;b++)
            {pared(10,b); pared(70,b);}
        for(b=29;b<=39;b++)
            {pared(10,b); pared(70,b);}
        for(b=22;b<=24;b++)
            {pared(20,b); pared(60,b);}
        for(b=27;b<=29;b++)
            {pared(20,b); pared(60,b);}
        *x=7;*y=8;*dir=4;
        break;

        case 6:
        for(b=2;b<=49;b+=2)
            {pared(20,b); pared(40,b);pared(60,b);}
        for(b=1;b<=80;b+=2)
            {pared(b,18); pared(b,34);}
        *x=70;*y=44;*dir=1;
        break;

        case 7:
        for(b=2;b<=17;b++)
            {pared(28,b); pared(53,b);}
        for(b=33;b<=49;b++)
            {pared(28,b); pared(53,b);}
        for(b=1;b<=26;b++)
            {pared(b,19); pared(b,31);}
        for(b=55;b<=80;b++)
            {pared(b,19); pared(b,31);}
        *x=55;*y=27;*dir=3;
        break;

        case 8:
        for(b=5;b<=24;b++)
            {pared(24,b); pared(57,b);}
        for(b=27;b<=46;b++)
            {pared(24,b); pared(57,b);}
        for(b=9;b<=39;b++)
            {pared(b,15); pared(b,36);}
        for(b=42;b<=72;b++)
            {pared(b,15); pared(b,36);}
        for(b=18;b<=33;b++)
            {pared(9,b); pared(72,b);}
        for(b=27;b<=54;b++)
            {pared(b,5); pared(b,46);}
        for(b=18;b<=33;b++)
            {pared(b+15, b); pared(81-(b+15),b);}
        *x=76;*y=7;*dir=2;
        break;

        case 9:
        for(b=2;b<=22;b++)
            {pared(28,b); pared(52,b);}
        for(b=28;b<=48;b++)
            {pared(28,b); pared(52,b);}
         for(b=2;b<=20;b++)
            {pared(16,b); pared(64,b);}
        for(b=30;b<=48;b++)
            {pared(16,b); pared(64,b);}
        for(b=24;b<=26;b++)
            {pared(28,b); pared(52,b);
             pared(16,b-2); pared(16,b+2);
             pared(64,b-2); pared(64,b+2);}
        for(b=2;b<=24;b++)
            {pared(40,b); pared(40,b+24);}
        *x=34;*y=40;*dir=1;
        break;

        case 10:
        for(b=2;b<=23;b++)
            {pared(10,b); pared(20,b);
             pared(30,b); pared(50,b);
             pared(60,b); pared(70,b);}
        for(b=27;b<=48;b++)
            {pared(10,b); pared(20,b);
             pared(30,b); pared(50,b);
             pared(60,b); pared(70,b);}
         for(b=2;b<=24;b++)
            {pared(40,b); pared(40,b+24);}
        *x=15;*y=7;*dir=2;
        break;

        case 11:
        for(b=2;b<=48;b+=2)
            {pared(10,b+1); pared(20,b);
             pared(30,b+1); pared(40,b);
             pared(50,b+1); pared(60,b);
             pared(70,b+1);}
        *x=15;*y=42;*dir=1;
        break;

        case 12:
        for(b=7;b<=38;b++)
            {pared(b,5); pared(b,46);}
        for(b=42;b<=74;b++)
            {pared(b,5); pared(b,46);}
        for(b=7;b<=44;b++)
            {pared(5,b); pared(76,b);}
        for(b=8;b<=23;b++)
            {pared(20,b); pared(40,b);pared(60,b);}
        for(b=27;b<=43;b++)
            {pared(20,b); pared(40,b);pared(60,b);}
        for(b=8;b<=17;b++)
            pared(b,25);
        for(b=63;b<=73;b++)
            pared(b,25);
        for(b=23;b<=37;b++)
            pared(b,25);
        for(b=43;b<=57;b++)
            pared(b,25);
        *x=40;*y=24;*dir=3;
        break;

        case 13:
        for(b=25;b<=43;b++)
             {pared(b-4,b-19); pared(b-14,b-19);
              pared(b+17,b+2); pared(b+27,b+2);
              pared(b+17,49-b); pared(b+27,49-b);
              pared(b-4,70-b); pared(b-14,70-b);}
        for(b=5;b<=46;b++)
            {pared(9,b); pared(72,b);}
        for(b=20;b<=61;b++)
            {pared(b,4); pared(b,47);}
        *x=5;*y=5;*dir=2;
        break;

        case 14:
        for(b=29;b<=40;b++)
             {pared(b,b-2); pared(b+13,b-15);
              pared(b,54-b); pared(b+13,67-b); }
        for(b=27;b<=47;b++)
             {pared(b-7,b); pared(b+15,b-22);
              pared(b-7,52-b); pared(b+15,74-b); }
        for(b=27;b<=49;b++)
             {pared(b-16,b); pared(b+22,b-24);
              pared(b-16,52-b); pared(b+22,76-b); }
        for(b=24;b<=28;b++)
             {pared(36,b); pared(46,b);}
        for(b=38;b<=44;b++)
             {pared(b,21); pared(b,31);}
        *x=5;*y=5;*dir=2;
        break;

        default:     // Todos los niveles siguientes tendran este esquema.
        for(b=3;b<=78;b+=6) {
            pared(b,4);
            pared(b+3,6);
            pared(b,8);
            pared(b+3,10);
            pared(b,12);
            pared(b+3,14);
            pared(b,16);
            pared(b+3,18);
            pared(b,20);
            pared(b+3,22);
            pared(b,24);
            pared(b+3,26);
            pared(b,28);
            pared(b+3,30);
            pared(b,32);
            pared(b+3,34);
            pared(b,36);
            pared(b+3,38);
            pared(b,40);
            pared(b+3,42);
            pared(b,44);
            pared(b+3,46);
            pared(b,48);
        }
        *x=8;*y=25;*dir=4;
        break;
   }
}
/**********************************************************/

int final(void)
{
    int a, fin=0;

    textbackground(0); clrscr();
    textcolor(7);gotoxy(8,20);
    cprintf("Perdiste. Apreta ENTER para volver a jugar o ESCAPE para salir.");
    gotoxy(1,50);
    while (!fin){
        a = getch();
        if(a==27) {fin=1;}
        if(a==13) {fin=2;}
    }
return fin;
}
/**********************************************************/

void mensaje(char texto[81], int tecla)
{
    int a,b;

    textcolor(10);
    gotoxy(18,23), cprintf("############################################");
    gotoxy(18,24); cprintf("#                                          #");
    gotoxy(18,25); cprintf("#                                          #");
    gotoxy(18,26); cprintf("#                                          #");
    gotoxy(18,27); cprintf("############################################");
    textcolor(15); gotoxy(20,25); cprintf("%s", texto);
    gotoxy(1,2);
    if (tecla>0)
        while((a=getch())!=tecla);
    else if (tecla<0)
        {a=getch(); if(a==83||a==115) salida=2;}
    else
        delay(2000);

    // Restaura lo que habia atras del mensaje
    for (b=23;b<=27;b++) {
        for(a=15;a<=65;a++){
            gotoxy(a,b);
            if (mat[a][b]==0) printf(" ");   // Si mat es 0 no habia nada.
            if (mat[a][b]==1) pared(a,b);    // Si mat es 1 habia una pared.
            if (mat[a][b]==2) gusano(a,b);   // Si mat es 2 estaba el gusano.
        }
    }
}


