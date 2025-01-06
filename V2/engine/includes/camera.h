#ifndef MGE_CAMERA_H
#define MGE_CAMERA_H

#include "cglm/cglm.h"

#include "common.h"

/*
    The camera class (?)
*/

typedef struct MGECamera MGECamera;

struct MGECamera
{
    vec3 camera_up;
    vec3 camera_front;
    vec3 camera_position;

    float yaw;
    float pitch;
};

MGEError mgeCameraInit(MGECamera* _out_camera, vec3 _in_origin);
MGEError mgeCameraFree(MGECamera* _in_camera);

void mgeCameraMove(MGECamera* _in_camera, vec3 _in_pos);
void mgeCameraMoveFromFront(MGECamera* _in_camera, vec3 _in_pos);
void mgeCameraMoveFromUp(MGECamera* _in_camera, vec3 _in_pos);
void mgeCameraMoveFromRight(MGECamera* _in_camera, vec3 _in_pos);

void mgeCameraLook(MGECamera* _in_camera, float _in_yaw, float _in_pitch);

void mgeCameraSetPos(MGECamera* _in_camera, vec3 _in_pos);
void mgeCameraSetLook(MGECamera* _in_camera, float _in_yaw, float _in_pitch);

void mgeCameraUpdate(MGECamera* _in_camera);
void mgeCameraGetView(MGECamera* _in_camera, mat4 _out_view);

#endif