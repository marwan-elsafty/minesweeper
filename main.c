#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#include "myConio.h"

#define SPACE "   " // 3 spaces

void header();
void insertUser();
void mainMenu();

void selectFromMainMenu();
void loadCurrentGame();
void scoreBoard();
void userManual();
bool appExitRequest();

void start();
void initGameBoard();
void drawGameBoard();

void initBoard();
void initMinesLocation();
void initCellRank();

void play();

void saveCurrentGame();
void saveScore();

bool checkWin();


/**********************************
MUST BE INITILIZED WITH HIGH VALUES
            W   H   Y
or else infinite loop happens in
initCellRank
**********************************/
int rows=100,cols=100;
int board[100][100];
char gameBoard[100][100];


typedef struct user
{
    char name[100];
    double time;


} userDataType;


userDataType user;


// for time difference needs
time_t startTime,endTime;

int main()
{

    do
    {
        mainMenu();
        fflush(stdin);

    }
    while( appExitRequest()==true );

    return 0;
}



void header()
{
    system("cls");
    printf("\n --------------------------- Welcome to Minesweeper ---------------------------\n\n");
}


void mainMenu()
{

    SetConsoleTitle("MineSweeper");

    header();

    printf("Start Game\t(S)\n");
    printf("User Manual\t(M)\n");
    printf("Score Board\t(B)\n");

    printf("\nExit\t\t(E)");


    // string instead of a char for run-time error
    char choice[20];

    do
    {
        printf("\n\n\nYour choice : ");
        gets(choice);

        // converts string into uppercase
        strupr(choice);

        if( strcmp(choice,"S")==0 )
            start();

        else if( strcmp(choice,"B")==0 )
            scoreBoard();

        else if( strcmp(choice,"M")==0 )
            userManual();

        else if( strcmp(choice,"E")==0 )
            exit(0);

        else
        {
            printf("\n\aError!\nusage: enter one of the given choices.");
            getche();
            mainMenu();
        }

    }
    while( strcmp(choice,"S")!=0 && strcmp(choice,"L")!=0 && strcmp(choice,"B")!=0 && strcmp(choice,"M")!=0 );
}




void scoreBoard()
{

    SetConsoleTitle("MineSweeper > Score Board");
    system("cls");

    FILE * scoreBoard = fopen("score-board-sheet.bin","r");

    // failure check
    if (scoreBoard==NULL)
    {
        perror("\a\nError");
        getche();
        return;
    }


    // table header
    printf("Name\t\t\t");
    printf("Best Score\t\t\t\n\n");

       userDataType userTemp;

    while( fread(&userTemp, sizeof(userDataType), 1, scoreBoard) )
    {

        printf("%s\t\t\t", userTemp.name);
        //printf("%.lf\t\t\t", userTemp.best.score);
        printf("%.1lf\t\t\n", userTemp.time);
    }


    fclose(scoreBoard);
    getche();
}



void userManual()
{

    SetConsoleTitle("MineSweeper > User Manual");
    system("cls");

    printf("Quick Start :\n\n\n\n");
    printf(">> You are presented with a board of squares.\n\n");
    printf(">> Some squares contain mines (bombs), others don't.\n\n");
    printf(">> If you click on a square containing a bomb, you lose.\n\n");
    printf(">> If you manage to click all the squares\n");
    printf("   (without clicking on any bombs) you win.\n\n");
    printf(">> Clicking a square which doesn't have a bomb\n");
    printf("   reveals the number of neighbouring squares containing bombs.\n\n");
    printf(">> Use this information plus some guess work to avoid the bombs.\n\n");
    printf(">> To open a square, write it's row and column value.");
    getche();
    mainMenu();

}



bool appExitRequest()
{

    SetConsoleTitle("MineSweeper Play > Exit");
    system("cls");


    // string instead of a char for run-time error
    char choice[20];

    do
    {
        //getche();
        printf("\aDo you want to quit ? (Y/N) : ");
        gets(choice);

        // converts string into uppercase
        strupr(choice);


        // exit
        if( strcmp(choice,"NO")==0 || strcmp(choice,"N")==0 )
            return true;

        // continue
        else if(strcmp(choice,"YES")==0 || strcmp(choice,"Y")==0)
            return false;

        else
            printf("\n\aError!\nusage: enter one of the given choices.");

    }
    while ( strcmp(choice,"N")!=0 && strcmp(choice,"Y")!=0 && strcmp(choice,"NO")!=0 && strcmp(choice,"YES")!=0 );
}



void insertUser()
{

    SetConsoleTitle("MineSweeper > Play > User Entry");
    system("cls");

    printf("Hello, enter your name so we can get to know you \1\n\n");
    printf("\nName : ");


    gets(user.name);
    toupper(user.name[0]);

    printf("Hello, %s. Press enter when you're ready to start !",user.name);
    getche();
}



void start()
{

    insertUser();

    initGameBoard();
    initBoard();
    initMinesLocation();
    initCellRank();

    time(&startTime);

    play();

}



void initGameBoard()
{

    SetConsoleTitle("MineSweeper > Play > Rows And Columns Entery");

    int i, j;
    char choice;

    do
    {
        do
        {
            system("cls");
            printf("Board configuration:\n\n");

            // rows and colsumns user inputs
            printf("<< Please enter number of rows :     ");
            scanf("%d",&rows);
            printf("<< Please enter number of columns :  ");
            scanf("%d",&cols);

            // rows and colsumns illogical entries check and printing them
            if(rows<=0 && cols<=0)
            {
                printf("\a\nError !\nusage: row(s) and col(s) must be positive integers.");
                getche();
            }
            else if(rows<=0)
            {
                printf("\a\nError !\nusage: row(s) must be a positive integer.");
                getche();
            }
            else if(cols<=0)
            {
                printf("\a\nError !\nusage: col(s) must be a positive integer.");
                getche();
            }

            // loop for illogical entries
        }
        while(rows<=0 || cols<=0);

        // preventing stream errors
        getchar();

        // user check for entered values
        printf("\n>> You entered %d rows and %d colsumns.\n",rows,cols);
        printf("   Do you want to change inputs? (Y/N) : ");
        scanf("%c",&choice);
        choice = toupper(choice);

        // loop for re-entering
    }
    while( choice!='N' );

    gameBoard[rows][cols];

    for(i=0 ; i<rows ; i++)
        for(j=0 ; j<cols ; j++)
            gameBoard[i][j]='x';
}



void drawGameBoard()
{

    int i,j;

    // since initBoard() displays output
    // this function will be used several times
    system("cls");

    // writing columns indexes
    for(j=0 ; j<cols ; j++)
        printf("%s%2d%s", (j==0?"\n" SPACE SPACE:"  "), (j+1), (j==(cols-1)?"\n":"\0") );

    for(i=0 ; i<rows ; i++)
    {
        // writing rows indexes
        printf("\n\n %2d%s",(i+1),SPACE" ");

        // printing holes
        for(j=0 ; j<cols ; j++)
        {


            printf("%c%s",gameBoard[i][j],SPACE);
        }
    }
}



void initBoard()
{

    int i,j;
    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++)
            board[i][j]=0;
}



void initMinesLocation()
{

    int i,j;
    int n=0;
    int minesNum = 1 + (int)ceil((double)(rows * cols)/10);

    srand(time(NULL));

    while(n<minesNum)
    {
        i=rand()%rows;
        j=rand()%cols;

        if(board[i][j]!=9)
        {
            board[i][j]=9;
            n++;
        }
    }
}



void initCellRank()
{

    int i,j;

    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++)
        {
            if(board[i][j]<9)
            {

                if(i-1>=0 && j-1>=0 && i-1<rows && j-1<cols)
                    if(board[i-1][j-1]==9)
                        board[i][j]++;

                if(i-1>=0 && j>=0 && i-1<rows && j<cols)
                    if(board[i-1][j]==9)
                        board[i][j]++;

                if(i-1>=0 && j+1>=0 && i-1<rows && j+1<cols)
                    if(board[i-1][j+1]==9)
                        board[i][j]++;

                if(i>=0 && j-1>=0  && i<rows && j-1<cols)
                    if(board[i][j-1]==9)
                        board[i][j]++;

                if(i>=0 && j+1>=0 && i<rows && j+1<cols)
                    if(board[i][j+1]==9)
                        board[i][j]++;

                if(i+1>=0 && j-1>=0 && i+1<rows && j-1<cols)
                    if(board[i+1][j-1]==9)
                        board[i][j]++;

                if(i+1>=0 && j>=0 && i+1<rows && j<cols)
                    if(board[i+1][j]==9)
                        board[i][j]++;

                if(i+1>=0 && j+1>=0 && i+1<rows && j+1<cols)
                    if(board[i+1][j+1]==9)
                        board[i][j]++;
            }
        }
}



void play()
{

    SetConsoleTitle("MineSweeper > Play");

    int i,j;
    int rowLocal,colLocal;

    char choice[10];

    do
    {
        system("cls");
        drawGameBoard();

        printf("\n\n\n");

        // for debugging
        /*for(i=0; i<rows; i++)
        {
            for(j=0; j<cols; j++)
                printf("%d   ",board[i][j]);
            printf("\n\n");
        }*/

        // rows and columns user inputs
        printf("<< Enter row :\t");
        scanf("%d",&rowLocal);
        printf("<< Enter col :\t");
        scanf("%d",&colLocal);

        getc(stdin);

        // flag and open user input
        printf("<< Hit [O] to open cell or Hit [F] to mark it as Flag : ");
        gets(choice);
        strupr(choice);

        // rows and columns illogical entries check and printing them
        if(rowLocal<=0 && colLocal<=0)
        {
            printf("\a\nError !\nusage: row and column indexes must be positive integers.");
            getche();
        }
        else if(rowLocal<=0)
        {
            printf("\a\nError !\nusage: row index must be a positive integer.");
            getche();
        }
        else if(colLocal<=0)
        {
            printf("\a\nError !\nusage: column index must be a positive integer.");
            getche();
        }
        else if(rowLocal>rows && colLocal>cols)
        {
            printf("\a\nError !\nusage: row and column index must be less than or equal number of rows and columns.");
            getche();
        }
        else if(rowLocal>rows)
        {
            printf("\a\nError !\nusage: row index must be less than or equal number of rows.");
            getche();
        }
        else if(colLocal>cols)
        {
            printf("\a\nError !\nusage: column index must be less than or equal number of columns.");
            getche();
        }




    }
    while(rowLocal<=0 || colLocal<=0
            || rowLocal>rows || colLocal>cols );

    rowLocal--;
    colLocal--;

    if(strcmp(choice,"O")==0)
    {

        // a mine
        if(board[rowLocal][colLocal]==9)
        {

            // reveal answer
            for(i=0 ; i<rows ; i++)

                for(j=0 ; j<cols ; j++)
                {
                    if( board[i][j] == 9)
                        gameBoard [i][j] = '*';

                    else if ( board[i][j]>0 && board[i][j]<9)
                        gameBoard[i][j] = board[i][j]+ '0' ;

                    else if ( board[i][j] == 0 )
                        gameBoard[i][j] = ' ' ;
                }

            drawGameBoard();
            textcolor(LIGHTRED);
            printf("\n\n\nBAM ! You've hit a mine..");
            textcolor(LIGHTGRAY);
            getche();
        }

        else if(board[rowLocal][colLocal]==0)
        {

            blankBlock(rowLocal,colLocal);

            if( checkWin() == true )
                win();
            else
                play();
        }

        else if ( board[rowLocal][colLocal]>0 && board[rowLocal][colLocal]<9 )
        {

            gameBoard[rowLocal][colLocal] = board[rowLocal][colLocal]+ '0' ;


            if( checkWin() == true )
                win();

            else
            {
                play();
            }
        }
    }

    else if(strcmp(choice,"F")==0)
    {

        gameBoard[rowLocal][colLocal] = 'F';

        if( checkWin() == true )
            win();

        else
        {
            play();

        }
    }
}



void blankBlock(int i,int j)
{

    gameBoard[i][j] = ' ' ;

    if(i-1>=0 && j-1>=0 && i-1<rows && j-1<cols)
    {

        if(gameBoard[i-1][j-1]==' ');

        else if(board[i-1][j-1]==0)
            blankBlock(i-1,j-1);

        else
            gameBoard[i-1][j-1]= board[i-1][j-1]+'0' ;
    }


    if(i-1>=0 && j>=0 && i-1<rows && j<cols)
    {

        if(gameBoard[i-1][j]==' ');

        else if(board[i-1][j]==0)
            blankBlock(i-1,j);

        else
            gameBoard[i-1][j]= board[i-1][j]+'0';
    }


    if(i-1>=0 && j+1>=0 && i-1<rows && j+1<cols)
    {

        if(gameBoard[i-1][j+1]==' ');

        else if(board[i-1][j+1]==0)
            blankBlock(i-1,j+1);

        else
            gameBoard[i-1][j+1]= board[i-1][j+1] + '0';
    }


    if(i>=0 && j-1>=0  && i<rows && j-1<cols)
    {

        if(gameBoard[i][j-1]==' ');

        else if(board[i][j-1]==0)
            blankBlock(i,j-1);

        else
            gameBoard[i][j-1]=board[i][j-1]+'0';
    }


    if(i>=0 && j+1>=0 && i<rows && j+1<cols)
    {

        if(gameBoard[i][j+1]==' ');

        else if(board[i][j+1]==0)
            blankBlock(i,j+1);

        else
            gameBoard[i][j+1]= board[i][j+11]+'0';
    }


    if(i+1>=0 && j-1>=0 && i+1<rows && j-1<cols)
    {
        if(gameBoard[i+1][j-1]==' ');

        else if(board[i+1][j-1]==0)
            blankBlock(i+1,j-1);

        else
            gameBoard[i+1][j-1]= board[i+1][j-1]+'0';
    }


    if(i+1>=0 && j>=0  && i+1<rows && j<cols)
    {

        if(gameBoard[i+1][j]==' ');

        else if(board[i+1][j]==0)
            blankBlock(i+1,j);

        else
            gameBoard[i+1][j]=board[i+1][j]+'0';
    }


    if(i+1>=0 && j+1>=0 && i+1<rows && j+1<cols)
    {

        if(gameBoard[i+1][j+1]==' ');

        else if(board[i+1][j+1]==0)
            blankBlock(i+1,j+1);

        else
            gameBoard[i+1][j+1]= board[i+1][j+1] + '0';
    }

    return;
}



bool checkWin()
{

    int checkFlag = 0;
    bool checkNum = false;
    int minesNum = 1 + (int)ceil((double)(rows * cols)/10);

    int i,j;

    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++)
            if(board[i][j]==9 && gameBoard[i][j]=='F')
                checkFlag++;

    for(i=0; i<rows; i++)
        for(j=0; j<cols; j++)
            if(gameBoard[i][j]=='x')
                checkNum=true;

    // debugging mode
    //printf("checkFlag=%d checkNum=%d mines=%d",checkFlag,checkNum,minesNum);

    if( (checkFlag == minesNum) && (checkNum == false) )
        return true;

    else
        return false;
}



void saveCurrentGame()
{

    // overwrite
    FILE * currentGame = fopen("current-game-progress.txt","w");

    // failure check
    if (currentGame==NULL)
    {
        perror("\a\nError");
        getche();
        return;
    }

    int i,j;

    for(i=0 ; i<rows ; i++)
        for(j=0 ; j<cols ; j++)
            fwrite(&gameBoard[i][j], sizeof(char), 1, currentGame);

    fclose(currentGame);
}



void saveScore()
{

    // append
    FILE * scoreBoard = fopen("score-board-sheet.bin","a");

    // failure check
    if (scoreBoard==NULL)
    {
        perror("\a\nError");
        getche();
        return;
    }

    fwrite(&user, sizeof(userDataType), 1, scoreBoard);

    fclose(scoreBoard);
}



void win()
{

    system("cls");

    time(&endTime);
    double para = 60.0;

    user.time = difftime(endTime,startTime);

    drawGameBoard();

    textcolor(LIGHTGREEN);
    printf("\n\nCONGRATULATIONS. YOU WON !");
    printf("\n\n%s, your time is : %.1lf seconds.",user.name, (user.time));
    textcolor(LIGHTGRAY);

    getche();

    saveScore();
}
