/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include<stdio.h>
#include<stdlib.h>

#define NUM_ROWS  6
#define NUM_COLUMNS  9
#define MAX  4

enum stype{
    NORMAL, OBSTACLE };

//colors of tokens
enum color {
    RED, BLU, GREEN, YELLOW, PINK, ORANGE
};

//defines a token.
//Note each token can be associated with a color
typedef struct token{
   enum color col;
}token;

//Defines a square of the board.
typedef struct square{
    //A square can be a NORMAL or an OBSTACLE square
     enum stype type;
     int  topOfStack;
     //the stack of tokens that can be placed on the board square
     token * squaretokens;
     enum color tokensColor[MAX];
}square;



/*
 * You need to fill this data structure
 * with the information about the player
 * such as a name and a color.
 */
typedef struct player{
    char name[20];
    enum color col;

}player;




void initialize_board(square board[NUM_ROWS][NUM_COLUMNS]);
void print_board(square board[NUM_ROWS][NUM_COLUMNS]);
void printLine();
int initialize_players(player players[]);
void place_tokens(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers);
int tokensPerSquare(square board[NUM_ROWS][NUM_COLUMNS], int row, int col);
int smallestTokenInColoumOne(square board[NUM_ROWS][NUM_COLUMNS]);
int checkWinner(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers);
int play_game(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers);
int isCorrectToken(square board[NUM_ROWS][NUM_COLUMNS], player players[], int playerNumber, int rowCord, int colCord);


int main(int argc, char** argv) {

    //the board is defined as a 2-Dimensional array of squares
    square board[NUM_ROWS][NUM_COLUMNS];

    //an array containing the players (MAX 6 players)
    player players[6];

    //the number of players
    int numPlayers =0;

    //creates the squares of the board
    initialize_board(board);

    //prints the board
    print_board(board);

    //creates the players
    numPlayers = initialize_players(players);

    //asks each player to place their tokens
    //on the first column of the board
    place_tokens(board, players, numPlayers);


    //manages the turns of the game and identifies a winner
   play_game(board, players, numPlayers);

    return 0;



}

void initialize_board(square board[NUM_ROWS][NUM_COLUMNS]){
    int i;
    for (i =0; i< NUM_ROWS; i++){
            int j;
        for(j =0; j < NUM_COLUMNS; j++){
            //creates an obstacle square at positions (0,3), (1,6), (2,4), (3,5), (4,2) and (5,7)
            if((i == 0 && j==3) || (i == 1 && j == 6) || (i ==2 && j ==4)
                    || (i == 3 && j ==5) || (i==4 && j==2) || (i==5 && j==7)){
                board[i][j].type = OBSTACLE;
            } else{
                //creates a normal square otherwise
                board[i][j].type = NORMAL;
            }
            board[i][j].squaretokens = NULL;
            board[i][j].topOfStack = -1;
        }
    }
}


 /*
 * This function creates players for the first time
 *
 * Input: the array of players to be initialized
 * Output: The number of players of the game
 */
int initialize_players(player players[]){
           int numPlayers;

            do {
            printf("Number of players");
            scanf("%d",&numPlayers);
            } while (numPlayers < 2 || numPlayers > 6);

    int count=0;
    int p;
    int choosenColor[numPlayers];
	for (p=0; p< numPlayers; p++) {

		int numberColor;
		int favorite_color;
		int colorSelectedByPlayer = 0;
        int sameColor=0;
		while (!colorSelectedByPlayer) {
        	numberColor = 1;
         printf("Enter a name of player: ");
         scanf("%s", players[p].name);
         while(!sameColor){
            sameColor=1;
            printf("Player %s choose your favorite color which is not taken by other players: (0:RED, 1:BLU, 2:GREEN, 3:YELLOW, 4:PINK, 5:ORANGE):  ",players[p].name);

        		scanf("%d", &favorite_color);
        	    // favorite_color 012345
        	    // get enum value from
        		int p1;
                for(p1=0; p1< numPlayers; p1++){
                        if((favorite_color)==choosenColor[p1]){
                            printf("\n color is already selected by other player please select any other color \n");
                            sameColor=0;
                        }
                }
                choosenColor[p] =favorite_color;
        }
            /* print out the result */
            switch (favorite_color)
            {
            case RED:
                printf("your favorite color is Red\n");
                break;
            case BLU:
                printf("your favorite color is BLU\n");
                break;
            case GREEN:
                printf("your favorite color is GREEN\n");
                break;
            case YELLOW:
                printf("your favorite color is YELLOW\n");
                break;
            case PINK:
                printf("your favorite color is PINK\n");
                break;
            case ORANGE:
                printf("your favorite color is ORANGE");
                break;
            default:
                printf("you did not choose any color");
            }

                players[p].col=favorite_color;
				colorSelectedByPlayer = 1;

			}
		}

    return numPlayers;
}
    /*
 * Returns the first letter associated with the color of the token
 *
 * Input: t - pointer to a token
 * Output: initial of the color of the token
 */
char print_token(token *t){
    if((*t).col== PINK) return 'P';
    if((*t).col== RED) return 'R';
    if((*t).col== BLU) return 'B';
    if((*t).col== GREEN) return 'G';
    if((*t).col== ORANGE) return 'O';
    if((*t).col== YELLOW) return 'Y';
    return '\0';
}


void print_board(square board[NUM_ROWS][NUM_COLUMNS]){
    printf("                THE BOARD\n");
    int i;
    for(i =0; i < NUM_ROWS; i++){

        //prints an horizontal line
        printLine();
        //prints the row number
        printf(" %d ", i);
        char c = '\0' ;
        //if the square (i,j) is occupied,
        //c is assigned the initial of the color of the token that occupies the square
        int j;
        for (j= 0; j < NUM_COLUMNS; j++){
            if(board[i][j].squaretokens != NULL){
                c = print_token(board[i][j].squaretokens);
            }
            //if the square (i,j) is empty
            else{
                //c is assigned 'X' if the square represents an obstacle
                if(board[i][j].type == OBSTACLE)
                    c = 'X';
                //c is assigned an empty space otherwise
                else c = ' ';
            }
            printf("| %c ", c);
        }
        printf ("|\n");
    }
    printLine();
    //prints the number of the columns at the end of the board
    printf("     0   1   2   3   4   5   6   7   8\n");
}

void printLine(){
  printf("   -------------------------------------\n");
}

void place_tokens(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers){
  printf("Starting the Game.........\n\n");
	printf("We require to place all our tokens on first column of the board\n");
	printf("RULE:\n");
	printf("A token should be placed first on top of one of the smallest stacks.\n");
	printf("A player cannot stack a token on top of his/her token.\n\n\n");

	int totalMoves = numPlayers * 4;
    int move;
	for (move=0; move < totalMoves; ++move) {
		int rowSelected = 0;
		int row;
		printf("Player turn: %s ",players[(move%numPlayers)].name,"\n");
		while(!rowSelected) {
			printf("Enter the row (0-5) you want to place your token\n");
			scanf("%d",&row);

			player player = players[move%numPlayers];

             while(smallestTokenInColoumOne(board) < board[row][0].topOfStack){
                printf("\nERROR:Place your token on smaller stack\n");
                printf("Player %s Enter the row 0-5) you want to place your token\n ", players[(move%numPlayers)].name);
                scanf("%d", &row);
            }

            if(board[row][0].topOfStack>-1){
			    while(board[row][0].squaretokens->col==player.col){
			        printf("You can not put your token on sameColor");
			        printf("Enter the row (0-5) you want to place your token\n");
			        scanf("%d",&row);
			    }
            }
           token *newToken =(struct token*) malloc(sizeof(token));
            newToken->col = player.col;
            board[row][0].squaretokens=newToken;
            board[row][0].topOfStack ++;
            board[row][0].tokensColor[board[row][0].topOfStack]=player.col;
            //print_board(board);
			rowSelected = 1;
		}
	}
}
 //check smallest token for placing the it initialy
int smallestTokenInColoumOne(square board[NUM_ROWS][NUM_COLUMNS])
{
    int tokenCount[6];
   int smallestTokenRowInColoumOne=board[0][0].topOfStack;
   int i;
    for(i = 0; i < NUM_ROWS; ++i){
        tokenCount[i] = board[i][0].topOfStack;
        if(smallestTokenRowInColoumOne>tokenCount[i]){
            smallestTokenRowInColoumOne=tokenCount[i];
        }
    }

    return  smallestTokenRowInColoumOne;
}

/*CHECKS IF THERE IS WINNER*/
int checkWinner(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers){
    int colorCounters = 0;
    int i;
    for (i = 0; i < numPlayers; ++i){
        int j;
        for (j = 0; j < NUM_ROWS; ++j){
            token *token = board[j][8].squaretokens;

            while (token != NULL){
                if(token->col == players[i].col){
                    colorCounters += 1;
                }
            }
        }
        if (colorCounters >= 3){
                return i;
        }
    }

    return 10;
}

int play_game(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers){
    int row, col, upOrDown;
    int isWinner = checkWinner(board, players, numPlayers);
    printf("Now we will start moving towards right.... \n\n");
	printf("RULE:\n");
	printf("Roll the dice\n");
	printf("Move your token up or down (Optional)\n");
	printf("Move forward one of the token at the row indicated by the dice(Mandatory) \n\n\n");
    int i;
   print_board(board);

      for(i = 0; i < numPlayers; ++i){
        int dice = rand()%6;
        printf("\nPlayer %s get %d number on dice", players[i].name,dice);
        int choice;
        printf("\nDo you want to move your token up or down \n1 for yes and rest other number for no ");
        scanf("%d", &choice);

        if (choice == 1){
            printf("\n  Choose the row and column of already placed token(your current position)");
            printf("\nEnter row number: ");
            scanf("%d", &row);

            printf("\nEnter column number: ");
            scanf("%d", &col);

            int correctToken = isCorrectToken(board, players, i, row, col);
            while (board[row][col].type == OBSTACLE){
                //If a player token is in obstacle
                if (obstacleCheck(board, row, col) == 0){
                    printf("\nERROR: You are in an obstacle. You can't move this token out until all tokens have reached column %d", col);
                }
                /*ASK USER FOR CO ORDINATES AGAIN*/
                printf("\n%s, choose which token you would like to move sideways.\n", players[i].name);

                printf("\nEnter row number: ");
                scanf("%d", &row);

                printf("\nEnter column number: ");
                scanf("%d", &col);

                correctToken = isCorrectToken(board, players, i, row, col);

                /*IF CORRECT TOKEN AND NORMAL SQUARE, BREAK*/
                if (obstacleCheck(board, row, col) == 1){
                    board[row][col].type = NORMAL; //set square to normal
                    correctToken = isCorrectToken(board, players, i, row, col); //set correct token to true
                }
            }
            if(correctToken == 1 && board[row][col].type == NORMAL){
                printf("\nEnter 0 to go up OR Enter 1 to go down: ");
                scanf("%d", &upOrDown);
                token *topestToken =(struct token*) malloc(sizeof(token));

                if(upOrDown == 0){
                    tokenTravel(board, row, col, row-1, col, topestToken,players[i]);
                    print_board(board);
                }

                else if(upOrDown == 1){
                    tokenTravel(board, row, col, row+1, col, topestToken,players[i]);
                    print_board(board);
                }
            }
        }


               printf("\n Player %s change to move token from row %d one step forwards"
                        "\nWhich column is the token which you would like to move: ", players[i].name, dice);
                scanf("%d", &col);
                token *topestToken =(struct token*) malloc(sizeof(token));

                if(board[dice][col].squaretokens == NULL){
                     print_board(board);
                     printf("\nyour token is not placed here : empty cube\n");
                }

                if (board[dice][col].type == OBSTACLE && board[dice][col].squaretokens != NULL){
                    if (obstacleCheck(board, dice, col) == 0){
                        print_board(board);
                        printf("\nYou are in an obstacle please wait until all the tokens have reached column %d", col);
                    }

                    else if (obstacleCheck(board, dice, col) == 1){
                        tokenTravel(board, dice, col, dice, col+1,topestToken, players[i]);
                        print_board(board);
                    }
                }

                if(board[dice][col].type == NORMAL && board[dice][col].squaretokens != NULL){

                    tokenTravel(board, dice, col, dice, col+1,topestToken, players[i]);
                    print_board(board);
                }
                isWinner = checkWinner(board, players, numPlayers);

                if (isWinner != 10){
                    printf("\nWINNER IS: %s", players[checkWinner(board, players, numPlayers)].name);
                    return 1;
                }
                    /*RESETING PLAYER*/
                if (i == numPlayers - 1){
                    i = -1;
                }
           }



}

/*CHECKS IF PLAYER CHOOSING THEIR TOKEN IN SIDEWAYS MOVE*/
int isCorrectToken(square board[NUM_ROWS][NUM_COLUMNS], player players[], int playerNumber, int row, int col){
    while(board[row][col].topOfStack == -1 || players[playerNumber].col != board[row][col].squaretokens->col){
            if(board[row][col].squaretokens == NULL){
                                  printf("\nERROR: Empty Square\n");
            }
          printf("\n Choose the row and column of already placed token(your current position)");
            printf("\nEnter row number: ");
            scanf("%d", &row);

            printf("\nEnter column number: ");
            scanf("%d", &col);

    }
    return 1;
}

/*MOVING TOKEN ON BOARD*/
void tokenTravel(square board[][NUM_COLUMNS], int currentRow, int currentCol, int toBeRow, int toBeCol, token *topestToken,player player){
    const char* dayNames[] = {"RED", "BLU", "GREEN", "YELLOW", "PINK", "ORANGE"};
    token *Currtoken = board[currentRow][currentCol].squaretokens;
    int Currtop = board[currentRow][currentCol].topOfStack;
    board[currentRow][currentCol].topOfStack--;
    if(Currtop>0){
        enum color c=board[currentRow][currentCol].tokensColor[Currtop-1];
        topestToken->col =c ;
        board[currentRow][currentCol].squaretokens=topestToken;

    }
    else{
        board[currentRow][currentCol].squaretokens=NULL;
    }
    board[toBeRow][toBeCol].squaretokens = Currtoken;
    board[toBeRow][toBeCol].topOfStack++;


}

//To remove obstacle
int obstacleCheck(square board[NUM_ROWS][NUM_COLUMNS], int dice, int col){
    int i = 0;
    for(i; i < 6; ++i){
         int j;
        for(j = 0; j < col; ++j){
            if (board[i][j].squaretokens != NULL){
                return 0;
            }
        }
    }

    return 1;
}
