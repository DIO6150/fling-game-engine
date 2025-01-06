#include "level_parser.h"

#include "cjson/cJSON.h"

TILE_ID** parse_level_file(char* path, vec2 level_size, vec2 start_pos, vec2 end_pos, char* next_level)
{
    FILE* level_file = fopen(path, "r");

    char to_parse[2048];
    int index = 0;

    while (!feof(level_file))
    {
        to_parse[index] = fgetc(level_file);
        index++;
    }
    to_parse[index] = '\0';

    cJSON* json = cJSON_Parse(to_parse);


    level_size[0] = cJSON_GetObjectItemCaseSensitive(json, "width")->valueint + 2;
    level_size[1] = cJSON_GetObjectItemCaseSensitive(json, "height")->valueint + 2;

    int** level_grid = malloc(sizeof(int*) * level_size[0]);

    for (int i = 0; i < level_size[0]; i++)
    {
        level_grid[i] = malloc (sizeof(int) * level_size[1]);
    }

    cJSON* array = cJSON_GetObjectItemCaseSensitive(json, "grid");

    for (int i = 0; i < level_size[1]; i ++)
    {
        for (int j = 0; j < level_size[0]; j ++)
        {
            int tile = WALL;

            if (!(i == 0 || j == 0 || i == (int)level_size[0] - 1 || j == (int)level_size[1] - 1))
                tile = cJSON_GetArrayItem(cJSON_GetArrayItem(array, j - 1), i - 1)->valueint;
            
            level_grid[i][j] = tile;
        }
    }

    cJSON* json_start_pos = cJSON_GetObjectItemCaseSensitive(json, "start");

    start_pos[0] = cJSON_GetArrayItem(json_start_pos, 0)->valueint + 1;
    start_pos[1] = cJSON_GetArrayItem(json_start_pos, 1)->valueint + 1;


    cJSON* json_end_pos = cJSON_GetObjectItemCaseSensitive(json, "end");

    end_pos[0] = cJSON_GetArrayItem(json_end_pos, 0)->valueint + 1;
    end_pos[1] = cJSON_GetArrayItem(json_end_pos, 1)->valueint + 1;

    cJSON* json_next_level = cJSON_GetObjectItemCaseSensitive(json, "next");

    strcpy(next_level, json_next_level->valuestring);

    cJSON_Delete(json);

    fclose(level_file);

    return ((TILE_ID**)level_grid);
}