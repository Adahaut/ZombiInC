#ifndef _H3_ZOMBI_H_
#define _H3_ZOMBI_H_

#include <h3.h>

#define ZOMBI_TYPEID 0x01000059

H3_CAPI_BEGIN_BLOCK
//void ZOMBI_Init(H3Handle object);
//void ZOMBI_Terminate(void* properties);
void ZOMBI_Update(H3Handle h3, H3Handle object, SH3Transform* transform, float t, float dt, void* properties);
void ZOMBI_ColliderON(H3Handle col1, SH3Collision col2);
//void ZOMBI_ColliderLeave(H3Handle object, H3Handle other);
//void ZOMBI_ColliderLeave(H3Handle object, H3Handle other);
void* ZOMBI_CreateProperties(float px, float py, float dist, H3Handle player, H3Handle scene);

H3_DECLARE_COMPONENT_PROPERTY_ACCESSORS_RW_EX(ZOMBI, H3Handle, object)
H3_CAPI_END_BLOCK

#define ZOMBI_CREATE(PX, PY, DIST, PLAYER, SCENE)											\
	(SH3Component) {																			\
		.Update             = ZOMBI_Update,													\
		.isInitialized      = false,															\
		.OnCollisionEnter   = ZOMBI_ColliderON,												\
		.componentType      = ZOMBI_TYPEID,													\
		.properties         = ZOMBI_CreateProperties(PX, PY, DIST, PLAYER, SCENE)			\
}
//.Terminate          = ZOMBI_Terminate,													\

//.OnCollisionLeave   = ZOMBI_ColliderLeave,												\

#endif /* _H3_COMPONENTS_ZOMBI_H_ */
