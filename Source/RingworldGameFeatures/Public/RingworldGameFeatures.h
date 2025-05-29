#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// This needs to match the module name in the .Build.cs file
#define GAME_MODULE_NAME "RingworldGameFeatures"

// Standard module API definition pattern for Unreal Engine
#if PLATFORM_WINDOWS
    #ifdef GAME_EXPORT
        #define RINGWORLDGAMEFEATURES_API__declspec(dllexport)
    #else
        #define RINGWORLDGAMEFEATURES_API __declspec(dllimport)
    #endif
#else
    #define RINGWORLDGAMEFEATURES_API
#endif

/**
 * Game Module
 */
class RINGWORLDGAMEFEATURES_API FGameModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    static inline FGameModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FGameModule>(GAME_MODULE_NAME);
    }

    static inline bool IsAvailable()
    {
        return FModuleManager::Get().IsModuleLoaded(GAME_MODULE_NAME);
    }
};
