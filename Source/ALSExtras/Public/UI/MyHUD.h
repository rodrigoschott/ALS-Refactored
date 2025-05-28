#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

UCLASS()
class ALSEXTRAS_API AMyHUD : public AHUD
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
};
