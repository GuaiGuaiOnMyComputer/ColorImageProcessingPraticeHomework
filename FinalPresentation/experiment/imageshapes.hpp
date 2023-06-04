#pragma once

#ifndef _DEBUG
#define TEST_COUNT 5000
#else
#define TEST_COUNT 20
#endif

#define SHAPE_OPT_COUNT 7

struct ImageShape
{
    int width;
    int height;
};

const ImageShape g_IMAGESHAPES[] = {
        {800, 600},
        {1024, 768},
        {1280, 1024},
        {1920, 1080},
        {10000, 10000},
        {15000, 15000}, 
        {20000, 20000}
};