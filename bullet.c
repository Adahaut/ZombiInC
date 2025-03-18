#include "bullet.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <components/spritecomponent.h>
#include <components/audiosourcecomponent.h>
#include "zombie.h"




typedef struct
{
    bool dest;
    int a;
    int b;
    int life;
    H3Handle object;
} bullet_Properties;

void bullet_Terminate(void* properties) {
	free(properties);
}

void bullet_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties) {

    bullet_Properties* props = (bullet_Properties*)properties;

    if (props->dest == false) {
        H3_Object_Destroy(object, false);
    }


} 



void bullet_ColliderON(H3Handle self, SH3Collision collide) {
    SH3Component* component = H3_Object_GetComponent(self, BULLET_TYPEID);
    bullet_Properties* props = (bullet_Properties*)(component->properties);
    printf("bine");
    props->dest = false;
    if (collide.other != NULL) {
        if (H3_Object_HasComponent(collide.other, ZOMBI_TYPEID)) {
            static int a = 2;
            a -= 1;
            if (a <= 0)
                H3_Object_Destroy(collide.other, true);
        }
    }
}


void* bullet_CreateProperties(int a, int b) {
    bullet_Properties* properties = malloc(sizeof(bullet_Properties));
    H3_ASSERT_CONSOLE(properties, "Failed to allocate properties");

    properties->a = a;
    properties->b = b;
    properties->life = 2;
    return properties;
}

