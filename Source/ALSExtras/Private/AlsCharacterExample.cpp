#include "AlsCharacterExample.h"

#include "AlsCameraComponent.h"
#include "AlsCameraSettings.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Utility/AlsVector.h"

// GAS-related includes - using proper module paths for UE5.5
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AlsCharacterExample)

AAlsCharacterExample::AAlsCharacterExample()
{
	Camera = CreateDefaultSubobject<UAlsCameraComponent>(FName{TEXTVIEW("Camera")});
	Camera->SetupAttachment(GetMesh());
	Camera->SetRelativeRotation_Direct({0.0f, 90.0f, 0.0f});

	// Create and configure the AbilitySystemComponent
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true); // Enable replication for the ASC
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
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
	if (Camera->IsActive())
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
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLookMouse);
		EnhancedInput->BindAction(LookMouseAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnLookMouse);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnLook);
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnLook);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnMove);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnMove);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSprint);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnSprint);
		EnhancedInput->BindAction(WalkAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnWalk);
		EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnCrouch);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnJump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnJump);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnAim);
		EnhancedInput->BindAction(AimAction, ETriggerEvent::Canceled, this, &ThisClass::Input_OnAim);
		EnhancedInput->BindAction(RagdollAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRagdoll);
		EnhancedInput->BindAction(RollAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRoll);
		EnhancedInput->BindAction(RotationModeAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnRotationMode);
		EnhancedInput->BindAction(ViewModeAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnViewMode);
		EnhancedInput->BindAction(SwitchShoulderAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnSwitchShoulder);
		
		// TopDown camera input bindings
		if (IsValid(TopDownCameraZoomAction))
		{
			EnhancedInput->BindAction(TopDownCameraZoomAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnTopDownCameraZoom);
		}
		
		if (IsValid(TopDownCameraRotateAction))
		{
			EnhancedInput->BindAction(TopDownCameraRotateAction, ETriggerEvent::Triggered, this, &ThisClass::Input_OnTopDownCameraRotate);
		}
	}
}

void AAlsCharacterExample::Input_OnLookMouse(const FInputActionValue& ActionValue)
{
	const FVector2f Value{ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpMouseSensitivity);
	AddControllerYawInput(Value.X * LookRightMouseSensitivity);
}

void AAlsCharacterExample::Input_OnLook(const FInputActionValue& ActionValue)
{
	const FVector2f Value{ActionValue.Get<FVector2D>()};

	AddControllerPitchInput(Value.Y * LookUpRate);
	AddControllerYawInput(Value.X * LookRightRate);
}

void AAlsCharacterExample::Input_OnMove(const FInputActionValue& ActionValue)
{
	const auto Value{UAlsVector::ClampMagnitude012D(ActionValue.Get<FVector2D>())};

	// Check if we're in TopDown view mode
	if (GetViewMode() == AlsViewModeTags::TopDown)
	{
		// SCREEN-RELATIVE INPUT for Top-Down View
		// For TopDown view, we'll use the actual camera rotation for screen-relative movement
		// This way we don't need to rely on the TopDownCameraCurrentYawAngle property
		auto ViewRotation{GetViewState().Rotation};

		if (IsValid(GetController()))
		{
			// Get the actual camera rotation
			FVector ViewLocation;
			GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
		}

		// Extract the camera's yaw to determine screen-relative directions
		const float CameraYaw = ViewRotation.Yaw;

		// Calculate world directions based on camera yaw
		// Screen Top (Input Y+) corresponds to the camera's "forward" on the XY plane
		// Screen Right (Input X+) corresponds to the camera's "right" on the XY plane
		const FVector ScreenTopWorldDirection = UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(CameraYaw));
		const FVector ScreenRightWorldDirection = UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(CameraYaw + 90.0f)); // Perpendicular

		// Apply input, Y-axis of input for screen top/bottom, X-axis for screen left/right
		AddMovementInput(ScreenTopWorldDirection * Value.Y);
		AddMovementInput(ScreenRightWorldDirection * Value.X);
	}
	else
	{
		// Original ALS logic for First/Third person
		auto ViewRotation{GetViewState().Rotation};

		if (IsValid(GetController()))
		{
			// Use exact camera rotation instead of target rotation whenever possible.
			FVector ViewLocation;
			GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
		}

		const auto ForwardDirection{UAlsVector::AngleToDirectionXY(UE_REAL_TO_FLOAT(ViewRotation.Yaw))};
		const auto RightDirection{UAlsVector::PerpendicularCounterClockwiseXY(ForwardDirection)};

		AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
	}
}

void AAlsCharacterExample::Input_OnSprint(const FInputActionValue& ActionValue)
{
	SetDesiredGait(ActionValue.Get<bool>() ? AlsGaitTags::Sprinting : AlsGaitTags::Running);
}

void AAlsCharacterExample::Input_OnWalk()
{
	if (GetDesiredGait() == AlsGaitTags::Walking)
	{
		SetDesiredGait(AlsGaitTags::Running);
	}
	else if (GetDesiredGait() == AlsGaitTags::Running)
	{
		SetDesiredGait(AlsGaitTags::Walking);
	}
}

void AAlsCharacterExample::Input_OnCrouch()
{
	if (GetDesiredStance() == AlsStanceTags::Standing)
	{
		SetDesiredStance(AlsStanceTags::Crouching);
	}
	else if (GetDesiredStance() == AlsStanceTags::Crouching)
	{
		SetDesiredStance(AlsStanceTags::Standing);
	}
}

void AAlsCharacterExample::Input_OnJump(const FInputActionValue& ActionValue)
{
	if (ActionValue.Get<bool>())
	{
		if (StopRagdolling())
		{
			return;
		}

		if (StartMantlingGrounded())
		{
			return;
		}

		if (GetStance() == AlsStanceTags::Crouching)
		{
			SetDesiredStance(AlsStanceTags::Standing);
			return;
		}

		Jump();
	}
	else
	{
		StopJumping();
	}
}

void AAlsCharacterExample::Input_OnAim(const FInputActionValue& ActionValue)
{
	SetDesiredAiming(ActionValue.Get<bool>());
}

void AAlsCharacterExample::Input_OnRagdoll()
{
	if (!StopRagdolling())
	{
		StartRagdolling();
	}
}

void AAlsCharacterExample::Input_OnRoll()
{
	static constexpr auto PlayRate{1.3f};

	StartRolling(PlayRate);
}

void AAlsCharacterExample::Input_OnRotationMode()
{
	SetDesiredRotationMode(GetDesiredRotationMode() == AlsRotationModeTags::VelocityDirection
		                       ? AlsRotationModeTags::ViewDirection
		                       : AlsRotationModeTags::VelocityDirection);
}

void AAlsCharacterExample::Input_OnViewMode()
{
	// Cycle through the three view modes: ThirdPerson -> FirstPerson -> TopDown -> ThirdPerson
	if (GetViewMode() == AlsViewModeTags::ThirdPerson)
	{
		SetViewMode(AlsViewModeTags::FirstPerson);
	}
	else if (GetViewMode() == AlsViewModeTags::FirstPerson)
	{
		SetViewMode(AlsViewModeTags::TopDown);
		
		// Initialize TopDown camera distance if needed
		if (IsValid(Camera) && FMath::IsNearlyZero(Camera->GetTopDownCurrentDistance()))
		{
			Camera->SetTopDownCameraDistance(Camera->GetCameraSettings()->TopDown.DefaultDistance);
		}
	}
	else
	{
		SetViewMode(AlsViewModeTags::ThirdPerson);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AAlsCharacterExample::Input_OnSwitchShoulder()
{
	Camera->SetRightShoulder(!Camera->IsRightShoulder());
}

void AAlsCharacterExample::DisplayDebug(UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& Unused, float& VerticalLocation)
{
	if (Camera->IsActive())
	{
		Camera->DisplayDebug(Canvas, DisplayInfo, VerticalLocation);
	}

	Super::DisplayDebug(Canvas, DisplayInfo, Unused, VerticalLocation);
}

void AAlsCharacterExample::Input_OnTopDownCameraZoom(const FInputActionValue& ActionValue)
{
	if (GetViewMode() == AlsViewModeTags::TopDown && IsValid(Camera))
	{
		// Get the zoom value from the input action
		const float ZoomValue = ActionValue.Get<float>();
		const auto* CameraSettings = Camera->GetCameraSettings();
		
		if (IsValid(CameraSettings))
		{
			// Apply zoom to the camera - negative value zooms in, positive zooms out
			// Use the ZoomInputSensitivity from settings for proper configuration
			Camera->AddTopDownCameraZoom(-ZoomValue * CameraSettings->TopDown.ZoomInputSensitivity);
		}
	}
}

void AAlsCharacterExample::Input_OnTopDownCameraRotate(const FInputActionValue& ActionValue)
{
	if (GetViewMode() == AlsViewModeTags::TopDown && IsValid(Camera))
	{
		// Get the rotation value from the input action
		const FVector2f Value{ActionValue.Get<FVector2D>()};
		const auto* CameraSettings = Camera->GetCameraSettings();
		
		if (IsValid(CameraSettings))
		{
			// X-axis controls camera yaw (rotation around the vertical axis)
			Camera->AddTopDownCameraYaw(Value.X * CameraSettings->TopDown.YawInputSensitivity);
			
			// Y-axis controls camera pitch (up/down angle)
			Camera->AddTopDownCameraPitch(Value.Y * CameraSettings->TopDown.PitchInputSensitivity);
		}
	}
}

// --- GAS INTERFACE AND INITIALIZATION METHODS ---

UAbilitySystemComponent* AAlsCharacterExample::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAlsCharacterExample::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController); // Call parent first!

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this); // 'this' is both Owner and Avatar
		InitializeDefaultAbilitiesAndEffects(); // Grant abilities/effects on server
	}
}

void AAlsCharacterExample::OnRep_PlayerState()
{
	Super::OnRep_PlayerState(); // Call parent!

	// Client-side ASC initialization
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		// Note: Do NOT call InitializeDefaultAbilitiesAndEffects() here on the client
		// The server-granted ability specs will replicate to the client.
	}
}

void AAlsCharacterExample::InitializeDefaultAbilitiesAndEffects()
{
	if (!AbilitySystemComponent || GetLocalRole() != ROLE_Authority) // Only Server should grant abilities
	{
		return;
	}

	// Grant Default Abilities
	for (TSubclassOf<UGameplayAbility>& AbilityClass : DefaultAbilities)
	{
		if (AbilityClass)
		{
			// InputID of -1 (or INDEX_NONE) means this ability is not directly bound to an input from this spec.
			// It will be activated via TryActivateAbilityByClass or by GameplayEvent.
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1, INDEX_NONE, this);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}

	// Apply Default Startup Effects (e.g., to initialize attributes)
	for (TSubclassOf<UGameplayEffect>& EffectClass : DefaultStartupEffects)
	{
		if (EffectClass)
		{
			FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
			ContextHandle.AddSourceObject(this); // The character is the source of these startup effects

			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1, ContextHandle);
			if (SpecHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}
