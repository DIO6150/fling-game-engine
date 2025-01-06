#ifndef H_MGE_ANIMATION
#define H_MGE_ANIMATION

#include "texture.h"

typedef struct TexAnimState TexAnimState;
typedef struct TextureAnimation TextureAnimation;

struct TexAnimState
{
    char* name;
    TextureQuad** frames;
    int current_frame;
    int frame_count;
    double speed;
};

TexAnimState mgeTexAnimStateCreate(TextureArray2D* _in_atlas, int _in_speed, char* name, int frame_count, ...);
TextureQuad* mgeTexAnimStateSpriteCreate(char* _in_name, TextureArray2D* _in_array);
int mgeTexAnimStateFree(TexAnimState* _in_state);

struct TextureAnimation
{
    TexAnimState* states;
    char* name;

    TextureArray2D* array;

    int state_count;
    int state_max_count;

    TexAnimState* current_state;

    double anim_time;
};

int mgeAnimationCreate(TextureAnimation** _out_animation, TextureArray2D* _in_array, char* _in_name);
int mgeAnimationFree(TextureAnimation* _in_animation);

int mgeAnimationAddState(TextureAnimation* _in_animation, TexAnimState _in_state);
int mgeAnimationSetCurrentState(TextureAnimation* _in_animation, char* _in_name);

TextureQuad* mgeAnimationGetQuad(TextureAnimation* _in_animation);

int mgeAnimationScroll(TextureAnimation* _in_animation, double _in_delta_time);

int mgeAnimationGetFrame(TextureAnimation* _in_animation);
int mgeAnimationGetFrameCount(TextureAnimation* _in_animation);

#endif