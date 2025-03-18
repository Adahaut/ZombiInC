#include <h3.h>
#include <components/cameracomponent.h>
#include <components/spritecomponent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <components/maplayercomponent.h>
#include <components/textcomponent.h>
#include "movement.h"


#ifndef NDEBUG
# pragma comment(lib, "h3-s-d.lib")
#else // !NDEBUG
# pragma comment(lib, "h3-s.lib")
#endif // !NDEBUG

int main(int argc, char** argv)
{

    H3Handle h3 = H3_Init((SH3InitParams) {
        .width = 1920,
            .height = 1024,
            .fullscreen = false,
            .windowTitle = "Zombie"
    });

    srand(time(NULL));

    bool running = true;
    int score = 0;
    int life = 3;

    H3Handle scn = H3_Scene_Create(h3, false);


    H3Handle map = H3_Map_Load("assets/map.tmx");


    H3Handle Map = H3_Map_Load("assets/map.tmx");
    H3Handle MapObject = H3_Object_Create(scn, "mapobject", NULL);
    H3Handle MapBackGround = H3_Object_Create(scn, "mapbackground", NULL);
    H3Handle SolidLayer = H3_Object_Create(scn, "solidlayer", NULL);

    H3_Object_AddComponent(MapBackGround, (MAPLAYERCOMPONENT_CREATE(Map, "BackGround")));
    H3_Object_AddComponent(SolidLayer, (MAPLAYERCOMPONENT_CREATE(Map, "SolidLayer")));
    H3_Object_AddComponent(MapObject, (MAPLAYERCOMPONENT_CREATE(Map, "Solid")));
    H3_Map_RegisterObjectLayerForPhysicsInScene(scn, Map, "Solid");


    H3Handle player = H3_Object_Create(scn, "player", NULL);
    H3_Object_AddComponent(player, SPRITECOMPONENT_CREATE("assets/survivor-idle_rifle_0.png", A_Middle | A_Center));
    H3_Object_EnablePhysics(player, H3_BOX_COLLIDER(CDT_Dynamic, 15, 25, 0x22, false));

    H3Handle cam = H3_Object_Create(scn, "camera", NULL);
    H3_Object_AddComponent(cam, CAMERACOMPONENT_CREATE(480, 256));

    H3_Object_AddComponent(player, MOVEMENT_CREATE(scn, cam, life));
    H3_Object_Translate(player, 100, 100);


    H3Handle font = H3_Font_Load("assets/Comfortaa-Regular.ttf");
    H3Handle lifeText = H3_Object_Create(scn, "lifeText", NULL);
    H3_Object_SetTranslation(lifeText, 10, 10);

    SH3TextProperties textProps1 = {
    .font = font,
    .size = 15,
    .fillColor = {255,0,0,255},
    .hasOutline = false,
    .outlineColor = {0,255,0,0},
    .outlineThickness = 5,
    .anchor = A_Left | A_Top,
    .isBold = false,
    .isItalic = false,
    .isUnderlined = false,
    .isViewLocal = true
    };

    SH3TextProperties textProps2 = {
        .font = font,
        .size = 15,
        .fillColor = {255,0,0,255},
        .hasOutline = false,
        .outlineColor = {0,255,0,0},
        .outlineThickness = 5,
        .anchor = A_Right | A_Top,
        .isBold = false,
        .isItalic = false,
        .isUnderlined = false,
        .isViewLocal = true
    };
    char lifeBuffer[256];

    H3_Object_AddComponent(lifeText, TEXTCOMPONENT_CREATE("", textProps1));





    typedef struct
    {
        const char* Text;
        SH3TextProperties Props;
    } TextComponent_Properties;


    do {
        running = H3_DoFrame(h3, scn);
    } while (running);


    H3_Object_Destroy(cam, true);
    H3_Object_Destroy(player, true);
    H3_Object_Destroy(MapObject, true);
    H3_Object_Destroy(MapBackGround, true);
    H3_Object_Destroy(SolidLayer, true);

    H3_Scene_Destroy(scn);

    H3_Terminate(&h3);


    return 0;
}

