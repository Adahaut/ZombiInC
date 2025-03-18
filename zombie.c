#include "zombie.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <components/spritecomponent.h>
#include <components/audiosourcecomponent.h>
#include "movement.h"
#include "bullet.h"


typedef struct
{
    bool dest;
    float px;
    float py;
    float dist;
    H3Handle player;
    int life;
    int plife;
    H3Handle object;
} ZOMBI_Properties;

void ZOMBI_Terminate(void* properties) {
	free(properties);
}

void ZOMBI_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {

    ZOMBI_Properties* props = (ZOMBI_Properties*)properties;
    float gspeed = 30;
    float vx, vy;
    H3_Object_GetVelocity(object, &vx, &vy);
    float px, py, zx, zy;

    
    H3_Transform_GetPosition(H3_Object_GetTransform(props->player), &px, &py);
    H3_Transform_GetPosition(H3_Object_GetTransform(object), &zx, &zy);
   
    float dx = zx - px;
    float dy = zy - py;

    float dist = sqrtf(dx * dx + dy * dy);

    if (dist <= 200) {
        H3_Object_SetVelocity(object, -dx/dist * 20, -dy/dist * 10);
    }
    else {
        static int time = 0;
        time++;
        if (time == 600) { // random move each 10 sec
            int d = rand() % 4;

            if (d == 0) {
                H3_Object_SetVelocity(object, vx, -1 * gspeed);
            }
            if (d == 1) {
                H3_Object_SetVelocity(object, -1 * gspeed, vy);
            }
            if (d == 2) {
                H3_Object_SetVelocity(object, vx, 1 * gspeed);
            }
            if (d == 3) {
                H3_Object_SetVelocity(object, 1 * gspeed, vy);
            }
            time = 0;
        }
    }
    static time = 300;
    time++;
    int plife = movement_GetlifeEx(props->player);
    if (dist <= 25 && time >= 300) {
        plife -= 1;
        if (plife <= 0)
        {
            plife = 0;
        }
        time = 0;
    }
    movement_SetlifeEx(props->player, plife);
    if (props->life <= 0) {
        H3_Object_Destroy(object, false);
    }
    props->object = object;
}



void ZOMBI_ColliderON(H3Handle col1, SH3Collision col2) {
    //SH3Component* component = H3_Object_GetComponent(col1, ZOMBI_TYPEID);
    //ZOMBI_Properties* props = (ZOMBI_Properties*)(component->properties);
    //if (col2.other != NULL) {
    //    if (H3_Object_HasComponent(col2.other, BULLET_TYPEID)) {
    //    printf("caca boudain\n");
    //        props->life -= 1;
    //    }
    //}
}


void* ZOMBI_CreateProperties(float px, float py, float dist, H3Handle player, int life) {
    ZOMBI_Properties* properties = malloc(sizeof(ZOMBI_Properties));
    H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

    properties->px = px;
    properties->py = py;
    properties->dist = dist;
    properties->player = player;
    properties->life = 2;
    return properties;
}

H3_DEFINE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(ZOMBI, ZOMBI_TYPEID, H3Handle, object)