#include "json_parser.h"
#include <string.h>
#include <stdlib.h>

void JSON_ParseCommand(char *json_str)
{
    extern uint8_t scan_mode;
    extern uint16_t current_angle;

    char *p;
    int angle;

    // 解析模式命令: {"mode":"auto"} 或 {"mode":"manual"}
    if(strstr(json_str, "\"mode\":\"auto\""))
    {
        scan_mode = 1;
    }
    else if(strstr(json_str, "\"mode\":\"manual\""))
    {
        scan_mode = 0;
    }

    // 解析角度命令: {"angle":90}
    p = strstr(json_str, "\"angle\":");
    if(p)
    {
        p += 8;  // 跳过"angle":
        angle = atoi(p);
        if(angle >= 0 && angle <= 180)
        {
            current_angle = angle;
        }
    }
}