#ifndef HEADER_FGE_CAMERA
#define HEADER_FGE_CAMERA

#include "cglm/cglm.h"

#include "common/common.h"

/*
    The camera class  (?)
*/

typedef struct FGECamera FGECamera;

struct FGECamera
{
    vec3 camera_up;
    vec3 camera_front;
    vec3 camera_position;

    float yaw;
    float pitch;
};

FGECamera FGECameraNew (vec3 in_origin);
FGEError FGECameraFree (FGECamera* in_camera);

/* Redo this because ugly as hell */
FGEError FGECameraMove (FGECamera* in_camera, vec3 in_pos);
FGEError FGECameraMoveFromFront (FGECamera* in_camera, vec3 in_pos);
FGEError FGECameraMoveFromUp (FGECamera* in_camera, vec3 in_pos);
FGEError FGECameraMoveFromRight (FGECamera* in_camera, vec3 in_pos);

FGEError FGECameraLook (FGECamera* in_camera, float in_yaw, float in_pitch);

FGEError FGECameraSetPos (FGECamera* in_camera, vec3 in_pos);
FGEError FGECameraSetLook (FGECamera* in_camera, float in_yaw, float in_pitch);

FGEError FGECameraUpdate (FGECamera* in_camera);
FGEError FGECameraGetView (FGECamera* in_camera, mat4 _out_view);

#endif