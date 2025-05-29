// Plugins/ALS-Refactored/Source/ALSExtras/Private/AlsCharacterExample.cpp
#include "AlsCharacterExample.h" // Should be Public/AlsCharacterExample.h if following convention

#include "AlsCameraComponent.h"
#include "AlsCameraSettings.h" 
#include "Components/DecalComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Materials/MaterialInstanceDynamic.h" 
#include "Utility/AlsVector.h" 

#include UE_INLINE_GENERATED_CPP_BY_NAME(AlsCharacterExample)

AAlsCharacterExample::AAlsCharacterExample()
{
	Camera = CreateDefaultSubobject<UAlsCameraComponent>(FName{TEXTVIEW("Camera")});
	Camera->SetupAttachment(GetMesh()); 
	Camera->SetRelativeRotation_Direct({0.0f, 90.0f, 0.0f});

	SelectionDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionDecal"));
	SelectionDecalComponent->SetupAttachment(GetRootComponent());
	SelectionDecalComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	SelectionDecalComponent->DecalSize = DefaultSelectionDecalSize;
	SelectionDecalComponent->SetVisibility(false);
}

void AAlsCharacterExample::BeginPlay()
{
	Super::BeginPlay();

	if (SelectionDecalComponent && DefaultSelectionDecalMaterial)
	{
		SelectionDecalComponent->SetDecalMaterial(DefaultSelectionDecalMaterial);
		UMaterialInstanceDynamic* DecalMID = SelectionDecalComponent->CreateDynamicMaterialInstance();
		if (DecalMID)
		{
			DecalMID->SetVectorParameterValue(FName("DecalColorParam"), DefaultSelectionDecalColor);
		}
	}
}

void AAlsCharacterExample::NotifyControllerChanged()
{
	const auto* PreviousPlayer{Cast<APlayerController>(PreviousController)};
	if (IsValid(PreviousPlayer))
	{
		auto* InputSubsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PreviousPlayer->GetLocalPlayer())};
		if (IsValid(InputSubsystem))
		{
			InputSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}

	auto* NewPlayer{Cast<APlayerController>(GetController())};
	if (IsValid(NewPlayer))
	{
		NewPlayer->InputYawScale_DEPRECATED = 1.0f;
		NewPlayer->InputPitchScale_DEPRECATED = 1.0f;
		NewPlayer->InputRollScale_DEPRECATED = 1.0f;

		auto* InputSubsystem{ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(NewPlayer->GetLocalPlayer())};
		if (IsValid(InputSubsystem))
		{
			FModifyContextOptions Options;
			Options.bNotifyUserSettings = true;
			InputSubsystem->AddMappingContext(InputMappingContext, 0, Options);
		}
	}

	Super::NotifyControllerChanged();
}

void AAlsCharacterExample::CalcCamera(const float DeltaTime, FMinimalViewInfo& ViewInfo)
{
	if (Camera && Camera->IsActive()) 
	{
		Camera->GetViewInfo(ViewInfo);
		return;
	}
	Super::CalcCamera(DeltaTime, ViewInfo);
}

void AAlsCharacterExample::SetupPlayerInputComponent(UInputComponent* Input)
{
	Super::SetupPlayerInputComponent(Input);
	auto* EnhancedInput{Cast<UEnhancedInputComponent>(Input)};
	if (IsValid(EnhancedInput))
	{
		if (IsValid(LookMouseAction)) { EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLookMouse); }
		if (IsValid(LookAction)) { EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLook); }
		if (IsValid(MoveAction)) { EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnMove); EnhancedInput->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnMove); }
		if (IsValid(SprintAction)) { EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSprint); EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ThisClass::Input_OnSprint); }
		if (IsValid(WalkAction)) { EnhancedInput->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnWalk); }
		if (IsValid(CrouchAction)) { EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnCrouch); }
		if (IsValid(JumpAction)) { EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Input_OnJump); EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::Input_OnJump); } // Changed Triggered to Started for Jump
		if (IsValid(AimAction)) { EnhancedInput->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnAim); EnhancedInput->BindAction(AimAction, ETriggerEvent::Completed, this, &ThisClass::Input_OnAim); }
		if (IsValid(RagdollAction)) { EnhancedInput->BindAction(RagdollAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRagdoll); }
		if (IsValid(RollAction)) { EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRoll); }
		if (IsValid(RotationModeAction)) { EnhancedInput->BindAction(RotationModeAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRotationMode); }
		if (IsValid(ViewModeAction)) { EnhancedInput->BindAction(ViewModeAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnViewMode); }
		if (IsValid(SwitchShoulderAction)) { EnhancedInput->BindAction(SwitchShoulderAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSwitchShoulder); }
		if (IsValid(TopDownCameraZoomAction)) { EnhancedInput->BindAction(TopDownCameraZoomAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnTopDownCameraZoom); }
		if (IsValid(TopDownCameraRotateAction)) { EnhancedInput->BindAction(TopDownCameraRotateAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnTopDownCameraRotate); }
	}
}
void AAlsCharacterExample::Input_OnLookMouse(const FInputActionValue& ActionValue){ const FVector2f Value{ActionValue.Get<FVector2D>()}; AddControllerPitchInput(Value.Y * LookUpMouseSensitivity); AddControllerYawInput(Value.X * LookRightMouseSensitivity); }
void AAlsCharacterExample::Input_OnLook(const FInputActionValue& ActionValue){ const FVector2f Value{ActionValue.Get<FVector2D>()}; AddControllerPitchInput(Value.Y * LookUpRate); AddControllerYawInput(Value.X * LookRightRate); }
void AAlsCharacterExample::Input_OnMove(const FInputActionValue& ActionValue){ const auto Value{UAlsVector::ClampMagnitude012D(ActionValue.Get<FVector2D>())}; if (GetViewMode() == AlsViewModeTags::TopDown) { auto ViewRotation{GetViewState().Rotation}; if (IsValid(GetController())) { FVector ViewLocation; GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation); } const float CameraYaw = ViewRotation.Yaw; const FVector ScreenTopWorldDirection = UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(CameraYaw)); const FVector ScreenRightWorldDirection = UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(CameraYaw + 90.0f)); AddMovementInput(ScreenTopWorldDirection * Value.Y); AddMovementInput(ScreenRightWorldDirection * Value.X); } else { auto ViewRotation{GetViewState().Rotation}; if (IsValid(GetController())) { FVector ViewLocation; GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation); } const auto ForwardDirection{UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(ViewRotation.Yaw))}; const auto RightDirection{UAlsVector::PerpendicularCounterClockwiseXY(ForwardDirection)}; AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X); } }
void AAlsCharacterExample::Input_OnSprint(const FInputActionValue& ActionValue){ SetDesiredGait(ActionValue.Get<bool>() ? AlsGaitTags::Sprinting : AlsGaitTags::Running); }
void AAlsCharacterExample::Input_OnWalk(){ if (GetDesiredGait() == AlsGaitTags::Walking) { SetDesiredGait(AlsGaitTags::Running); } else if (GetDesiredGait() == AlsGaitTags::Running) { SetDesiredGait(AlsGaitTags::Walking); } }
void AAlsCharacterExample::Input_OnCrouch(){ if (GetDesiredStance() == AlsStanceTags::Standing) { SetDesiredStance(AlsStanceTags::Crouching); } else if (GetDesiredStance() == AlsStanceTags::Crouching) { SetDesiredStance(AlsStanceTags::Standing); } }
void AAlsCharacterExample::Input_OnJump(const FInputActionValue& ActionValue){ if (ActionValue.Get<bool>()) { if (StopRagdolling()) { return; } if (StartMantlingGrounded()) { return; } if (GetStance() == AlsStanceTags::Crouching) { SetDesiredStance(AlsStanceTags::Standing); return; } Jump(); } else { StopJumping(); } }
void AAlsCharacterExample::Input_OnAim(const FInputActionValue& ActionValue){ SetDesiredAiming(ActionValue.Get<bool>()); }
void AAlsCharacterExample::Input_OnRagdoll(){ if (!StopRagdolling()) { StartRagdolling(); } }
void AAlsCharacterExample::Input_OnRoll(){ static constexpr auto PlayRate{1.3f}; StartRolling(PlayRate); }
void AAlsCharacterExample::Input_OnRotationMode(){ SetDesiredRotationMode(GetDesiredRotationMode() == AlsRotationModeTags::VelocityDirection ? AlsRotationModeTags::ViewDirection : AlsRotationModeTags::VelocityDirection); }
void AAlsCharacterExample::Input_OnViewMode(){ if (GetViewMode() == AlsViewModeTags::ThirdPerson) { SetViewMode(AlsViewModeTags::FirstPerson); } else if (GetViewMode() == AlsViewModeTags::FirstPerson) { SetViewMode(AlsViewModeTags::TopDown); if (Camera && Camera->GetCameraSettings() && FMath::IsNearlyZero(Camera->GetTopDownCurrentDistance())) { Camera->SetTopDownCameraDistance(Camera->GetCameraSettings()->TopDown.DefaultDistance); } } else { SetViewMode(AlsViewModeTags::ThirdPerson); } }
void AAlsCharacterExample::Input_OnSwitchShoulder(){ if(Camera) {Camera->SetRightShoulder(!Camera->IsRightShoulder());} }
void AAlsCharacterExample::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused, float& VerticalLocation){ if (Camera && Camera->IsActive()) { Camera->DisplayDebug(Canvas, DisplayInfo, VerticalLocation); } Super::DisplayDebug(Canvas, DisplayInfo, Unused, VerticalLocation); }
void AAlsCharacterExample::Input_OnTopDownCameraZoom(const FInputActionValue& ActionValue){ if (GetViewMode() == AlsViewModeTags::TopDown && Camera) { const float ZoomValue = ActionValue.Get<float>(); const auto* CameraSettings = Camera->GetCameraSettings(); if (CameraSettings) { Camera->AddTopDownCameraZoom(-ZoomValue * CameraSettings->TopDown.ZoomInputSensitivity); } } }
void AAlsCharacterExample::Input_OnTopDownCameraRotate(const FInputActionValue& ActionValue){ if (GetViewMode() == AlsViewModeTags::TopDown && Camera) { const FVector2f Value{ActionValue.Get<FVector2D>()}; const auto* CameraSettings = Camera->GetCameraSettings(); if (CameraSettings) { Camera->AddTopDownCameraYaw(Value.X * CameraSettings->TopDown.YawInputSensitivity); Camera->AddTopDownCameraPitch(Value.Y * CameraSettings->TopDown.PitchInputSensitivity); } } }