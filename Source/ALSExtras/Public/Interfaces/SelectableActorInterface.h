#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectableActorInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USelectableActorInterface : public UInterface
{
    GENERATED_BODY()
};

class ALSEXTRAS_API ISelectableActorInterface
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selection")
    void OnSelectedByMarquee(APlayerController* SelectingPlayer);

    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selection")
    void OnDeselectedByMarquee(APlayerController* SelectingPlayer);

    // Allows the actor to determine if it can be selected by this specific player/marquee action
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selection")
    bool IsCurrentlySelectableByMarquee(APlayerController* SelectingPlayer) const;
};
