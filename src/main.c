#include <stdint.h>
#include <raylib.h>

#define FlipFlag(x, y, z) (*(x) = (*(x) == (y)) ? (z) : (y))

typedef enum {
    FlagUp,
    FlagDown,
    FlagLeft,
    FlagRight,
    FlagPlayer1Score,
    FlagPlayer2Score,
} Flags;

typedef enum {
    Counting,
    Playing,
    Scored,
    Winner,
} GameScreens;

typedef struct {
    uint16_t x;
    uint16_t y;
    Flags xSpeed;
    Flags ySpeed;
} Ball;

const Color BLACK_COLOR = { .r = 25, .g = 25, .b = 25, .a = 255 };
const Color WHITE_COLOR = { .r = 255, .g = 255, .b = 255, .a = 255 };
const Color RED_COLOR = { .r = 199, .g = 60, .b = 63, .a = 255 };
const Color YELLOW_COLOR = { .r = 225, .g = 221, .b = 51, .a = 255 };
const Color TRANS_WHITE_COLOR = { .r = 255, .g = 255, .b = 255, .a = 15};

const uint16_t SCREEN_WIDTH = 800;
const uint16_t SCREEN_HEIGHT = 450;
const uint16_t TARGET_FPS = 60;
const char GAME_NAME[] = "cPong";

const uint8_t HIGHEST_SCORE = 10;
const uint16_t BORDER_PADDING = 10;
const uint16_t PLAYER_SPEED = 10;
const uint16_t BALL_SPEED = 5;
const uint16_t BALL_RADIUS = (SCREEN_WIDTH * 0.01);
const uint16_t PLATFORM_WIDTH = (SCREEN_WIDTH * 0.025);
const uint16_t PLATFORM_HEIGHT = 120;

static Sound score;
static Sound platform_hit;

static uint16_t framesCounter = 0;
static uint8_t player1Score = 0;
static uint8_t player2Score = 0;
static GameScreens gameState = Counting;

Rectangle player1 = { .x = 0.0, .y = (SCREEN_HEIGHT / 2.0) - (PLATFORM_HEIGHT / 2.0), .width = PLATFORM_WIDTH, .height = PLATFORM_HEIGHT };
Rectangle player2 = { .x = SCREEN_WIDTH - PLATFORM_WIDTH, .y = (SCREEN_HEIGHT / 2.0) - (PLATFORM_HEIGHT / 2.0), .width = PLATFORM_WIDTH, .height = PLATFORM_HEIGHT };
Ball ball = { .x = SCREEN_WIDTH / 2.0, .y = SCREEN_HEIGHT / 2.0, .xSpeed = FlagLeft, .ySpeed = FlagDown };

float startBallX, startBallY;
float endBallX = SCREEN_WIDTH / 2.0, endBallY = SCREEN_HEIGHT / 2.0;

float startPlayer1Y, startPlayer2Y;
float endPlayersY = (SCREEN_HEIGHT / 2.0) - (PLATFORM_HEIGHT / 2.0);

uint16_t max(const int16_t a, const int16_t b) { return (a > b) ? a : b; }
uint16_t min(const int16_t a, const int16_t b) { return (a < b) ? a : b; }

float lerp(float start, float end, float t)
{
    return start + t * (end - start);
}

void setAnimationValues(void)
{
    startBallX = ball.x;
    startBallY = ball.y;
    startPlayer1Y = player1.y;
    startPlayer2Y = player2.y;
}

void drawTextCenteredAt(const char *text, const uint16_t x, const uint16_t y, const uint16_t fontSize, const Color color)
{
    uint16_t textWidth = MeasureText(text, fontSize);
    DrawText(text, x - textWidth / 2, y - fontSize / 2, fontSize, color);
}

void checkBallCollision(void)
{
    Vector2 ballPos = { .x = ball.x, .y = ball.y };
    if (CheckCollisionCircleRec(ballPos, BALL_RADIUS, player1)&& ball.xSpeed == FlagLeft) {
        PlaySound(platform_hit);
        FlipFlag(&ball.xSpeed, FlagRight, FlagLeft);
    } else if (CheckCollisionCircleRec(ballPos, BALL_RADIUS, player2) && ball.xSpeed == FlagRight) { 
        PlaySound(platform_hit);
        FlipFlag(&ball.xSpeed, FlagRight, FlagLeft);
    }
}

void updateScoreboard(Flags player)
{
    if (player == FlagPlayer1Score) {
        ++player1Score;
    } else {
        ++player2Score;
    }
}

void updateBallPos(Ball *ball, float speedMultiplier)
{
    ball->y += (ball->ySpeed == FlagUp ? -1 : 1) * (BALL_SPEED + speedMultiplier);
    if (ball->y >= SCREEN_HEIGHT - BALL_RADIUS - BORDER_PADDING) {
        FlipFlag(&ball->ySpeed, FlagUp, FlagDown);
        ball->y = SCREEN_HEIGHT - BALL_RADIUS - BORDER_PADDING;
    }
    if (ball->y <= BORDER_PADDING) {
        FlipFlag(&ball->ySpeed, FlagUp, FlagDown);
        ball->y = BORDER_PADDING;
    }

    ball->x += (ball->xSpeed == FlagLeft ? -1 : 1) * (BALL_SPEED + speedMultiplier);

    checkBallCollision();

    if (ball->x + BALL_RADIUS >= SCREEN_WIDTH) {
        FlipFlag(&ball->xSpeed, FlagRight, FlagLeft);
        updateScoreboard(FlagPlayer1Score);

        PlaySound(score);
        setAnimationValues();
        ball->x = SCREEN_WIDTH - BALL_RADIUS;

        framesCounter = TARGET_FPS * 3;
        gameState = Scored;
    }
    if (ball->x - BALL_RADIUS <= 0) {
        FlipFlag(&ball->xSpeed, FlagRight, FlagLeft);
        updateScoreboard(FlagPlayer2Score);

        PlaySound(score);
        setAnimationValues();
        ball->x = BALL_RADIUS;

        framesCounter = TARGET_FPS * 3;
        gameState = Scored;
    }
}

void updatePlayersPos(void)
{
    //if (IsKeyDown(KEY_W)) {
    //    player1.y = max(10, player1.y - PLAYER_SPEED);
    //}
    //if (IsKeyDown(KEY_S)) {
    //    player1.y = min(SCREEN_HEIGHT - PLATFORM_HEIGHT - 10, player1.y + PLAYER_SPEED);
    //}

    //if (IsKeyDown(KEY_UP)) {
    //    player2.y = max(10, player2.y - PLAYER_SPEED);
    //}
    //if (IsKeyDown(KEY_DOWN)) {
    //    player2.y = min(SCREEN_HEIGHT - PLATFORM_HEIGHT - 10, player2.y + PLAYER_SPEED);
    //}
    
    if (ball.y < player1.y + PLATFORM_HEIGHT / 2.0f) {
        player1.y = max(10, player1.y - PLAYER_SPEED);
    }
    if (ball.y > player1.y + PLATFORM_HEIGHT / 2.0f) {
        player1.y = min(SCREEN_HEIGHT - PLATFORM_HEIGHT - 10, player1.y + PLAYER_SPEED);
    }

    if (ball.y < player2.y + PLATFORM_HEIGHT / 2.0f) {
        player2.y = max(10, player2.y - PLAYER_SPEED);
    }
    if (ball.y > player2.y + PLATFORM_HEIGHT / 2.0f) {
        player2.y = min(SCREEN_HEIGHT - PLATFORM_HEIGHT - 10, player2.y + PLAYER_SPEED);
    }
}

int main(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
    SetTargetFPS(TARGET_FPS);

    InitAudioDevice();

    Music soundtrack = LoadMusicStream("sounds/soundtrack.ogg");
    platform_hit = LoadSound("sounds/platform-hit.wav");
    score = LoadSound("sounds/score.wav");

    SetMusicVolume(soundtrack, 0.2);
    SetSoundVolume(platform_hit, 0.6);
    SetSoundVolume(score, 0.3);

    PlayMusicStream(soundtrack);

    framesCounter = TARGET_FPS * 3;
    float velocityMultiplier = 0.0f;
    float t = 0.0f;

    while (!WindowShouldClose())
    {
        UpdateMusicStream(soundtrack);

        // Update
        switch (gameState) {
            case Counting:
                    --framesCounter;
                    if (framesCounter <= 0)
                        gameState = Playing;
                    break;
            case Playing:
                    velocityMultiplier += 0.001f;
                    updatePlayersPos();
                    updateBallPos(&ball, velocityMultiplier);
                    
                    if (player1Score == HIGHEST_SCORE || player2Score == HIGHEST_SCORE)
                        gameState = Winner;

                    break;
            case Scored:
                    t = 1.0 - (float)framesCounter / (TARGET_FPS * 3);
                    ball.x = lerp(startBallX, endBallX, t);
                    ball.y = lerp(startBallY, endBallY, t);
                    player1.y = lerp(startPlayer1Y, endPlayersY, t);
                    player2.y = lerp(startPlayer2Y, endPlayersY, t);

                    --framesCounter;
                    if (framesCounter <= 0) {
                        gameState = Playing;
                    }
                    break;
            case Winner:
                    if (IsKeyDown(KEY_R)) {
                        gameState = Counting;
                        framesCounter = TARGET_FPS * 3;
                        velocityMultiplier = 0.0f;

                        player1Score = 0;
                        player2Score = 0;

                        player1.y = (SCREEN_HEIGHT / 2.0) - (PLATFORM_HEIGHT / 2.0);
                        player2.y = (SCREEN_HEIGHT / 2.0) - (PLATFORM_HEIGHT / 2.0);
                        ball.x = SCREEN_WIDTH / 2;
                        ball.y = SCREEN_HEIGHT / 2;
                    }
                    if (IsKeyDown(KEY_Q)) {
                        CloseWindow();
                    }

                    break;
        }

        // Draw
        BeginDrawing();
        {
            ClearBackground(BLACK_COLOR);

            switch (gameState) {
                case Counting:
                    // Draw counter
                    drawTextCenteredAt(TextFormat("%i", (int)framesCounter / TARGET_FPS + 1), SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, SCREEN_HEIGHT * 0.2f, WHITE_COLOR);

                    // Draw players and ball ghosts
                    DrawRectangleRounded(player1, 0.2f, 0.0f, TRANS_WHITE_COLOR);
                    DrawRectangleRounded(player2, 0.2f, 0.0f, TRANS_WHITE_COLOR);
                    break;
                case Playing:
                    // Draw scoreboard
                    drawTextCenteredAt(TextFormat("%i", player1Score), SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f, SCREEN_HEIGHT * 0.3f, TRANS_WHITE_COLOR);
                    drawTextCenteredAt(TextFormat("%i", player2Score), SCREEN_WIDTH / 4.0f * 3, SCREEN_HEIGHT / 2.0f, SCREEN_HEIGHT * 0.3f, TRANS_WHITE_COLOR);
                    
                    // Draw players and ball
                    DrawRectangleRounded(player1, 0.2f, 0.0f, WHITE_COLOR);
                    DrawRectangleRounded(player2, 0.2f, 0.0f, WHITE_COLOR);
                    DrawCircle(ball.x, ball.y, BALL_RADIUS, WHITE_COLOR);
                    break;
                case Scored:
                    // Draw scoreboard
                    drawTextCenteredAt(TextFormat("%i", (int)framesCounter / TARGET_FPS + 1), SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, SCREEN_HEIGHT * 0.2f, WHITE_COLOR);
                    drawTextCenteredAt(TextFormat("%i", player1Score), SCREEN_WIDTH / 4.0f, SCREEN_HEIGHT / 2.0f, SCREEN_HEIGHT * 0.3f, TRANS_WHITE_COLOR);
                    drawTextCenteredAt(TextFormat("%i", player2Score), SCREEN_WIDTH / 4.0f * 3, SCREEN_HEIGHT / 2.0f, SCREEN_HEIGHT * 0.3f, TRANS_WHITE_COLOR);
                    
                    // Draw players and ball
                    DrawRectangleRounded(player1, 0.2f, 0.0f, RED_COLOR);
                    DrawRectangleRounded(player2, 0.2f, 0.0f, RED_COLOR);
                    DrawCircle(ball.x, ball.y, BALL_RADIUS, RED_COLOR);
                    break;
                case Winner:
                    // Draw scoreboard
                    if (player1Score == HIGHEST_SCORE) {
                        drawTextCenteredAt(TextFormat("%i", player1Score), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, SCREEN_HEIGHT * 0.3, WHITE_COLOR);
                        drawTextCenteredAt("Player 1 Wins", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT * 0.2, 20, WHITE_COLOR);
                    } else {
                        drawTextCenteredAt(TextFormat("%i", player1Score), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2, SCREEN_HEIGHT * 0.3, TRANS_WHITE_COLOR);
                    }

                    if (player2Score == HIGHEST_SCORE) {
                        drawTextCenteredAt(TextFormat("%i", player2Score), SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 2, SCREEN_HEIGHT * 0.3, WHITE_COLOR);
                        drawTextCenteredAt("Player 2 Wins", SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT * 0.2, 20, WHITE_COLOR);
                    } else {
                        drawTextCenteredAt(TextFormat("%i", player2Score), SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 2, SCREEN_HEIGHT * 0.3, TRANS_WHITE_COLOR);
                    }

                    drawTextCenteredAt("Press \'R\' to RESTART game or \'Q\' to QUIT.", SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30, 20, WHITE_COLOR);

                    break;
            }
        }
        EndDrawing();
    }

    UnloadMusicStream(soundtrack);
    UnloadSound(platform_hit);
    UnloadSound(score);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
