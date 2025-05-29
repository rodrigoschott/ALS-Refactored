// Copyright YourCompanyName. All Rights Reserved.
// Location: Source/Game/Private/Blueprints/UI/Widgets/MyHealthBarWidget.cpp

#include "Blueprints/UI/Widgets/MyHealthBarWidget.h" // Corrected relative path
#include "AbilitySystemComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "AbilitySystem/Attributes/MyAttributeSet_CoreVitality.h" // Assuming this path is correct from Game module root

UMyHealthBarWidget::UMyHealthBarWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Default visibility for UUserWidget is Visible, which is fine.
    // Our logic will control it further.
}

void UMyHealthBarWidget::SetAbilitySystemComponent(UAbilitySystemComponent* InASC)
{
    // Unbind from old ASC if any
    if (AbilitySystemComponent.IsValid())
    {
        if (HealthChangedDelegateHandle.IsValid())
        {
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHealthAttribute()).Remove(HealthChangedDelegateHandle);
            HealthChangedDelegateHandle.Reset();
        }
        if (MaxHealthChangedDelegateHandle.IsValid())
        {
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetMaxHealthAttribute()).Remove(MaxHealthChangedDelegateHandle);
            MaxHealthChangedDelegateHandle.Reset();
        }
    }

    AbilitySystemComponent = InASC;

    if (AbilitySystemComponent.IsValid())
    {
        HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHealthAttribute())
            .AddUObject(this, &UMyHealthBarWidget::OnHealthChanged);
        MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetMaxHealthAttribute())
            .AddUObject(this, &UMyHealthBarWidget::OnMaxHealthChanged);

        // Fetch initial values
        CurrentHealth = AbilitySystemComponent->GetNumericAttribute(GetHealthAttribute());
        CurrentMaxHealth = AbilitySystemComponent->GetNumericAttribute(GetMaxHealthAttribute());
        
        UpdateHealthBarVisuals(); // Update visuals first
        UpdateFinalVisibility();  // Then update visibility based on new state
    }
    else
    {
        CurrentHealth = 0.0f;
        CurrentMaxHealth = 0.0f;
        UpdateHealthBarVisuals();
        UpdateFinalVisibility(); 
    }
}

void UMyHealthBarWidget::Notify_AimFocusGained()
{
    bHasAimFocus = true;
    bIsAimLingerActive = false; // If focus is gained, linger is no longer active
    GetWorld()->GetTimerManager().ClearTimer(AimLingerTimerHandle);
    UpdateFinalVisibility();
}

void UMyHealthBarWidget::Notify_AimFocusLost(float LingerDuration /*= -1.0f*/)
{
    bHasAimFocus = false;
    
    float ActualLingerDuration = (LingerDuration < 0.0f) ? ConfiguredAimLingerDuration : LingerDuration;

    if (ActualLingerDuration > 0.0f)
    {
        bIsAimLingerActive = true;
        GetWorld()->GetTimerManager().SetTimer(AimLingerTimerHandle, this, &UMyHealthBarWidget::OnAimLingerTimerExpired, ActualLingerDuration, false);
    }
    else
    {
        bIsAimLingerActive = false; // No linger, update visibility immediately
        GetWorld()->GetTimerManager().ClearTimer(AimLingerTimerHandle); // Clear just in case
    }
    UpdateFinalVisibility();
}

void UMyHealthBarWidget::Notify_BeingSelected(bool bIsNowSelected)
{
    bIsSelected = bIsNowSelected;
    UpdateFinalVisibility();
}

void UMyHealthBarWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // Initial update. If ASC was set via ExposeOnSpawn or some other means before explicit call,
    // this ensures visuals and visibility are correct.
    // If SetAbilitySystemComponent was already called, this will re-evaluate.
    if (AbilitySystemComponent.IsValid())
    {
        // Re-fetch values in case they changed between SetAbilitySystemComponent and NativeConstruct
        CurrentHealth = AbilitySystemComponent->GetNumericAttribute(GetHealthAttribute());
        CurrentMaxHealth = AbilitySystemComponent->GetNumericAttribute(GetMaxHealthAttribute());
    }
    else
    {
        CurrentHealth = 0.0f;
        CurrentMaxHealth = 0.0f;
    }
    UpdateHealthBarVisuals();
    UpdateFinalVisibility();
}

void UMyHealthBarWidget::NativeDestruct()
{
    if (AbilitySystemComponent.IsValid())
    {
        if (HealthChangedDelegateHandle.IsValid())
        {
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetHealthAttribute()).Remove(HealthChangedDelegateHandle);
        }
        if (MaxHealthChangedDelegateHandle.IsValid())
        {
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetMaxHealthAttribute()).Remove(MaxHealthChangedDelegateHandle);
        }
    }
    GetWorld()->GetTimerManager().ClearTimer(AimLingerTimerHandle); // Clear any active timer
    Super::NativeDestruct();
}

void UMyHealthBarWidget::OnHealthChanged(const FOnAttributeChangeData& Data)
{
    CurrentHealth = Data.NewValue;
    UpdateHealthBarVisuals();
    UpdateFinalVisibility(); // Visibility might change (e.g., if bHideWhenHealthFull)
}

void UMyHealthBarWidget::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
    CurrentMaxHealth = Data.NewValue;
    UpdateHealthBarVisuals();
    UpdateFinalVisibility(); // Visibility might change
}

void UMyHealthBarWidget::UpdateHealthBarVisuals()
{
    if (HealthProgressBar)
    {
        if (CurrentMaxHealth > 0.0f)
        {
            HealthProgressBar->SetPercent(FMath::Clamp(CurrentHealth / CurrentMaxHealth, 0.0f, 1.0f));
        }
        else
        {
            HealthProgressBar->SetPercent(0.0f);
        }
    }

    if (HealthText)
    {
        // Using FPlatformMath::RoundToInt for cleaner display of whole numbers
        FString HealthString = FString::Printf(TEXT("%d / %d"), FPlatformMath::RoundToInt(CurrentHealth), FPlatformMath::RoundToInt(CurrentMaxHealth));
        HealthText->SetText(FText::FromString(HealthString));
    }
}

void UMyHealthBarWidget::UpdateFinalVisibility()
{
    // Priority 1: No ASC = Hide (if configured)
    if (bHideWhenNoASC && !AbilitySystemComponent.IsValid())
    {
        SetActualUMGVisibility(ESlateVisibility::Collapsed, true);
        return;
    }

    bool bShouldBeVisible = false;

    // Priority 2: Explicit Triggers (Aim or Selection)
    if (bHasAimFocus || bIsAimLingerActive || bIsSelected)
    {
        bShouldBeVisible = true;
    }

    if (bShouldBeVisible)
    {
        SetActualUMGVisibility(ESlateVisibility::SelfHitTestInvisible, true); // Show
        return;
    }

    // Priority 3: Auto-Hide Rule (Health Full)
    // This is only reached if not aimed, not selected, and not lingering.
    if (bHideWhenHealthFull && IsHealthFull())
    {
        SetActualUMGVisibility(ESlateVisibility::Collapsed, true); // Hide
        return;
    }
    
    // Priority 4: Default Visibility if no other rules apply to hide it
    // If bHideWhenHealthFull is false, it should be visible (unless no ASC, which is handled above).
    // If bHideWhenHealthFull is true BUT health is NOT full, it should be visible.
    SetActualUMGVisibility(ESlateVisibility::SelfHitTestInvisible, true); // Show
}

void UMyHealthBarWidget::SetActualUMGVisibility(ESlateVisibility NewSlateVisibility, bool bShouldAnimate)
{
    const ESlateVisibility CurrentUMGVisibility = GetVisibility();

    if (NewSlateVisibility == ESlateVisibility::Collapsed || NewSlateVisibility == ESlateVisibility::Hidden)
    {
        if (CurrentUMGVisibility != ESlateVisibility::Collapsed && CurrentUMGVisibility != ESlateVisibility::Hidden)
        {
            if (bShouldAnimate)
            {
                OnRequestHideUI(); // Blueprint plays animation, then sets final visibility (e.g., Collapsed)
            }
            else
            {
                SetVisibility(ESlateVisibility::Collapsed); // Default to Collapsed if not animating
            }
        }
        // If already hidden/collapsed, do nothing to avoid re-triggering animations
    }
    else // Trying to make it visible (e.g., SelfHitTestInvisible, Visible)
    {
        if (CurrentUMGVisibility == ESlateVisibility::Collapsed || CurrentUMGVisibility == ESlateVisibility::Hidden)
        {
            if (bShouldAnimate)
            {
                OnRequestShowUI(); // Blueprint plays animation, then sets final visibility
            }
            else
            {
                SetVisibility(NewSlateVisibility); // Set directly if not animating
            }
        }
        else
        {
             // If already visible in some form, ensure it's the *correct* visible state
             // This handles cases where it might be Visible but should be SelfHitTestInvisible, etc.
             // However, typically OnRequestShowUI would handle setting the final desired visible state.
             // For simplicity here, if it's already some form of visible, we assume it's fine
             // or the animation will correct it. If not animating, just set it.
            if (!bShouldAnimate) {
                SetVisibility(NewSlateVisibility);
            }
        }
    }
}

bool UMyHealthBarWidget::IsHealthFull() const
{
    if (AbilitySystemComponent.IsValid()) // Ensure ASC is valid before checking attributes
    {
        // Re-fetch for accuracy in this check, as CurrentHealth/MaxHealth might not be from the latest tick
        // if no attribute change delegate fired to update them.
        const float LatestHealth = AbilitySystemComponent->GetNumericAttribute(GetHealthAttribute());
        const float LatestMaxHealth = AbilitySystemComponent->GetNumericAttribute(GetMaxHealthAttribute());
        return LatestMaxHealth > 0.0f && LatestHealth >= LatestMaxHealth;
    }
    return true; // If no ASC, effectively treat as "full" for hiding purposes if bHideWhenHealthFull is true
}

void UMyHealthBarWidget::OnAimLingerTimerExpired()
{
    bIsAimLingerActive = false;
    UpdateFinalVisibility();
}

FGameplayAttribute UMyHealthBarWidget::GetHealthAttribute() const
{
    return UMyAttributeSet_CoreVitality::GetHealthAttribute();
}

FGameplayAttribute UMyHealthBarWidget::GetMaxHealthAttribute() const
{
    return UMyAttributeSet_CoreVitality::GetMaxHealthAttribute();
}