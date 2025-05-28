#include "UI/MyHUD.h"
#include "Player/MyPlayerController.h" // Your player controller
#include "Engine/Canvas.h"    // For DrawRect

void AMyHUD::DrawHUD()
{
    Super::DrawHUD();
    
    // Add a static test rectangle to verify HUD drawing works at all
    //DrawRect(FLinearColor::Red, 100, 100, 200, 200); // Draw a static red box for testing
    //UE_LOG(LogTemp, Log, TEXT("HUD DrawHUD: Drawing static test rectangle at (100,100) with size 200x200"));

    AMyPlayerController* PC = Cast<AMyPlayerController>(GetOwningPlayerController());
    if (!PC)
    {
        UE_LOG(LogTemp, Verbose, TEXT("AMyHUD::DrawHUD: No MyPlayerController found"));
        return;
    }
    
    if (PC && PC->bIsDrawingMarquee)
    {
        UE_LOG(LogTemp, Log, TEXT("HUD DrawHUD: Drawing Marquee! Start:(%f,%f), Current:(%f,%f)"), 
            PC->MarqueeStartScreenPosition.X, PC->MarqueeStartScreenPosition.Y,
            PC->MarqueeCurrentScreenPosition.X, PC->MarqueeCurrentScreenPosition.Y);
        FVector2D StartPos = PC->MarqueeStartScreenPosition;
        FVector2D CurrentPos = PC->MarqueeCurrentScreenPosition;

        float MinX = FMath::Min(StartPos.X, CurrentPos.X);
        float MinY = FMath::Min(StartPos.Y, CurrentPos.Y);
        float MaxX = FMath::Max(StartPos.X, CurrentPos.X);
        float MaxY = FMath::Max(StartPos.Y, CurrentPos.Y);
        
        UE_LOG(LogTemp, Log, TEXT("HUD DrawHUD: Marquee rectangle coords: MinXY:(%f,%f), MaxXY:(%f,%f), Width:%f, Height:%f"), 
            MinX, MinY, MaxX, MaxY, MaxX - MinX, MaxY - MinY);
            
        // Check if the marquee has valid dimensions
        if (MaxX - MinX <= 0 || MaxY - MinY <= 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("HUD DrawHUD: Marquee width or height is zero or negative! W:%f, H:%f"), MaxX - MinX, MaxY - MinY);
        }

        // Draw filled rectangle (the marquee itself)
        UE_LOG(LogTemp, Log, TEXT("HUD DrawHUD: Drawing filled rectangle at (%f,%f) with size (%f,%f) and color RGBA(%f,%f,%f,%f)"), 
            MinX, MinY, MaxX - MinX, MaxY - MinY, MarqueeColor.R, MarqueeColor.G, MarqueeColor.B, MarqueeColor.A);
        DrawRect(MarqueeColor, MinX, MinY, MaxX - MinX, MaxY - MinY);

        // Draw border lines
        UE_LOG(LogTemp, Log, TEXT("HUD DrawHUD: Drawing border lines with color RGBA(%f,%f,%f,%f) and thickness %f"), 
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
