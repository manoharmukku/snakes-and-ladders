# snakes-and-ladders
A program to play the Snakes and Ladders game.
Recall that the game is played using a 10×10 board, with the cells numbered 1–100.

Each cell is of one of the following three types:
- blank: Nothing special happens when the player lands on such a cell.
- ladder: The cell contains the lower end of a ladder. If a player’s marker lands on such
a cell, it moves at once to the upper end of the ladder (i.e., a cell of a higher number).
- snake: The cell contains the mouth of a snake. If a player’s marker lands on such a cell, it moves at once to the tail end of the snake (i.e., a cell of a lower number).

Any board may be described using 2 lines. The first line contains a list of cell numbers corresponding to the 2 end points of snakes; similarly, the second line contains a list of cell numbers corresponding to the end points of the ladders. For example, if a board is described using the following 2 lines:
21 15 99 79 35 18
41 59 70 94 77 84 12 28
it contains 3 snakes (mouths at 21, 99 and 35; tails at 15, 79 and 18 respectively), and 4 ladders (from 41 to 59, 70 to 94, etc.).
The game is played as follows. Each player in turn rolls a regular die (six faces) and moves her marker an appropriate number of cells. If the marker lands on a cell containing the bottom of a ladder or the mouth of a snake, the marker is moved to the top of the ladder / tail end of the snake. The game ends when one of the player’s markers reaches cell number 100.
Special conditions:
• Each player has to roll a 1 on the die to get started (by moving to cell 1); otherwise, she remains stuck at zero, i.e., outside the board.
• A player’s marker will not move at all if its target position is outside the board (e.g., if the marker is on cell 97, and the player rolls a 4).
Input: Your program should take two command line arguments: a file name containing the board description, and an integer between 2 and 6 specifying the number of players.
It should first check that the board is valid, i.e., (i) all cell numbers are between 1 and 100; (ii) snakes go from higher numbered cells to lower numbered cells, while ladders go from lower numbered cells to higher numbered cells; (iii) no cell contains both the lower end of a ladder and the mouth of a snake; and (iv) there are no loops formed by the snakes and ladders (e.g., a snake from 21 to 19, a ladder from 19 to 34, and a snake from 34 to 21). Note that other combinations are permitted. A cell may contain both the upper end of a ladder, and the tail end of a snake. Snakes and / or ladders may also form chains (but no loops).
If the board is invalid, your program should print an error message and terminate. Othwer- wise, it should describe how the game progresses, by printing 1 line corresponding to each roll of the die. You should use an appropriate random number generator to simulate the die.
