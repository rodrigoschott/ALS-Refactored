#include "RingworldGameFeatures.h"

#define LOCTEXT_NAMESPACE "FGameModule"

void FGameModule::StartupModule()
{
    // This code will execute after your module is loaded into memory
}

void FGameModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FGameModule, RingworldGameFeatures)
