
// #include<stdio.h>
void printmsg(char* msg)
{
    printstr(msg);
    
    return;
   // printf(msg);
    
}

int print_board(int (*board)[10]) {
  int i;
  int j;
  for (i = 0; i < 10; i++) {

    for (j = 0; j < 10; j++)

      if (board[i][j] != 0)

	        printmsg("Q ");

      else

      	printmsg(". ");

    printmsg("\n");

  }

  printmsg("\n\n");
  return 0;

}



int conflict(int (*board)[10], int row, int col) {
  int i;
  for ( i = 0; i < row; i++) {

    if (board[i][col] != 0)

      return 1;

    int j;
    j= row - i;

    if (0 < col - j + 1)
    {
      if(board[i][col - j] != 0)

      return 1;
    }

    if (col + j < 10)
    {
    if( board[i][col + j]!= 0)

      return 1;
    }

  }

  return 0;

}



int solve(int (*board)[10], int row) {

  if (row > 9) {

    print_board(board);

    return 0;

  }
  int i;
  for ( i = 0; i < 10; i++) {
   //print(conflict(board, row, i));

    if (conflict(board, row, i) == 0) {

      board[row][i] = 1;
     // printstr("row = ");
     // print(row);

      solve(board, row + 1);

      board[row][i] = 0;

    }

  }
  return 2;

}



int main() {

  int board[10][10];
  int j;
  int i;
  for( i= 0;i<10;i++)
  for ( j = 0; j < 10; j++)

    board[i][j] = 0;

  solve(board, 0);
 //print_board(board);

  return 0;

}


