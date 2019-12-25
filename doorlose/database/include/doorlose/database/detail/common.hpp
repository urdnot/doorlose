#pragma once

#ifdef DOORLOSE_DATABASE_BUILD
    #define DOORLOSE_DATABASE_EXPORT __declspec(dllexport)
#else
    #define DOORLOSE_DATABASE_EXPORT __declspec(dllimport)
#endif