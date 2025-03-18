#include "movement.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <components/spritecomponent.h>
#include <components/audiosourcecomponent.h>
#include "bullet.h"
#include "zombie.h"
typedef struct
{
    H3Handle scene;
    H3Handle cam;
    H3Handle z;
    int life;
} movement_Properties;

//void movement_Init(void* properties) {
//
//}


void movement_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties)
{
    movement_Properties* props = (movement_Properties*)properties;

    int mx;
    int my;
    float px;
    float py;
    static int _time = 0;

    H3_Input_GetMousePos(h3, &mx, &my); //Mouse's Position
        H3_Transform_GetPosition(H3_Object_GetTransform(object), &px, &py); //Ship's Position


    /*printf("Sprite :%f , %f\n", px, py);*/


    float x, y, w, h, vpw, vph;
    H3_GetView(h3, &x, &y, &w, &h, &vpw, &vph);
    float x0 = x - w / 2;
    float x1 = x + w / 2;
    float y0 = y - h / 2;
    float y1 = y + h / 2;
    float mx01 = mx / vpw;
    float my01 = my / vph;
    float mxScene = x0 + (x1 - x0) * mx01;
    float myScene = y0 + (y1 - y0) * my01;

    float dx = mxScene - px;
    float dy = myScene - py;
    float angle = atan2(dy, dx);
    float dist = sqrtf(dx * dx + dy * dy);
    H3_Object_SetRotation(object, angle * H3_RAD2DEG + 90);
    float finalCamX = min(max(px, w / 2), 2976 - (w / 2));
    float finalCamY = min(max(py, h / 2), 2976 - (h / 2));
    H3_Object_SetTranslation(props->cam, finalCamX, finalCamY);



     if (H3_Input_IsKeyDown(K_Q))
    {
        float pvx = -200.0f;
        float pvy;
        float bin;

        H3_Object_GetVelocity(object, &bin, &pvy);
        H3_Object_SetVelocity(object, pvx, pvy);
    }
    else if (H3_Input_IsKeyDown(K_Z)) 
    {
        float pvx = 0;
        float pvy = -200.0f;

        H3_Object_SetVelocity(object, pvx, pvy);
    }
    else if (H3_Input_IsKeyDown(K_D))
     {
         float pvx = 200;
         float pvy = 0;

         H3_Object_SetVelocity(object, pvx, pvy);
     }
    else if (H3_Input_IsKeyDown(K_S)) 
    {
        float pvx = 0;
        float pvy = 200.0f;

        H3_Object_SetVelocity(object, pvx, pvy);
    }
    else
    {
        float pvx;
        float pvy;
       
        H3_Object_GetVelocity(object, &pvx, &pvy);

        pvx *= 0.8f;
        pvy *= 0.8f;
         
        H3_Object_SetVelocity(object, pvx, pvy);
    }

    if (H3_Input_IsMouseBtnPressed(MB_Left))
    {
        float bdx = dx / dist;
        float bdy = dy / dist;

        float bx = px + bdx * 20.0f;
        float by = py + bdy * 20.0f;


        static int numBullets = 0;
        char buffer[256];
        snprintf(buffer, 256, "bullet%d", ++numBullets);

        //static H3Handle pew = NULL;
        //if (!pew)
        //    pew = H3_Sound_Load("assets/exp.flac");


        H3Handle bullet = H3_Object_Create(props->scene, buffer, NULL);
        H3_Object_EnablePhysics(bullet, H3_CIRCLE_COLLIDER(CDT_Dynamic, 5.0f, 0x22));
        H3_Object_Translate(bullet, bx, by);
        H3_Object_Rotate(bullet, angle * H3_RAD2DEG + 90);
        H3_Object_AddComponent(bullet, SPRITECOMPONENT_CREATE("assets/Bullet.png", A_Left | A_Center));
        H3_Object_AddComponent(bullet, BULLET_CREATE(0, 0));
        H3_Object_SetVelocity(bullet, bdx * 1200.0f, bdy * 1200.0f);

    }

    static int a = 0;
    a++;
    if (a >= 600) {
        static int numZ = 0;
        char buff[256];
        snprintf(buff, 256, "zombi%d", ++numZ);

        
        int zax = rand() % 2977;
        int zay = rand() % 2977;

        H3Handle zombi = H3_Object_Create(props->scene, buff, NULL);
        H3_Object_EnablePhysics(zombi, H3_BOX_COLLIDER(CDT_Dynamic, 25, 25, 0x22, false));
        H3_Object_SetTranslation(zombi, zax, zay);
        H3_Object_AddComponent(zombi, SPRITECOMPONENT_CREATE("assets/Zombie.png", A_Middle | A_Center));
        props->z = zombi;

        float zx, zy;

        H3_Transform_GetPosition(H3_Object_GetTransform(zombi), &zx, &zy); //Ship's Position

        float dzpx = zx - px;
        float dzpy = zy - py;
        float dist = sqrtf(dzpx * dzpx + dzpy * dzpy);


        H3_Object_AddComponent(zombi, ZOMBI_CREATE(px, py, dist, object, props->scene));
        a = 0;
        printf("hello");
    }
    printf("\nplayer life: %d", props->life);

    if (props->life == 0) {
        printf("\nVous êtes mort");
        H3_Terminate(&h3);
    }

}


void movement_Terminate(void* properties)
{
    free(properties);
}

void* movement_CreateProperties(H3Handle scene, H3Handle cam, int life)
{
    movement_Properties* properties = malloc(sizeof(movement_Properties));
    H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

    properties->scene = scene;
    properties->cam = cam;
    properties->life = life;

    return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(movement, MOVEMENT_TYPEID, int, life)
