#ifndef _H3_BULLET_H_
#define _H3_BULLET_H_

#include <h3.h>

#define BULLET_TYPEID 0x01000009

H3_CAPI_BEGIN_BLOCK
void bullet_Terminate(void* properties);
void bullet_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void bullet_ColliderON(H3Handle object, SH3Collision collide);
void* bullet_CreateProperties(int a, int b);

H3_CAPI_END_BLOCK

#define BULLET_CREATE(A, B)											\
	(SH3Component) {																			\
		.Update             = bullet_Update,													\
		.isInitialized      = false,															\
		.OnCollisionEnter   = bullet_ColliderON,												\
		.componentType      = BULLET_TYPEID,													\
		.properties         = bullet_CreateProperties(A, B)			\
}
//.Terminate          = bullet_Terminate,													\

//.OnCollisionLeave   = bullet_ColliderLeave,												\

#endif /* _H3_COMPONENTS_BULLET_H_ */
