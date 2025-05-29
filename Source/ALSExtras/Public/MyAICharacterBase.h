// Plugins/ALS-Refactored/Source/ALSExtras/Public/MyAICharacterBase.h
#pragma once

#include "CoreMinimal.h"
#include "AlsCharacterExample.h"      
#include "AbilitySystemInterface.h"     
#include "AbilitySystem/Attributes/MyAttributeSet_CoreVitality.h" // Full include for TObjectPtr
#include "MyAICharacterBase.generated.h"

// Forward declarations
class UAbilitySystemComponent;
// class UMyAttributeSet_CoreVitality; // No longer needed due to full include above
class UGameplayEffect;
class UGameplayAbility;

UCLASS()
class ALSEXTRAS_API AMyAICharacterBase : public AAlsCharacterExample, public IAbilitySystemInterface 
{
	GENERATED_BODY()

public:
	AMyAICharacterBase();

	//~ Begin IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface

	UFUNCTION(BlueprintCallable, Category = "Abilities|Attributes")
	UMyAttributeSet_CoreVitality* GetCoreVitalityAttributeSet() const { return CoreVitalitySet; }

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override; // Added declaration

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities|Attributes", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMyAttributeSet_CoreVitality> CoreVitalitySet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Initialization", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> DefaultCoreVitalityAttributesEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Initialization")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities|Initialization")
	TArray<TSubclassOf<UGameplayEffect>> DefaultPassiveEffects;

	bool bAbilitiesInitialized;

	virtual void InitializeGAS();       
	virtual void GrantAttributes();
	virtual void ApplyStartupEffects();
	virtual void GrantStartupAbilities();
};