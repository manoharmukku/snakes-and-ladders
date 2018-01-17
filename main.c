#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INVALID 0
#define VALID 1
#define FALSE 0
#define TRUE 1
#define EMPTY 'e'
#define MOUTH 'm'
#define TAIL 't'
#define START 's'
#define FINISH 'f'

int checkAndFillBoard(char* board, int cell, char input) {
    char current = board[cell];
    // If current cell is empty, fill it with the input
    if (current == EMPTY) {
        board[cell] = input;
        return VALID;
    }
    
    // Check for invalid cases and fill appropirately
    if (input == MOUTH || input == START) {
        if (current == MOUTH || current == START)
            return INVALID;
        if (current == TAIL || current == FINISH) {
            board[cell] = input;
            return VALID;
        }
    }

    return VALID;
}

typedef struct node {
    int data;
    struct node* next;
} Node;

typedef struct {
    int V;
    Node** adj;
} Graph;

// Function to initialize graph
void initGraph(Graph *G, int V) {
    G->V = V;
    if (NULL == ((G->adj) = (Node**)malloc(V * sizeof(Node*)))) {
        perror("Out of memory");
        exit(-1);
    }
    int i;
    for (i = 0; i < V; i++)
        (G->adj)[i] = NULL;
}

// Helper function to get a new node
Node* getNewNode(int x) {
    Node* newNode;
    if (NULL == (newNode = (Node*)malloc(sizeof(Node)))) {
        perror("Out of memory");
        exit(-1);
    }
    newNode->data = x;
    newNode->next = NULL;
    return newNode;
}

// Function to add an edge between i -> j
void addEdge(Graph *G, int i, int j) {
    Node* newNode = getNewNode(j);

    Node* next = (G->adj)[i];
    (G->adj)[i] = newNode;
    newNode->next = next;
}

// Helper function to check cycle
int isCyclePresentUtil(Graph *G, int v, int* visited, int* recStack) {
    if (!visited[v]) {
        // Mark the current node and part of recStack as visited
        visited[v] = TRUE;
        recStack[v] = TRUE;

        // Recur for adjacent vertices to this vertex
        Node* current = (G->adj)[v];
        while(current != NULL) {
            if (!visited[current->data] && isCyclePresentUtil(G, current->data, visited, recStack))
                return TRUE;
            else if (recStack[current->data])
                return TRUE;
            current = current->next;
        }
    }
    recStack[v] = FALSE; // remove this vertex from recStack
    return FALSE;
}

// Function to check if a cycle is present 
int isCyclePresent(int* snakes, int* ladders) {
    // Create a graph
    Graph *G;
    if (NULL == (G = (Graph*)malloc(sizeof(Graph)))) {
        perror("Out of memory");
        exit(-1);
    }

    // Initialize graph
    initGraph(G, 101);

    // Add Edges to graph using snakes and ladders input
    int i;
    for (i = 0; i < 101; i++) {
        if (snakes[i] != -1)
            addEdge(G, i, snakes[i]);
        if (ladders[i] != -1)
            addEdge(G, i, ladders[i]);
    }

    int *visited, *recStack;
    if ((NULL == (visited = (int*)malloc((G->V) * sizeof(int)))) || (NULL == (recStack = (int*)malloc((G->V) * sizeof(int))))) {
        perror("Out of memory");
        exit(-1);
    }
    // Make all vertices and part of recStack as not visited
    for (i = 0; i < (G->V); i++) {
        visited[i] = FALSE;
        recStack[i] = FALSE;
    }

    // Check if cycle is present in different DFS trees
    int result = FALSE;
    for (i = 0; i < (G->V); i++) {
        if (isCyclePresentUtil(G, i, visited, recStack)) {
            result = TRUE;
            break;
        }
    }

    // Free memory
    for (i = 0; i < (G->V); i++) {
        Node* current = (G->adj)[i];
        while (current != NULL) {
            Node* next = current->next;
            free(current);
            current = next;
        }
    }
    free(G->adj);
    free(G);
    free(visited);
    free(recStack);

    return result;
}

int main(int argc, char** argv) {
    // Sanity check of command line arguments
    if (argc != 3) {
        perror("Usage: ./a.out fileName no_of_players");
        return -1;
    }

    // Open file
    FILE* fp;
    if (NULL == (fp = fopen(argv[1], "r"))) {
        perror("Error: Cannot open file");
        return -1;
    }

    // Number of players
    int n = atoi(argv[2]);
    // Sanity check on the value of n
    if (n <= 0) {
        perror("Error: Number of players should be positive");
        return -1;
    }

    // Allocate memory for the board
    char* board;
    if (NULL == (board = (char*)malloc(101 * sizeof(char)))) {
        perror("Out of memory: Cannot allocate memory for board");
        return -1;
    }

    /*
    n - neutral cell
    e - empty cell
    m - mouth of a snake
    t - tail of a snake
    s - start of a ladder
    f - finish of a ladder
    */

    // Initialize all the cells to empty
    int i;
    board[0] = 'n';
    for (i = 1; i < 101; i++) 
        board[i] = 'e';

    // Create two additional arrays snakes and ladders
    // which stores where to go when a snake or ladder is encountered
    int *snakes, *ladders;
    if ((NULL == (snakes = (int*)malloc(101 * sizeof(int)))) || (NULL == (ladders = (int*)malloc(101 * sizeof(int))))) {
        perror("Out of memory: Cannot allocate memory for snakes or ladders arrays");
        return -1;
    }
    // Initialize entires in both the arrays to -1
    for (i = 0; i < 101; i++)
        snakes[i] = ladders[i] = -1;

    // String to store the line for fgets()
    char* line;
    if (NULL == (line = (char*)malloc(500 * sizeof(char)))) {
        perror("Error: Out of memory");
        return -1;
    }

    // Fill snakes array along with board array
    if (NULL == fgets(line, 500, fp)) {
        perror("Error: Cannot read from file");
        return -1;
    }
    char* token = strtok(line, " ");
    int mouthCell = atoi(token);
    if (mouthCell < 1 || mouthCell >= 100 || INVALID == checkAndFillBoard(board, mouthCell, MOUTH)) {
        printf("The board is INVALID\n");
        return 0;
    }
    int tailCell, flag = 1;
    while (token != NULL) {
        if (flag == 1) {
            token = strtok(NULL, " ");
            if (token == NULL)
                break;
            tailCell = atoi(token);
            if (tailCell < 1 || tailCell > 100 || tailCell > mouthCell || INVALID == checkAndFillBoard(board, tailCell, TAIL)) {
                printf("The board is INVALID\n");
                return 0;
            }
            snakes[mouthCell] = tailCell;
            flag = 0;
        }
        else {
            token = strtok(NULL, " ");
            if (token == NULL)
                break;
            mouthCell = atoi(token);
            if (mouthCell < 1 || mouthCell >= 100 || INVALID == checkAndFillBoard(board, mouthCell, MOUTH)) {
                printf("The board is INVALID\n");
                return 0;
            }
            flag = 1;
        }
    }
    if (flag == 1) {
        perror("Incomplete input: Mouth of a snake given without tail");
        return -1;
    }

    // Fill the ladders array along with board array
    if (NULL == fgets(line, 500, fp)) {
        perror("Error: Cannot read from file");
        return -1;
    }
    token = strtok(line, " ");
    int startCell = atoi(token);
    if (startCell < 1 || startCell >= 100 || INVALID == checkAndFillBoard(board, startCell, START)) {
        printf("The board is INVALID\n");
        return 0;
    }
    int finishCell;
    flag = 1;
    while (token != NULL) {
        if (flag == 1) {
            token = strtok(NULL, " ");
            if (token == NULL)
                break;
            finishCell = atoi(token);
            if (finishCell < 1 || finishCell > 100 || finishCell < startCell || INVALID == checkAndFillBoard(board, finishCell, FINISH)) {
                printf("The board is INVALID\n");
                return 0;
            }
            ladders[startCell] = finishCell;
            flag = 0;
        }
        else {
            token = strtok(NULL, " ");
            if (token == NULL)
                break;
            startCell = atoi(token);
            if (startCell < 1 || startCell >= 100 || INVALID == checkAndFillBoard(board, startCell, START)) {
                printf("The board is INVALID\n");
                return 0;
            }
            flag = 1;
        }
    }
    if (flag == 1) {
        perror("Incomplete input: Bottom of a ladder given without end");
        return -1;
    }

    // Close the file
    fclose(fp);

    // Check if cycles are present
    if (isCyclePresent(snakes, ladders)) {
        printf("The board is INVALID\n");
        return 0;
    }

    // Start playing the game and print the progress
    srand(time(0));

    int* currentCell;
    if (NULL == (currentCell = (int*)malloc((n+1) * sizeof(int)))) {
        perror("Out of memory");
        return -1;
    }
    for (i = 0; i <= n; i++)
        currentCell[i] = 0;

    int gameOver = FALSE;
    while(!gameOver) {
        for (i = 1; i <= n; i++) {
            int roll = (rand() % 6) + 1;
            int nextCell = currentCell[i] + roll;

            if (currentCell[i] == 0) {
                printf("Player %d (cell %d): rolls %d", i, currentCell[i], roll);
                if (roll != 1)
                    printf(", no movement.\n");
                else {
                    printf(", moves to cell %d", nextCell);
                    currentCell[i] = nextCell;

                    int done = FALSE;
                    while (!done) {
                        if (board[currentCell[i]] == EMPTY || board[currentCell[i]] == TAIL || board[currentCell[i]] == FINISH) {
                            if (currentCell[i] == 100) {
                                printf(", wins.\n");
                                gameOver = TRUE;
                            }
                            else
                                printf(".\n");
                            done = TRUE;
                        }
                        else if (board[currentCell[i]] == START) {
                            nextCell = ladders[currentCell[i]];
                            printf(", climbs ladder to cell %d", nextCell);
                            currentCell[i] = nextCell;
                        }
                        else if (board[currentCell[i]] == MOUTH) {
                            nextCell = snakes[currentCell[i]];
                            printf(", goes down snake to cell %d", nextCell);
                            currentCell[i] = nextCell;
                        }
                    }
                    if (gameOver)
                        break;
                }
            }
            else {
                printf("Player %d (cell %d): rolls %d", i, currentCell[i], roll);
                if (nextCell > 100) 
                    printf(", no movement.\n");
                else if (nextCell == 100) {
                    printf(", wins.\n");
                    currentCell[i] = nextCell;
                    gameOver = TRUE;
                    break;
                }
                else {
                    printf(", moves to cell %d", nextCell);
                    currentCell[i] = nextCell;

                    int done = FALSE;
                    while (!done) {
                        if (board[currentCell[i]] == EMPTY || board[currentCell[i]] == TAIL || board[currentCell[i]] == FINISH) {
                            if (currentCell[i] == 100) {
                                printf(", wins.\n");
                                gameOver = TRUE;
                            }
                            else
                                printf(".\n");
                            done = TRUE;
                        }
                        else if (board[currentCell[i]] == START) {
                            nextCell = ladders[currentCell[i]];
                            printf(", climbs ladder to cell %d", nextCell);
                            currentCell[i] = nextCell;
                        }
                        else if (board[currentCell[i]] == MOUTH) {
                            nextCell = snakes[currentCell[i]];
                            printf(", goes down snake to cell %d", nextCell);
                            currentCell[i] = nextCell;
                        }
                    }
                    if (gameOver)
                        break;
                }
            }
        }
    }
    printf("Game over.\n");

    // Free the allocated memory
    free(board);
    free(snakes);
    free(ladders);
    free(line);
    free(currentCell);

    return 0;
}
