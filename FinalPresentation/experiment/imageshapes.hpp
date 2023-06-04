#pragma once

#ifndef _DEBUG
#define TEST_COUNT 5000
#else
#define TEST_COUNT 20
#endif

struct ImageShape
{
    int width;
    int height;
};

const ImageShape g_IMAGESHAPES[] = {
        {800, 600},
        {1024, 768},
        {1280, 1024},
        {1920, 1080}
};