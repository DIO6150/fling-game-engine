#ifndef HEADER_FGE_ANIMATION
#define HEADER_FGE_ANIMATION

#include "tex_array.h"

typedef struct TexAnimState TexAnimState;
typedef struct TextureAnimation TextureAnimation;

// TODO : redo entire system to include state machines

struct TexAnimState
{
    char* name;
    FGETexQuad** frames;
    int current_frame;
    int frame_count;
    double speed;
};

TexAnimState FGETexAnimStateCreate(FGETexArray* in_atlas, int in_speed, char* name, int frame_count, ...);
FGETexQuad* FGETexAnimStateSpriteCreate(char* in_name, FGETexArray* in_array);
int FGETexAnimStateFree(TexAnimState* in_state);

struct TextureAnimation
{
    TexAnimState* states;
    char* name;

    FGETexArray* array;

    int state_count;
    int state_max_count;

    TexAnimState* current_state;

    double anim_time;
};

int FGEAnimationCreate(TextureAnimation** out_animation, FGETexArray* in_array, char* in_name);
int FGEAnimationFree(TextureAnimation* in_animation);

int FGEAnimationAddState(TextureAnimation* in_animation, TexAnimState in_state);
int FGEAnimationSetCurrentState(TextureAnimation* in_animation, char* in_name);

FGETexQuad* FGEAnimationGetQuad(TextureAnimation* in_animation);

int FGEAnimationScroll(TextureAnimation* in_animation, double in_delta_time);

int FGEAnimationGetFrame(TextureAnimation* in_animation);
int FGEAnimationGetFrameCount(TextureAnimation* in_animation);

#endif