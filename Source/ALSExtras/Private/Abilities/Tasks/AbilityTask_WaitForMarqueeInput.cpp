#include "Abilities/Tasks/AbilityTask_WaitForMarqueeInput.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Player/MyPlayerController.h"
#include "AbilitySystemComponent.h"
#include "InputActionValue.h"

UAbilityTask_WaitForMarqueeInput* UAbilityTask_WaitForMarqueeInput::CreateTask(
    UGameplayAbility* OwningAbility, 
    FName TaskInstanceName,
    const UInputAction* InConfirmAction,
    const UInputAction* InCancelAction,
    const UInputAction* InSelectionModifierAction,
    TSubclassOf<UGameplayAbility> TriggerOnCancelAbility)
{
    UE_LOG(LogTemp, Log, TEXT("CreateTask: Creating MarqueeInput task with name %s"), *TaskInstanceName.ToString());
    
    UAbilityTask_WaitForMarqueeInput* MyTask = NewAbilityTask<UAbilityTask_WaitForMarqueeInput>(OwningAbility, TaskInstanceName);
    // MyTask->TriggerOnCancelAbility = TriggerOnCancelAbility; // If you want to trigger another ability on cancel
    
    // Assign InputActions from parameters instead of LoadObject
    UE_LOG(LogTemp, Log, TEXT("CreateTask: Assigning input actions from parameters"));
    MyTask->ConfirmAction = InConfirmAction;
    MyTask->CancelAction = InCancelAction;
    MyTask->SelectionModifierAction = InSelectionModifierAction;
    
    // Log whether input actions were properly assigned
    UE_LOG(LogTemp, Log, TEXT("CreateTask: ConfirmAction assigned: %s (%s)"), 
           MyTask->ConfirmAction ? TEXT("YES") : TEXT("NO"), 
           MyTask->ConfirmAction ? *MyTask->ConfirmAction->GetName() : TEXT("NULL"));
    UE_LOG(LogTemp, Log, TEXT("CreateTask: CancelAction assigned: %s (%s)"), 
           MyTask->CancelAction ? TEXT("YES") : TEXT("NO"), 
           MyTask->CancelAction ? *MyTask->CancelAction->GetName() : TEXT("NULL"));
    UE_LOG(LogTemp, Log, TEXT("CreateTask: SelectionModifierAction assigned: %s (%s)"), 
           MyTask->SelectionModifierAction ? TEXT("YES") : TEXT("NO"), 
           MyTask->SelectionModifierAction ? *MyTask->SelectionModifierAction->GetName() : TEXT("NULL"));
    
    // Initialize handles to an invalid state (0)
    MyTask->ConfirmPressedBindingHandle = 0;
    MyTask->ConfirmReleasedBindingHandle = 0;
    MyTask->CancelPressedBindingHandle = 0;
    UE_LOG(LogTemp, Log, TEXT("CreateTask: Initialized binding handles to 0"));

    return MyTask;
}

void UAbilityTask_WaitForMarqueeInput::Activate()
{
    UE_LOG(LogTemp, Log, TEXT("Activate: Starting activation of MarqueeInput task"));
    Super::Activate();

    if (!Ability) { 
        UE_LOG(LogTemp, Error, TEXT("Activate: No valid ability found, ending task"));
        EndTask(); 
        return; 
    }
    
    PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
    if (!PlayerController.IsValid()) { 
        UE_LOG(LogTemp, Error, TEXT("Activate: No valid PlayerController found, ending task"));
        EndTask(); 
        return; 
    }
    UE_LOG(LogTemp, Log, TEXT("Activate: Found valid PlayerController: %s"), *PlayerController->GetName());

    MyPlayerController = Cast<AMyPlayerController>(PlayerController.Get());
    if (!MyPlayerController.IsValid()) { 
        UE_LOG(LogTemp, Error, TEXT("Activate: Failed to cast to AMyPlayerController, ending task"));
        EndTask(); 
        return; 
    }
    UE_LOG(LogTemp, Log, TEXT("Activate: Successfully cast to AMyPlayerController: %s"), *MyPlayerController->GetName());

    UE_LOG(LogTemp, Log, TEXT("Activate: Getting EnhancedInputLocalPlayerSubsystem"));
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    {
        UE_LOG(LogTemp, Log, TEXT("Activate: Got EnhancedInputLocalPlayerSubsystem, checking for EnhancedInputComponent"));
        if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            UE_LOG(LogTemp, Log, TEXT("Activate: Found valid EnhancedInputComponent, binding actions"));
            
            if(ConfirmAction) {
                UE_LOG(LogTemp, Log, TEXT("Activate: Binding ConfirmAction - Released event"));
                // We are interested in when LMB is *released* to finalize selection
                FEnhancedInputActionEventBinding& ReleasedBinding = InputComponent->BindAction(ConfirmAction, ETriggerEvent::Completed, this, &UAbilityTask_WaitForMarqueeInput::HandleConfirmReleased);
                ConfirmReleasedBindingHandle = ReleasedBinding.GetHandle();
                UE_LOG(LogTemp, Log, TEXT("Activate: ConfirmReleasedBindingHandle: %u"), ConfirmReleasedBindingHandle);
                
                UE_LOG(LogTemp, Log, TEXT("Activate: Binding ConfirmAction - Pressed event"));
                // Also need to know when it's pressed to start drawing
                FEnhancedInputActionEventBinding& PressedBinding = InputComponent->BindAction(ConfirmAction, ETriggerEvent::Started, this, &UAbilityTask_WaitForMarqueeInput::HandleConfirmPressed);
                ConfirmPressedBindingHandle = PressedBinding.GetHandle();
                UE_LOG(LogTemp, Log, TEXT("Activate: ConfirmPressedBindingHandle: %u"), ConfirmPressedBindingHandle);
            } else {
                UE_LOG(LogTemp, Warning, TEXT("Activate: ConfirmAction is null, skipping binding"));
            }
            
            if(CancelAction) {
                UE_LOG(LogTemp, Log, TEXT("Activate: Binding CancelAction"));
                FEnhancedInputActionEventBinding& CancelBinding = InputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &UAbilityTask_WaitForMarqueeInput::HandleCancelPressed);
                CancelPressedBindingHandle = CancelBinding.GetHandle();
                UE_LOG(LogTemp, Log, TEXT("Activate: CancelPressedBindingHandle: %u"), CancelPressedBindingHandle);
            } else {
                UE_LOG(LogTemp, Warning, TEXT("Activate: CancelAction is null, skipping binding"));
            }
        } else { 
            UE_LOG(LogTemp, Error, TEXT("Activate: InputComponent is not EnhancedInputComponent, ending task"));
            EndTask(); 
            return; 
        }
    } else { 
        UE_LOG(LogTemp, Error, TEXT("Activate: Failed to get EnhancedInputLocalPlayerSubsystem, ending task"));
        EndTask(); 
        return; 
    }
    
    UE_LOG(LogTemp, Log, TEXT("Activate: Checking if we can start drawing marquee"));
    if (MyPlayerController.IsValid() && !MyPlayerController->bIsDrawingMarquee) {
         UE_LOG(LogTemp, Log, TEXT("Activate: Getting initial mouse position"));
         PlayerController->GetMousePosition(InitialMouseScreenPosition.X, InitialMouseScreenPosition.Y);
         UE_LOG(LogTemp, Log, TEXT("Activate: Initial mouse position: X=%f, Y=%f"), InitialMouseScreenPosition.X, InitialMouseScreenPosition.Y);
         
         UE_LOG(LogTemp, Log, TEXT("Activate: Calling Client_BeginMarquee"));
         MyPlayerController->Client_BeginMarquee(InitialMouseScreenPosition);
         bIsMouseDown = true; 
         UE_LOG(LogTemp, Log, TEXT("Activate: Marquee drawing started"));
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Activate: Could not start marquee drawing. Valid PC: %s, Already drawing: %s"), 
            MyPlayerController.IsValid() ? TEXT("YES") : TEXT("NO"),
            MyPlayerController.IsValid() && MyPlayerController->bIsDrawingMarquee ? TEXT("YES") : TEXT("NO"));
    }
}

void UAbilityTask_WaitForMarqueeInput::HandleConfirmPressed(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Log, TEXT("HandleConfirmPressed: Confirm button pressed"));
    
    if (MyPlayerController.IsValid() && !MyPlayerController->bIsDrawingMarquee)
    {
        UE_LOG(LogTemp, Log, TEXT("HandleConfirmPressed: Valid controller and not already drawing"));
        
        if(PlayerController.IsValid()) // Ensure PlayerController is still valid
        {
            UE_LOG(LogTemp, Log, TEXT("HandleConfirmPressed: Getting initial mouse position"));
            PlayerController->GetMousePosition(InitialMouseScreenPosition.X, InitialMouseScreenPosition.Y);
            UE_LOG(LogTemp, Log, TEXT("HandleConfirmPressed: Initial mouse position: X=%f, Y=%f"), InitialMouseScreenPosition.X, InitialMouseScreenPosition.Y);
            
            UE_LOG(LogTemp, Log, TEXT("HandleConfirmPressed: Calling Client_BeginMarquee"));
            MyPlayerController->Client_BeginMarquee(InitialMouseScreenPosition);
            bIsMouseDown = true;
            UE_LOG(LogTemp, Log, TEXT("HandleConfirmPressed: Marquee drawing started"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("HandleConfirmPressed: PlayerController is no longer valid"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HandleConfirmPressed: Cannot start marquee. Valid PC: %s, Already drawing: %s"), 
            MyPlayerController.IsValid() ? TEXT("YES") : TEXT("NO"),
            MyPlayerController.IsValid() && MyPlayerController->bIsDrawingMarquee ? TEXT("YES") : TEXT("NO"));
    }
}

void UAbilityTask_WaitForMarqueeInput::HandleConfirmReleased(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Log, TEXT("HandleConfirmReleased: Confirm button released"));
    
    if (bIsMouseDown && MyPlayerController.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("HandleConfirmReleased: Mouse is down and valid controller"));
        
        FVector2D EndPos;
        PlayerController->GetMousePosition(EndPos.X, EndPos.Y);
        UE_LOG(LogTemp, Log, TEXT("HandleConfirmReleased: Ending mouse position: X=%f, Y=%f"), EndPos.X, EndPos.Y);
        
        UE_LOG(LogTemp, Log, TEXT("HandleConfirmReleased: Calling Client_EndMarquee"));
        MyPlayerController->Client_EndMarquee(); // Stop drawing

        if (ShouldBroadcastAbilityTaskDelegates())
        {
            UE_LOG(LogTemp, Log, TEXT("HandleConfirmReleased: Broadcasting OnMarqueeFinished. Start: X=%f, Y=%f, End: X=%f, Y=%f"), 
                InitialMouseScreenPosition.X, InitialMouseScreenPosition.Y, EndPos.X, EndPos.Y);
            OnMarqueeFinished.Broadcast(InitialMouseScreenPosition, EndPos);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("HandleConfirmReleased: Cannot broadcast delegates"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HandleConfirmReleased: Cannot finish marquee. Mouse down: %s, Valid PC: %s"), 
            bIsMouseDown ? TEXT("YES") : TEXT("NO"),
            MyPlayerController.IsValid() ? TEXT("YES") : TEXT("NO"));
    }
    
    UE_LOG(LogTemp, Log, TEXT("HandleConfirmReleased: Ending task"));
    EndTask();
}

void UAbilityTask_WaitForMarqueeInput::HandleCancelPressed(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Log, TEXT("HandleCancelPressed: Cancel button pressed"));
    
    if (MyPlayerController.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("HandleCancelPressed: Stopping marquee drawing"));
        MyPlayerController->Client_EndMarquee(); // Stop drawing
    }
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        UE_LOG(LogTemp, Log, TEXT("HandleCancelPressed: Broadcasting OnCancelled"));
        OnCancelled.Broadcast();
    }
    UE_LOG(LogTemp, Log, TEXT("HandleCancelPressed: Ending task"));
    EndTask();
}

void UAbilityTask_WaitForMarqueeInput::TickTask(float DeltaTime)
{
    UE_LOG(LogTemp, Verbose, TEXT("TickTask: Updating marquee"));
    Super::TickTask(DeltaTime);
    if (bIsMouseDown && PlayerController.IsValid() && MyPlayerController.IsValid())
    {
        FVector2D CurrentMousePos;
        PlayerController->GetMousePosition(CurrentMousePos.X, CurrentMousePos.Y);
        UE_LOG(LogTemp, Verbose, TEXT("TickTask: Current mouse position: X=%f, Y=%f"), CurrentMousePos.X, CurrentMousePos.Y);
        MyPlayerController->Client_UpdateMarquee(CurrentMousePos);
    }
    else if (bIsMouseDown)
    {
        UE_LOG(LogTemp, Verbose, TEXT("TickTask: Cannot update marquee. Valid PC: %s, Valid MyPC: %s"), 
            PlayerController.IsValid() ? TEXT("YES") : TEXT("NO"),
            MyPlayerController.IsValid() ? TEXT("YES") : TEXT("NO"));
    }
}

void UAbilityTask_WaitForMarqueeInput::OnDestroy(bool bInOwnerFinished)
{
    UE_LOG(LogTemp, Log, TEXT("OnDestroy: Destroying MarqueeInput task"));
    if (MyPlayerController.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("OnDestroy: Stopping marquee drawing"));
        MyPlayerController->Client_EndMarquee(); // Ensure drawing stops
    }

    if (PlayerController.IsValid()) {
        UE_LOG(LogTemp, Log, TEXT("OnDestroy: PlayerController is valid, cleaning up input bindings"));
        
        if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            UE_LOG(LogTemp, Log, TEXT("OnDestroy: Found valid EnhancedInputComponent"));
            
            // Clean up any active bindings using the stored uint32 handles
            if (ConfirmPressedBindingHandle != 0) // Check if handle is valid (not 0)
            {
                UE_LOG(LogTemp, Log, TEXT("OnDestroy: Removing ConfirmPressedBinding with handle: %u"), ConfirmPressedBindingHandle);
                InputComponent->RemoveBindingByHandle(ConfirmPressedBindingHandle);
                ConfirmPressedBindingHandle = 0; // Reset handle
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("OnDestroy: ConfirmPressedBindingHandle is invalid (0)"));
            }
                
            if (ConfirmReleasedBindingHandle != 0)
            {
                UE_LOG(LogTemp, Log, TEXT("OnDestroy: Removing ConfirmReleasedBinding with handle: %u"), ConfirmReleasedBindingHandle);
                InputComponent->RemoveBindingByHandle(ConfirmReleasedBindingHandle);
                ConfirmReleasedBindingHandle = 0;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("OnDestroy: ConfirmReleasedBindingHandle is invalid (0)"));
            }
                
            if (CancelPressedBindingHandle != 0)
            {
                UE_LOG(LogTemp, Log, TEXT("OnDestroy: Removing CancelPressedBinding with handle: %u"), CancelPressedBindingHandle);
                InputComponent->RemoveBindingByHandle(CancelPressedBindingHandle);
                CancelPressedBindingHandle = 0;
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("OnDestroy: CancelPressedBindingHandle is invalid (0)"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("OnDestroy: InputComponent is not EnhancedInputComponent"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnDestroy: PlayerController is no longer valid"));
    }
    UE_LOG(LogTemp, Log, TEXT("OnDestroy: Calling Super::OnDestroy with bInOwnerFinished=%s"), bInOwnerFinished ? TEXT("true") : TEXT("false"));
    Super::OnDestroy(bInOwnerFinished);
    UE_LOG(LogTemp, Log, TEXT("OnDestroy: MarqueeInput task fully destroyed"));
}
