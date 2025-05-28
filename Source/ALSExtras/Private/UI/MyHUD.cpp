#include "UI/MyHUD.h"
#include "Player/MyPlayerController.h" // Your player controller
#include "Engine/Canvas.h"    // For DrawRect

void AMyHUD::DrawHUD()
{
    Super::DrawHUD();
    
    UE_LOG(LogTemp, Verbose, TEXT("AMyHUD::DrawHUD: Drawing HUD frame"));

    AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwningPlayerController());
    if (!PC)
    {
        UE_LOG(LogTemp, Verbose, TEXT("AMyHUD::DrawHUD: No MyPlayerController found"));
        return;
    }
    
    if (PC && PC->bIsDrawingMarquee)
    {
        UE_LOG(LogTemp, Log, TEXT("AMyHUD::DrawHUD: Drawing marquee selection rectangle"));
        FVector2D StartPos = PC->MarqueeStartScreenPosition;
        FVector2D CurrentPos = PC->MarqueeCurrentScreenPosition;

        float MinX = FMath::Min(StartPos.X, CurrentPos.X);
        float MinY = FMath::Min(StartPos.Y, CurrentPos.Y);
        float MaxX = FMath::Max(StartPos.X, CurrentPos.X);
        float MaxY = FMath::Max(StartPos.Y, CurrentPos.Y);
        
        UE_LOG(LogTemp, Verbose, TEXT("AMyHUD::DrawHUD: Marquee rectangle coords: (%f,%f) to (%f,%f), size: %fx%f"), 
            MinX, MinY, MaxX, MaxY, MaxX - MinX, MaxY - MinY);

        // Draw filled rectangle (the marquee itself)
        UE_LOG(LogTemp, Verbose, TEXT("AMyHUD::DrawHUD: Drawing filled rectangle with color RGBA(%f,%f,%f,%f)"), 
            MarqueeColor.R, MarqueeColor.G, MarqueeColor.B, MarqueeColor.A);
        DrawRect(MarqueeColor, MinX, MinY, MaxX - MinX, MaxY - MinY);

        // Draw border lines
        UE_LOG(LogTemp, Verbose, TEXT("AMyHUD::DrawHUD: Drawing border lines with color RGBA(%f,%f,%f,%f) and thickness %f"), 
            MarqueeBorderColor.R, MarqueeBorderColor.G, MarqueeBorderColor.B, MarqueeBorderColor.A, MarqueeBorderThickness);
        DrawLine(MinX, MinY, MaxX, MinY, MarqueeBorderColor, MarqueeBorderThickness); // Top
        DrawLine(MinX, MaxY, MaxX, MaxY, MarqueeBorderColor, MarqueeBorderThickness); // Bottom
        DrawLine(MinX, MinY, MinX, MaxY, MarqueeBorderColor, MarqueeBorderThickness); // Left
        DrawLine(MaxX, MinY, MaxX, MaxY, MarqueeBorderColor, MarqueeBorderThickness); // Right
    }
    else
    {
        UE_LOG(LogTemp, Verbose, TEXT("AMyHUD::DrawHUD: No marquee being drawn"));
    }
}
