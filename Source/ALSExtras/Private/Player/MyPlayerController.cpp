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

// Function to help build the frustum
bool GetFrustumForMarquee(APlayerController* PC, const FVector2D& ScreenPos1, const FVector2D& ScreenPos2, FConvexVolume& OutFrustum, float FrustumFarDistance = 10000.0f)
{
    UE_LOG(LogTemp, Log, TEXT("GetFrustumForMarquee: Starting frustum creation from screen points (%f,%f) to (%f,%f)"), ScreenPos1.X, ScreenPos1.Y, ScreenPos2.X, ScreenPos2.Y);
    if (!PC || !PC->PlayerCameraManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetFrustumForMarquee: Invalid PlayerController or PlayerCameraManager"));
        return false;
    }

    FVector CamLocation = PC->PlayerCameraManager->GetCameraLocation();
    FRotator CamRotation = PC->PlayerCameraManager->GetCameraRotation();
    float CamFOV = PC->PlayerCameraManager->GetFOVAngle(); // Get the actual FOV

    int32 ViewportSizeX, ViewportSizeY;
    PC->GetViewportSize(ViewportSizeX, ViewportSizeY);
    UE_LOG(LogTemp, Log, TEXT("GetFrustumForMarquee: Viewport size: %d x %d"), ViewportSizeX, ViewportSizeY);
    if (ViewportSizeX == 0 || ViewportSizeY == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetFrustumForMarquee: Invalid viewport size"));
        return false;
    }

    // Ensure Min is actually Min, Max is Max
    FVector2D MarqueeMin(FMath::Min(ScreenPos1.X, ScreenPos2.X), FMath::Min(ScreenPos1.Y, ScreenPos2.Y));
    FVector2D MarqueeMax(FMath::Max(ScreenPos1.X, ScreenPos2.X), FMath::Max(ScreenPos1.Y, ScreenPos2.Y));

    // Create points for the near plane of the frustum
    FVector WorldPos_NearTopLeft, WorldDir_NearTopLeft;
    FVector WorldPos_NearTopRight, WorldDir_NearTopRight;
    FVector WorldPos_NearBottomLeft, WorldDir_NearBottomLeft;
    FVector WorldPos_NearBottomRight, WorldDir_NearBottomRight;

    bool bSuccess = true;
    bSuccess &= UGameplayStatics::DeprojectScreenToWorld(PC, MarqueeMin, WorldPos_NearTopLeft, WorldDir_NearTopLeft);
    bSuccess &= UGameplayStatics::DeprojectScreenToWorld(PC, FVector2D(MarqueeMax.X, MarqueeMin.Y), WorldPos_NearTopRight, WorldDir_NearTopRight);
    bSuccess &= UGameplayStatics::DeprojectScreenToWorld(PC, FVector2D(MarqueeMin.X, MarqueeMax.Y), WorldPos_NearBottomLeft, WorldDir_NearBottomLeft);
    bSuccess &= UGameplayStatics::DeprojectScreenToWorld(PC, MarqueeMax, WorldPos_NearBottomRight, WorldDir_NearBottomRight);

    if (!bSuccess)
    {
        UE_LOG(LogTemp, Warning, TEXT("GetFrustumForMarquee: Failed to deproject screen points to world"));
        return false;
    }
    UE_LOG(LogTemp, Log, TEXT("GetFrustumForMarquee: Successfully deprojected screen points to world rays"));

    // Define the 8 points of the frustum
    // Near plane points are essentially the camera location because deprojection gives a ray
    FVector NearPlanePoints[4];
    NearPlanePoints[0] = WorldPos_NearTopLeft;     // Top-Left
    NearPlanePoints[1] = WorldPos_NearTopRight;    // Top-Right
    NearPlanePoints[2] = WorldPos_NearBottomRight; // Bottom-Right
    NearPlanePoints[3] = WorldPos_NearBottomLeft;  // Bottom-Left

    // Far plane points
    FVector FarPlanePoints[4];
    FarPlanePoints[0] = WorldPos_NearTopLeft + WorldDir_NearTopLeft * FrustumFarDistance;
    FarPlanePoints[1] = WorldPos_NearTopRight + WorldDir_NearTopRight * FrustumFarDistance;
    FarPlanePoints[2] = WorldPos_NearBottomRight + WorldDir_NearBottomRight * FrustumFarDistance;
    FarPlanePoints[3] = WorldPos_NearBottomLeft + WorldDir_NearBottomLeft * FrustumFarDistance;
    
    // Build the planes of the frustum
    // Order: Near, Far, Right, Left, Top, Bottom
    OutFrustum.Planes.Empty();
    OutFrustum.Planes.Add(FPlane(NearPlanePoints[2], NearPlanePoints[1], NearPlanePoints[0]));      // Near
    OutFrustum.Planes.Add(FPlane(FarPlanePoints[0], FarPlanePoints[1], FarPlanePoints[2]));          // Far
    OutFrustum.Planes.Add(FPlane(NearPlanePoints[1], NearPlanePoints[2], FarPlanePoints[2]));        // Right
    OutFrustum.Planes.Add(FPlane(NearPlanePoints[3], NearPlanePoints[0], FarPlanePoints[0]));        // Left
    OutFrustum.Planes.Add(FPlane(NearPlanePoints[0], NearPlanePoints[1], FarPlanePoints[1]));        // Top
    OutFrustum.Planes.Add(FPlane(NearPlanePoints[2], NearPlanePoints[3], FarPlanePoints[3]));        // Bottom
    OutFrustum.Init(); // Finalizes plane normals
    
    UE_LOG(LogTemp, Log, TEXT("GetFrustumForMarquee: Successfully created frustum with %d planes"), OutFrustum.Planes.Num());
    return true;
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
        MarqueeCurrentScreenPosition = ScreenPosition;
        UE_LOG(LogTemp, Verbose, TEXT("Client_UpdateMarquee: Updated to position (%f,%f), size: (%f,%f)"), 
            ScreenPosition.X, ScreenPosition.Y, 
            FMath::Abs(MarqueeCurrentScreenPosition.X - MarqueeStartScreenPosition.X),
            FMath::Abs(MarqueeCurrentScreenPosition.Y - MarqueeStartScreenPosition.Y));
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
                                                              bool bAddToSelection, bool bRemoveFromSelection)
{
    return true;
}

void AMyPlayerController::Server_PerformMarqueeSelection_Implementation(FVector2D ScreenStart, FVector2D ScreenEnd,
                                                                      const TArray<FName>& RequiredActorTags,
                                                                      const FGameplayTagContainer& RequiredGameplayTags,
                                                                      bool bAddToSelection, bool bRemoveFromSelection)
{
    UE_LOG(LogTemp, Log, TEXT("Server_PerformMarqueeSelection: Start=(%f,%f), End=(%f,%f), AddToSelection=%d, RemoveFromSelection=%d"),
        ScreenStart.X, ScreenStart.Y, ScreenEnd.X, ScreenEnd.Y, bAddToSelection, bRemoveFromSelection);
    
    if (RequiredActorTags.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Server_PerformMarqueeSelection: Filtering by %d actor tags"), RequiredActorTags.Num());
    }
    
    if (!RequiredGameplayTags.IsEmpty())
    {
        UE_LOG(LogTemp, Log, TEXT("Server_PerformMarqueeSelection: Filtering by gameplay tags: %s"), *RequiredGameplayTags.ToStringSimple());
    }
    FVector2D RectMin = FVector2D(FMath::Min(ScreenStart.X, ScreenEnd.X), FMath::Min(ScreenStart.Y, ScreenEnd.Y));
    FVector2D RectMax = FVector2D(FMath::Max(ScreenStart.X, ScreenEnd.X), FMath::Max(ScreenStart.Y, ScreenEnd.Y));
    
    TArray<AActor*> ActorsInFrustum;
    FConvexVolume SelectionFrustum;

    // Build the frustum based on the screen rectangle
    UE_LOG(LogTemp, Log, TEXT("Server_PerformMarqueeSelection: Building frustum with RectMin=(%f,%f), RectMax=(%f,%f)"),
        RectMin.X, RectMin.Y, RectMax.X, RectMax.Y);
        
    // Declare counter outside the if block so it's accessible later
    int32 TotalActorsChecked = 0;
        
    if (GetFrustumForMarquee(this, RectMin, RectMax, SelectionFrustum))
    {
        UE_LOG(LogTemp, Log, TEXT("Server_PerformMarqueeSelection: Iterating through world actors to find those in frustum"));
        
        for (TActorIterator<AActor> It(GetWorld()); It; ++It)
        {
            AActor* Actor = *It;
            TotalActorsChecked++;
            
            if (!Actor || !Actor->Implements<USelectableActorInterface>()) // Early out if not selectable interface
            {
                continue;
            }
            
            UE_LOG(LogTemp, Verbose, TEXT("Checking selectable actor: %s"), *Actor->GetName());

            // Check if actor's bounding box intersects with the frustum
            FVector Origin, BoxExtent;
            Actor->GetActorBounds(false, Origin, BoxExtent, false); // false for only collidable components

            if (SelectionFrustum.IntersectBox(Origin, BoxExtent))
            {
                UE_LOG(LogTemp, Log, TEXT("Actor in frustum: %s at location (%f,%f,%f) with extent (%f,%f,%f)"), 
                    *Actor->GetName(), Origin.X, Origin.Y, Origin.Z, BoxExtent.X, BoxExtent.Y, BoxExtent.Z);
                ActorsInFrustum.Add(Actor);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Server_PerformMarqueeSelection: Failed to generate selection frustum."));
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Server_PerformMarqueeSelection: Found %d actors in frustum (checked %d total actors)"), ActorsInFrustum.Num(), TotalActorsChecked);

    TArray<AActor*> NewlySelectedActors;
    TArray<AActor*> ActorsToDeselect;

    if (!bAddToSelection && !bRemoveFromSelection) 
    {
        UE_LOG(LogTemp, Log, TEXT("Server_PerformMarqueeSelection: Standard selection mode (clearing previous selection)"));
        ClearCurrentSelection(); 
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Server_PerformMarqueeSelection: Modified selection mode (Add=%d, Remove=%d)"), bAddToSelection, bRemoveFromSelection);
    }

    for (AActor* Actor : ActorsInFrustum)
    {
        if (!IsValid(Actor)) continue;

        bool bPassesActorTagCheck = RequiredActorTags.IsEmpty(); 
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
            bPassesActorTagCheck = (MatchCount == RequiredActorTags.Num());
        }
        if (!bPassesActorTagCheck) continue;

        bool bPassesGameplayTagCheck = RequiredGameplayTags.IsEmpty(); 
        if (!bPassesGameplayTagCheck)
        {
            IAbilitySystemInterface* AbilityInterface = Cast<IAbilitySystemInterface>(Actor);
            if (AbilityInterface && AbilityInterface->GetAbilitySystemComponent())
            {
                bPassesGameplayTagCheck = AbilityInterface->GetAbilitySystemComponent()->HasAllMatchingGameplayTags(RequiredGameplayTags);
            }
            else
            {
                bPassesGameplayTagCheck = false; 
            }
        }
        if (!bPassesGameplayTagCheck) continue;

        ISelectableActorInterface* Selectable = Cast<ISelectableActorInterface>(Actor);
        if (Selectable && Selectable->Execute_IsCurrentlySelectableByMarquee(Actor, this))
        {
            bool bIsAlreadySelected = SelectedActors.Contains(Actor);

            if (bRemoveFromSelection) // If Shift is held, and it's selected, prepare to deselect
            {
                if (bIsAlreadySelected)
                {
                    ActorsToDeselect.Add(Actor);
                }
            }
            else if (bAddToSelection) // If Shift is held, and it's NOT selected, prepare to select
            {
                if (!bIsAlreadySelected)
                {
                    NewlySelectedActors.Add(Actor);
                }
            }
            else // Not holding Shift (standard selection)
            {
                NewlySelectedActors.Add(Actor); // Will be added to the (now empty) SelectedActors list
            }
        }
    }

    // Process Deselections
    UE_LOG(LogTemp, Log, TEXT("Server_PerformMarqueeSelection: Deselecting %d actors"), ActorsToDeselect.Num());
    for (AActor* ActorToDeselect : ActorsToDeselect)
    {
        if (ISelectableActorInterface* Selectable = Cast<ISelectableActorInterface>(ActorToDeselect))
        {
            UE_LOG(LogTemp, Log, TEXT("Deselecting actor: %s"), *ActorToDeselect->GetName());
            Selectable->Execute_OnDeselectedByMarquee(ActorToDeselect, this);
        }
        SelectedActors.Remove(ActorToDeselect);
    }

    // Process New Selections
    UE_LOG(LogTemp, Log, TEXT("Server_PerformMarqueeSelection: Selecting %d new actors"), NewlySelectedActors.Num());
    for (AActor* ActorToSelect : NewlySelectedActors)
    {
        if (ISelectableActorInterface* Selectable = Cast<ISelectableActorInterface>(ActorToSelect))
        {
            UE_LOG(LogTemp, Log, TEXT("Selecting actor: %s"), *ActorToSelect->GetName());
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
