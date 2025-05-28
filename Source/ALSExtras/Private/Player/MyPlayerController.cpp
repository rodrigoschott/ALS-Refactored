#include "Player/MyPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/SelectableActorInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Pawn.h"
#include "SceneView.h"
#include "DrawDebugHelpers.h"
#include "Engine/LocalPlayer.h"  // For ULocalPlayer
#include "Camera/PlayerCameraManager.h" // Ensure this is included
#include "UI/MyHUD.h"  // Include for custom HUD class

// Helper function to get essential view data
bool GetPlayerViewInformation(APlayerController* PC, FVector& OutViewOrigin, FRotator& OutViewRotation, FMatrix& OutViewMatrix, FMatrix& OutProjectionMatrix, FIntRect& OutViewRect)
{
    if (!PC || !PC->PlayerCameraManager || !PC->GetLocalPlayer() || !PC->GetLocalPlayer()->ViewportClient || !PC->GetLocalPlayer()->ViewportClient->Viewport)
    {
        return false;
    }

    PC->GetPlayerViewPoint(OutViewOrigin, OutViewRotation);

    OutViewMatrix = FTranslationMatrix(-OutViewOrigin) * FInverseRotationMatrix(OutViewRotation);

    const FIntPoint ViewportSize = PC->GetLocalPlayer()->ViewportClient->Viewport->GetSizeXY();
    OutViewRect = FIntRect(0, 0, ViewportSize.X, ViewportSize.Y);

    float FOVDegrees = PC->PlayerCameraManager->GetFOVAngle();
    float AspectRatio = (ViewportSize.X > 0 && ViewportSize.Y > 0) ? (float)ViewportSize.X / (float)ViewportSize.Y : 1.0f;
    float NearClip = 10.0f; // Use a fixed value since GetNearClipPlane is unavailable

    OutProjectionMatrix = FReversedZPerspectiveMatrix(
        FOVDegrees * 0.5f,
        AspectRatio,
        1.0f,
        1.0f,
        NearClip
    );
    return true;
}

// Helper function to get the view projection data
bool GetViewProjectionData(APlayerController* PC, FSceneViewProjectionData& OutProjectionData)
{
    if (!PC) return false;
    
    if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
    {
        if (LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
        {
            // Calculate the view matrix
            FVector ViewLocation;
            FRotator ViewRotation;
            PC->GetPlayerViewPoint(ViewLocation, ViewRotation); // Use the actual player view point

            // Get the camera properties directly from the PlayerCameraManager
            FVector CameraLocation = PC->PlayerCameraManager ? PC->PlayerCameraManager->GetCameraLocation() : ViewLocation;
            FRotator CameraRotation = PC->PlayerCameraManager ? PC->PlayerCameraManager->GetCameraRotation() : ViewRotation;
            float FOVDegrees = PC->PlayerCameraManager ? PC->PlayerCameraManager->GetFOVAngle() : 90.0f;

            // Construct FSceneViewProjectionData
            const FIntRect ViewRect(0, 0, LocalPlayer->ViewportClient->Viewport->GetSizeXY().X, LocalPlayer->ViewportClient->Viewport->GetSizeXY().Y);

            FMatrix ViewMatrix = FTranslationMatrix(-ViewLocation) * FInverseRotationMatrix(ViewRotation);
            FMatrix ProjectionMatrix;

            // Calculate a projection matrix for the main view
            float AspectRatio = (ViewRect.Width() > 0 && ViewRect.Height() > 0) ? (float)ViewRect.Width() / (float)ViewRect.Height() : 1.0f;
            float NearClip = 10.0f; // Standard near clip plane

            ProjectionMatrix = FReversedZPerspectiveMatrix(
                FOVDegrees * 0.5f, // Half FOV in degrees
                AspectRatio,
                1.0f,      // X axis multiplier
                1.0f,      // Y axis multiplier
                NearClip
            );
            
            OutProjectionData.ViewOrigin = ViewLocation;
            OutProjectionData.ViewRotationMatrix = FInverseRotationMatrix(ViewRotation) * FMatrix(
                FPlane(0, 0, 1, 0),
                FPlane(1, 0, 0, 0),
                FPlane(0, 1, 0, 0),
                FPlane(0, 0, 0, 1));
            OutProjectionData.ProjectionMatrix = ProjectionMatrix;
            OutProjectionData.ViewRect = ViewRect;

            return true;
        }
    }
    return false;
}



AMyPlayerController::AMyPlayerController()
{
    bIsDrawingMarquee = false;
}

void AMyPlayerController::Client_BeginMarquee(FVector2D ScreenPosition)
{
    UE_LOG(LogTemp, Log, TEXT("Client_BeginMarquee: Starting marquee at position (%f,%f)"), ScreenPosition.X, ScreenPosition.Y);
    bIsDrawingMarquee = true;
    MarqueeStartScreenPosition = ScreenPosition;
    MarqueeCurrentScreenPosition = ScreenPosition;
}

void AMyPlayerController::Client_UpdateMarquee(FVector2D ScreenPosition)
{
    if (bIsDrawingMarquee)
    {
        // Only log if the position actually changes to reduce spam
        if (!MarqueeCurrentScreenPosition.Equals(ScreenPosition, 0.1f)) 
        {
            UE_LOG(LogTemp, Log, TEXT("PCON Client_UpdateMarquee: OldCurrentPos: %s, NewScreenPosition: %s. bIsDrawingMarquee: true"),
                *MarqueeCurrentScreenPosition.ToString(), *ScreenPosition.ToString());
        }
        MarqueeCurrentScreenPosition = ScreenPosition;
    }
    else
    {
        // This should ideally not happen if TickTask only calls when bIsMouseDown is true
        // and bIsMouseDown is only true when bIsDrawingMarquee was set to true.
        UE_LOG(LogTemp, Warning, TEXT("PCON Client_UpdateMarquee: Called but bIsDrawingMarquee is FALSE!"));
    }
}

void AMyPlayerController::Client_EndMarquee()
{
    UE_LOG(LogTemp, Log, TEXT("Client_EndMarquee: Ending marquee from (%f,%f) to (%f,%f)"), 
        MarqueeStartScreenPosition.X, MarqueeStartScreenPosition.Y,
        MarqueeCurrentScreenPosition.X, MarqueeCurrentScreenPosition.Y);
    bIsDrawingMarquee = false;
}

void AMyPlayerController::ClearCurrentSelection()
{
    UE_LOG(LogTemp, Log, TEXT("ClearCurrentSelection: Clearing %d selected actors"), SelectedActors.Num());
    // Tell previously selected actors they are deselected
    for (TWeakObjectPtr<AActor> PreviouslySelectedActorPtr : SelectedActors)
    {
        if (AActor* PreviouslySelectedActor = PreviouslySelectedActorPtr.Get())
        {
            // MODIFIED: Use Implements<> check and Execute_ for Blueprint-implemented interfaces
            if (PreviouslySelectedActor->Implements<USelectableActorInterface>())
            {
                ISelectableActorInterface::Execute_OnDeselectedByMarquee(PreviouslySelectedActor, this);
            }
        }
    }
    SelectedActors.Empty();
}

bool AMyPlayerController::Server_PerformMarqueeSelection_Validate(FVector2D ScreenStart, FVector2D ScreenEnd,
                                                              const TArray<FName>& RequiredActorTags,
                                                              const FGameplayTagContainer& RequiredGameplayTags,
                                                              bool bAddToSelection, bool bRemoveFromSelection)
{
    return true;
}

void AMyPlayerController::Server_PerformMarqueeSelection_Implementation(FVector2D ScreenStart, FVector2D ScreenEnd,
                                                                      const TArray<FName>& RequiredActorTags,
                                                                      const FGameplayTagContainer& RequiredGameplayTags,
                                                                      bool bAddToSelection, bool bRemoveFromSelection)
{
    UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Server_PerformMarqueeSelection (2D Method) - Start:(%s), End:(%s), Add:%d, Remove:%d"),
        *ScreenStart.ToString(), *ScreenEnd.ToString(), bAddToSelection, bRemoveFromSelection);

    // Use the 2D screen-space projection method to find actors in the marquee box
    TArray<AActor*> ActorsFoundInMarquee;
    if (!Perform2DScreenSpaceSelection(ScreenStart, ScreenEnd, ActorsFoundInMarquee))
    {
        // If the box has no area or selection failed, handle appropriately
        UE_LOG(LogTemp, Warning, TEXT("SERVER PCON: 2D screen-space selection failed or found no actors geometrically."));
        if (!bAddToSelection && !bRemoveFromSelection) ClearCurrentSelection();
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Geometrically found %d actors in marquee."), ActorsFoundInMarquee.Num());

    // Check if tag filtering is required
    bool bActorTagsRequired = !RequiredActorTags.IsEmpty();
    bool bGameplayTagsRequired = !RequiredGameplayTags.IsEmpty();
    
    if (bGameplayTagsRequired)
    {
        UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Filtering by gameplay tags: %s"), *RequiredGameplayTags.ToStringSimple());
    }
    if (bActorTagsRequired)
    {
        UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Filtering by actor tags: %d tags specified"), RequiredActorTags.Num());
    }

    TArray<AActor*> FilteredSelectableActors; // Actors that pass all criteria

    for (AActor* Actor : ActorsFoundInMarquee)
    {
        if (!IsValid(Actor))
        {
            UE_LOG(LogTemp, Warning, TEXT("SERVER PCON: Actor in ActorsFoundInMarquee is !IsValid. Skipping."));
            continue;
        }

        UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Processing Actor for filtering: %s"), *Actor->GetName());

        // Step 1: Check the Interface (This is primary)
        // MODIFIED: Use Implements<> check and Execute_ for Blueprint-implemented interfaces
        if (!Actor->Implements<USelectableActorInterface>())
        {
            UE_LOG(LogTemp, Warning, TEXT("SERVER PCON: Actor %s - Does NOT implement ISelectableActorInterface. Skipping."), *Actor->GetName());
            continue;
        }

        bool bIsCurrentlySelectableByInterface = ISelectableActorInterface::Execute_IsCurrentlySelectableByMarquee(Actor, this);
        UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Actor %s - IsCurrentlySelectableByMarquee (Interface): %s"), *Actor->GetName(), bIsCurrentlySelectableByInterface ? TEXT("TRUE") : TEXT("FALSE"));
        if (!bIsCurrentlySelectableByInterface)
        {
            UE_LOG(LogTemp, Warning, TEXT("SERVER PCON: Actor %s - IsCurrentlySelectableByMarquee returned FALSE. Skipping."), *Actor->GetName());
            continue;
        }

        // Step 2: Check Actor Tags OR Gameplay Tags (if specified in ability)
        bool bPassedTagChecks = false;

        if (!bActorTagsRequired && !bGameplayTagsRequired) 
        {
            // If NO tags are required by the ability, it passes the tag check part by default
            bPassedTagChecks = true;
            UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Actor %s - No specific tags required by ability. Passed tag check part."), *Actor->GetName());
        }
        else
        {
            // Check Actor Tags
            bool bPassesActorTagCheck = false;
            if (bActorTagsRequired)
            {
                int32 MatchCount = 0;
                for (const FName& RequiredTag : RequiredActorTags)
                {
                    bool bHasTag = Actor->ActorHasTag(RequiredTag);
                    UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Actor %s - Has actor tag '%s': %s"), *Actor->GetName(), *RequiredTag.ToString(), bHasTag ? TEXT("TRUE") : TEXT("FALSE"));
                    if (bHasTag)
                    {
                        MatchCount++;
                    }
                }
                bPassesActorTagCheck = (MatchCount == RequiredActorTags.Num()); // Must have ALL specified actor tags
                UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Actor %s - Actor Tag Check (Needed %d tags): %s"), 
                    *Actor->GetName(), RequiredActorTags.Num(), bPassesActorTagCheck ? TEXT("TRUE") : TEXT("FALSE"));
            }

            // Check Gameplay Tags (only if actor has ASC)
            bool bPassesGameplayTagCheck = false;
            if (bGameplayTagsRequired)
            {
                IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(Actor);
                if (ASI && ASI->GetAbilitySystemComponent())
                {
                    bPassesGameplayTagCheck = ASI->GetAbilitySystemComponent()->HasAllMatchingGameplayTags(RequiredGameplayTags);
                    UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Actor %s - Gameplay Tag Check (Required: %s): %s"), 
                        *Actor->GetName(), 
                        *RequiredGameplayTags.ToStringSimple(),
                        bPassesGameplayTagCheck ? TEXT("TRUE") : TEXT("FALSE"));
                }
                else
                {
                    // Adding more detailed log for ASC issue
                    if (!ASI)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("SERVER PCON: Actor %s - Cast to IAbilitySystemInterface failed. No ASC for GameplayTag check."), *Actor->GetName());
                    }
                    else // ASI is valid, but GetAbilitySystemComponent() is null
                    {
                        UE_LOG(LogTemp, Warning, TEXT("SERVER PCON: Actor %s - IAbilitySystemInterface valid, but GetAbilitySystemComponent() returned null. No ASC for GameplayTag check."), *Actor->GetName());
                    }
                    bPassesGameplayTagCheck = false; // Cannot pass if no ASC and gameplay tags are required
                }
            }

            // Combine with OR logic if both types of tags could qualify it
            if (bActorTagsRequired && bGameplayTagsRequired)
            {
                bPassedTagChecks = bPassesActorTagCheck || bPassesGameplayTagCheck; // OR logic
                UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Actor %s - Combined Tag Check (Actor OR Gameplay): %s"), *Actor->GetName(), bPassedTagChecks ? TEXT("TRUE") : TEXT("FALSE"));
            }
            else if (bActorTagsRequired)
            {
                bPassedTagChecks = bPassesActorTagCheck;
            }
            else if (bGameplayTagsRequired)
            {
                bPassedTagChecks = bPassesGameplayTagCheck;
            }
        }
        
        if (bPassedTagChecks) // Already passed interface check
        {
            UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Actor %s PASSED ALL FILTERS. Adding to potential selection."), *Actor->GetName());
            FilteredSelectableActors.Add(Actor);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("SERVER PCON: Actor %s FAILED tag checks. Skipping."), *Actor->GetName());
        }
    }
    UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Found %d actors after ALL filtering."), FilteredSelectableActors.Num());

    // --- Selection processing logic (using FilteredSelectableActors) ---
    TArray<AActor*> NewlySelectedActors;
    TArray<AActor*> ActorsToDeselect;

    if (!bAddToSelection && !bRemoveFromSelection) 
    {
        UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Standard selection: Clearing previous selection."));
        ClearCurrentSelection(); 
        // All actors in FilteredSelectableActors are new selections
        NewlySelectedActors = FilteredSelectableActors;
    }
    else // Modifier key (Shift) is held - toggle or add/remove logic
    {
        UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Modifier selection: AddTo=%d, RemoveFrom=%d"), bAddToSelection, bRemoveFromSelection);
        for (AActor* FilteredActor : FilteredSelectableActors)
        {
            bool bIsAlreadySelected = SelectedActors.Contains(FilteredActor);
            if (bRemoveFromSelection) // If Shift means toggle (and it's currently selected)
            {
                if (bIsAlreadySelected)
                {
                    UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Actor %s marked for DESELECTION (Shift toggle)."), *FilteredActor->GetName());
                    ActorsToDeselect.Add(FilteredActor);
                }
                else if (bAddToSelection) // If Shift means toggle (and it's NOT currently selected)
                {
                    UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Actor %s marked for ADDITION to selection (Shift toggle)."), *FilteredActor->GetName());
                    NewlySelectedActors.Add(FilteredActor);
                }
            }
            else if (bAddToSelection) // If only adding (e.g., Shift + Left Click on empty space then drag)
            {
                if (!bIsAlreadySelected)
                {
                    UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Actor %s marked for ADDITION to selection."), *FilteredActor->GetName());
                    NewlySelectedActors.Add(FilteredActor);
                }
            }
        }
    }
    
    // Process Deselections
    UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Processing %d actors for deselection"), ActorsToDeselect.Num());
    for (AActor* ActorToDeselect : ActorsToDeselect) 
    { 
        if (IsValid(ActorToDeselect))
        {
            SelectedActors.Remove(ActorToDeselect);
            // MODIFIED: Use Implements<> check and Execute_ for Blueprint-implemented interfaces
            if (ActorToDeselect->Implements<USelectableActorInterface>())
            {
                ISelectableActorInterface::Execute_OnDeselectedByMarquee(ActorToDeselect, this);
                UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Deselected %s"), *ActorToDeselect->GetName());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("SERVER PCON: Failed to deselect %s - Selectable interface not available (after IsValid)"), *ActorToDeselect->GetName());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("SERVER PCON: Invalid actor in ActorsToDeselect list"));
        }
    }
    
    // Process New Selections
    UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Processing %d actors for selection"), NewlySelectedActors.Num());
    for (AActor* ActorToSelect : NewlySelectedActors) 
    { 
        if (IsValid(ActorToSelect))
        {
            UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Adding %s to selected actors array"), *ActorToSelect->GetName());
            SelectedActors.AddUnique(ActorToSelect);
            // MODIFIED: Use Implements<> check and Execute_ for Blueprint-implemented interfaces
            if (ActorToSelect->Implements<USelectableActorInterface>())
            {
                UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Calling OnSelectedByMarquee for %s"), *ActorToSelect->GetName());
                ISelectableActorInterface::Execute_OnSelectedByMarquee(ActorToSelect, this);
                UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Selected %s"), *ActorToSelect->GetName());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("SERVER PCON: Failed to select %s - Selectable interface not available (after IsValid)"), *ActorToSelect->GetName());
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("SERVER PCON: Invalid actor in NewlySelectedActors list"));
        }
    }
    
    // Log final selection state
    UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Final selection count: %d"), SelectedActors.Num());
    if (SelectedActors.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("SERVER PCON: Selected actors list:"));
        for (const TWeakObjectPtr<AActor>& SelectedActor : SelectedActors)
        {
            if (SelectedActor.IsValid())
            {
                UE_LOG(LogTemp, Log, TEXT("  - %s"), *SelectedActor->GetName());
            }
        }
    }
}

//====================================================================================
// 2D SCREEN-SPACE PROJECTION SELECTION METHOD
//====================================================================================

bool AMyPlayerController::Perform2DScreenSpaceSelection(FVector2D ScreenStart, FVector2D ScreenEnd, TArray<AActor*>& OutActorsInBox)
{
    UE_LOG(LogTemp, Log, TEXT("Perform2DScreenSpaceSelection: Start:(%s), End:(%s)"),
        *ScreenStart.ToString(), *ScreenEnd.ToString());

    // Define the 2D screen rectangle from the input points
    const FVector2D MarqueeTopLeft(FMath::Min(ScreenStart.X, ScreenEnd.X), FMath::Min(ScreenStart.Y, ScreenEnd.Y));
    const FVector2D MarqueeBottomRight(FMath::Max(ScreenStart.X, ScreenEnd.X), FMath::Max(ScreenStart.Y, ScreenEnd.Y));

    // If the box has no area, don't select anything
    if ((MarqueeBottomRight - MarqueeTopLeft).SizeSquared() <= KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogTemp, Log, TEXT("Perform2DScreenSpaceSelection: Marquee selection box has zero area."));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("Perform2DScreenSpaceSelection: Marquee Box Min:(%s), Max:(%s)"), 
           *MarqueeTopLeft.ToString(), *MarqueeBottomRight.ToString());

    OutActorsInBox.Empty();
    
    // Use manual screen-space projection checking
    UWorld* World = this->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("Perform2DScreenSpaceSelection: Invalid World."));
        return false;
    }

    int32 TotalActorsChecked = 0;
    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        TotalActorsChecked++;
        
        // This check already uses Implements<USelectableActorInterface>() correctly
        if (!IsValid(Actor) || !Actor->Implements<USelectableActorInterface>() || Actor == GetPawn()) // Skip self
        {
            continue;
        }
        
        FVector ActorLocation = Actor->GetActorLocation();
        FVector2D ScreenLocation;
        
        if (this->ProjectWorldLocationToScreen(ActorLocation, ScreenLocation, true /*bPlayerViewportRelative*/))
        {
            if (ScreenLocation.X >= MarqueeTopLeft.X && ScreenLocation.X <= MarqueeBottomRight.X &&
                ScreenLocation.Y >= MarqueeTopLeft.Y && ScreenLocation.Y <= MarqueeBottomRight.Y)
            {
                OutActorsInBox.Add(Actor);
                
                if (this->bDrawDebugFrustum)
                {
                    DrawDebugSphere(World, ActorLocation, 25.f, 12, FColor::Green, false, 5.0f);
                    UE_LOG(LogTemp, Log, TEXT("Perform2DScreenSpaceSelection: Actor %s IS INSIDE marquee box."), 
                           *Actor->GetName());
                }
            }
            else if (this->bDrawDebugFrustum)
            {
                // Debug visualization for actors outside selection
                DrawDebugSphere(World, ActorLocation, 25.f, 12, FColor::Red, false, 5.0f);
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Perform2DScreenSpaceSelection: Found %d actors (checked %d total)"), 
           OutActorsInBox.Num(), TotalActorsChecked);
    
    return true;
}


void AMyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPLIFETIME(AMyPlayerController, SelectedActors);
    // SelectedActors is NOT replicated directly from PC to all clients.
    // Server tells individual actors they are selected/deselected.
    // If other clients *needed* to know this PC's selection, you would replicate it.
}


// Function to help build the frustum using view projection matrices - COMMENTED OUT
// This was the original frustum selection method, kept for reference but not used anymore
/*
bool GetFrustumForMarquee(APlayerController* PC, const FVector2D& ScreenPos1, const FVector2D& ScreenPos2, FConvexVolume& OutFrustum, float FrustumFarDistance = 10000.0f)
{
    UE_LOG(LogTemp, Log, TEXT("GetFrustumForMarquee: Input ScreenPos1(%s), ScreenPos2(%s)"), *ScreenPos1.ToString(), *ScreenPos2.ToString());

    if (!PC || !PC->PlayerCameraManager || !PC->GetLocalPlayer() || !PC->GetLocalPlayer()->ViewportClient || !PC->GetLocalPlayer()->ViewportClient->Viewport)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetFrustumForMarquee: Invalid PC, PlayerCameraManager, LocalPlayer, ViewportClient, or Viewport."));
        return false;
    }

    FVector ViewOrigin;
    FRotator ViewRotation;
    PC->GetPlayerViewPoint(ViewOrigin, ViewRotation); // This gets the camera's viewpoint

    const FIntPoint ViewportSize = PC->GetLocalPlayer()->ViewportClient->Viewport->GetSizeXY();
    if (ViewportSize.X == 0 || ViewportSize.Y == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetFrustumForMarquee: Viewport size is zero."));
        return false;
    }

    FVector2D MarqueeMin(FMath::Min(ScreenPos1.X, ScreenPos2.X), FMath::Min(ScreenPos1.Y, ScreenPos2.Y));
    FVector2D MarqueeMax(FMath::Max(ScreenPos1.X, ScreenPos2.X), FMath::Max(ScreenPos1.Y, ScreenPos2.Y));

    UE_LOG(LogTemp, Log, TEXT("GetFrustumForMarquee: Calculated MarqueeMin:(%s), MarqueeMax:(%s)"), *MarqueeMin.ToString(), *MarqueeMax.ToString());

    if (MarqueeMin.X >= MarqueeMax.X - KINDA_SMALL_NUMBER || MarqueeMin.Y >= MarqueeMax.Y - KINDA_SMALL_NUMBER)
    {
        UE_LOG(LogTemp, Log, TEXT("GetFrustumForMarquee: Marquee has zero or negative area."));
        return false;
    }

    // Get Near Clip Plane distance using a fixed value instead of the PlayerCameraManager method
    const float NearClipDistance = 10.0f; // Fixed value to avoid compiler error
    UE_LOG(LogTemp, Log, TEXT("GetFrustumForMarquee: Using NearClipDistance: %f"), NearClipDistance);

    // Deproject the four corners of the MARQUEE to get world space points on the NEAR plane
    FVector NearMarqueeVertices[4]; // 0:TL, 1:TR, 2:BR, 3:BL
    FVector FarMarqueeVertices[4];  // 0:TL, 1:TR, 2:BR, 3:BL
    FVector RayOrigin_Unused, TempRayDir;

    bool bDepSuccess = true;
    // Top-Left corner
    bDepSuccess &= UGameplayStatics::DeprojectScreenToWorld(PC, MarqueeMin, RayOrigin_Unused, TempRayDir);
    TempRayDir.Normalize();
    NearMarqueeVertices[0] = ViewOrigin + TempRayDir * NearClipDistance;
    FarMarqueeVertices[0] = ViewOrigin + TempRayDir * FrustumFarDistance;
    
    // Top-Right corner
    bDepSuccess &= UGameplayStatics::DeprojectScreenToWorld(PC, FVector2D(MarqueeMax.X, MarqueeMin.Y), RayOrigin_Unused, TempRayDir);
    TempRayDir.Normalize();
    NearMarqueeVertices[1] = ViewOrigin + TempRayDir * NearClipDistance;
    FarMarqueeVertices[1] = ViewOrigin + TempRayDir * FrustumFarDistance;
    
    // Bottom-Right corner
    bDepSuccess &= UGameplayStatics::DeprojectScreenToWorld(PC, MarqueeMax, RayOrigin_Unused, TempRayDir);
    TempRayDir.Normalize();
    NearMarqueeVertices[2] = ViewOrigin + TempRayDir * NearClipDistance;
    FarMarqueeVertices[2] = ViewOrigin + TempRayDir * FrustumFarDistance;
    
    // Bottom-Left corner
    bDepSuccess &= UGameplayStatics::DeprojectScreenToWorld(PC, FVector2D(MarqueeMin.X, MarqueeMax.Y), RayOrigin_Unused, TempRayDir);
    TempRayDir.Normalize();
    NearMarqueeVertices[3] = ViewOrigin + TempRayDir * NearClipDistance;
    FarMarqueeVertices[3] = ViewOrigin + TempRayDir * FrustumFarDistance;

    if(!bDepSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetFrustumForMarquee: Deprojection of one or more marquee corners failed."));
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("GetFrustumForMarquee: Near Marquee Vertices: TL=%s, TR=%s, BR=%s, BL=%s"), 
        *NearMarqueeVertices[0].ToString(), *NearMarqueeVertices[1].ToString(), 
        *NearMarqueeVertices[2].ToString(), *NearMarqueeVertices[3].ToString());

    OutFrustum.Planes.Empty(6);

    // 1. Near Plane (Points: NBR, NTR, NTL) - Normal should point INTO frustum (away from camera)
    OutFrustum.Planes.Emplace(NearMarqueeVertices[2], NearMarqueeVertices[1], NearMarqueeVertices[0]);

    // 2. Far Plane (Points: FBR, FTR, FTL) - Normal should point INTO frustum (towards camera)
    OutFrustum.Planes.Emplace(FarMarqueeVertices[2], FarMarqueeVertices[1], FarMarqueeVertices[0]);

    // 3. Left Plane (ViewOrigin, NearTopLeft, NearBottomLeft) - Normal points RIGHT
    OutFrustum.Planes.Emplace(ViewOrigin, NearMarqueeVertices[0], NearMarqueeVertices[3]);
    
    // 4. Right Plane (ViewOrigin, NearBottomRight, NearTopRight) - Normal points LEFT
    OutFrustum.Planes.Emplace(ViewOrigin, NearMarqueeVertices[2], NearMarqueeVertices[1]);

    // 5. Top Plane (ViewOrigin, NearTopRight, NearTopLeft) - Normal points DOWN
    OutFrustum.Planes.Emplace(ViewOrigin, NearMarqueeVertices[1], NearMarqueeVertices[0]);

    // 6. Bottom Plane (ViewOrigin, NearBottomLeft, NearBottomRight) - Normal points UP
    OutFrustum.Planes.Emplace(ViewOrigin, NearMarqueeVertices[3], NearMarqueeVertices[2]);

    OutFrustum.Init(); 

    const char* PlaneNames[] = { "Near", "Far", "Left", "Right", "Top", "Bottom" };
    for (int32 i = 0; i < OutFrustum.Planes.Num(); i++)
    {
        const FPlane& Plane = OutFrustum.Planes[i];
        UE_LOG(LogTemp, Log, TEXT("GetFrustumForMarquee: Final %s Plane Normal: %s, W: %f"), 
            (i < 6) ? ANSI_TO_TCHAR(PlaneNames[i]) : TEXT("Unknown"), 
            *Plane.GetNormal().ToString(), Plane.W);
    }
    
    UE_LOG(LogTemp, Log, TEXT("GetFrustumForMarquee: Created custom marquee frustum with %d planes."), OutFrustum.Planes.Num());
    return OutFrustum.Planes.Num() == 6;
}
*/