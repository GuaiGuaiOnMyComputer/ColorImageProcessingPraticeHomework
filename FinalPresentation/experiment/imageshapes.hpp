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
        {2000, 2500},
        {4000, 3000},
        {5000, 3500}
};