#include "raylib.h"
#include <cstdio>

struct Player {
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    float moveSpeed;
    float jumpForce;
    bool isGrounded;
    bool isFalling;
};

struct JumpState {
    bool isHoldingJump = false;
    bool hasTarget = false;
    float targetY = 0.0f;

    // Jump buffer state
    float bufferTimer = 0.0f;
    float bufferDuration = 0.12f; // 120 ms before landing

    // Coyote time state (continuous window after walking off a ledge)
    float coyoteTimer = 0.0f;
    float coyoteDuration = 0.12f;
};

void updateJumpTimers(const Player &P, JumpState &JS, float dt, bool keyPressed) {
    // Arm jump buffer on press (even while airborne)
    if (keyPressed) {
        JS.bufferTimer = JS.bufferDuration;
    } else if (JS.bufferTimer > 0.0f) {
        JS.bufferTimer -= dt;
        if (JS.bufferTimer < 0.0f) JS.bufferTimer = 0.0f;
    }

    // Track coyote time when airborne without having jumped
    if (P.isGrounded) {
        JS.coyoteTimer = JS.coyoteDuration;
    } else if (JS.coyoteTimer > 0.0f) {
        JS.coyoteTimer -= dt;
        if (JS.coyoteTimer < 0.0f) JS.coyoteTimer = 0.0f;
    }
}

void jumpSystem(Player &P, JumpState &JS, float dt, bool keyPressed, bool keyHeld, bool keyReleased) {
    updateJumpTimers(P, JS, dt, keyPressed);

    // 1. On release, clear hold/target state
    if (keyReleased) {
        JS.isHoldingJump = false;
        JS.hasTarget = false;
    }

    if (keyPressed) {
        JS.isHoldingJump = true;
    }

    // 2. Consume buffered jump when grounded or within coyote window
    bool canJumpNow = P.isGrounded || (JS.coyoteTimer > 0.0f);
    if (JS.bufferTimer > 0.0f && canJumpNow) {
        float limit_jump = 100.395f;
        JS.targetY = P.position.y - limit_jump;
        JS.hasTarget = true;
        JS.isHoldingJump = keyHeld || keyPressed;

        P.velocity.y = -P.jumpForce;
        P.isGrounded = false;
        P.isFalling = false;

        // Consume buffer and coyote time so they don't double-trigger
        JS.bufferTimer = 0.0f;
        JS.coyoteTimer = 0.0f;
    }

    // 3. While holding jump and above targetY (higher Y on screen = lower in air), keep upward velocity
    if (keyHeld && JS.isHoldingJump && JS.hasTarget) {
        if (P.position.y > JS.targetY) {
            P.velocity.y = -P.jumpForce;
        } else {
            JS.hasTarget = false;
        }
    }
}

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Personaje, posicion y salto basico - Raylib");
    SetTargetFPS(60);

    const float groundY = 380.0f;
    const float gravity = 1200.0f; // píxeles / s^2

    Player player = {
        .position   = { 200.0f, groundY - 60.0f },
        .velocity   = { 0.0f, 0.0f },
        .size       = { 40.0f, 60.0f },
        .moveSpeed  = 260.0f,
        .jumpForce  = 520.0f,
        .isGrounded = true,
        .isFalling  = false
    };

    JumpState jumpState;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // 1. Horizontal movement
        player.velocity.x = 0.0f;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.velocity.x += player.moveSpeed;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) player.velocity.x -= player.moveSpeed;

        player.position.x += player.velocity.x * dt;

        // 2. Input state & jump system
        bool keyPressed  = IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP);
        bool keyHeld     = IsKeyDown(KEY_SPACE)    || IsKeyDown(KEY_W)    || IsKeyDown(KEY_UP);
        bool keyReleased = IsKeyReleased(KEY_SPACE) || IsKeyReleased(KEY_W) || IsKeyReleased(KEY_UP);

        jumpSystem(player, jumpState, dt, keyPressed, keyHeld, keyReleased);

        // 3. Apply gravity and move Y
        player.velocity.y += gravity * dt;
        player.position.y += player.velocity.y * dt;
        player.isFalling = (!player.isGrounded && player.velocity.y > 0.0f);

        // 4. Ground collision
        float bottomY = player.position.y + player.size.y;
        if (bottomY >= groundY) {
            player.position.y = groundY - player.size.y;
            player.velocity.y = 0.0f;
            player.isGrounded = true;
            player.isFalling = false;
        } else {
            player.isGrounded = false;
        }

        Vector2 currentPos = player.position;

        // 5. Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawRectangle(0, (int)groundY, screenWidth, screenHeight - (int)groundY, LIGHTGRAY);
            DrawRectangleV(player.position, player.size, BLUE);

            char infoText[200];
            std::snprintf(
                infoText,
                sizeof(infoText),
                "Pos: (%.1f, %.1f) | VelY: %.1f | Ground: %s | Buf: %.2f | Coy: %.2f",
                currentPos.x,
                currentPos.y,
                player.velocity.y,
                player.isGrounded ? "YES" : "NO",
                jumpState.bufferTimer,
                jumpState.coyoteTimer
            );
            DrawText(infoText, 20, 20, 18, DARKGRAY);
            DrawText("A/D or Arrows: Move | Space/W/Up: Jump (buffered)", 20, 46, 18, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
