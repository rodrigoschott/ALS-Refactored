// MyPlayerState.cpp
#include "Player/MyPlayerState.h" // Adjust to your Public include path e.g., "Public/Player/MyPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"    
#include "GameplayEffect.h"     
#include "AbilitySystem/Attributes/MyAttributeSet_CoreVitality.h" // Include your AttributeSet
// #include "Net/UnrealNetwork.h" // Only if you add custom replicated properties to PlayerState itself

// Forward declare AAlsCharacterExample if needed by other functions, but not directly for this setup
// class AAlsCharacterExample; 

AMyPlayerState::AMyPlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // CoreVitalitySet will be created and assigned later
    CoreVitalitySet = nullptr;
    bHasInitializedDefaultAttributesAndAbilities = false;
    
    // PlayerStates are replicated by default. NetUpdateFrequency is usually fine.
}

UAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AMyPlayerState::BeginPlay()
{
    Super::BeginPlay();

    // Server is responsible for initializing attributes, abilities, and effects.
    // The Character's PossessedBy/OnRep_PlayerState will call InitAbilityActorInfo on our ASC.
    // We can initialize player-wide things here.
    if (GetLocalRole() == ROLE_Authority && AbilitySystemComponent && !bHasInitializedDefaultAttributesAndAbilities)
    {
        // Note: InitAbilityActorInfo (setting the AvatarActor) will be called by the AMyPlayerCharacterBase
        // when it is possessed or its PlayerState replicates.
        // For now, the OwnerActor is this PlayerState.
        if (!AbilitySystemComponent->AbilityActorInfo.IsValid() || AbilitySystemComponent->AbilityActorInfo->OwnerActor != this)
        {
             AbilitySystemComponent->InitAbilityActorInfo(this, nullptr); // Owner is self, Avatar initially null
        }

        InitializeDefaultAttributesAbilitiesAndEffects();
    }
}

void AMyPlayerState::InitializeDefaultAttributesAbilitiesAndEffects()
{
    if (!AbilitySystemComponent || bHasInitializedDefaultAttributesAndAbilities || GetLocalRole() != ROLE_Authority)
    {
        return;
    }

    // --- Grant AttributeSets ---
    if (!CoreVitalitySet)
    {
        const UMyAttributeSet_CoreVitality* ConstExistingSet = AbilitySystemComponent->GetSet<UMyAttributeSet_CoreVitality>();
        UMyAttributeSet_CoreVitality* ExistingSet = const_cast<UMyAttributeSet_CoreVitality*>(ConstExistingSet);

        if (ExistingSet)
        {
            CoreVitalitySet = ExistingSet;
            UE_LOG(LogTemp, Log, TEXT("AMyPlayerState::InitializeDefault: Found existing CoreVitalitySet for %s"), *GetName());
        }
        else
        {
            UMyAttributeSet_CoreVitality* NewSet = NewObject<UMyAttributeSet_CoreVitality>(this, TEXT("CoreVitalitySet_Player"));
            CoreVitalitySet = NewSet;
            AbilitySystemComponent->AddAttributeSetSubobject(NewSet);
            UE_LOG(LogTemp, Log, TEXT("AMyPlayerState::InitializeDefault: Created and granted CoreVitalitySet to %s"), *GetName());
        }
    }
    // Grant other attribute sets here if the PlayerState should own them

    // --- Apply Core Vitality Initialization Effect ---
    if (IsValid(DefaultCoreVitalityEffect))
    {
        FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
        ContextHandle.AddSourceObject(this); 
        FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultCoreVitalityEffect, 1, ContextHandle);
        if (SpecHandle.IsValid())
        {
            AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            UE_LOG(LogTemp, Log, TEXT("AMyPlayerState::InitializeDefault: Applied DefaultCoreVitalityEffect for %s"), *GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AMyPlayerState::InitializeDefault: DefaultCoreVitalityEffect is not set for %s! Player core attributes might not be initialized."), *GetName());
    }

    // --- Grant Default Abilities ---
    for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
    {
        if (AbilityClass)
        {
            FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, this); // SourceObject is 'this' PlayerState
            AbilitySystemComponent->GiveAbility(AbilitySpec);
        }
    }
    UE_LOG(LogTemp, Log, TEXT("AMyPlayerState::InitializeDefault: Granted %d default abilities for %s"), DefaultAbilities.Num(), *GetName());


    // --- Apply Other Default Passive Startup Effects ---
    for (const TSubclassOf<UGameplayEffect>& EffectClass : DefaultPassiveStartupEffects)
    {
        if (EffectClass)
        {
            FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
            ContextHandle.AddSourceObject(this);
            FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1, ContextHandle);
            if (SpecHandle.IsValid())
            {
                AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            }
        }
    }
    UE_LOG(LogTemp, Log, TEXT("AMyPlayerState::InitializeDefault: Applied %d passive startup effects for %s"), DefaultPassiveStartupEffects.Num(), *GetName());

    bHasInitializedDefaultAttributesAndAbilities = true;
}