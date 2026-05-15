#include <stdio.h>
#include <raylib.h>
#include <stdbool.h>
#define TILE_SIZE 64
#define MAP_ROWS 17
#define MAP_COLS 25

int map[MAP_ROWS][MAP_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

typedef struct
{
    float x;
    float speed;
    float accelaration;
    float dashtimer;
    int dashflag;
    float dashcooldown;

    float y;
    float gravity;
    float velocityY;
    bool onground;
    bool doublejump;
    bool dashing;
} Player;

typedef struct
{
    Rectangle rect;
    Color colour;
} platform;

int main(void)
{
    Player P = {
        200.0f,   // x
        500.0f,   // speed
        2.0f,     // accelaration
        0.2f,     // dashtimer
        0,        // dashflag
        0.0f,     // dashcooldown
        824.0f,   // y
        10000.0f, // gravity
        0.0f,     // velocityY
        true,     // onground
        true,     // doublejump
        false     // dashing
    };

    // platform platforms[] = {
    //     {{300, 800, 200, 20}, GREEN},
    //     {{600, 650, 200, 20}, GREEN},
    //     {{900, 500, 200, 20}, GREEN},
    //     {{1500, 500, 200, 50}, RED},
    //     {{2500, 500, 2, 1000}, RED},
    //     {{0, 1080, 10000, 5}, BLUE}};
    // int platformCount = 6;
    //  used for testing platforms and collisions

    InitWindow(1440, 1080, "Title bitch");
    ToggleFullscreen();
    int screen_h = GetScreenHeight();
    int screen_w = GetScreenWidth();
    SetTargetFPS(60);

    // initialing the scrolling camera for the 1st frame
    Camera2D camera = {0};
    camera.target = (Vector2){P.x, P.y};                        // what it looks at
    camera.offset = (Vector2){screen_w / 2 - 50, screen_h / 2}; // where on screen
    camera.zoom = 0.8f;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // X movement
        if (IsKeyDown(KEY_A) && !P.dashing)
        {
            if (IsKeyDown(KEY_LEFT_SHIFT))
                P.x -= P.speed * P.accelaration * dt;
            else
                P.x -= P.speed * dt;
        }
        if (IsKeyDown(KEY_D) && !P.dashing)
        {
            if (IsKeyDown(KEY_LEFT_SHIFT))
                P.x += P.speed * P.accelaration * dt;
            else
                P.x += P.speed * dt;
        }

        // dash start
        if (IsKeyDown(KEY_LEFT_CONTROL) && !P.dashing && P.dashcooldown <= 0)
        {
            if (IsKeyDown(KEY_A))
            {
                P.dashflag = -1;
                P.dashing = true;
            }
            if (IsKeyDown(KEY_D))
            {
                P.dashflag = 1;
                P.dashing = true;
            }
        }

        // dash movement
        if (P.dashing)
        {
            P.x += P.dashflag * 3000 * dt;
            P.dashtimer -= dt;
            if (P.dashtimer <= 0)
            {
                P.dashing = false;
                P.dashcooldown = 2.0f;
                P.dashtimer = 0.2f;
            }
        }
        P.dashcooldown -= dt;
        if (P.dashcooldown < 0)
            P.dashcooldown = 0;

        // X collision
        // for (int i = 0; i < platformCount; i++)
        // {
        //     Rectangle playerRect = {P.x, P.y, 100, 200};
        //     if (CheckCollisionRecs(playerRect, platforms[i].rect))
        //     {
        //         float overlapLeft = (P.x + 100) - platforms[i].rect.x;
        //         float overlapRight = (platforms[i].rect.x + platforms[i].rect.width) - P.x;
        //         if (overlapLeft < overlapRight)
        //             P.x = platforms[i].rect.x - 100;
        //         else
        //             P.x = platforms[i].rect.x + platforms[i].rect.width;
        //     }
        // } used for testing

        // X collision final start
        for (int i = 0; i < MAP_ROWS; i++)
        {
            for (int j = 0; j < MAP_COLS; j++)
            {
                if (map[i][j] == 1)
                {
                    Rectangle tileRect = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                    Rectangle playerRect = {P.x, P.y, 100, 200};
                    if (CheckCollisionRecs(playerRect, tileRect))
                    {
                        float overlapLeft = (P.x + 100) - tileRect.x;
                        float overlapRight = (tileRect.x + tileRect.width) - P.x;
                        if (overlapLeft < overlapRight)
                            P.x = tileRect.x - 100;
                        else
                            P.x = tileRect.x + tileRect.width;
                    }
                }
            }
        }
        // xcollision final end

        // wall clamp
        // if (P.x >= screen_w - 100)
        //     P.x = screen_w - 100;
        // if (P.x <= 0)
        //     P.x = 0; used for testing

        // jump starts
        if (P.onground && IsKeyPressed(KEY_SPACE))
        {
            P.velocityY = -3000.0f;
            P.onground = false;
        }
        else if (!P.onground && P.doublejump && IsKeyPressed(KEY_SPACE))
        {
            P.velocityY = -3000.0f;
            P.doublejump = false;
        }
        // jump ends

        // Y movement starts
        P.velocityY += P.gravity * dt;
        P.y += P.velocityY * dt;
        P.onground = false;
        // y movement ends

        // Y collision - platforms
        // for (int i = 0; i < platformCount; i++)
        // {
        //     Rectangle playerRect = {P.x, P.y, 100, 200};
        //     if (CheckCollisionRecs(playerRect, platforms[i].rect))
        //     {
        //         float overlapTop = (P.y + 200) - platforms[i].rect.y;
        //         float overlapBottom = (platforms[i].rect.y + platforms[i].rect.height) - P.y;
        //         if (overlapTop < overlapBottom)
        //         {
        //             P.y = platforms[i].rect.y - 200;
        //             P.velocityY = 0;
        //             P.onground = true;
        //             P.doublejump = true;
        //         }
        //         else
        //         {
        //             P.y = platforms[i].rect.y + platforms[i].rect.height;
        //             P.velocityY = 0;
        //         }
        //     }
        // } used for testing

        for (int i = 0; i < MAP_ROWS; i++)
        {
            for (int j = 0; j < MAP_COLS; j++)
            {
                if (map[i][j] == 1)
                {
                    Rectangle tileRect = {j * TILE_SIZE,i*TILE_SIZE,TILE_SIZE,TILE_SIZE};
                    Rectangle playerRect ={P.x,P.y,100,200};
                    if(CheckCollisionRecs(tileRect,playerRect))
                    {
                        float overlaptop =(P.y + 200)-tileRect.y;
                        float overlapbottom=(tileRect.y+tileRect.height)-P.y;
                        if(overlaptop<overlapbottom)
                        {
                            P.y=tileRect.y-200;
                            P.velocityY=0;
                            P.onground=true;
                            P.doublejump=true;
                        }
                        else
                        {
                            P.y=tileRect.y+tileRect.height;
                            P.velocityY=0;
                        }
                    }
                }
            }
        }

        // Y collision -ends

        // Y collision - floor
        // if (P.y >= screen_h - 200)
        // {
        //     P.y = screen_h - 200;
        //     P.velocityY = 0;
        //     P.onground = true;
        //     P.doublejump = true;
        // } used for testing

        // camera lerping starts
        camera.target.x += (P.x - camera.target.x) * 6.0f * dt;
        camera.target.y += (P.y - camera.target.y) * 6.0f * dt;
        // camera larping ends

        // drawing starts
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        DrawRectangle(P.x, P.y, 100, 200, WHITE);
        for (int i = 0; i < MAP_ROWS; i++)
        {
            for (int j = 0; j < MAP_COLS; j++)
            {
                if (map[i][j] == 1)
                    DrawRectangle((j * TILE_SIZE), (i * TILE_SIZE), TILE_SIZE, TILE_SIZE, GRAY);
            }
        }

        EndMode2D();
        DrawText(TextFormat("Dash Cooldown: %.1f", P.dashcooldown), 20, 20, 30, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}