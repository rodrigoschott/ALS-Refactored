// Example Project Structure: MyProject/Source/MyProject/AbilitySystem/Attributes/
// File: MyAttributeSet_CoreVitality.h
#pragma once

#include "CoreMinimal.h"
#include "RingworldGameFeatures.h" // For GAME_API definition
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"

// These macros are required for the attribute system
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#include "MyAttributeSet_CoreVitality.generated.h" // Ensure this matches your filename

/**
 * UAS_CoreVitality
 *
 * Defines core vitality attributes like Health and MaxHealth.
 */
UCLASS()
class RINGWORLDGAMEFEATURES_API UMyAttributeSet_CoreVitality : public UAttributeSet 
{
    GENERATED_BODY()

public:
    UMyAttributeSet_CoreVitality();

    // AttributeSet Overrides
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    // --- Health Attribute ---
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|CoreVitality", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UMyAttributeSet_CoreVitality, Health)

    // --- MaxHealth Attribute ---
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|CoreVitality", ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UMyAttributeSet_CoreVitality, MaxHealth)

    // --- HealingRate Attribute ---
    // Multiplier for healing effects. Can be modified by buffs/debuffs.
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|CoreVitality", ReplicatedUsing = OnRep_HealingRate)
    FGameplayAttributeData HealingRate;
    ATTRIBUTE_ACCESSORS(UMyAttributeSet_CoreVitality, HealingRate)

protected:
    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

    UFUNCTION()
    virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

    UFUNCTION()
    virtual void OnRep_HealingRate(const FGameplayAttributeData& OldHealingRate);

    /** Helper function to proportionally adjust the value of an attribute when its corresponding max attribute changes. Used in PostGameplayEffectExecute. */
    void AdjustAttributeForMaxChange(const FGameplayAttributeData& MaxAttribute, const FGameplayAttribute& AffectedAttributeProperty, float NewMaxValue, const FGameplayAttributeData& AffectedAttribute);
};