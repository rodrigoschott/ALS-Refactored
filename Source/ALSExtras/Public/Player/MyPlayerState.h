// MyPlayerState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h" 
#include "GameplayEffectTypes.h"    
#include "MyPlayerState.generated.h"

// Forward declarations
class UAbilitySystemComponent;
class UGameplayAbility;       
class UGameplayEffect;      
class UMyAttributeSet_CoreVitality; // Forward declare your Core Vitality AttributeSet

UCLASS()
class ALSEXTRAS_API AMyPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AMyPlayerState();

    //~ Begin IAbilitySystemInterface
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    //~ End IAbilitySystemInterface

    // Getter for the Core Vitality attribute set
    UFUNCTION(BlueprintCallable, Category = "Abilities|Attributes")
    UMyAttributeSet_CoreVitality* GetCoreVitalityAttributeSet() const { return CoreVitalitySet; }

    virtual void BeginPlay() override;

    /** Server-side function to grant startup abilities and apply startup effects. */
    virtual void InitializeDefaultAttributesAbilitiesAndEffects(); // Renamed for clarity

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    // Core Vitality AttributeSet instance
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Attributes", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UMyAttributeSet_CoreVitality> CoreVitalitySet;

    /** List of abilities to grant on startup. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Initialization")
    TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

    /** GameplayEffect to initialize core vitality attributes (Health, MaxHealth, etc.). */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Initialization")
    TSubclassOf<UGameplayEffect> DefaultCoreVitalityEffect;

    /** List of other passive GameplayEffects to apply on startup. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Initialization")
    TArray<TSubclassOf<UGameplayEffect>> DefaultPassiveStartupEffects;

    bool bHasInitializedDefaultAttributesAndAbilities; // Renamed flag
};