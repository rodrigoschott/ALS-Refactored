// MyPlayerState.cpp
#include "../../Public/Player/MyPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"    // For UGameplayAbility
#include "GameplayEffect.h"     // For UGameplayEffect
#include "Net/UnrealNetwork.h"  // For DOREPLIFETIME if you add replicated props later
// #include "MyAttributeSet.h" // If you have a custom attribute set, include its .h
// Removed AlsCharacterExample.h include to prevent circular dependency
// Forward declare instead if needed
class AAlsCharacterExample;

AMyPlayerState::AMyPlayerState()
{
    // Create the Ability System Component.
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); // Or Minimal, depending on your needs

    // Create AttributeSet if you have one
    // AttributeSet = CreateDefaultSubobject<UMyAttributeSet>(TEXT("AttributeSet"));

    // PlayerStates are replicated by default.
    // NetUpdateFrequency is important for how often PlayerState replicates.
    // For GAS, attributes are usually replicated via their own OnRep functions or through GameplayEffects.
    // Default is fine for most cases.
}

UAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AMyPlayerState::BeginPlay()
{
    Super::BeginPlay();

    // Server grants abilities and applies startup effects.
    // This can also be called from the Character's PossessedBy if you want to ensure the Pawn (AvatarActor) is valid
    // at the time of initialization, but generally, PlayerState's BeginPlay is a good spot for player-wide abilities.
    if (GetNetMode() != NM_Client && AbilitySystemComponent && !bAbilitiesInitialized)
    {
        // The PlayerState itself is the OwnerActor.
        // The AvatarActor will be set by the Character in PossessedBy/OnRep_PlayerState.
        // For now, we can initialize with the PlayerState as both if no pawn is yet available,
        // but the Character will update the AvatarActor.
        // Or, we can defer InitializeDefaultAbilitiesAndEffects until the character calls InitAbilityActorInfo.
        // For simplicity here, let's assume we grant player-wide abilities that don't immediately need an avatar.
        // The Character's InitAbilityActorInfo call will be crucial.

        // We don't call InitAbilityActorInfo here because the Avatar (Pawn) might not be set yet.
        // The Character will call it.
        // We can, however, grant abilities that are inherent to the player.
        InitializeDefaultAbilitiesAndEffects();
    }
}

void AMyPlayerState::InitializeDefaultAbilitiesAndEffects()
{
    if (!AbilitySystemComponent || bAbilitiesInitialized || GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    // Grant Default Abilities
    for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
    {
        if (AbilityClass)
        {
            // SourceObject is 'this' (the PlayerState)
            FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, this);
            AbilitySystemComponent->GiveAbility(AbilitySpec);
        }
    }

    // Apply Default Startup Effects
    for (TSubclassOf<UGameplayEffect>& EffectClass : DefaultStartupEffects)
    {
        if (EffectClass)
        {
            FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
            ContextHandle.AddSourceObject(this); // The PlayerState is the source of these startup effects

            FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1, ContextHandle);
            if (SpecHandle.IsValid())
            {
                AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            }
        }
    }
    bAbilitiesInitialized = true;
}