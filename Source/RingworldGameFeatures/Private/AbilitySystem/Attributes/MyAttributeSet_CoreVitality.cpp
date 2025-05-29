// Example Project Structure: MyProject/Source/MyProject/AbilitySystem/Attributes/
// File: MyAttributeSet_CoreVitality.cpp
#include "AbilitySystem/Attributes/MyAttributeSet_CoreVitality.h" // Adjust path as needed
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h" // Required for FGameplayEffectModCallbackData
#include "AbilitySystemComponent.h"

UMyAttributeSet_CoreVitality::UMyAttributeSet_CoreVitality()
    : Health(100.0f) // Default starting health
    , MaxHealth(100.0f) // Default starting max health
    , HealingRate(1.0f) // Default healing rate multiplier
{
}

void UMyAttributeSet_CoreVitality::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet_CoreVitality, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet_CoreVitality, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet_CoreVitality, HealingRate, COND_None, REPNOTIFY_Always);
}

void UMyAttributeSet_CoreVitality::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    Super::PreAttributeBaseChange(Attribute, NewValue);

    if (Attribute == GetHealthAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
    }
    // No explicit clamping for MaxHealth here, as it might be set by GEs.
    // HealingRate could also be clamped if needed (e.g., to non-negative).
}

void UMyAttributeSet_CoreVitality::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    Super::PreAttributeChange(Attribute, NewValue);

    // This is called before an attribute is changed by a GE.
    // If we are changing MaxHealth, Health should be clamped to the new MaxHealth.
    // This is useful if MaxHealth is reduced below current Health.
    if (Attribute == GetMaxHealthAttribute()) 
    {
        UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
        if (ASC)
        {
            const float CurrentMaxHealth = GetMaxHealth();
            const float CurrentHealth = GetHealth();
            
            // If MaxHealth is about to change, and Health is greater than the new MaxHealth, adjust Health.
            // This specific logic might be better handled in PostGameplayEffectExecute or by the GE itself,
            // but it's an option here. Often, Health is clamped against MaxHealth in PreAttributeBaseChange.
            // For now, let's ensure Health doesn't exceed the new MaxHealth when MaxHealth changes.
            if (NewValue < CurrentHealth && NewValue > 0.f) // If new MaxHealth is less than current Health
            {
                // It's tricky to directly set Health here because this function is about the *Attribute* changing.
                // The GE that changes MaxHealth might also need to adjust Health.
                // Or, PostGameplayEffectExecute is a better place.
                // For now, primarily rely on clamping in PreAttributeBaseChange for Health.
            }
        }
    }
}

void UMyAttributeSet_CoreVitality::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* SourceASC = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
    AActor* TargetActor = nullptr;
    AController* TargetController = nullptr;
    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
    }

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        // Clamp Health to MaxHealth. MaxHealth could have changed in the same effect.
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
        
        if (GetHealth() <= 0.0f && !Data.Target.HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")))) // Example Dead Tag
        {
            // Character has died.
            // Here you can:
            // 1. Broadcast an event (e.g., an OnDeath delegate on the ASC or Character).
            // 2. Apply a "Dead" Gameplay Tag or Gameplay Effect.
            // 3. Trigger a "Death" Gameplay Ability.
            // For now, let's assume applying a tag is a good first step.
            // Data.Target.AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
            // Data.Target.AddReplicatedLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
            
            // Consider if death logic should be centralized in the ASC or Character itself, listening to this event.
            // For example:
            // if (IMyCharacterInterface* CharacterInterface = Cast<IMyCharacterInterface>(TargetActor))
            // {
            //    CharacterInterface->HandleDeath();
            // }
            UE_LOG(LogTemp, Warning, TEXT("%s has died."), *GetNameSafe(TargetActor));
        }
    }
    else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
    {
        // MaxHealth has changed. We need to adjust current Health proportionally
        // or at least ensure it's clamped. PreAttributeBaseChange handles clamping.
        // If you want proportional adjustment (e.g. if MaxHealth doubles, Health doubles, up to new Max):
        // AdjustAttributeForMaxChange(MaxHealth, GetHealthAttribute(), NewMaxHealthValue, Health);
        // For now, simple clamping is usually sufficient and handled by PreAttributeBaseChange of Health.
        // Ensure Health is still valid after MaxHealth change.
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
    }
    else if (Data.EvaluatedData.Attribute == GetHealingRateAttribute())
    {
        SetHealingRate(FMath::Max(GetHealingRate(), 0.0f)); // Ensure HealingRate is not negative
    }
}

void UMyAttributeSet_CoreVitality::AdjustAttributeForMaxChange(const FGameplayAttributeData& MaxAttribute, const FGameplayAttribute& AffectedAttributeProperty, float NewMaxValue, const FGameplayAttributeData& AffectedAttribute)
{
    UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
    if (ASC && NewMaxValue > 0.f) // MaxAttribute.GetCurrentValue() is the old Max value
    {
        const float CurrentValue = AffectedAttribute.GetCurrentValue();
        const float OldMaxValue = MaxAttribute.GetCurrentValue(); // This should be the value *before* the change that triggered PostGameplayEffectExecute for MaxAttribute
                                                                // This is tricky because MaxAttribute.GetCurrentValue() is already the *new* max value here.
                                                                // We need the old max value, which is not directly available in this function's scope
                                                                // without capturing it beforehand or having it passed in.

        // A common way is to just clamp:
        ASC->SetNumericAttributeBase(AffectedAttributeProperty, FMath::Clamp(CurrentValue, 0.f, NewMaxValue));

        // If you really need proportional change, the GE that changes MaxHealth should also calculate and apply the proportional change to Health.
        // Example for proportional:
        // float Ratio = CurrentValue / OldMaxValue; (OldMaxValue needs to be correct)
        // ASC->SetNumericAttributeBase(AffectedAttributeProperty, FMath::Clamp(Ratio * NewMaxValue, 0.f, NewMaxValue));
    }
}


void UMyAttributeSet_CoreVitality::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet_CoreVitality, Health, OldHealth);
}

void UMyAttributeSet_CoreVitality::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet_CoreVitality, MaxHealth, OldMaxHealth);
}

void UMyAttributeSet_CoreVitality::OnRep_HealingRate(const FGameplayAttributeData& OldHealingRate)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet_CoreVitality, HealingRate, OldHealingRate);
}