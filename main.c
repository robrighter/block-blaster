#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Constants
#define GRID_SIZE 10
#define CELL_SIZE 50
#define GRID_PADDING 20
#define SCREEN_WIDTH (2 * GRID_PADDING + GRID_SIZE * CELL_SIZE + 200)  // Extra space for UI
#define SCREEN_HEIGHT (2 * GRID_PADDING + GRID_SIZE * CELL_SIZE + 150) // Extra space for blocks selection

// Structure for block shapes
typedef struct {
    bool cells[6][6];  // 6x6 grid to accommodate larger shapes
    int width;
    int height;
    Color colorStart;  // For gradient effect
    Color colorEnd;    // For gradient effect
} BlockShape;

// Game state
typedef struct {
    int grid[GRID_SIZE][GRID_SIZE];  // 0 = empty, >0 = color index
    Color gridColors[GRID_SIZE][GRID_SIZE][2];  // Store start and end colors for gradient
    BlockShape currentBlocks[3];     // Three blocks to choose from
    int score;
    bool gameOver;
    bool isDragging;
    int draggedBlockIndex;
    Vector2 dragPosition;
} GameState;

// Function prototypes
void InitGame(GameState *game);
void GenerateRandomBlocks(GameState *game);
bool IsValidPlacement(GameState *game, BlockShape block, int gridX, int gridY);
void PlaceBlock(GameState *game, BlockShape block, int gridX, int gridY);
void CheckAndClearLines(GameState *game);
void UpdateGame(GameState *game);
void DrawGame(GameState *game);
void DrawBlockShape(BlockShape block, int x, int y, bool transparent);
void DrawGradientRect(int x, int y, int width, int height, Color colorStart, Color colorEnd);

// Define block shapes
BlockShape ShapeLibrary[] = {
    // 1x1 block
    {
        .cells = {{1}},
        .width = 1,
        .height = 1,
        .colorStart = {230, 41, 55, 255},   // RED
        .colorEnd = {255, 161, 150, 255}    // Light red
    },
    // 2x2 block
    {
        .cells = {{1,1},
                  {1,1}},
        .width = 2,
        .height = 2,
        .colorStart = {0, 228, 48, 255},    // GREEN
        .colorEnd = {161, 255, 150, 255}    // Light green
    },
    // L shape
    {
        .cells = {{1,0},
                  {1,0},
                  {1,1}},
        .width = 2,
        .height = 3,
        .colorStart = {0, 121, 241, 255},   // BLUE
        .colorEnd = {150, 220, 255, 255}    // Light blue
    },
    // Tetris line (4 blocks)
    {
        .cells = {{1},
                  {1},
                  {1},
                  {1}},
        .width = 1,
        .height = 4,
        .colorStart = {253, 249, 0, 255},   // YELLOW
        .colorEnd = {255, 253, 150, 255}    // Light yellow
    },
    // T shape
    {
        .cells = {{1,1,1},
                  {0,1,0}},
        .width = 3,
        .height = 2,
        .colorStart = {200, 122, 255, 255}, // PURPLE
        .colorEnd = {230, 190, 255, 255}    // Light purple
    },
    // Horizontal line (6 blocks)
    {
        .cells = {{1,1,1,1,1,1}},
        .width = 6,
        .height = 1,
        .colorStart = {255, 109, 194, 255}, // PINK
        .colorEnd = {255, 200, 230, 255}    // Light pink
    },
    // Vertical line (6 blocks)
    {
        .cells = {{1},
                  {1},
                  {1},
                  {1},
                  {1},
                  {1}},
        .width = 1,
        .height = 6,
        .colorStart = {255, 144, 30, 255},  // ORANGE
        .colorEnd = {255, 210, 161, 255}    // Light orange
    }
};

int main(void)
{
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Block Blast");
    SetTargetFPS(60);
    
    // Initialize random seed
    srand(time(NULL));
    
    // Initialize game state
    GameState game;
    InitGame(&game);
    
    // Main game loop
    while (!WindowShouldClose())
    {
        UpdateGame(&game);
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawGame(&game);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

void InitGame(GameState *game)
{
    // Initialize grid to empty
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            game->grid[y][x] = 0;
        }
    }
    
    game->score = 0;
    game->gameOver = false;
    game->isDragging = false;
    
    // Generate initial blocks
    GenerateRandomBlocks(game);
}

void GenerateRandomBlocks(GameState *game)
{
    // Generate 3 random blocks from the shape library
    int numShapes = sizeof(ShapeLibrary) / sizeof(ShapeLibrary[0]);
    
    for (int i = 0; i < 3; i++) {
        int shapeIndex = rand() % numShapes;
        game->currentBlocks[i] = ShapeLibrary[shapeIndex];
    }
}

bool IsValidPlacement(GameState *game, BlockShape block, int gridX, int gridY)
{
    // Check if the block can be placed at the specified position
    for (int y = 0; y < block.height; y++) {
        for (int x = 0; x < block.width; x++) {
            if (block.cells[y][x]) {
                int posX = gridX + x;
                int posY = gridY + y;
                
                // Check boundaries
                if (posX < 0 || posX >= GRID_SIZE || posY < 0 || posY >= GRID_SIZE) {
                    return false;
                }
                
                // Check collision with existing blocks
                if (game->grid[posY][posX] != 0) {
                    return false;
                }
            }
        }
    }
    
    return true;
}

void PlaceBlock(GameState *game, BlockShape block, int gridX, int gridY)
{
    // Place the block on the grid
    int colorIndex = 1 + rand() % 1000; // Random color index
    
    for (int y = 0; y < block.height; y++) {
        for (int x = 0; x < block.width; x++) {
            if (block.cells[y][x]) {
                game->grid[gridY + y][gridX + x] = colorIndex;
                game->gridColors[gridY + y][gridX + x][0] = block.colorStart;
                game->gridColors[gridY + y][gridX + x][1] = block.colorEnd;
            }
        }
    }
    
    // Remove the placed block and generate a new one
    int numShapes = sizeof(ShapeLibrary) / sizeof(ShapeLibrary[0]);
    game->currentBlocks[game->draggedBlockIndex] = ShapeLibrary[rand() % numShapes];
    game->isDragging = false;
    
    // Check for completed lines
    CheckAndClearLines(game);
    
    // Check game over condition: no valid moves for any of the three blocks
    bool canPlaceAny = false;
    for (int i = 0; i < 3; i++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int x = 0; x < GRID_SIZE; x++) {
                if (IsValidPlacement(game, game->currentBlocks[i], x, y)) {
                    canPlaceAny = true;
                    break;
                }
            }
            if (canPlaceAny) break;
        }
        if (canPlaceAny) break;
    }
    
    if (!canPlaceAny) {
        game->gameOver = true;
    }
}

void CheckAndClearLines(GameState *game)
{
    int linesCleared = 0;
    
    // Check horizontal lines
    for (int y = 0; y < GRID_SIZE; y++) {
        bool lineComplete = true;
        for (int x = 0; x < GRID_SIZE; x++) {
            if (game->grid[y][x] == 0) {
                lineComplete = false;
                break;
            }
        }
        
        if (lineComplete) {
            linesCleared++;
            // Clear the line
            for (int x = 0; x < GRID_SIZE; x++) {
                game->grid[y][x] = 0;
            }
        }
    }
    
    // Check vertical lines
    for (int x = 0; x < GRID_SIZE; x++) {
        bool lineComplete = true;
        for (int y = 0; y < GRID_SIZE; y++) {
            if (game->grid[y][x] == 0) {
                lineComplete = false;
                break;
            }
        }
        
        if (lineComplete) {
            linesCleared++;
            // Clear the line
            for (int y = 0; y < GRID_SIZE; y++) {
                game->grid[y][x] = 0;
            }
        }
    }
    
    // Award points based on lines cleared
    if (linesCleared == 1) {
        game->score += 100;
    } else if (linesCleared == 2) {
        game->score += 300; // Bonus for two lines
    } else if (linesCleared >= 3) {
        game->score += 500 * linesCleared; // Big bonus for 3+ lines
    }
}

void UpdateGame(GameState *game)
{
    if (game->gameOver) {
        // Handle game over state
        if (IsKeyPressed(KEY_R)) {
            InitGame(game);
        }
        return;
    }
    
    // Handle mouse input for dragging blocks
    Vector2 mousePos = GetMousePosition();
    
    // Convert mouse position to grid coordinates
    int gridX = (mousePos.x - GRID_PADDING - CELL_SIZE/2) / CELL_SIZE;
    int gridY = (mousePos.y - GRID_PADDING - CELL_SIZE/2) / CELL_SIZE;
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Check if clicking on one of the three blocks at bottom
        for (int i = 0; i < 3; i++) {
            int blockX = GRID_PADDING-90 + CELL_SIZE * (i * 4 + 1);
            int blockY = SCREEN_HEIGHT - CELL_SIZE * 2;
            int blockWidth = CELL_SIZE * 3;
            int blockHeight = CELL_SIZE;
            
            if (mousePos.x >= blockX && mousePos.x < blockX + blockWidth &&
                mousePos.y >= blockY && mousePos.y < blockY + blockHeight) {
                game->isDragging = true;
                game->draggedBlockIndex = i;
                game->dragPosition = mousePos;
                break;
            }
        }
    } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && game->isDragging) {
        // Try to place the block on the grid
        if (gridX >= 0 && gridX < GRID_SIZE && gridY >= 0 && gridY < GRID_SIZE) {
            // Adjust placement for the block's size
            gridX = gridX - game->currentBlocks[game->draggedBlockIndex].width / 2;
            gridY = gridY - game->currentBlocks[game->draggedBlockIndex].height / 2;
            
            if (IsValidPlacement(game, game->currentBlocks[game->draggedBlockIndex], gridX, gridY)) {
                PlaceBlock(game, game->currentBlocks[game->draggedBlockIndex], gridX, gridY);
            }
        }
        game->isDragging = false;
    }
    
    // Update drag position if dragging
    if (game->isDragging) {
        game->dragPosition = mousePos;
    }
}

void DrawGame(GameState *game)
{
    // Draw outer frame for the grid
    DrawRectangle(GRID_PADDING - 5, GRID_PADDING - 5, 
                 GRID_SIZE * CELL_SIZE + 10, GRID_SIZE * CELL_SIZE + 10, 
                 DARKGRAY);
    
    // Draw grid background
    DrawRectangle(GRID_PADDING, GRID_PADDING, 
                 GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE, 
                 LIGHTGRAY);
    
    // Draw grid lines
    for (int y = 0; y <= GRID_SIZE; y++) {
        DrawLine(GRID_PADDING, GRID_PADDING + y * CELL_SIZE, 
                GRID_PADDING + GRID_SIZE * CELL_SIZE, GRID_PADDING + y * CELL_SIZE, 
                DARKGRAY);
    }
    
    for (int x = 0; x <= GRID_SIZE; x++) {
        DrawLine(GRID_PADDING + x * CELL_SIZE, GRID_PADDING, 
                GRID_PADDING + x * CELL_SIZE, GRID_PADDING + GRID_SIZE * CELL_SIZE, 
                DARKGRAY);
    }
    
    // Draw blocks on the grid
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (game->grid[y][x] != 0) {
                // Get colors for gradient
                Color colorStart = game->gridColors[y][x][0];
                Color colorEnd = game->gridColors[y][x][1];
                
                // Draw gradient cell
                DrawGradientRect(
                    GRID_PADDING + x * CELL_SIZE, 
                    GRID_PADDING + y * CELL_SIZE, 
                    CELL_SIZE, 
                    CELL_SIZE, 
                    colorStart, 
                    colorEnd
                );
                
                // Draw cell border
                DrawRectangleLines(
                    GRID_PADDING + x * CELL_SIZE, 
                    GRID_PADDING + y * CELL_SIZE, 
                    CELL_SIZE, 
                    CELL_SIZE, 
                    BLACK
                );
            }
        }
    }
    
    // Draw the three available blocks container
    DrawRectangle(GRID_PADDING, SCREEN_HEIGHT - CELL_SIZE * 3, 
                 GRID_SIZE * CELL_SIZE, CELL_SIZE * 2.5, 
                 DARKGRAY);
    DrawRectangle(GRID_PADDING + 5, SCREEN_HEIGHT - CELL_SIZE * 3 + 5, 
                 GRID_SIZE * CELL_SIZE - 10, CELL_SIZE * 2.5 - 10, 
                 LIGHTGRAY);
    
    // Draw the three available blocks
    for (int i = 0; i < 3; i++) {
        int blockX = GRID_PADDING-90 + CELL_SIZE * (i * 4 + 1);
        int blockY = SCREEN_HEIGHT - CELL_SIZE * 2;
        
        DrawBlockShape(game->currentBlocks[i], blockX, blockY, false);
    }
    
    // Draw currently dragged block
    if (game->isDragging) {
        // Draw at mouse position with transparency
        //DrawBlockShape(game->currentBlocks[game->draggedBlockIndex], 
        //              game->dragPosition.x - CELL_SIZE, 
        //              game->dragPosition.y - CELL_SIZE, 
        //              true);
        
        // Show ghost placement on grid
        int gridX = (game->dragPosition.x - GRID_PADDING - CELL_SIZE/2) / CELL_SIZE - game->currentBlocks[game->draggedBlockIndex].width / 2;
        int gridY = (game->dragPosition.y - GRID_PADDING - CELL_SIZE/2) / CELL_SIZE - game->currentBlocks[game->draggedBlockIndex].height / 2;
        
        if (gridX >= 0 && gridY >= 0 && gridX + game->currentBlocks[game->draggedBlockIndex].width <= GRID_SIZE && 
            gridY + game->currentBlocks[game->draggedBlockIndex].height <= GRID_SIZE) {
            
            bool isValid = IsValidPlacement(game, game->currentBlocks[game->draggedBlockIndex], gridX, gridY);
            
            // Draw ghost of where block would be placed
            for (int y = 0; y < game->currentBlocks[game->draggedBlockIndex].height; y++) {
                for (int x = 0; x < game->currentBlocks[game->draggedBlockIndex].width; x++) {
                    if (game->currentBlocks[game->draggedBlockIndex].cells[y][x]) {
                        Color ghostColor = game->currentBlocks[game->draggedBlockIndex].colorStart;
                        ghostColor.a = 100; // Semi-transparent
                        
                        if (!isValid) {
                            // Red  for invalid placement
                            ghostColor.r = 255;
                            ghostColor.g = 100;
                            ghostColor.b = 100;
                        }
                        
                        DrawRectangle(
                            GRID_PADDING + (gridX + x) * CELL_SIZE, 
                            GRID_PADDING + (gridY + y) * CELL_SIZE, 
                            CELL_SIZE, 
                            CELL_SIZE, 
                            ghostColor
                        );
                    }
                }
            }
        }
    }
    
    // Draw score in a nice frame
    DrawRectangle(GRID_PADDING + GRID_SIZE * CELL_SIZE + 20, GRID_PADDING, 160, 60, DARKGRAY);
    DrawRectangle(GRID_PADDING + GRID_SIZE * CELL_SIZE + 25, GRID_PADDING + 5, 150, 50, LIGHTGRAY);
    DrawText("SCORE", GRID_PADDING + GRID_SIZE * CELL_SIZE + 65, GRID_PADDING + 10, 20, BLACK);
    DrawText(TextFormat("%d", game->score), GRID_PADDING + GRID_SIZE * CELL_SIZE + 75, GRID_PADDING + 30, 24, DARKBLUE);
    
    // Draw game title
    DrawText("BLOCK BLAST", GRID_PADDING + GRID_SIZE * CELL_SIZE + 30, GRID_PADDING + 80, 20, DARKBLUE);
    
    // Draw instructions
    DrawText("Drag blocks to grid", GRID_PADDING + GRID_SIZE * CELL_SIZE + 20, GRID_PADDING + 120, 16, DARKGRAY);
    DrawText("Clear rows or columns", GRID_PADDING + GRID_SIZE * CELL_SIZE + 20, GRID_PADDING + 145, 16, DARKGRAY);
    DrawText("for points!", GRID_PADDING + GRID_SIZE * CELL_SIZE + 20, GRID_PADDING + 170, 16, DARKGRAY);
    
    // Draw game over message if game is over
    if (game->gameOver) {
        // Semi-transparent overlay
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 150});
        
        // Game over box
        DrawRectangle(SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 75, 300, 150, DARKGRAY);
        DrawRectangle(SCREEN_WIDTH/2 - 145, SCREEN_HEIGHT/2 - 70, 290, 140, LIGHTGRAY);
        
        DrawText("GAME OVER", SCREEN_WIDTH/2 - 110, SCREEN_HEIGHT/2 - 50, 30, RED);
        DrawText(TextFormat("Final Score: %d", game->score), SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 10, 20, BLACK);
        DrawText("Press R to Restart", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 + 30, 20, DARKBLUE);
    }
}

void DrawGradientRect(int x, int y, int width, int height, Color colorStart, Color colorEnd)
{
    // Draw a rectangle with vertical gradient
    for (int i = 0; i < height; i++) {
        float factor = (float)i / height;
        Color color = {
            (unsigned char)(colorStart.r + (colorEnd.r - colorStart.r) * factor),
            (unsigned char)(colorStart.g + (colorEnd.g - colorStart.g) * factor),
            (unsigned char)(colorStart.b + (colorEnd.b - colorStart.b) * factor),
            (unsigned char)(colorStart.a + (colorEnd.a - colorStart.a) * factor)
        };
        
        DrawLine(x, y + i, x + width - 1, y + i, color);
    }
}

void DrawBlockShape(BlockShape block, int x, int y, bool transparent)
{
    // Scale factor for blocks in selection area
    float scale = 0.5;
    int cellSize = CELL_SIZE * scale;
    
    // Adjust for smaller blocks in the selection area
    if (!transparent) {
        // Center the block in its display area
        x += (CELL_SIZE * 3 - block.width * cellSize) / 2;
        y += (CELL_SIZE - block.height * cellSize) / 2;
    } else {
        cellSize = CELL_SIZE; // Full size when dragging
    }
    
    Color colorStart = block.colorStart;
    Color colorEnd = block.colorEnd;
    
    if (transparent) {
        colorStart.a = 200;
        colorEnd.a = 200;
    }
    
    for (int blockY = 0; blockY < block.height; blockY++) {
        for (int blockX = 0; blockX < block.width; blockX++) {
            if (block.cells[blockY][blockX]) {
                // Draw gradient cell
                DrawGradientRect(
                    x + blockX * cellSize, 
                    y + blockY * cellSize, 
                    cellSize, 
                    cellSize, 
                    colorStart, 
                    colorEnd
                );
                
                // Draw cell border
                DrawRectangleLines(
                    x + blockX * cellSize, 
                    y + blockY * cellSize, 
                    cellSize, 
                    cellSize, 
                    BLACK
                );
            }
        }
    }
}