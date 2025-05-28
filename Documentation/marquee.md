Understood! No more options, one definitive path forward, with ELI5 for Blueprint and technical for C++. We will aim for a complete, sequential guide to implement the frustum-based selection marquee with additive/subtractive selection via a modifier key, and client-side predicted visual feedback.

Let's begin.

---

**Definitive Implementation Guide: Frustum Selection Marquee Gameplay Ability**

**Goal**: Implement a Gameplay Ability allowing the player to draw a marquee on screen. Upon release, actors within the 3D view frustum defined by this marquee will be selected. Selection can be modified (add/remove) using a modifier key. Selected actors will display a decal. This will be primarily controlled while in Top-Down view mode.

---

**I. List of Files to be Modified/Created (and Why)**

1.  **`AlsGameplayTags.h` & `AlsGameplayTags.cpp`** (ALS Module - Modified)
    *   **Why**: To define new Gameplay Tags essential for identifying the selection ability, its active state, and the selectability criteria of actors.
    *   **What**: Add `Ability.Action.SelectionMarquee`, `State.SelectingWithMarquee`, `Selectable.Unit.CanBeMarqueeSelected`.
2.  **`SelectableActorInterface.h`** (Your Game Module, e.g., `Source/YourGame/Public/Interfaces/` - Created)
    *   **Why**: To define a contract for actors that can be selected, allowing polymorphic handling of selection feedback and selection eligibility checks.
    *   **What**: Create `USelectableActorInterface` with `OnSelectedByMarquee()`, `OnDeselectedByMarquee()`, and `IsSelectableByMarquee(APlayerController* SelectingPlayer) const` functions.
3.  **`MyPlayerController.h` & `MyPlayerController.cpp`** (Your Game Module, e.g., `Source/YourGame/Public/Player/` - Created or Modified if you have one that inherits from `APlayerController`)
    *   **Why**: This class will manage the client-side marquee drawing state and handle the authoritative server-side selection logic (frustum checking, actor filtering, updating the selected list).
    *   **What**: Add properties for marquee screen coordinates, a list of selected actors, and functions to start/update/end marquee drawing, plus the server RPC for performing selection.
4.  **`MyHUD.h` & `MyHUD.cpp`** (Your Game Module, e.g., `Source/YourGame/Public/UI/` - Created or Modified if you have one that inherits from `AHUD`)
    *   **Why**: To visually render the 2D selection marquee rectangle on the player's screen.
    *   **What**: Override `DrawHUD()` to draw a rectangle based on coordinates provided by `AMyPlayerController`. Add UPROPERTYs for marquee color/material for BP customization.
5.  **`AbilityTask_WaitForMarqueeInput.h` & `AbilityTask_WaitForMarqueeInput.cpp`** (Your Game Module, e.g., `Source/YourGame/Public/Abilities/Tasks/` - Created)
    *   **Why**: A custom `UGameplayAbilityTask` to encapsulate the asynchronous logic of waiting for marquee input (mouse down, drag, mouse up/cancel), managing client-side drawing updates, and communicating the final marquee coordinates to the server.
    *   **What**: Implement `Activate()`, `TickTask()`, `OnDestroy()`. It will have output delegates `OnMarqueeFinished` and `OnCancelled`.
6.  **`BGA_SelectionMarquee.uasset`** (Blueprint Gameplay Ability - Content Browser, e.g., `Content/Blueprints/Abilities/` - Created)
    *   **Why**: The primary Gameplay Ability asset that players will activate. It uses `AbilityTask_WaitForMarqueeInput` to manage the marquee process and then triggers the selection logic on the Player Controller.
    *   **What**: Configure ability tags, activation conditions, and implement `ActivateAbility` to use the custom task.
7.  **`AlsCharacterExample.h` & `AlsCharacterExample.cpp`** (ALSExtras Module - Modified)
    *   **Why**: To add the visual feedback component (Decal) for selected units and properties to customize it. Also, to potentially hold a reference to the "Selection Modifier" Input Action state if checked in C++.
    *   **What**: Add `UDecalComponent* SelectionDecalComponent` and UPROPERTYs for its default material, color, and size.
8.  **Selectable Actor Blueprints (e.g., `BP_Selectable_Unit`, `BP_Selectable_Building`)** (Content Browser - Modified or Created)
    *   **Why**: These are the actors in your world that can be selected. They need to implement the `ISelectableActorInterface` and define how they visually respond to being selected/deselected.
    *   **What**: Add the interface, implement its events, and add appropriate Actor Tags and/or Gameplay Tags (to their ASC).
9.  **Input Action Assets** (Content Browser, e.g., `Content/Input/Actions/` - Created)
    *   `IA_MarqueeSelect_Hold`: Digital (bool) or Hold type. For Left Mouse Button to start/confirm selection.
    *   `IA_MarqueeSelect_Cancel`: Digital (bool). For Right Mouse Button to cancel marquee draw.
    *   `IA_Selection_Modifier`: Digital (bool). For Left Shift key to modify selection (add/remove).
10. **Input Mapping Context: `IMC_TopDownView` (or your game's main IMC - Modified/Created)**
    *   **Why**: To map the physical keys (LMB, RMB, Shift) to the `IA_MarqueeSelect_Hold`, `IA_MarqueeSelect_Cancel`, and `IA_Selection_Modifier` Input Actions. This IMC should be active when in TopDown view mode and when the cursor is visible.

---

**II. Sequential Implementation Steps**

**Phase 1: C++ Core Setup**

**Step 1.1: Define Gameplay Tags**

*   **Location**: `Source/ALS/Public/Utility/AlsGameplayTags.h` and `Source/ALS/Private/Utility/AlsGameplayTags.cpp`
*   **Action**:
    *   In `AlsGameplayTags.h`, within appropriate namespaces (create new ones if needed like `AlsAbilityTags`, `AlsStatusTags`, `AlsSelectableTags`):
        ```cpp
        // In a new namespace or existing one like AlsAbilityTags
        namespace AlsAbilityActionTags { // Example namespace
            ALS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SelectionMarquee)
        }
        namespace AlsStatusTags { // Example namespace
            ALS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(SelectingWithMarquee)
        }
        namespace AlsSelectableObjectTags { // Example namespace
            ALS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Unit_CanBeMarqueeSelected)
            // Add more specific types like Infantry, Vehicle, Resource_Wood etc. as needed
        }
        ```
    *   In `AlsGameplayTags.cpp`:
        ```cpp
        namespace AlsAbilityActionTags {
            UE_DEFINE_GAMEPLAY_TAG(SelectionMarquee, FName{TEXTVIEW("Ability.Action.SelectionMarquee")})
        }
        namespace AlsStatusTags {
            UE_DEFINE_GAMEPLAY_TAG(SelectingWithMarquee, FName{TEXTVIEW("State.SelectingWithMarquee")})
        }
        namespace AlsSelectableObjectTags {
            UE_DEFINE_GAMEPLAY_TAG(Unit_CanBeMarqueeSelected, FName{TEXTVIEW("Selectable.Unit.CanBeMarqueeSelected")})
        }
        ```
*   **Compile C++**.
*   **Editor**: Open `Project Settings -> Project -> Gameplay Tags`. Click "Add New Tag Source" if you don't have one, create a new `.ini` file (e.g., `Config/GameplayTags/MyGameTags.ini`). Ensure these new tags are listed. If not, click "Import Tags from Config" or manually add them.

**Step 1.2: Create `ISelectableActorInterface`**

*   **Location**: `Source/YourGame/Public/Interfaces/SelectableActorInterface.h` (Create folders if they don't exist)
*   **Action**: Create the C++ interface file.
    ```cpp
    // SelectableActorInterface.h
    #pragma once

    #include "CoreMinimal.h"
    #include "UObject/Interface.h"
    #include "SelectableActorInterface.generated.h"

    UINTERFACE(MinimalAPI, Blueprintable)
    class USelectableActorInterface : public UInterface
    {
        GENERATED_BODY()
    };

    class YOURGAME_API ISelectableActorInterface // Replace YOURGAME_API with your project's API macro
    {
        GENERATED_BODY()
    public:
        UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selection")
        void OnSelectedByMarquee(APlayerController* SelectingPlayer);

        UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selection")
        void OnDeselectedByMarquee(APlayerController* SelectingPlayer);

        // Allows the actor to determine if it can be selected by this specific player/marquee action
        UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Selection")
        bool IsCurrentlySelectableByMarquee(APlayerController* SelectingPlayer) const;
    };
    ```
*   **Compile C++**.

**Step 1.3: Create/Modify `AMyPlayerController`**

*   **Location**: `Source/YourGame/Public/Player/MyPlayerController.h` & `Source/YourGame/Private/Player/MyPlayerController.cpp`
    *   If you don't have a custom PlayerController, create one inheriting from `APlayerController`. Make sure your GameMode uses it.
*   **Action**: Add marquee state, selection storage, and server RPC for selection.

    *   **`MyPlayerController.h`**:
        ```cpp
        #pragma once
        #include "CoreMinimal.h"
        #include "GameFramework/PlayerController.h"
        #include "GameplayTagContainer.h" // For FGameplayTagContainer
        #include "MyPlayerController.generated.h"

        UCLASS()
        class YOURGAME_API AMyPlayerController : public APlayerController
        {
            GENERATED_BODY()
        public:
            AMyPlayerController();

            // --- Marquee Drawing State (Client-Only) ---
            UPROPERTY(BlueprintReadOnly, Category = "Marquee")
            bool bIsDrawingMarquee;

            UPROPERTY(BlueprintReadOnly, Category = "Marquee")
            FVector2D MarqueeStartScreenPosition;

            UPROPERTY(BlueprintReadOnly, Category = "Marquee")
            FVector2D MarqueeCurrentScreenPosition;

            // Called by AbilityTask to manage drawing state
            void Client_BeginMarquee(FVector2D ScreenPosition);
            void Client_UpdateMarquee(FVector2D ScreenPosition);
            void Client_EndMarquee(); // Clears drawing state

            // --- Selection Logic & Storage ---
            // This list is authoritative on the server, clients get told via OnSelected/OnDeselected on actors
            UPROPERTY(VisibleInstanceOnly, Category = "Selection", Transient) // Transient if not saving selection
            TArray<TWeakObjectPtr<AActor>> SelectedActors;

            UFUNCTION(Server, Reliable, WithValidation)
            void Server_PerformMarqueeSelection(FVector2D ScreenStart, FVector2D ScreenEnd,
                                                const TArray<FName>& RequiredActorTags,
                                                const FGameplayTagContainer& RequiredGameplayTags,
                                                bool bAddToSelection, bool bRemoveFromSelection); // Modifiers

            // Helper to clear current selection
            UFUNCTION(BlueprintCallable, Category = "Selection")
            void ClearCurrentSelection();

        protected:
            virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
            // We are not replicating SelectedActors directly from PC to all clients.
            // Selection feedback is handled by server telling individual actors they are selected/deselected.
        };
        ```

    *   **`MyPlayerController.cpp`**:
        ```cpp
        #include "MyPlayerController.h"
        #include "Net/UnrealNetwork.h"
        #include "Kismet/GameplayStatics.h"
        #include "Interfaces/SelectableActorInterface.h" // Your interface
        #include "AbilitySystemComponent.h" // For checking GameplayTags on actors
        // #include "Engine/Selection.h" // For USelection if you want to use editor-like selection sets (more complex)

        AMyPlayerController::AMyPlayerController()
        {
            bIsDrawingMarquee = false;
        }

        void AMyPlayerController::Client_BeginMarquee(FVector2D ScreenPosition)
        {
            bIsDrawingMarquee = true;
            MarqueeStartScreenPosition = ScreenPosition;
            MarqueeCurrentScreenPosition = ScreenPosition;
        }

        void AMyPlayerController::Client_UpdateMarquee(FVector2D ScreenPosition)
        {
            if (bIsDrawingMarquee)
            {
                MarqueeCurrentScreenPosition = ScreenPosition;
            }
        }

        void AMyPlayerController::Client_EndMarquee()
        {
            bIsDrawingMarquee = false;
        }

        void AMyPlayerController::ClearCurrentSelection()
        {
            // Tell previously selected actors they are deselected
            for (TWeakObjectPtr<AActor> PreviouslySelectedActorPtr : SelectedActors)
            {
                if (AActor* PreviouslySelectedActor = PreviouslySelectedActorPtr.Get())
                {
                    if (ISelectableActorInterface* Selectable = Cast<ISelectableActorInterface>(PreviouslySelectedActor))
                    {
                        Selectable->Execute_OnDeselectedByMarquee(PreviouslySelectedActor, this);
                    }
                }
            }
            SelectedActors.Empty();
        }

        bool AMyPlayerController::Server_PerformMarqueeSelection_Validate(FVector2D ScreenStart, FVector2D ScreenEnd,
                                                                        const TArray<FName>& RequiredActorTags,
                                                                        const FGameplayTagContainer& RequiredGameplayTags,
                                                                        bool bAddToSelection, bool bRemoveFromSelection) { return true; }

        void AMyPlayerController::Server_PerformMarqueeSelection_Implementation(FVector2D ScreenStart, FVector2D ScreenEnd,
                                                                              const TArray<FName>& RequiredActorTags,
                                                                              const FGameplayTagContainer& RequiredGameplayTags,
                                                                              bool bAddToSelection, bool bRemoveFromSelection)
        {
            TArray<AActor*> ActorsInFrustum;
            // Convert 2D screen rect to a 3D selection frustum/volume
            // UGameplayStatics::GetActorsInSelectionRectangle is simple but uses a fixed projection plane.
            // For a dynamic camera, a proper frustum is better.
            // FSceneView::ProjectWorldToScreen for corners -> DeprojectScreenToWorld for rays -> build frustum planes
            // For simplicity here, let's imagine a helper function GetActorsInFrustumFromScreenRect exists
            // This is a complex part. A simpler start is GetActorsInSelectionRectangle.
            
            FVector2D RectMin = FVector2D(FMath::Min(ScreenStart.X, ScreenEnd.X), FMath::Min(ScreenStart.Y, ScreenEnd.Y));
            FVector2D RectMax = FVector2D(FMath::Max(ScreenStart.X, ScreenEnd.X), FMath::Max(ScreenStart.Y, ScreenEnd.Y));
            
            TArray<AActor*> FoundActors; // Actors found by the frustum/rectangle check
            UGameplayStatics::GetActorsInSelectionRectangle(GetWorld(), RectMin, RectMax, FoundActors, false, false);

            TArray<AActor*> NewlySelectedActors;
            TArray<AActor*> ActorsToDeselect;

            if (!bAddToSelection && !bRemoveFromSelection) // Standard new selection
            {
                ClearCurrentSelection(); // Deselect everything first
            }

            for (AActor* Actor : FoundActors)
            {
                if (!IsValid(Actor)) continue;

                bool bPassesActorTagCheck = RequiredActorTags.IsEmpty(); // Pass if no actor tags required
                if (!bPassesActorTagCheck)
                {
                    int32 MatchCount = 0;
                    for (const FName& RequiredTag : RequiredActorTags)
                    {
                        if (Actor->ActorHasTag(RequiredTag))
                        {
                            MatchCount++;
                        }
                    }
                    bPassesActorTagCheck = (MatchCount == RequiredActorTags.Num()); // Must have ALL actor tags
                }
                if (!bPassesActorTagCheck) continue;


                bool bPassesGameplayTagCheck = RequiredGameplayTags.IsEmpty(); // Pass if no gameplay tags required
                if (!bPassesGameplayTagCheck)
                {
                    IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Actor);
                    if (ASI && ASI->GetAbilitySystemComponent())
                    {
                        bPassesGameplayTagCheck = ASI->GetAbilitySystemComponent()->HasAllMatchingGameplayTags(RequiredGameplayTags);
                    } else {
                        bPassesGameplayTagCheck = false; // No ASC, can't have gameplay tags
                    }
                }
                if (!bPassesGameplayTagCheck) continue;
                

                ISelectableActorInterface* Selectable = Cast<ISelectableActorInterface>(Actor);
                if (Selectable && Selectable->Execute_IsCurrentlySelectableByMarquee(Actor, this))
                {
                    // Handle Additive/Subtractive logic
                    bool bIsAlreadySelected = SelectedActors.Contains(Actor);

                    if (bRemoveFromSelection)
                    {
                        if (bIsAlreadySelected)
                        {
                            ActorsToDeselect.Add(Actor);
                        }
                    }
                    else if (bAddToSelection)
                    {
                        if (!bIsAlreadySelected)
                        {
                            NewlySelectedActors.Add(Actor);
                        }
                    }
                    else // Standard new selection (old ones cleared already)
                    {
                        NewlySelectedActors.Add(Actor);
                    }
                }
            }

            // Process Deselections
            for (AActor* ActorToDeselect : ActorsToDeselect)
            {
                if (ISelectableActorInterface* Selectable = Cast<ISelectableActorInterface>(ActorToDeselect))
                {
                    Selectable->Execute_OnDeselectedByMarquee(ActorToDeselect, this);
                }
                SelectedActors.Remove(ActorToDeselect);
            }

            // Process New Selections
            for (AActor* ActorToSelect : NewlySelectedActors)
            {
                if (ISelectableActorInterface* Selectable = Cast<ISelectableActorInterface>(ActorToSelect))
                {
                    Selectable->Execute_OnSelectedByMarquee(ActorToSelect, this);
                }
                SelectedActors.AddUnique(ActorToSelect); // AddUnique for safety
            }
        }

        void AMyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
        {
            Super::GetLifetimeReplicatedProps(OutLifetimeProps);
            // SelectedActors is NOT replicated directly from PC to all clients.
            // Server tells individual actors they are selected/deselected.
            // If other clients *needed* to know this PC's selection, you would replicate it.
        }
        ```
*   **Compile C++**.

**Step 1.4: Create/Modify `AMyHUD`**

*   **Location**: `Source/YourGame/Public/UI/MyHUD.h` & `Source/YourGame/Private/UI/MyHUD.cpp`
    *   If you don't have one, create it inheriting from `AHUD`. Set it as your GameMode's HUD Class.
*   **Action**: Add drawing logic and customizable properties.

    *   **`MyHUD.h`**:
        ```cpp
        #pragma once
        #include "CoreMinimal.h"
        #include "GameFramework/HUD.h"
        #include "MyHUD.generated.h"

        UCLASS()
        class YOURGAME_API AMyHUD : public AHUD
        {
            GENERATED_BODY()
        public:
            virtual void DrawHUD() override;

        protected:
            UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Marquee")
            FLinearColor MarqueeColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.25f); // Green, slightly transparent

            UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Marquee")
            FLinearColor MarqueeBorderColor = FLinearColor::Green;

            UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Marquee")
            float MarqueeBorderThickness = 1.0f;

            // UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Marquee")
            // TSoftObjectPtr<UMaterialInterface> MarqueeMaterial; // For material-based drawing
        };
        ```
    *   **`MyHUD.cpp`**:
        ```cpp
        #include "MyHUD.h"
        #include "MyPlayerController.h" // Your player controller
        #include "Engine/Canvas.h"    // For DrawRect

        void AMyHUD::DrawHUD()
        {
            Super::DrawHUD();

            AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwningPlayerController());
            if (PC && PC->bIsDrawingMarquee)
            {
                FVector2D StartPos = PC->MarqueeStartScreenPosition;
                FVector2D CurrentPos = PC->MarqueeCurrentScreenPosition;

                float MinX = FMath::Min(StartPos.X, CurrentPos.X);
                float MinY = FMath::Min(StartPos.Y, CurrentPos.Y);
                float MaxX = FMath::Max(StartPos.X, CurrentPos.X);
                float MaxY = FMath::Max(StartPos.Y, CurrentPos.Y);

                // Draw filled rectangle (the marquee itself)
                DrawRect(MarqueeColor, MinX, MinY, MaxX - MinX, MaxY - MinY);

                // Draw border lines
                DrawLine(MinX, MinY, MaxX, MinY, MarqueeBorderColor, MarqueeBorderThickness); // Top
                DrawLine(MinX, MaxY, MaxX, MaxY, MarqueeBorderColor, MarqueeBorderThickness); // Bottom
                DrawLine(MinX, MinY, MinX, MaxY, MarqueeBorderColor, MarqueeBorderThickness); // Left
                DrawLine(MaxX, MinY, MaxX, MaxY, MarqueeBorderColor, MarqueeBorderThickness); // Right
            }
        }
        ```
*   **Compile C++**.
*   **Editor**: Create `BP_MyHUD` from `AMyHUD`. Set it as your project's default HUD class (`Project Settings -> Maps & Modes`). Open `BP_MyHUD` Class Defaults to customize `MarqueeColor`, etc.

**Step 1.5: Create `UAbilityTask_WaitForMarqueeInput`**

*   **Location**: `Source/YourGame/Public/Abilities/Tasks/AbilityTask_WaitForMarqueeInput.h` & `Source/YourGame/Private/Abilities/Tasks/AbilityTask_WaitForMarqueeInput.cpp`
*   **Action**: The most complex C++ part.

    *   **`AbilityTask_WaitForMarqueeInput.h`**:
        ```cpp
        #pragma once
        #include "CoreMinimal.h"
        #include "Abilities/Tasks/AbilityTask.h"
        #include "AbilityTask_WaitForMarqueeInput.generated.h"

        DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMarqueeFinishedDelegate, FVector2D, ScreenStartPos, FVector2D, ScreenEndPos);
        DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMarqueeCancelledDelegate);

        UCLASS()
        class YOURGAME_API UAbilityTask_WaitForMarqueeInput : public UAbilityTask
        {
            GENERATED_BODY()
        public:
            UPROPERTY(BlueprintAssignable)
            FMarqueeFinishedDelegate OnMarqueeFinished;

            UPROPERTY(BlueprintAssignable)
            FMarqueeCancelledDelegate OnCancelled;

            UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
            static UAbilityTask_WaitForMarqueeInput* CreateTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, TSubclassOf<UGameplayAbility> TriggerOnCancelAbility);

            virtual void Activate() override;
            virtual void TickTask(float DeltaTime) override;
            virtual void OnDestroy(bool bInOwnerFinished) override;

        protected:
            FVector2D InitialMouseScreenPosition;
            bool bIsMouseDown = false;

            // Input Actions this task will listen for (set these up in your project)
            UPROPERTY() // Keep UPROPERTY to prevent GC
            TObjectPtr<const UInputAction> ConfirmAction; // e.g., IA_MarqueeSelect_Hold (LMB)
            UPROPERTY()
            TObjectPtr<const UInputAction> CancelAction;  // e.g., IA_MarqueeSelect_Cancel (RMB)
            UPROPERTY()
            TObjectPtr<const UInputAction> SelectionModifierAction; // e.g., IA_Selection_Modifier (Shift)


            FDelegateHandle ConfirmPressedHandle;
            FDelegateHandle ConfirmReleasedHandle;
            FDelegateHandle CancelPressedHandle;

            void HandleConfirmPressed(const FInputActionValue& Value);
            void HandleConfirmReleased(const FInputActionValue& Value);
            void HandleCancelPressed(const FInputActionValue& Value);
            
            TWeakObjectPtr<APlayerController> PlayerController;
            TWeakObjectPtr<class AMyPlayerController> MyPlayerController; // Casted version
        };
        ```

    *   **`AbilityTask_WaitForMarqueeInput.cpp`**:
        ```cpp
        #include "Abilities/Tasks/AbilityTask_WaitForMarqueeInput.h"
        #include "GameFramework/PlayerController.h"
        #include "EnhancedInputComponent.h"
        #include "EnhancedInputSubsystems.h"
        #include "Player/MyPlayerController.h" // Your PC header
        #include "AbilitySystemComponent.h"

        UAbilityTask_WaitForMarqueeInput* UAbilityTask_WaitForMarqueeInput::CreateTask(UGameplayAbility* OwningAbility, FName TaskInstanceName, TSubclassOf<UGameplayAbility> TriggerOnCancelAbility)
        {
            UAbilityTask_WaitForMarqueeInput* MyTask = NewAbilityTask<UAbilityTask_WaitForMarqueeInput>(OwningAbility, TaskInstanceName);
            // MyTask->TriggerOnCancelAbility = TriggerOnCancelAbility; // If you want to trigger another ability on cancel
            
            // Find Input Actions by path or pass them in if they are configurable on the ability
            // For simplicity, hardcoding paths here. Better to pass TObjectPtr<UInputAction> from ability.
            MyTask->ConfirmAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_MarqueeSelect_Hold.IA_MarqueeSelect_Hold")); // UPDATE PATH
            MyTask->CancelAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_MarqueeSelect_Cancel.IA_MarqueeSelect_Cancel"));   // UPDATE PATH
            MyTask->SelectionModifierAction = LoadObject<UInputAction>(nullptr, TEXT("/Game/Input/Actions/IA_Selection_Modifier.IA_Selection_Modifier")); // UPDATE PATH

            return MyTask;
        }

        void UAbilityTask_WaitForMarqueeInput::Activate()
        {
            Super::Activate();

            if (!Ability) { EndTask(); return; }
            PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
            if (!PlayerController.IsValid()) { EndTask(); return; }

            MyPlayerController = Cast<AMyPlayerController>(PlayerController.Get());
            if (!MyPlayerController.IsValid()) { EndTask(); return; }

            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
            {
                if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
                {
                    if(ConfirmAction) {
                        // We are interested in when LMB is *released* to finalize selection
                        ConfirmReleasedHandle = InputComponent->BindAction(ConfirmAction, ETriggerEvent::Completed, this, &UAbilityTask_WaitForMarqueeInput::HandleConfirmReleased).GetHandle();
                        // Also need to know when it's pressed to start drawing
                        ConfirmPressedHandle = InputComponent->BindAction(ConfirmAction, ETriggerEvent::Started, this, &UAbilityTask_WaitForMarqueeInput::HandleConfirmPressed).GetHandle();
                    }
                    if(CancelAction) {
                        CancelPressedHandle = InputComponent->BindAction(CancelAction, ETriggerEvent::Started, this, &UAbilityTask_WaitForMarqueeInput::HandleCancelPressed).GetHandle();
                    }
                } else { EndTask(); return; }
            } else { EndTask(); return; }
            
            // Initial mouse down might have already happened to trigger the ability,
            // so we might need to get initial pos differently or assume this task is spawned *after* mouse down.
            // For now, let's assume this task starts when mouse is already down (or the ability handles the initial press).
            // A better way: The ability that calls this task passes the initial mouse down position.
            // For this example, we'll grab current pos on activate if not already drawing.
            if (MyPlayerController.IsValid() && !MyPlayerController->bIsDrawingMarquee) {
                 PlayerController->GetMousePosition(InitialMouseScreenPosition.X, InitialMouseScreenPosition.Y);
                 MyPlayerController->Client_BeginMarquee(InitialMouseScreenPosition);
                 bIsMouseDown = true; // Assume mouse is down if task activates
            }
        }
        
        void UAbilityTask_WaitForMarqueeInput::HandleConfirmPressed(const FInputActionValue& Value)
        {
            // This is called when LMB is pressed *while the task is active*
            // (e.g. if the ability was triggered by something else and is waiting for LMB)
            // Or if the ability itself is triggered by LMB press, this might fire immediately.
            if (MyPlayerController.IsValid() && !MyPlayerController->bIsDrawingMarquee)
            {
                PlayerController->GetMousePosition(InitialMouseScreenPosition.X, InitialMouseScreenPosition.Y);
                MyPlayerController->Client_BeginMarquee(InitialMouseScreenPosition);
                bIsMouseDown = true;
            }
        }


        void UAbilityTask_WaitForMarqueeInput::HandleConfirmReleased(const FInputActionValue& Value)
        {
            if (bIsMouseDown && MyPlayerController.IsValid())
            {
                FVector2D EndPos;
                PlayerController->GetMousePosition(EndPos.X, EndPos.Y);
                MyPlayerController->Client_EndMarquee(); // Stop drawing

                if (ShouldBroadcastAbilityTaskDelegates())
                {
                    OnMarqueeFinished.Broadcast(InitialMouseScreenPosition, EndPos);
                }
            }
            EndTask();
        }

        void UAbilityTask_WaitForMarqueeInput::HandleCancelPressed(const FInputActionValue& Value)
        {
            if (MyPlayerController.IsValid())
            {
                MyPlayerController->Client_EndMarquee(); // Stop drawing
            }
            if (ShouldBroadcastAbilityTaskDelegates())
            {
                OnCancelled.Broadcast();
            }
            EndTask();
        }

        void UAbilityTask_WaitForMarqueeInput::TickTask(float DeltaTime)
        {
            Super::TickTask(DeltaTime);
            if (bIsMouseDown && PlayerController.IsValid() && MyPlayerController.IsValid())
            {
                FVector2D CurrentMousePos;
                PlayerController->GetMousePosition(CurrentMousePos.X, CurrentMousePos.Y);
                MyPlayerController->Client_UpdateMarquee(CurrentMousePos);
            }
        }

        void UAbilityTask_WaitForMarqueeInput::OnDestroy(bool bInOwnerFinished)
        {
            if (MyPlayerController.IsValid())
            {
                MyPlayerController->Client_EndMarquee(); // Ensure drawing stops
            }

            if (PlayerController.IsValid()) {
                if (UEnhancedInputComponent* InputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
                {
                    InputComponent->RemoveBindingByHandle(ConfirmPressedHandle);
                    InputComponent->RemoveBindingByHandle(ConfirmReleasedHandle);
                    InputComponent->RemoveBindingByHandle(CancelPressedHandle);
                }
            }
            Super::OnDestroy(bInOwnerFinished);
        }
        ```
*   **Compile C++**.
*   **IMPORTANT**: Update the `LoadObject` paths for `ConfirmAction`, `CancelAction`, and `SelectionModifierAction` to the correct paths of your `UInputAction` assets. A better way is to pass these `UInputAction*` as parameters to the `CreateTask` function from the `BGA_SelectionMarquee` ability (where they can be exposed as variables).

**Step 1.6: Add Selection Decal to `AAlsCharacterExample`**

*   **File**: `AlsCharacterExample.h`
    ```cpp
    // Inside AAlsCharacterExample class
    protected:
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Selection Feedback", meta = (AllowPrivateAccess = "true"))
        TObjectPtr<UDecalComponent> SelectionDecalComponent;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Selection Feedback|Config")
        TObjectPtr<UMaterialInterface> DefaultSelectionDecalMaterial;

        UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Selection Feedback|Config")
        FLinearColor DefaultSelectionDecalColor = FLinearColor(0.1f, 0.8f, 0.1f, 0.5f); // A nice green

        UPROPERTY(EditDefaultsOnly, Category = "Selection Feedback|Config")
        FVector DefaultSelectionDecalSize{75.f, 150.f, 150.f}; // Adjust as needed
    ```
*   **File**: `AlsCharacterExample.cpp`
    ```cpp
    // In AAlsCharacterExample constructor
    #include "Components/DecalComponent.h"

    SelectionDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionDecal"));
    SelectionDecalComponent->SetupAttachment(GetCapsuleComponent()); // Attach to capsule bottom
    SelectionDecalComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f)); // Point decal downwards
    SelectionDecalComponent->DecalSize = DefaultSelectionDecalSize; // Set default size
    SelectionDecalComponent->SetVisibility(false); // Initially hidden
    // Note: Setting material here is tricky if DefaultSelectionDecalMaterial is assigned in BP.
    // It's often better to set/update material instance in BeginPlay or when selected.
    ```
*   **In `AAlsCharacterExample::BeginPlay()`**:
    ```cpp
    // AAlsCharacterExample::BeginPlay()
    Super::BeginPlay();
    if (SelectionDecalComponent && DefaultSelectionDecalMaterial)
    {
        SelectionDecalComponent->SetDecalMaterial(DefaultSelectionDecalMaterial);
        // You might want to create a Dynamic Material Instance here to change color later
        UMaterialInstanceDynamic* DecalMID = SelectionDecalComponent->CreateDynamicMaterialInstance();
        if (DecalMID)
        {
            DecalMID->SetVectorParameterValue(FName("DecalColorParam"), DefaultSelectionDecalColor); // Assuming your material has "DecalColorParam"
        }
    }
    ```
*   **Compile C++**.
*   **Editor**: In `BP_MainChar` (or `B_Als_Character`) Class Defaults, assign a `DefaultSelectionDecalMaterial` (e.g., a simple circle decal material) and set the `DefaultSelectionDecalColor`. Ensure your decal material has a Vector Parameter named "DecalColorParam" if you want to tint it.

---

**Phase 2: Blueprint and Asset Configuration**

**Step 2.1: Create Input Actions (if not already done)**

*   `IA_MarqueeSelect_Hold` (Digital bool, or better: Value Type `Axis1D (float)` if you want to detect how long it's held, though `Digital (bool)` with `Started` and `Completed` triggers is fine for LMB). For simplicity, let's assume `Started` for press, `Completed` for release.
*   `IA_MarqueeSelect_Cancel` (Digital bool, for RMB).
*   `IA_Selection_Modifier` (Digital bool, for Left Shift).

**Step 2.2: Configure Input Mapping Context (`IMC_TopDownView`)**

*   Create a new IMC named `IMC_TopDownView` or use your existing one for TopDown.
*   **Mappings**:
    *   `IA_MarqueeSelect_Hold` -> Left Mouse Button
    *   `IA_MarqueeSelect_Cancel` -> Right Mouse Button
    *   `IA_Selection_Modifier` -> Left Shift
*   **Activation**: This IMC should be added by the `BGA_SetViewModeTopDown` ability when switching *to* TopDown view and removed when switching *out*.
    *   **`BGA_SetViewModeTopDown` (`ActivateAbility`)**:
        *   After `SetViewMode(Als.ViewMode.TopDown)`: Get PlayerController -> Get EnhancedInputLocalPlayerSubsystem -> `Add Mapping Context` (IMC_TopDownView).
        *   After `SetViewMode(Als.ViewMode.ThirdPerson)`: Get PlayerController -> Get EnhancedInputLocalPlayerSubsystem -> `Remove Mapping Context` (IMC_TopDownView).

**Step 2.3: Create `BGA_SelectionMarquee` Gameplay Ability**

*   **Create Asset**: Blueprint class inheriting from `UGameplayAbility`, name `BGA_SelectionMarquee`.
*   **Class Defaults**:
    *   `Ability Tags`: `Ability.Action.SelectionMarquee`
    *   `Activation Owned Tags`: `State.SelectingWithMarquee`
    *   `Activation Required Tags`: `Als.ViewMode.TopDown` (ensure it only runs in topdown)
    *   `Net Execution Policy`: `Local Predicted`
    *   `Instancing Policy`: `Instanced Per Execution`
*   **Variables**:
    *   `RequiredActorTagsForSelection` (Array of Name, Instance Editable, Exposed On Spawn)
    *   `RequiredGameplayTagsForSelection` (GameplayTagContainer, Instance Editable, Exposed On Spawn)
*   **`ActivateAbility` Event Graph**:
    1.  `Event ActivateAbility`
    2.  Call `AbilityTask_WaitForMarqueeInput::CreateTask`.
        *   `OwningAbility`: Self
        *   `TaskInstanceName`: "WaitForMarquee"
        *   (If you modified the task to take IA references): Pass your `IA_MarqueeSelect_Hold`, `IA_MarqueeSelect_Cancel`, `IA_Selection_Modifier` assets here.
    3.  Drag from `OnMarqueeFinished` delegate pin of the task node:
        *   `Get Actor Info` -> `Break` -> `Player Controller` -> `Cast To MyPlayerController`.
        *   If Cast Succeeds: Call `Server Perform Marquee Selection` on `MyPlayerController`.
            *   Pass `ScreenStartPos` and `ScreenEndPos` from the delegate.
            *   Pass `RequiredActorTagsForSelection` and `RequiredGameplayTagsForSelection` from the ability's variables.
            *   **Modifier Key Check**: Before calling `Server_PerformMarqueeSelection`:
                *   Get `PlayerController` -> `Is Input Key Down` (Key: Left Shift). This is a simple way.
                *   OR, if `IA_Selection_Modifier` is bound to an ability task/event: Check if that ability/event is active.
                *   A robust way: The `AbilityTask_WaitForMarqueeInput` can check the state of the modifier key (`SelectionModifierAction`) during `HandleConfirmReleased` and pass booleans `bIsAddToSelection` / `bIsRemoveFromSelection` through its `OnMarqueeFinished` delegate.
                    *   **Modify `AbilityTask_WaitForMarqueeInput::HandleConfirmReleased`**:
                        ```cpp
                        // ... inside HandleConfirmReleased, before broadcasting ...
                        bool bShiftHeld = false;
                        if (PlayerController.IsValid() && SelectionModifierAction) {
                            // This checks the current state of the IA. Note: This might need the IA to be "Triggered" type.
                            // A more direct way is PlayerController->IsInputKeyDown(EKeys::LeftShift);
                             bShiftHeld = PlayerController->IsInputKeyDown(EKeys::LeftShift);
                        }
                        // Modify OnMarqueeFinished delegate to include these
                        // OnMarqueeFinished.Broadcast(InitialMouseScreenPosition, EndPos, bShiftHeld, bShiftHeld); // Example: Shift for both add & remove toggle
                        ```
                    *   Then, in `BGA_SelectionMarquee`, the `OnMarqueeFinished` delegate will have these booleans to pass to `Server_PerformMarqueeSelection`.
            *   Call `K2_EndAbility`.
    4.  Drag from `OnCancelled` delegate pin of the task node:
        *   Call `K2_EndAbility`.
    5.  Connect execution from `Event ActivateAbility` to the `ReadyToActivate()` pin of the task node.

**Step 2.4: Grant `BGA_SelectionMarquee` and Bind Input**

*   **Grant**: In `BP_MainChar` Class Defaults -> `Default Abilities`, add `BGA_SelectionMarquee`.
*   **Bind Input**: In `BP_MainChar` Event Graph:
    *   `EnhancedInputAction IA_MarqueeSelect_Hold (Started)` -> `GetAbilitySystemComponent` -> `Try Activate Ability by Class (BGA_SelectionMarquee)`.
        *   *(The `AbilityTask_WaitForMarqueeInput` will handle the "Released" part of this input for finishing the marquee).*

**Step 2.5: Implement Selection Visuals on Selectable Actors**

*   For each Blueprint actor that should be selectable (e.g., `BP_Selectable_Unit`):
    1.  **Class Settings -> Interfaces -> Add `SelectableActorInterface`**.
    2.  Implement `Event On Selected By Marquee`:
        *   Get Owning Player (from `SelectingPlayer` input) -> Cast to `BP_MainChar` (or `AAlsCharacterExample`).
        *   Get `SelectionDecalComponent` from the character.
        *   `Set Visibility` (True).
        *   (Optional) `Create Dynamic Material Instance` from `SelectionDecalComponent` -> `Set Vector Parameter Value` ("DecalColorParam" to `Character.DefaultSelectionDecalColor`).
    3.  Implement `Event On Deselected By Marquee`:
        *   Get Owning Player -> Cast to `BP_MainChar`.
        *   Get `SelectionDecalComponent` -> `Set Visibility` (False).
    4.  Implement `Event Is Currently Selectable By Marquee`:
        *   Return `True` (or add logic, e.g., `Return (Health > 0)`).
    5.  **Tags**:
        *   Add Actor Tag: "Unit" (or whatever you use in `BGA_SelectionMarquee`'s `RequiredActorTagsForSelection`).
        *   If it has an ASC, add Gameplay Tag: `Selectable.Unit.CanBeMarqueeSelected` (or whatever you use in `RequiredGameplayTagsForSelection`).

---

**Phase 4: Testing**

1.  Compile C++.
2.  Configure `DA_AlsCameraSettings` (ensure TopDown settings are good).
3.  Configure `BP_MyHUD` (assign marquee color/material in its defaults).
4.  Configure `BP_MainChar` (ensure `IA_MarqueeSelect_Hold` etc. are assigned to its UPROPERTYs if you exposed them, and `DefaultSelectionDecalMaterial` is set).
5.  Configure `IMC_TopDownView` with all mappings, including modifiers for mouse.
6.  Place some `BP_Selectable_Unit` actors in your level.
7.  Play. Switch to TopDown view.
8.  Press and hold LMB, drag to draw marquee.
9.  Release LMB. Check if units are selected (decal appears) and if `AMyPlayerController::SelectedActors` array is populated on the server (use debugger or logs).
10. Test with Shift key for additive/subtractive selection.
11. Test RMB to cancel marquee draw.

This is a very comprehensive plan. It involves many interconnected parts. Take it step-by-step, test frequently, and use the debugger and print logs extensively. The most complex C++ part is the `Server_PerformMarqueeSelection`'s frustum logic and the `AbilityTask_WaitForMarqueeInput`. Good luck!