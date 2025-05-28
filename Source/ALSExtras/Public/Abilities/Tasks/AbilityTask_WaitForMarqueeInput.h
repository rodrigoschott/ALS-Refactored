#pragma once
#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "AbilityTask_WaitForMarqueeInput.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMarqueeFinishedDelegate, FVector2D, ScreenStartPos, FVector2D, ScreenEndPos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMarqueeCancelledDelegate);

UCLASS()
class ALSEXTRAS_API UAbilityTask_WaitForMarqueeInput : public UAbilityTask
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintAssignable)
    FMarqueeFinishedDelegate OnMarqueeFinished;

    UPROPERTY(BlueprintAssignable)
    FMarqueeCancelledDelegate OnCancelled;

    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UAbilityTask_WaitForMarqueeInput* CreateTask(
        UGameplayAbility* OwningAbility, 
        FName TaskInstanceName,
        const UInputAction* InConfirmAction,
        const UInputAction* InCancelAction,
        const UInputAction* InSelectionModifierAction = nullptr,
        TSubclassOf<UGameplayAbility> TriggerOnCancelAbility = nullptr);

    virtual void Activate() override;
    virtual void TickTask(float DeltaTime) override;
    virtual void OnDestroy(bool bInOwnerFinished) override;

protected:
    FVector2D InitialMouseScreenPosition;
    bool bIsMouseDown = false;

    // Input Actions this task will listen for (set these up in your project)
    UPROPERTY() // Keep UPROPERTY to prevent GC
    TObjectPtr<const UInputAction> ConfirmAction; // e.g., IA_MarqueeSelect_Hold (LMB)
    UPROPERTY()
    TObjectPtr<const UInputAction> CancelAction;  // e.g., IA_MarqueeSelect_Cancel (RMB)
    UPROPERTY()
    TObjectPtr<const UInputAction> SelectionModifierAction; // e.g., IA_Selection_Modifier (Shift)

    // Store the uint32 handles for removal
    uint32 ConfirmPressedBindingHandle = 0;
    uint32 ConfirmReleasedBindingHandle = 0;
    uint32 CancelPressedBindingHandle = 0;

    void HandleConfirmPressed(const FInputActionValue& Value);
    void HandleConfirmReleased(const FInputActionValue& Value);
    void HandleCancelPressed(const FInputActionValue& Value);
    
    TWeakObjectPtr<APlayerController> PlayerController;
    TWeakObjectPtr<class AMyPlayerController> MyPlayerController; // Casted version
};
