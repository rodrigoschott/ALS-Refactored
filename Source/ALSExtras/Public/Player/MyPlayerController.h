#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h" // For FGameplayTagContainer
#include "MyPlayerController.generated.h"

UCLASS()
class ALSEXTRAS_API AMyPlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    AMyPlayerController();

    // --- Marquee Drawing State (Client-Only) ---
    UPROPERTY(BlueprintReadOnly, Category = "Marquee")
    bool bIsDrawingMarquee;

    UPROPERTY(BlueprintReadOnly, Category = "Marquee")
    FVector2D MarqueeStartScreenPosition;

    UPROPERTY(BlueprintReadOnly, Category = "Marquee")
    FVector2D MarqueeCurrentScreenPosition;

    // Called by AbilityTask to manage drawing state
    void Client_BeginMarquee(FVector2D ScreenPosition);
    void Client_UpdateMarquee(FVector2D ScreenPosition);
    void Client_EndMarquee(); // Clears drawing state

    // --- Selection Logic & Storage ---
    // This list is authoritative on the server, clients get told via OnSelected/OnDeselected on actors
    UPROPERTY(VisibleInstanceOnly, Category = "Selection", Transient) // Transient if not saving selection
    TArray<TWeakObjectPtr<AActor>> SelectedActors;

    UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable,Category="Selection")
    void Server_PerformMarqueeSelection(FVector2D ScreenStart, FVector2D ScreenEnd,
                                       const TArray<FName>& RequiredActorTags,
                                       const FGameplayTagContainer& RequiredGameplayTags,
                                       bool bAddToSelection, bool bRemoveFromSelection); // Modifiers

    // Helper to clear current selection
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void ClearCurrentSelection();

    // Debug visualization options
    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bDrawDebugFrustum = true;
    
    // New 2D screen-space projection selection method for actor selection
    bool Perform2DScreenSpaceSelection(FVector2D ScreenStart, FVector2D ScreenEnd, TArray<AActor*>& OutActorsInBox);

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    // We are not replicating SelectedActors directly from PC to all clients.
    // Selection feedback is handled by server telling individual actors they are selected/deselected.
};
