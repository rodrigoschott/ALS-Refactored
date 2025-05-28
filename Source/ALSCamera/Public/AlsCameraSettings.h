#pragma once

#include "Engine/DataAsset.h"
#include "Engine/Scene.h"
#include "Utility/AlsConstants.h"
#include "AlsCameraSettings.generated.h"

USTRUCT(BlueprintType)
struct ALSCAMERA_API FAlsFirstPersonCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 5, ClampMax = 175, ForceUnits = "deg"))
	float FieldOfView{90.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FName CameraSocketName{TEXTVIEW("FirstPersonCamera")};
};

USTRUCT(BlueprintType)
struct ALSCAMERA_API FAlsTraceDistanceSmoothingSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0))
	float InterpolationSpeed{3.0f};
};

USTRUCT(BlueprintType)
struct ALSCAMERA_API FAlsThirdPersonCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 5, ClampMax = 175, ForceUnits = "deg"))
	float FieldOfView{90.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FName FirstPivotSocketName{UAlsConstants::RootBoneName()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FName SecondPivotSocketName{UAlsConstants::HeadBoneName()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float TraceRadius{15.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	TEnumAsByte<ECollisionChannel> TraceChannel{ECC_Visibility};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FName TraceShoulderLeftSocketName{TEXTVIEW("ThirdPersonTraceShoulderLeft")};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FName TraceShoulderRightSocketName{TEXTVIEW("ThirdPersonTraceShoulderRight")};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
	FVector3f TraceOverrideOffset{0.0f, 0.0f, 40.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (InlineEditConditionToggle))
	uint8 bEnableTraceDistanceSmoothing : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		DisplayName = "Enable Trace Distance Smoothing", Meta = (EditCondition = "bEnableTraceDistanceSmoothing"))
	FAlsTraceDistanceSmoothingSettings TraceDistanceSmoothing;
};

USTRUCT(BlueprintType)
struct ALSCAMERA_API FAlsTopDownCameraSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|FOV", Meta = (ClampMin = 5, ClampMax = 175, ForceUnits = "deg"))
	float FieldOfView{70.0f}; // Renamed from Pitch for clarity, was likely FOV before

	// ZOOM SETTINGS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Zoom", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float MinDistance{400.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Zoom", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float MaxDistance{2000.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Zoom", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float DefaultDistance{1000.0f}; // Used for initialization

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Zoom", Meta = (ClampMin = 0, ToolTip = "How many cm per unit of zoom input"))
	float ZoomInputSensitivity{100.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Zoom", Meta = (ClampMin = 0, ToolTip = "How quickly the camera distance smooths to the target"))
	float ZoomLagSpeed{8.0f};

	// PITCH (UP/DOWN LOOK) SETTINGS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Pitch", Meta = (ForceUnits = "deg", ToolTip = "Min Pitch (e.g., -85 looks more down), Max Pitch (e.g., -30 looks more horizontal)"))
	FVector2D CameraPitchAngleLimits{-85.0f, -30.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Pitch", Meta = (ClampMin = -90.0, ClampMax = 0.0, ForceUnits = "deg"))
	float DefaultCameraPitchAngle{-60.0f}; // Initial pitch, must be within PitchAngleLimits

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Pitch", Meta = (ClampMin = 0, ToolTip = "How many degrees per unit of pitch input"))
	float PitchInputSensitivity{1.0f}; // Will be scaled by DeltaSeconds for framerate independence for stick

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Pitch", Meta = (ClampMin = 0, ToolTip = "How quickly the camera pitch smooths to the target"))
	float PitchLagSpeed{10.0f};

	// YAW (LEFT/RIGHT ROTATION) SETTINGS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Yaw", Meta = (ClampMin = 0, ToolTip = "How many degrees per unit of yaw input"))
	float YawInputSensitivity{1.5f}; // Will be scaled by DeltaSeconds for framerate independence for stick

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Yaw", Meta = (ClampMin = 0, ToolTip = "How quickly the camera yaw smooths to the target"))
	float YawLagSpeed{10.0f};
	
	// If true, camera yaw is fixed in world space (player input orbits around this fixed yaw).
	// If false, camera yaw tries to maintain an offset from the character's forward direction.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Yaw")
	uint8 bFixedWorldYaw : 1 {false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Yaw", Meta = (EditCondition = "bFixedWorldYaw", ForceUnits = "deg", ToolTip = "Initial world yaw if bFixedWorldYaw is true"))
	float InitialFixedWorldYawAngle{0.0f};

	// GENERAL LAG & TRACE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Lag", Meta = (ClampMin = 0, ToolTip = "How quickly the camera's XY position follows the character"))
	float LocationLagSpeed{12.0f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Collision", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float TraceRadius{10.0f}; // Reduced from 15 for topdown, less likely to hit small things

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|TopDown|Collision")
	TEnumAsByte<ECollisionChannel> TraceChannel{ECC_Camera}; // Changed to ECC_Camera, often better

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (InlineEditConditionToggle))
	uint8 bEnableTraceDistanceSmoothing : 1 {true};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS",
		DisplayName = "Enable Trace Distance Smoothing", Meta = (EditCondition = "bEnableTraceDistanceSmoothing"))
	FAlsTraceDistanceSmoothingSettings TraceDistanceSmoothing;
};

UCLASS(Blueprintable, BlueprintType)
class ALSCAMERA_API UAlsCameraSettings : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	uint8 bIgnoreTimeDilation : 1 {true};

	// Camera will be teleported if the actor has moved farther than this
	// distance in 1 frame. If zero is specified, teleportation will be disabled.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ForceUnits = "cm"))
	float TeleportDistanceThreshold{200.0f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsFirstPersonCameraSettings FirstPerson;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsThirdPersonCameraSettings ThirdPerson;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FAlsTopDownCameraSettings TopDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	FPostProcessSettings PostProcess;

public:
#if WITH_EDITORONLY_DATA
	virtual void Serialize(FArchive& Archive) override;
#endif
};
