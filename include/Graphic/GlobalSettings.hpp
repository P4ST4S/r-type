/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** GlobalSettings
*/

#pragma once

namespace GlobalSettings
{
    enum Resolution {
        RESOLUTION_848_480,
        RESOLUTION_1280_720,
        RESOLUTION_1920_1080,
        RESOLUTION_2560_1440,
    };

    enum Fps {
        FPS_30,
        FPS_60,
        FPS_90,
        FPS_120,
        FPS_144,
        FPS_165,
        FPS_240,
    };

    extern bool _fullscreen;
    extern int _selectedResolution;
    extern double _screenWidth;
    extern double _screenHeight;
    extern int _selectedFps;
    extern double _volume;
}

