// Copyright YourCompanyName. All Rights Reserved.
// Location: Source/Game/Public/Blueprints/UI/Widgets/MyHealthBarWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h" // For FOnAttributeChangeData
#include "MyHealthBarWidget.generated.h"

// Forward Declarations
class UAbilitySystemComponent;
class UProgressBar;
class UTextBlock;
class UMyAttributeSet_CoreVitality; // Will be included in CPP
struct FGameplayAttribute;

UCLASS(Abstract) // Abstract because it's intended to be a base for Blueprint Widgets
class RINGWORLDGAMEFEATURES_API UMyHealthBarWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMyHealthBarWidget(const FObjectInitializer& ObjectInitializer);

    /**
     * Initializes the widget with the specified Ability System Component.
     * Binds to Health and MaxHealth attribute change delegates.
     * @param InASC The Ability System Component to monitor.
     */
    UFUNCTION(BlueprintCallable, Category = "Health Bar")
    void SetAbilitySystemComponent(UAbilitySystemComponent* InASC);

    /**
     * Call this when an external system (e.g., aiming trace) starts focusing on this widget's owner.
     * The health bar will become visible and stay visible while focus is maintained.
     */
    UFUNCTION(BlueprintCallable, Category = "Health Bar|Visibility")
    void Notify_AimFocusGained();

    /**
     * Call this when an external system (e.g., aiming trace) stops focusing on this widget's owner.
     * @param LingerDuration How long the health bar should remain visible after focus is lost. If < 0, uses ConfiguredAimLingerDuration.
     */
    UFUNCTION(BlueprintCallable, Category = "Health Bar|Visibility")
    void Notify_AimFocusLost(float LingerDuration = -1.0f);

    /**
     * Call this when an external system (e.g., selection manager) selects/deselects this widget's owner.
     * @param bIsNowSelected True if the owner is now selected, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Health Bar|Visibility")
    void Notify_BeingSelected(bool bIsNowSelected);

protected:
    //~ Begin UUserWidget Interface
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    //~ End UUserWidget Interface

    // --- Bound Widgets (Must match names in derived UMG Blueprint) ---
    UPROPERTY(BlueprintReadOnly, Category = "Health Bar|Widgets", meta = (BindWidget))
    TObjectPtr<UProgressBar> HealthProgressBar;

    UPROPERTY(BlueprintReadOnly, Category = "Health Bar|Widgets", meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> HealthText;

    // --- Editor-Configurable Visibility Rules ---
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Bar|Visibility")
    bool bHideWhenNoASC = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Bar|Visibility")
    bool bHideWhenHealthFull = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Bar|Visibility", meta = (ClampMin = "0.0", UIMin = "0.0"))
    float ConfiguredAimLingerDuration = 2.0f; // Default linger time in seconds

    // --- Attribute Change Handlers ---
    virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
    virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);

    // --- Internal Logic ---
    /** Updates the visual representation of the health bar (progress and text). */
    virtual void UpdateHealthBarVisuals();

    /** Determines and applies the final visibility state of the widget based on all rules. */
    virtual void UpdateFinalVisibility();

    /**
     * Sets the actual UMG visibility and triggers Blueprint animations.
     * @param NewSlateVisibility The target visibility state.
     * @param bShouldAnimate If true, Blueprint animation events will be called.
     */
    virtual void SetActualUMGVisibility(ESlateVisibility NewSlateVisibility, bool bShouldAnimate);

    UFUNCTION(BlueprintImplementableEvent, Category = "Health Bar|Animation")
    void OnRequestShowUI();

    UFUNCTION(BlueprintImplementableEvent, Category = "Health Bar|Animation")
    void OnRequestHideUI();

    /** Checks if health is currently full (Health >= MaxHealth). */
    bool IsHealthFull() const;

    /** Callback for when the aim linger timer expires. */
    void OnAimLingerTimerExpired();

    // --- Attribute Accessors ---
    FGameplayAttribute GetHealthAttribute() const;
    FGameplayAttribute GetMaxHealthAttribute() const;

    // --- Internal State ---
    UPROPERTY(Transient) // Does not need to be saved, state is runtime
    TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    FDelegateHandle HealthChangedDelegateHandle;
    FDelegateHandle MaxHealthChangedDelegateHandle;

    float CurrentHealth = 0.0f;
    float CurrentMaxHealth = 0.0f;

    // Visibility state flags
    bool bHasAimFocus = false;
    bool bIsAimLingerActive = false;
    bool bIsSelected = false;

    FTimerHandle AimLingerTimerHandle;
};