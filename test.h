#ifndef INCLUDE_TEST_H_
#define INCLUDE_TEST_H_
void initTiles();
void resetEmptyTiles();
void getEmptyTiles();
void getMovesLeft();
void getMoveList(int dir);
void UpdateAnimations(float delta);
void DrawGameGrid(float delta);
void MoveRight();
void MoveLeft();
void MoveUp();
void MoveDown();
void getGesture();
bool leftInput();
bool rightInput();
bool upInput();
bool downInput();
void autoMovement();
bool moveableUp();
bool moveableDown();
bool moveableLeft();
bool moveableRight();
void processMoveable();
void processInput();
unsigned int getScore();
void drawScore();
void SpawnRandomTile();
void setScreenSizes();
void drawAllTiles();
void resetGame();
void handleGameOver(bool update);
void processGameOver();
int main();
#endif //INCLUDE_TEST_H_
