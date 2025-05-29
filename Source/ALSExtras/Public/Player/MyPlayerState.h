// MyPlayerState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h" // Required for IAbilitySystemInterface
#include "GameplayEffectTypes.h"    // Required for FGameplayEffectContextHandle
#include "MyPlayerState.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;       // Forward declare UGameplayAbility
class UGameplayEffect;      // Forward declare UGameplayEffect
// class UMyAttributeSet; // If you have a custom attribute set, forward declare and include its .h in .cpp

UCLASS()
class ALSEXTRAS_API AMyPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AMyPlayerState();

    //~ Begin IAbilitySystemInterface
    /** Returns our Ability System Component. */
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    //~ End IAbilitySystemInterface

    // If you have a custom AttributeSet, add a getter for it
    // UMyAttributeSet* GetAttributeSet() const { return AttributeSet; }

    /** Called on the server when the PlayerState is initialized for a new player. */
    virtual void BeginPlay() override;

    /**
     * Server-side function to grant startup abilities and apply startup effects.
     * Typically called from BeginPlay or when a pawn is possessed if abilities are pawn-dependent.
     */
    virtual void InitializeDefaultAbilitiesAndEffects();


protected:
    // The AbilitySystemComponent for this PlayerState.
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    // If you have a custom AttributeSet, declare it here
    // UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
    // TObjectPtr<UMyAttributeSet> AttributeSet;

    /** List of abilities to grant on startup. Configure in derived Blueprints or C++. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

    /** List of GameplayEffects to apply on startup (e.g., default attributes). Configure in derived Blueprints or C++. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
    TArray<TSubclassOf<UGameplayEffect>> DefaultStartupEffects;

    // Flag to ensure abilities are only initialized once
    bool bAbilitiesInitialized = false;
};