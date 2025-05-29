// Plugins/ALS-Refactored/Source/ALSExtras/Public/MyPlayerCharacterBase.h
#pragma once

#include "CoreMinimal.h"
#include "AlsCharacterExample.h" // The neutral base from this module
#include "MyPlayerCharacterBase.generated.h"

class UAbilitySystemComponent;
class AMyPlayerState; // Forward declaration for your game's PlayerState (assumed in main game module)

UCLASS()
class ALSEXTRAS_API AMyPlayerCharacterBase : public AAlsCharacterExample
{
	GENERATED_BODY()

public:
	AMyPlayerCharacterBase();

	/** Returns the ASC from the PlayerState. Can be nullptr. */
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	UAbilitySystemComponent* GetPlayerAbilitySystemComponent() const; // Renamed for clarity

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
};