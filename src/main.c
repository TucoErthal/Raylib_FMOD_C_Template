#include <stdlib.h>
#include <stdio.h>
#include "include/raylib.h"
#include "include/fmod_studio.h"
#include "include/fmod_errors.h"

int main(void)
{
    // FMOD INIT
    FMOD_RESULT result;
    FMOD_STUDIO_BANK* bank;
    FMOD_STUDIO_BANK* stringsBank;
    FMOD_STUDIO_EVENTDESCRIPTION* musicEventDescription;
    FMOD_STUDIO_EVENTINSTANCE* musicEventInstance;
    
    FMOD_STUDIO_SYSTEM* studioSystem;
    result = FMOD_Studio_System_Create(&studioSystem, FMOD_VERSION); if (result != FMOD_OK){printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result)); exit(-1);}

    FMOD_Studio_System_Initialize(studioSystem, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    FMOD_Studio_System_LoadBankFile(studioSystem, "assets/FMOD/Build/Desktop/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
    FMOD_Studio_System_LoadBankFile(studioSystem, "assets/FMOD/Build/Desktop/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank);

    FMOD_Studio_System_GetEvent(studioSystem, "event:/music", &musicEventDescription);
    FMOD_Studio_EventDescription_CreateInstance(musicEventDescription, &musicEventInstance);
    FMOD_Studio_EventInstance_Start(musicEventInstance);

    // RAYLIB INIT
    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE);
    InitWindow(0, 0, "Main");
    
    Image icon = LoadImage("assets/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);

    
    const int nativeWidth = 256;
    const int nativeHeight = 192;
    RenderTexture2D nativeScreen = LoadRenderTexture(nativeWidth, nativeHeight);
    
    while (!WindowShouldClose())
    {
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        // FMOD Update
        float bulletTimeParameter = GetMousePosition().y / GetScreenHeight();
        float stateParameter = GetMousePosition().x / screenW;
        FMOD_Studio_EventInstance_SetParameterByName(musicEventInstance, "bulletTime", bulletTimeParameter, true);
        FMOD_Studio_EventInstance_SetParameterByName(musicEventInstance, "state", stateParameter, true);
        printf("bulletTime = %f\tstate = %f\n", bulletTimeParameter, stateParameter);
        FMOD_Studio_System_Update(studioSystem);
        
        BeginTextureMode(nativeScreen);
        {
            ClearBackground(BLACK);
            DrawRectangle
            (
                GetMousePosition().x * ((float) nativeWidth / screenW),
                GetMousePosition().y * ((float) nativeHeight / screenH),
                8, 8, WHITE
            );
        }
        EndTextureMode();

        BeginDrawing();
        {
            ClearBackground(BLACK);
            DrawTexturePro
            (
                nativeScreen.texture,
                (Rectangle){ 0, 0, (float)nativeScreen.texture.width, -(float)nativeScreen.texture.height },  // Source rectangle with flipped height
                (Rectangle){ 0, 0, screenW, screenH },  // Destination rectangle (full screen)
                (Vector2){ 0, 0 },
                0.0f,
                WHITE
            );
        }
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}