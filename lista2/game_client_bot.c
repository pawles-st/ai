/****************************
Maciej Gębala (CC BY-NC 4.0)
Client ver. 0.1
2023-03-30
****************************/
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <time.h>

#include "board.h"

int max(int x, int y) {
	return x < y ? y : x;
}

int min(int x, int y) {
	return x < y ? x : y;
}

const int rpairs[20][2][2] = { 
  { {0,0}, {0,1} }, 
  { {0,1}, {0,2} }, 
  { {0,2}, {0,3} }, 
  { {0,3}, {0,4} }, 
  { {1,0}, {1,1} }, 
  { {1,1}, {1,2} }, 
  { {1,2}, {1,3} }, 
  { {1,3}, {1,4} }, 
  { {2,0}, {2,1} }, 
  { {2,1}, {2,2} }, 
  { {2,2}, {2,3} }, 
  { {2,3}, {2,4} }, 
  { {3,0}, {3,1} }, 
  { {3,1}, {3,2} }, 
  { {3,2}, {3,3} }, 
  { {3,3}, {3,4} }, 
  { {4,0}, {4,1} }, 
  { {4,1}, {4,2} }, 
  { {4,2}, {4,3} }, 
  { {4,3}, {4,4} }, 
};

const int cpairs[20][2][2] = { 
  { {0,0}, {1,0} },
  { {1,0}, {2,0} },
  { {2,0}, {3,0} },
  { {3,0}, {4,0} },
  { {0,1}, {1,1} },
  { {1,1}, {2,1} },
  { {2,1}, {3,1} },
  { {3,1}, {4,1} },
  { {0,2}, {1,2} },
  { {1,2}, {2,2} },
  { {2,2}, {3,2} },
  { {3,2}, {4,2} },
  { {0,3}, {1,3} },
  { {1,3}, {2,3} },
  { {2,3}, {3,3} },
  { {3,3}, {4,3} },
  { {0,4}, {1,4} },
  { {1,4}, {2,4} },
  { {2,4}, {3,4} },
  { {3,4}, {4,4} },
};

const int d1pairs[16][2][2] = { 
  { {0,0}, {1,1} },
  { {0,1}, {1,2} },
  { {0,2}, {1,3} },
  { {0,3}, {1,4} },
  { {1,0}, {2,1} },
  { {1,1}, {2,2} },
  { {1,2}, {2,3} },
  { {1,3}, {2,4} },
  { {2,0}, {3,1} },
  { {2,1}, {3,2} },
  { {2,2}, {3,3} },
  { {2,3}, {3,4} },
  { {3,0}, {4,1} },
  { {3,1}, {4,2} },
  { {3,2}, {4,3} },
  { {3,3}, {4,4} },
};

const int d2pairs[16][2][2] = { 
  { {0,4}, {1,3} },
  { {0,3}, {1,2} },
  { {0,2}, {1,1} },
  { {0,1}, {1,0} },
  { {1,4}, {2,3} },
  { {1,3}, {2,2} },
  { {1,2}, {2,1} },
  { {1,1}, {2,0} },
  { {2,4}, {3,3} },
  { {2,3}, {3,2} },
  { {2,2}, {3,1} },
  { {2,1}, {3,0} },
  { {3,4}, {4,3} },
  { {3,3}, {4,2} },
  { {3,2}, {4,1} },
  { {3,1}, {4,0} },
};

const int pairs[72][2][2] = { 
  { {0,0}, {0,1} }, 
  { {0,1}, {0,2} }, 
  { {0,2}, {0,3} }, 
  { {0,3}, {0,4} }, 
  { {1,0}, {1,1} }, 
  { {1,1}, {1,2} }, 
  { {1,2}, {1,3} }, 
  { {1,3}, {1,4} }, 
  { {2,0}, {2,1} }, 
  { {2,1}, {2,2} }, 
  { {2,2}, {2,3} }, 
  { {2,3}, {2,4} }, 
  { {3,0}, {3,1} }, 
  { {3,1}, {3,2} }, 
  { {3,2}, {3,3} }, 
  { {3,3}, {3,4} }, 
  { {4,0}, {4,1} }, 
  { {4,1}, {4,2} }, 
  { {4,2}, {4,3} }, 
  { {4,3}, {4,4} }, 
  { {0,0}, {1,0} },
  { {1,0}, {2,0} },
  { {2,0}, {3,0} },
  { {3,0}, {4,0} },
  { {0,1}, {1,1} },
  { {1,1}, {2,1} },
  { {2,1}, {3,1} },
  { {3,1}, {4,1} },
  { {0,2}, {1,2} },
  { {1,2}, {2,2} },
  { {2,2}, {3,2} },
  { {3,2}, {4,2} },
  { {0,3}, {1,3} },
  { {1,3}, {2,3} },
  { {2,3}, {3,3} },
  { {3,3}, {4,3} },
  { {0,4}, {1,4} },
  { {1,4}, {2,4} },
  { {2,4}, {3,4} },
  { {3,4}, {4,4} },
  { {0,0}, {1,1} },
  { {0,1}, {1,2} },
  { {0,2}, {1,3} },
  { {0,3}, {1,4} },
  { {1,0}, {2,1} },
  { {1,1}, {2,2} },
  { {1,2}, {2,3} },
  { {1,3}, {2,4} },
  { {2,0}, {3,1} },
  { {2,1}, {3,2} },
  { {2,2}, {3,3} },
  { {2,3}, {3,4} },
  { {3,0}, {4,1} },
  { {3,1}, {4,2} },
  { {3,2}, {4,3} },
  { {3,3}, {4,4} },
  { {0,4}, {1,3} },
  { {0,3}, {1,2} },
  { {0,2}, {1,1} },
  { {0,1}, {1,0} },
  { {1,4}, {2,3} },
  { {1,3}, {2,2} },
  { {1,2}, {2,1} },
  { {1,1}, {2,0} },
  { {2,4}, {3,3} },
  { {2,3}, {3,2} },
  { {2,2}, {3,1} },
  { {2,1}, {3,0} },
  { {3,4}, {4,3} },
  { {3,3}, {4,2} },
  { {3,2}, {4,1} },
  { {3,1}, {4,0} },
};

void fisher_yates(int *arr, int n) {
     int i, j, tmp;
     for (i = n - 1; i > 0; i--) {
         j = rand() % (i + 1);
         tmp = arr[j];
         arr[j] = arr[i];
         arr[i] = tmp;
     }
}

typedef struct {
	int value;
	int heuristic;
} Move;

int weight1[4]; // vector of heuristic weights for player 1
int weight2[4]; // vector of heuristic weights for player 2
int player_no; // which side the bot is playing as

int calc_heuristic(int player, int board[5][5]) {
	int no_fours = 0;
	int no_threes = 0;
	int no_pairs = 0;
	int block_pairs = 0;
		
	// check win
	
	for (int i = 0; i < 28; ++i) {
		if( (board[win[i][0][0]][win[i][0][1]]==player) && (board[win[i][1][0]][win[i][1][1]]==player) && (board[win[i][2][0]][win[i][2][1]]==player) && (board[win[i][3][0]][win[i][3][1]]==player) ) {
			return INT_MAX / 4;
		}
	}

	// check lose

	for (int i = 0; i < 48; ++i) {
		if( (board[lose[i][0][0]][lose[i][0][1]]==player) && (board[lose[i][1][0]][lose[i][1][1]]==player) && (board[lose[i][2][0]][lose[i][2][1]]==player) ) {
			return -1 * (INT_MAX / 4);
		}
	}

	// count fours

	for (int i = 0; i < 28; ++i) {
		if( (board[win[i][0][0]][win[i][0][1]] != 3 - player) && (board[win[i][1][0]][win[i][1][1]] != 3 - player) && (board[win[i][2][0]][win[i][2][1]] != 3 - player) && (board[win[i][3][0]][win[i][3][1]] != 3 - player) ) {
			++no_fours;
		}
	}

	// count threes

	for (int i = 0; i < 48; ++i) {
		if( (board[lose[i][0][0]][lose[i][0][1]] != 3 - player) && (board[lose[i][1][0]][lose[i][1][1]] != 3 - player) && (board[lose[i][2][0]][lose[i][2][1]] != 3 - player) ) {
			++no_threes;
		}
	}
	
	// count pairs
	
	for (int i = 0; i < 72; ++i) {
		if( (board[pairs[i][0][0]][pairs[i][0][1]] == player) && (board[pairs[i][1][0]][pairs[i][1][1]] == player)) {
			++no_pairs;
		}
	}

	// blocking pairs
	
	for (int i = 0; i < 20; ++i) {
		if( ((board[rpairs[i][0][0]][rpairs[i][0][1]] == 3 - player) && (board[rpairs[i][1][0]][rpairs[i][1][1]] == 3 - player) && (board[rpairs[i][0][1]][min(rpairs[i][0][1] + 1, 4)] == player)) || ((board[rpairs[i][0][0]][rpairs[i][0][1]] == 3 - player) && (board[rpairs[i][1][0]][rpairs[i][1][1]] == 3 - player) && (board[rpairs[i][0][1]][max(rpairs[i][0][1] - 1, 0)] == player))) {
			++block_pairs;
		}
	}
	for (int i = 0; i < 20; ++i) {
		if( ((board[cpairs[i][0][0]][cpairs[i][0][1]] == 3 - player) && (board[cpairs[i][1][0]][cpairs[i][1][1]] == 3 - player) && (board[min(cpairs[i][0][1] + 1, 4)][cpairs[i][0][1]] == player)) || ((board[cpairs[i][0][0]][cpairs[i][0][1]] == 3 - player) && (board[cpairs[i][1][0]][cpairs[i][1][1]] == 3 - player) && (board[max(cpairs[i][0][1] - 1, 0)][cpairs[i][0][1]] == player))) {
			++block_pairs;
		}
	}

	if (player_no == 1) {
		return weight1[0] * no_fours + weight1[1] * no_threes + weight1[2] * no_pairs + weight1[3] * block_pairs;
	} else {
		return weight2[0] * no_fours + weight2[1] * no_threes + weight2[2] * no_pairs + weight2[3] * block_pairs;
	}
}

Move minmax(int player, int curr_board[5][5], int curr_depth, int max_depth, int alpha, int beta, int *states) {
	
	Move optimal_move;
	bool prunning = false;

	// shuffle move order

	int idx[25] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
	fisher_yates(idx, 25);	

	if (curr_depth == max_depth) {

		// find the optimal move by calculating heuristic values on the board positions
		
		if (player == 1) { // MAX player
			
			optimal_move.heuristic = -INT_MAX;

			for (int x = 0; x < 5; ++x) {

				if (prunning) {
					break;
				}

				for (int y = 0; y < 5; ++y) {

					// current move

					int i = idx[5 * x + y] / 5;
					int j = idx[5 * x + y] % 5;

					// calculate move heuristic value

					if (board[i][j] == 0) {
						*states += 1;
						board[i][j] = player;
						int heuristic_value = calc_heuristic(1, board) - calc_heuristic(2, board);

						// replace best move

						if (heuristic_value > optimal_move.heuristic) {
							optimal_move.value = 10 * (i + 1) + (j + 1);
							optimal_move.heuristic = heuristic_value;
						}
						board[i][j] = 0;

						// prunning

						alpha = optimal_move.heuristic;
						if (beta <= alpha) {
							prunning = true;
							break;
						}
					}
				}
			}
		} else { // MIN player

			optimal_move.heuristic = INT_MAX;

			for (int x = 0; x < 5; ++x) {

				if (prunning) {
					break;
				}

				for (int y = 0; y < 5; ++y) {
					
					// current move
					
					int i = idx[5 * x + y] / 5;
					int j = idx[5 * x + y] % 5;
					
					// calculate move heuristic value
					
					if (board[i][j] == 0) {
						*states += 1;
						board[i][j] = player;
						int heuristic_value = calc_heuristic(1, board) - calc_heuristic(2, board);
						
						// replace best move
						
						if (heuristic_value < optimal_move.heuristic) {
							optimal_move.value = 10 * (i + 1) + (j + 1);
							optimal_move.heuristic = heuristic_value;
						}
						board[i][j] = 0;
						
						// prunning
						
						beta = optimal_move.heuristic;
						if (beta <= alpha) {
							prunning = true;
							break;
						}
					}
				}
			}
		}

		return optimal_move;

	} else {

		// find optimal move recursively

		if (player == 1) { // MAX player

			optimal_move.heuristic = -INT_MAX;

			for (int x = 0; x < 5; ++x) {

				if (prunning) {
					break;
				}

				for (int y = 0; y < 5; ++y) {
					
					// current move
					
					int i = idx[5 * x + y] / 5;
					int j = idx[5 * x + y] % 5;
					
					// calculate move heuristic value

					if (board[i][j] == 0) {
						*states += 1;
						int heuristic_value;
						board[i][j] = 1;
						if (winCheck(1) || loseCheck(1)) { // if the move wins/loses, don't branch
							heuristic_value = calc_heuristic(1, board) - calc_heuristic(2, board);
							if (heuristic_value > optimal_move.heuristic) {
								optimal_move.value = 10 * (i + 1) + (j + 1);
								optimal_move.heuristic = heuristic_value;
							}
						} else {
							Move current = minmax(2, board, curr_depth + 1, max_depth, alpha, beta, states);
							heuristic_value = current.heuristic;
						}
						
						// replace best move
						
						if (heuristic_value > optimal_move.heuristic) {
							optimal_move.value = 10 * (i + 1) + (j + 1);
							optimal_move.heuristic = heuristic_value;
						}
						board[i][j] = 0;

						// prunning
						
						alpha = optimal_move.heuristic;
						if (beta <= alpha) {
							prunning = true;
							break;
						}
					}
				}
			}
		} else { // MIN player

			optimal_move.heuristic = INT_MAX;

			for (int x = 0; x < 5; ++x) {

				if (prunning) {
					break;
				}

				for (int y = 0; y < 5; ++y) {
					
					// current move
					
					int i = idx[5 * x + y] / 5;
					int j = idx[5 * x + y] % 5;
					
					// calculate move heuristic value

					if (board[i][j] == 0) {
						*states += 1;
						int heuristic_value;
						board[i][j] = 2;
						if (winCheck(2) || loseCheck(2)) {
							heuristic_value = calc_heuristic(1, board) - calc_heuristic(2, board);
							assert(heuristic_value != 0);
							if (heuristic_value < optimal_move.heuristic) {
								optimal_move.value = 10 * (i + 1) + (j + 1);
								optimal_move.heuristic = heuristic_value;
							}
						} else {
							Move current = minmax(1, board, curr_depth + 1, max_depth, alpha, beta, states);
							heuristic_value = current.heuristic;
						}
						
						// replace best move
						
						if (heuristic_value < optimal_move.heuristic) {
							optimal_move.value = 10 * (i + 1) + (j + 1);
							optimal_move.heuristic = heuristic_value;
						}
						board[i][j] = 0;

						// prunning
						
						alpha = optimal_move.heuristic;
						if (beta <= alpha) {
							prunning = true;
							break;
						}
					}
				}
			}
		}
	}

	return optimal_move;

}

int main(int argc, char *argv[])
{
  srand(time(NULL));

  int socket_desc;
  struct sockaddr_in server_addr;
  char server_message[16], client_message[16];

  bool end_game;
  int player, msg, move;

/*  if( argc!=4 ) {
    printf("Wrong number of arguments\n");
    return -1;
  }*/

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if( socket_desc<0 ) {
    printf("Unable to create socket\n");
    return -1;
  }
//  printf("Socket created successfully\n");

  // Set port and IP the same as server-side
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  // Send connection request to server
  if( connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0 ) {
	perror(NULL);
    printf("Unable to connect\n");
    return -1;
  }
//  printf("Connected with server successfully\n");

  // Receive the server message
  memset(server_message, '\0', sizeof(server_message));
  if( recv(socket_desc, server_message, sizeof(server_message), 0)<0 ) {
    printf("Error while receiving server's message\n");
    return -1;
  }
//  printf("Server message: %s\n",server_message);

  memset(client_message, '\0', sizeof(client_message));
  strcpy(client_message, argv[3]);
  // Send the message to server
  if( send(socket_desc, client_message, strlen(client_message), 0)<0 ) {
    printf("Unable to send message\n");
    return -1;
  }

  setBoard(); 
  end_game = false;
  player = atoi(argv[3]);
  player_no = player;
  
  // set search depth

  int depth = atoi(argv[4]);

  // set vector of weights

  weight1[0] = atoi(argv[5]);
  weight1[1] = atoi(argv[6]);
  weight1[2] = atoi(argv[7]);
  weight1[3] = atoi(argv[8]);
  
  weight2[0] = atoi(argv[9]);
  weight2[1] = atoi(argv[10]);
  weight2[2] = atoi(argv[11]);
  weight2[3] = atoi(argv[12]);

  // result

  int exit_code;

  while( !end_game ) {
    memset(server_message, '\0', sizeof(server_message));
    if( recv(socket_desc, server_message, sizeof(server_message), 0)<0 ) {
      printf("Error while receiving server's message\n");
      return -1;
    }
//    printf("Server message: %s\n", server_message);
    msg = atoi(server_message);
    move = msg%100;
    msg = msg/100;
    if( move!=0 ) {
      setMove(move, 3-player);
//      printBoard();
    }
    if( (msg==0) || (msg==6) ) {
//      printf("Your move: \n");
	  int states = 0;
      Move move = minmax(player, board, 1, depth, -INT_MAX, INT_MAX, &states);
//	  printf("visited %d states\n", states);
//	  printf("%d,%d -> %d\n", move.value / 10, move.value % 10, move.heuristic, &states);
      /*if( fgets(client_message, sizeof(client_message), stdin)==NULL ) {
        printf("Error while reading move\n");
        return -1;
      };
      move = atoi(client_message);*/
      setMove(move.value, player);
//      printBoard();
      memset(client_message, '\0', sizeof(client_message));
      sprintf(client_message, "%d", move);
      if( send(socket_desc, client_message, strlen(client_message), 0)<0 ) {
        printf("Unable to send message\n");
        return -1;
      }
//      printf("Client message: %s\n", client_message);
     }
     else {
       end_game = true;
       switch( msg ) {
         case 1 : printf("You won.\n"); exit_code = 1; break;
         case 2 : printf("You lost.\n"); exit_code = -1; break;
         case 3 : printf("Draw.\n"); exit_code = 0; break;
         case 4 : printf("You won. Opponent error.\n"); exit_code = 1; break;
         case 5 : printf("You lost. Your error.\n"); exit_code = -1; break;
       }
     }
   }

  // Close socket
  close(socket_desc);

  return exit_code;
}
