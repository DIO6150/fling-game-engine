
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core/texture_animation.h"

TexAnimState mgeTexAnimStateCreate(TextureArray2D* _in_atlas, int _in_speed, char* name, int frame_count, ...)
{
    TexAnimState state;

    state.name = malloc ( ( strlen(name) + 1) * sizeof (char));
    strcpy(state.name, name);

    state.frame_count = frame_count;

    state.frames = malloc (sizeof (TextureQuad*) * frame_count);

    va_list args;
    va_start(args, frame_count);

    for (int i = 0; i < frame_count; i ++)
    {
        state.frames[i] = mgeTexAnimStateSpriteCreate(va_arg(args, char*), _in_atlas);
    }

    va_end(args);

    state.speed = _in_speed;

    state.current_frame = 0;

    return (state);
}

TextureQuad* mgeTexAnimStateSpriteCreate(char* _in_name, TextureArray2D* _in_array)
{
    TextureQuad* quad;
    if (!mgeTextureArray2DCreateQuad(&quad, _in_name, _in_array))
    {
        mgeTextureArray2DCreateQuad(&quad, "", _in_array);
    }

    return (quad);
}

int mgeTexAnimStateFree (TexAnimState* _in_state)
{
    for (int i = 0; i < _in_state->frame_count; i ++)
    {
        free(_in_state->frames[i]);
    }
    free(_in_state->frames);
    free(_in_state->name);

    return (1);
}

int mgeAnimationCreate(TextureAnimation** _out_animation, TextureArray2D* _in_array, char* _in_name)
{
    (*_out_animation) = malloc(sizeof (TextureAnimation));

    if (!(*_out_animation))
    {
        printf("%s(%s) : Couldn't initialize TextureAnimation.\n", __func__, __FILE__);
        return (0);
    }

    (*_out_animation)->states = malloc (sizeof (TexAnimState) * 16);
    (*_out_animation)->state_count = 0;
    (*_out_animation)->state_max_count = 16;

    (*_out_animation)->name = malloc(sizeof (char) * (strlen(_in_name) + 1));
    strcpy((*_out_animation)->name, _in_name);

    (*_out_animation)->anim_time = 0.0;
    (*_out_animation)->current_state = NULL;

    (*_out_animation)->array = _in_array;

    return (1);
}

int mgeAnimationFree(TextureAnimation* _in_animation)
{
    if (!_in_animation)
    {
        printf("%s(%s) : TextureAnimation is NULL.\n", __func__, __FILE__);
        return (0);
    }


    if (_in_animation->states)
    {
        mgeTexAnimStateFree(_in_animation->states);
    }

    free(_in_animation->name);

    free(_in_animation);

    return (1);
}

int mgeAnimationAddState(TextureAnimation* _in_animation, TexAnimState _in_state)
{
    if (_in_animation->state_count >= _in_animation->state_max_count)
    {
        _in_animation->state_max_count *= 2;
        TexAnimState* states = realloc(_in_animation->states, sizeof (TexAnimState) * _in_animation->state_max_count);
        assert (states);

        _in_animation->states = states;
    }

    _in_animation->states[_in_animation->state_count] = _in_state;
    _in_animation->current_state = _in_animation->state_count + _in_animation->states;

    _in_animation->state_count ++;
    return (1);
}

int mgeAnimationSetCurrentState (TextureAnimation* _in_animation, char* _in_name)
{
    assert(_in_animation);

    assert(_in_animation->current_state);

    _in_animation->current_state->current_frame = 0;

    for (int i = 0; i < _in_animation->state_count; i ++)
    {
        if (!strcmp(_in_animation->states[i].name, _in_name))
        {
            _in_animation->current_state = _in_animation->states + i;
            return (1);
        }
    }

    return (0);
}

TextureQuad* mgeAnimationGetQuad (TextureAnimation* _in_animation)
{
    assert (_in_animation);

    for (int i = 0; i < _in_animation->state_count; i++)
    {
       int frame = _in_animation->current_state->current_frame;

       frame = frame > _in_animation->current_state->frame_count ? 0 : frame;

       return (_in_animation->current_state->frames[frame]);
    }

    return (NULL);
}

int mgeAnimationScroll(TextureAnimation* _in_animation, double _in_delta_time)
{
    TexAnimState* state = _in_animation->current_state;

    _in_animation->anim_time += _in_delta_time;

    if (_in_animation->anim_time >= _in_delta_time * state->speed * 60.0)
    {
        _in_animation->anim_time = 0.0;
        state->current_frame ++;
    }

    if (state->current_frame >= state->frame_count) state->current_frame = 0;

    return (1);
}

int mgeAnimationGetFrame(TextureAnimation* _in_animation)
{
    return (_in_animation->current_state->current_frame);
}
int mgeAnimationGetFrameCount(TextureAnimation* _in_animation)
{
    return (_in_animation->current_state->frame_count);
}