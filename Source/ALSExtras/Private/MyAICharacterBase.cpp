// Plugins/ALS-Refactored/Source/ALSExtras/Private/MyAICharacterBase.cpp
#include "MyAICharacterBase.h" // Should be Public/MyAICharacterBase.h
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/MyAttributeSet_CoreVitality.h" // CRITICAL: Adjust this path
                                                                 // e.g., "Ringworld/Public/AbilitySystem/Attributes/MyAttributeSet_CoreVitality.h"
#include "GameplayEffect.h"
#include "Abilities/GameplayAbility.h"
#include "Net/UnrealNetwork.h" 

AMyAICharacterBase::AMyAICharacterBase()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed); 
	CoreVitalitySet = nullptr;
	bAbilitiesInitialized = false;
}

UAbilitySystemComponent* AMyAICharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMyAICharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); 
	if (GetLocalRole() == ROLE_Authority)
	{
		InitializeGAS();
	}
}

void AMyAICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (GetLocalRole() == ROLE_Authority && !bAbilitiesInitialized)
	{
        if (AbilitySystemComponent && !AbilitySystemComponent->AbilityActorInfo.IsValid())
        {
            AbilitySystemComponent->InitAbilityActorInfo(this, this);
        }
		InitializeGAS(); 
	}
}

void AMyAICharacterBase::InitializeGAS()
{
    if (bAbilitiesInitialized || !AbilitySystemComponent) return;
    if (!AbilitySystemComponent->AbilityActorInfo.IsValid())
    {
        AbilitySystemComponent->InitAbilityActorInfo(this, this);
        if (!AbilitySystemComponent->AbilityActorInfo.IsValid()) 
        {
            UE_LOG(LogTemp, Error, TEXT("AMyAICharacterBase::InitializeGAS: Failed to InitAbilityActorInfo for %s."), *GetName());
            return; 
        }
    }
    GrantAttributes();
    ApplyStartupEffects();
    GrantStartupAbilities();
    bAbilitiesInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("AMyAICharacterBase::InitializeGAS: Completed for %s"), *GetName());
}

void AMyAICharacterBase::GrantAttributes()
{
    if (!AbilitySystemComponent) return;
    if (!CoreVitalitySet) 
    {
        const UMyAttributeSet_CoreVitality* ConstExistingSet = AbilitySystemComponent->GetSet<UMyAttributeSet_CoreVitality>();
        UMyAttributeSet_CoreVitality* ExistingSet = const_cast<UMyAttributeSet_CoreVitality*>(ConstExistingSet);
        if (ExistingSet)
        {
            CoreVitalitySet = ExistingSet;
            UE_LOG(LogTemp, Log, TEXT("AMyAICharacterBase::GrantAttributes: Found existing CoreVitalitySet for %s"), *GetName());
        }
        else
        {
            UMyAttributeSet_CoreVitality* NewSet = NewObject<UMyAttributeSet_CoreVitality>(this, TEXT("CoreVitalitySet"));
            CoreVitalitySet = NewSet;
            AbilitySystemComponent->AddAttributeSetSubobject(NewSet);
            UE_LOG(LogTemp, Log, TEXT("AMyAICharacterBase::GrantAttributes: Created and granted CoreVitalitySet to %s"), *GetName());
        }
    }
}

void AMyAICharacterBase::ApplyStartupEffects()
{
    if (!AbilitySystemComponent || !CoreVitalitySet) return;
    if (IsValid(DefaultCoreVitalityAttributesEffect))
    {
        FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
        EffectContext.AddSourceObject(this);
        FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultCoreVitalityAttributesEffect, 1, EffectContext);
        if (SpecHandle.IsValid())
        {
            AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
            UE_LOG(LogTemp, Log, TEXT("AMyAICharacterBase::ApplyStartupEffects: Applied DefaultCoreVitalityAttributesEffect to %s"), *GetName());
        }
        else { UE_LOG(LogTemp, Warning, TEXT("AMyAICharacterBase::ApplyStartupEffects: Failed to make spec for GE for %s"), *GetName()); }
    }
    else { UE_LOG(LogTemp, Warning, TEXT("AMyAICharacterBase::ApplyStartupEffects: DefaultCoreVitalityAttributesEffect not set for %s."), *GetName()); }

    FGameplayEffectContextHandle PassiveEffectContext = AbilitySystemComponent->MakeEffectContext();
    PassiveEffectContext.AddSourceObject(this);
    for (const TSubclassOf<UGameplayEffect>& EffectClass : DefaultPassiveEffects)
    {
        if (IsValid(EffectClass))
        {
            FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1, PassiveEffectContext);
            if (SpecHandle.IsValid()) { AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get()); }
        }
    }
    UE_LOG(LogTemp, Log, TEXT("AMyAICharacterBase::ApplyStartupEffects: Applied %d passive effects to %s"), DefaultPassiveEffects.Num(), *GetName());
}

void AMyAICharacterBase::GrantStartupAbilities()
{
    if (!AbilitySystemComponent) return;
    for (const TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
    {
        if (IsValid(AbilityClass))
        {
            FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
            AbilitySystemComponent->GiveAbility(AbilitySpec);
        }
    }
    UE_LOG(LogTemp, Log, TEXT("AMyAICharacterBase::GrantStartupAbilities: Granted %d abilities to %s"), DefaultAbilities.Num(), *GetName());
}

void AMyAICharacterBase::UnPossessed()
{
    Super::UnPossessed();
    // bAbilitiesInitialized = false; // Consider if AI can be re-possessed and needs re-init.
                                     // If AI is destroyed on unpossess, this isn't strictly necessary.
}