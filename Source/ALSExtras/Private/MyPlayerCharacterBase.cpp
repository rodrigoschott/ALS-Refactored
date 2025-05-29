// Plugins/ALS-Refactored/Source/ALSExtras/Private/MyPlayerCharacterBase.cpp
#include "MyPlayerCharacterBase.h" // Should be Public/MyPlayerCharacterBase.h
#include "Player/MyPlayerState.h"   // CRITICAL: Adjust this path to your AMyPlayerState.h location
                                    // e.g., "Ringworld/Public/Player/MyPlayerState.h" if MyPlayerState is in Ringworld module
#include "AbilitySystemComponent.h"

AMyPlayerCharacterBase::AMyPlayerCharacterBase()
{
}

UAbilitySystemComponent* AMyPlayerCharacterBase::GetPlayerAbilitySystemComponent() const
{
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		// Assuming AMyPlayerState implements IAbilitySystemInterface or has a public getter
		return PS->GetAbilitySystemComponent(); 
	}
	return nullptr;
}

void AMyPlayerCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); 

	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent(); 
		if (ASC)
		{
			ASC->InitAbilityActorInfo(PS, this); 
			UE_LOG(LogTemp, Log, TEXT("AMyPlayerCharacterBase::PossessedBy: Initialized PlayerState ASC for %s with Avatar %s"), *GetNameSafe(PS), *GetNameSafe(this));
		}
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("AMyPlayerCharacterBase::PossessedBy: PlayerState %s has no AbilitySystemComponent!"), *GetNameSafe(PS));
        }
	}
    else
    {
        // This can happen briefly during seamless travel or if a non-player controller possesses this pawn type.
        if (NewController && NewController->IsPlayerController())
        {
            UE_LOG(LogTemp, Warning, TEXT("AMyPlayerCharacterBase::PossessedBy: No PlayerState found for PlayerController %s when possessing %s."), *GetNameSafe(NewController), *GetNameSafe(this));
        }
    }
}

void AMyPlayerCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState(); 

	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	if (PS)
	{
		UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent(); 
		if (ASC)
		{
			ASC->InitAbilityActorInfo(PS, this);
			UE_LOG(LogTemp, Log, TEXT("AMyPlayerCharacterBase::OnRep_PlayerState: Initialized PlayerState ASC for %s with Avatar %s (Client)"), *GetNameSafe(PS), *GetNameSafe(this));
		}
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("AMyPlayerCharacterBase::OnRep_PlayerState: PlayerState %s has no AbilitySystemComponent! (Client)"), *GetNameSafe(PS));
        }
	}
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AMyPlayerCharacterBase::OnRep_PlayerState: No PlayerState found for %s. (Client)"), *GetNameSafe(this));
    }
}