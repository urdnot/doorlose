#pragma once

#ifdef ADDON_DATABASE_BUILD
    #define ADDON_DATABASE_EXPORT __declspec(dllexport)
#else
    #define ADDON_DATABASE_EXPORT __declspec(dllimport)
#endif