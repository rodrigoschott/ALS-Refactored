
## **ALS (Advanced Locomotion System) - Manual Index**

### **I. Core Character System**

This section covers the fundamental components that define and control the character's behavior and animation.

#### **A. `AAlsCharacter` (Main Character Class)**

*   **Purpose**: The central class representing the playable or AI-controlled character. It manages input, desired states, locomotion logic, action execution (mantling, rolling, ragdolling), and integrates with animation and movement components.
*   **Key Functions/Features**:
    *   **State Management**: Tracks and updates `ViewMode`, `LocomotionMode`, `RotationMode`, `Stance`, `Gait`, `OverlayMode`, and `LocomotionAction` (e.g., Mantling, Ragdolling, Rolling).
    *   **Desired State Replication**: `bDesiredAiming`, `DesiredRotationMode`, `DesiredStance`, `DesiredGait`, `ViewMode`, `OverlayMode` are replicated and applied across the network.
    *   **Input Handling**: Processes movement input and updates `InputDirection` and `InputYawAngle`.
    *   **View & Camera Integration**: Overrides `CalcCamera` and manages `ViewState` including network smoothing.
    *   **Movement Mode Handling**: Responds to changes in Unreal's `EMovementMode` to update `LocomotionMode`.
    *   **Gait Refresh**: Dynamically determines the character's gait (walking, running, sprinting) based on speed, input, and allowed gaits.
    *   **Rotation Logic**: Handles character rotation (`RefreshGroundedRotation`, `RefreshInAirRotation`) based on `RotationMode`, velocity, input, and view. Includes `ConstrainAimingRotation` to limit rotation during aiming.
    *   **Character Actions**:
        *   **Jumping**: Standard character jumping functionality.
        *   **Rolling**: Initiates and manages character rolls (`StartRolling`, `RefreshRolling`, `RefreshRollingPhysics`).
        *   **Mantling**: Detects and executes mantling actions over obstacles (`StartMantlingGrounded`, `StartMantlingInAir`, `RefreshMantling`, `StopMantling`). Uses a custom root motion source.
        *   **Ragdolling**: Toggles ragdoll physics simulation (`StartRagdolling`, `StopRagdolling`, `RefreshRagdolling`). Includes logic for getting up from ragdoll and speed limiting.
    *   **Network Synchronization**: Replicates key states (`ReplicatedViewRotation`, `InputDirection`, `DesiredVelocityYawAngle`, `RagdollTargetLocation`) and manages network smoothing for views and movement.
    *   **Debug Visualization**: Provides extensive debug drawing for various states and traces (`DisplayDebug`).

#### **B. `UAlsCharacterMovementComponent` (Custom Movement Logic)**

*   **Purpose**: A custom character movement component that extends Unreal's default movement component to incorporate ALS-specific locomotion rules, gait settings, and network prediction improvements.
*   **Key Functions/Features**:
    *   **Gait Settings**: Manages `FAlsMovementGaitSettings` for different gaits (walk, run, sprint) and stances (standing, crouching), dynamically adjusting `MaxWalkSpeed`, `MaxAccelerationWalking`, `BrakingDecelerationWalking`, and `GroundFriction` via curves.
    *   **State Transmission**: Extends network move data (`FAlsCharacterNetworkMoveData`, `FAlsSavedMove`) to include `RotationMode`, `Stance`, and `MaxAllowedGait` for accurate client-server synchronization.
    *   **Physics Rotation Delegate**: Provides `OnPhysicsRotation` broadcast for custom physics logic that needs to run after character movement's physics rotation.
    *   **Input Blocking**: Allows external systems to temporarily block character input.
    *   **Penetration Adjustment**: Includes a flag (`bAllowImprovedPenetrationAdjustment`) for refined collision handling.
    *   **Movement Base Interaction**: Handles character movement relative to moving platforms, including rotation inheritance.

#### **C. `UAlsAnimationInstance` (Main Animation Blueprint Logic)**

*   **Purpose**: The primary animation blueprint instance for the character, responsible for processing character state into animation parameters, handling animation logic, and managing animation curves.
*   **Key Functions/Features**:
    *   **State Synchronization**: Receives and updates various `FAls...State` structs (e.g., `LayeringState`, `PoseState`, `ViewState`, `LocomotionState`, `FeetState`, `TransitionsState`) based on `AAlsCharacter` properties.
    *   **Animation Graph Input**: Provides data (`FAlsControlRigInput`) for Control Rig and other animation graph nodes.
    *   **Update Phases**: Organized animation logic into `NativeUpdateAnimation` (game thread), `NativeThreadSafeUpdateAnimation` (worker thread), and `NativePostUpdateAnimation` for optimal performance.
    *   **Layering and Pose Blending**: Calculates blend amounts for different body parts and poses based on character state and animation curves.
    *   **View & Spine**: Manages view and spine rotation and blending based on camera and character rotation.
    *   **Locomotion & Lean**: Calculates velocity blends, lean amounts, and movement direction.
    *   **Foot IK**: Manages foot placement (`RefreshFeet`) and locking (`RefreshFootLock`) to ensure feet adhere to surfaces.
    *   **In-Air Logic**: Handles jumping (`Jump`) and ground prediction (`RefreshGroundPrediction`).
    *   **Transitions**: Manages quick stop, dynamic transitions, rotate-in-place, and turn-in-place animations by playing montages and adjusting parameters.
    *   **Ragdolling Animation**: Provides final ragdoll pose snapshotting and flail animation control.
    *   **Curve Management**: Retrieves animation curve values and manages their state.

#### **D. `UAlsLinkedAnimationInstance` (Linked Animation Instances)**

*   **Purpose**: A base class for linked animation instances that can access and synchronize with the main `UAlsAnimationInstance`, allowing for modular animation logic within sub-animation blueprints.
*   **Key Functions/Features**:
    *   **Parent Reference**: Holds a weak pointer to the main `UAlsAnimationInstance` (`Parent`) for data access.
    *   **Function Proxying**: Provides methods to call parent animation instance functions (e.g., `InitializeLook`, `RefreshGrounded`, `RefreshDynamicTransitions`).

#### **E. Settings Data Assets**

*   **Purpose**: Centralized configuration for the core character system.
*   **Key Classes**:
    *   **`UAlsCharacterSettings`**: General character behavior settings (teleport thresholds, in-air rotation modes, sprint priorities, mantling, ragdolling, rolling).
    *   **`UAlsMovementSettings`**: Defines movement gait settings (walk, run, sprint speeds, acceleration, deceleration, friction) per rotation mode and stance.
    *   **`UAlsAnimationInstanceSettings`**: Configuration for animation instance behaviors (general, view, grounded, standing, crouching, in-air, feet, transitions, rotate-in-place, turn-in-place).

#### **F. State Structs**

*   **Purpose**: Encapsulate various aspects of the character's dynamic state for efficient management and replication.
*   **Key Structs**:
    *   `FAlsControlRigInput`: Data passed to Control Rig for character animation.
    *   `FAlsCrouchingState`: Current state of crouching.
    *   `FAlsDynamicTransitionsState`: State for dynamic animation transitions.
    *   `FAlsFootState`, `FAlsFeetState`: Detailed state for foot placement and IK.
    *   `FAlsGroundedState`, `FAlsVelocityBlendState`, `FAlsRotationYawOffsetsState`: State for grounded movement, velocity blending, and rotation offsets.
    *   `FAlsInAirState`: Current state for in-air movement.
    *   `FAlsLayeringState`: Current blend amounts for animation layering.
    *   `FAlsLeanState`: Current lean amounts.
    *   `FAlsLocomotionAnimationState`: Animation-specific locomotion parameters.
    *   `FAlsLocomotionState`: General locomotion parameters (speed, velocity, input).
    *   `FAlsLookState`: State for the character's head/eye look-at behavior.
    *   `FAlsMantlingParameters`, `FAlsMantlingState`: Parameters and state for mantling actions.
    *   `FAlsMovementBaseState`: Information about the character's current movement base.
    *   `FAlsMovementDirectionCache`: Cached movement direction.
    *   `FAlsPoseState`: Current blend amounts for various character poses.
    *   `FAlsRagdollingAnimationState`, `FAlsRagdollingState`: Animation and physics state for ragdolling.
    *   `FAlsRotateInPlaceState`: State for rotate-in-place animation.
    *   `FAlsRollingState`: State for rolling action.
    *   `FAlsSpineState`: State for spine rotation.
    *   `FAlsStandingState`: Current state of standing movement.
    *   `FAlsTransitionsState`: State for animation transitions.
    *   `FAlsTurnInPlaceState`: State for turn-in-place animation.
    *   `FAlsViewAnimationState`: Animation-specific view parameters.
    *   `FAlsViewState`, `FAlsViewNetworkSmoothingState`: Overall view state, including network smoothing.

### **II. Camera System**

This section details the custom camera implementation that complements the character system.

#### **A. `UAlsCameraComponent` (Camera Logic)**

*   **Purpose**: Handles the camera's position, rotation, and field of view, supporting both first-person and third-person perspectives, with features like camera lag and collision.
*   **Key Functions/Features**:
    *   **View Info Generation**: Provides `FMinimalViewInfo` for the engine's camera system.
    *   **View Modes**: Supports `FirstPerson` and `ThirdPerson` camera modes.
    *   **Camera Lag**: Implements axis-independent camera lag for smooth follow behavior (`CalculateCameraRotation`, `CalculatePivotLagLocation`).
    *   **Camera Collision**: Traces for objects between the camera and character to prevent clipping (`CalculateCameraTrace`).
    *   **Pivot & Offset Calculation**: Determines camera pivot and offset locations based on animation curves and character state.
    *   **Shoulder Switching**: Allows dynamic switching between left and right shoulder views in third-person.
    *   **Debug Visualization**: Draws camera debug shapes and traces (`DisplayDebug`).

#### **B. `UAlsCameraAnimationInstance` (Camera Animation Logic)**

*   **Purpose**: A dedicated animation instance for the camera, similar to `UAlsAnimationInstance` but focused on driving camera-specific animation curves.
*   **Key Functions/Features**:
    *   **State Synchronization**: Updates its internal state (`ViewMode`, `LocomotionMode`, `RotationMode`, `Stance`, `Gait`, `LocomotionAction`, `bRightShoulder`) from the main character.
    *   **Curve Output**: Provides curve values for camera logic (e.g., `CameraOffsetX`, `RotationLag`).

#### **C. Settings Data Asset**

*   **Purpose**: Centralized configuration for the camera system.
*   **Key Class**:
    *   **`UAlsCameraSettings`**: Contains settings for first-person and third-person camera behaviors, trace parameters, and post-process effects.

### **III. Locomotion Features (Detailed Breakdown)**

This section elaborates on specific locomotion behaviors implemented in the system.

#### **A. State Management (Gameplay Tags)**

*   **Purpose**: Utilizes Unreal Engine's Gameplay Tags for robust and extensible state categorization.
*   **Key Tag Categories**:
    *   `AlsViewModeTags`: `FirstPerson`, `ThirdPerson`
    *   `AlsLocomotionModeTags`: `Grounded`, `InAir`
    *   `AlsRotationModeTags`: `VelocityDirection`, `ViewDirection`, `Aiming`
    *   `AlsStanceTags`: `Standing`, `Crouching`
    *   `AlsGaitTags`: `Walking`, `Running`, `Sprinting`
    *   `AlsOverlayModeTags`: `Default`, `Masculine`, `Feminine`, `Rifle`, etc.
    *   `AlsLocomotionActionTags`: `Mantling`, `Ragdolling`, `GettingUp`, `Rolling`
    *   `AlsGroundedEntryModeTags`: `FromRoll` (for specific grounded entry animations)

#### **B. Animation Layering & Blending**

*   **Purpose**: Provides fine-grained control over which body parts are animated by different animation sources, ensuring smooth transitions and complex motion.
*   **Key Concepts**: Driven by `FAlsLayeringState` and various "Layer" animation curves (e.g., `LayerHead`, `LayerArmLeft`, `LayerSpine`).

#### **C. Foot IK & Placement**

*   **Purpose**: Ensures realistic foot placement on uneven terrain and prevents foot sliding.
*   **Key Concepts**:
    *   Driven by `FAlsFeetState`.
    *   `FootLeftIkCurve`, `FootRightIkCurve`: Control foot IK blending.
    *   `FootLeftLockCurve`, `FootRightLockCurve`: Control foot locking (keeping foot planted).
    *   `FootPlantedCurve`: Determines which foot is planted or about to plant.
    *   `FeetCrossingCurve`: Controls the degree of foot crossing.
    *   Integration with Control Rig units for final foot adjustment.

#### **D. Dynamic Transitions**

*   **Purpose**: Manages automatic and contextual animation transitions for a more fluid and responsive character.
*   **Key Concepts**:
    *   **Quick Stop**: Plays a quick stop animation when the character rapidly decelerates.
    *   **Dynamic Transitions**: Triggers small additive transitions when a foot is "locked" in place but the character's desired foot position has moved significantly (e.g., when slowly rotating in place).
    *   **Rotate In Place**: Smoothly rotates the character's upper body when the camera rotates beyond a threshold while standing idle.
    *   **Turn In Place**: Plays a full body turn animation when the camera rotates significantly while standing idle.
    *   Driven by `FAlsTransitionsState`, `FAlsDynamicTransitionsState`, `FAlsRotateInPlaceState`, `FAlsTurnInPlaceState` and their respective settings (`FAlsTransitionsSettings`, `FAlsDynamicTransitionsSettings`, `FAlsRotateInPlaceSettings`, `FAlsGeneralTurnInPlaceSettings`).

#### **E. Advanced Locomotion Actions**

1.  **Mantling**
    *   **Purpose**: Enables the character to smoothly vault or climb over obstacles.
    *   **Key Concepts**:
        *   Detailed trace logic (`FAlsMantlingTraceSettings`) to detect valid mantle locations and heights.
        *   Categorizes mantles as `High`, `Low`, or `InAir`.
        *   Uses a custom `FAlsRootMotionSource_Mantling` to drive the character's movement precisely via root motion.
        *   Settings in `FAlsGeneralMantlingSettings` and `UAlsMantlingSettings`.

2.  **Rolling**
    *   **Purpose**: Allows the character to perform a dodge or evasive roll.
    *   **Key Concepts**:
        *   Plays a root motion montage.
        *   Optionally crouches on start and rotates to input direction.
        *   Can be triggered on landing from a fall.
        *   Interruptible if the character becomes airborne.
        *   Settings in `FAlsRollingSettings`.

3.  **Ragdolling & Getting Up**
    *   **Purpose**: Transitions the character into and out of a physics-driven ragdoll state.
    *   **Key Concepts**:
        *   Can be initiated manually or automatically (e.g., on hard land, or if a mantled object is destroyed).
        *   Disables character movement and enables physics simulation on the skeletal mesh.
        *   Applies forces to stabilize the ragdoll.
        *   Limits initial ragdoll speed for stability.
        *   Captures a `FPoseSnapshot` of the ragdoll's final pose for a smooth blend-out.
        *   Plays "get up" montages (front or back) based on the ragdoll's final orientation.
        *   Settings in `FAlsRagdollingSettings`.

### **IV. Core Utilities**

These modules provide foundational functionalities used throughout the system.

*   **A. Math Utilities (`UAlsMath`)**: Common math functions like `Clamp01`, `LerpClamped`, `Damp`, `ExponentialDecay`, `SpringDamp`, `CalculateMovementDirection`, and `TryCalculatePoleVector`. Includes `FAlsSpringFloatState` for spring damping.
*   **B. Vector Utilities (`UAlsVector`)**: Vector-specific math operations like `ClampMagnitude01`, converting between radians/angles and directions, perpendicular vectors, signed angles, `Slerp`, and `SpringDampVector`. Includes `FAlsSpringVectorState`.
*   **C. Rotation Utilities (`UAlsRotation`)**: Rotation-specific math functions including `RemapAngleForCounterClockwiseRotation`, `LerpAngle`, `LerpRotation`, `InterpolateAngleConstant`, `DampAngle`, `DampRotation`, `ExponentialDecayAngle`, `ExponentialDecayRotation`, `InterpolateQuaternionFast`, and `GetTwist`.
*   **D. General Utilities (`UAlsUtility`)**: Miscellaneous helper functions, e.g., getting animation curve values, Gameplay Tag helpers, ping measurement, and movement base rotation speed.
*   **E. Constants (`UAlsConstants`, `UAlsCameraConstants`)**: Centralized definitions for bone names, animation slots, animation curve names, and debug display names used across the system.
*   **F. Logging (`AlsLog`)**: Custom logging category for ALS-specific messages.
*   **G. Macros (`AlsMacros`)**: Custom `ALS_ENSURE` macro for lightweight assertions.
*   **H. Enum Utilities (`AlsEnumUtility`)**: Helper functions for working with C++ enums.

### **V. Editor & Developer Tools**

This section covers the tools and extensions provided to assist developers in using and extending the ALS system within the Unreal Editor.

#### **A. Animation Modifiers**

*   **Purpose**: Editor-time tools to automatically process and modify animation sequences.
*   **Key Classes**:
    *   `UAlsAnimationModifier_CalculateRotationYawSpeed`: Adds a root rotation yaw speed curve.
    *   `UAlsAnimationModifier_CopyCurves`: Copies animation curves from one sequence to another.
    *   `UAlsAnimationModifier_CreateCurves`: Creates new float curves in an animation sequence.
    *   `UAlsAnimationModifier_CreateLayeringCurves`: Creates specific layering-related curves.

#### **B. AnimGraph Nodes**

*   **Purpose**: Custom nodes for Unreal Engine's Animation Blueprint editor, enabling specialized blending and logic directly within the visual graph.
*   **Key Classes**:
    *   `UAlsAnimGraphNode_CurvesBlend` / `FAlsAnimNode_CurvesBlend`: Blends animation curves using various modes (blend by amount, combine, override, etc.).
    *   `UAlsAnimGraphNode_GameplayTagsBlend` / `FAlsAnimNode_GameplayTagsBlend`: Blends animation poses based on an active Gameplay Tag, allowing for flexible state-driven animation logic.

#### **C. Animation Notifies**

*   **Purpose**: Events triggered at specific points in animations.
*   **Key Classes**:
    *   `UAlsAnimNotify_CameraShake`: Triggers a camera shake.
    *   `UAlsAnimNotify_FootstepEffects`: Spawns footstep sounds, decals, and particle effects based on surface and physical material.
    *   `UAlsAnimNotify_SetGroundedEntryMode`: Sets the character's grounded entry mode (e.g., after a roll).

#### **D. Animation Notify States**

*   **Purpose**: Regions within an animation that trigger events on begin and end, or tick during their duration.
*   **Key Classes**:
    *   `UAlsAnimNotifyState_EarlyBlendOut`: Allows an animation montage to blend out early based on character state conditions (input, locomotion mode, rotation mode, stance).
    *   `UAlsAnimNotifyState_SetLocomotionAction`: Sets the character's `LocomotionAction` gameplay tag for the duration of the notify state.
    *   `UAlsAnimNotifyState_SetRootMotionScale`: Temporarily scales the root motion translation of the character.

#### **E. Control Rig / RigVM Units**

*   **Purpose**: Custom nodes for Unreal Engine's Control Rig system, allowing for procedural animation adjustments and IK solving.
*   **Key Classes**:
    *   `FAlsRigUnit_ApplyFootOffsetLocation`: Adjusts foot location based on surface and other offsets.
    *   `FAlsRigUnit_ApplyFootOffsetRotation`: Adjusts foot rotation to align with surface normals, with limits for realistic joint movement.
    *   `FAlsRigUnit_ChainLength`: Calculates the length of a bone chain.
    *   `FAlsRigUnit_DistributeRotationSimple`: Distributes a rotation uniformly among a list of bones.
    *   `FAlsRigUnit_FootOffsetTrace`: Performs a trace to find ground for foot offset calculations.
    *   `FAlsRigUnit_HandIkRetargeting`: Adjusts hand IK targets for retargeting purposes (e.g., holding weapons).
    *   **General RigVM Functions (`AlsRigUnits.h`)**: Includes mathematical utilities (`Clamp01`, `ExponentialDecayVector`, `ExponentialDecayQuaternion`), a pole vector calculation (`CalculatePoleVector`), and a check for `IsGameWorld`.

#### **F. Custom Root Motion Sources (`FAlsRootMotionSource_Mantling`)**

*   **Purpose**: Defines a custom way for root motion to be applied to the character, offering precise control over specific character actions like mantling.

#### **G. Skeleton Utilities (`UAlsSkeletonUtility`)**

*   **Purpose**: Editor-time functions to modify skeletal assets, such as adding curves, animation slots, virtual bones, sockets, blend profiles, and setting bone retargeting modes.

#### **H. Debugging Tools (`UAlsDebugUtility`)**

*   **Purpose**: Provides helper functions for drawing various debug shapes, lines, and traces in the game world, aiding in visual debugging of character and camera logic.



























----------------------------------------------------------------------------------------------------------------
























## **I. Core Character System**

### **A. `AAlsCharacter` (Main Character Class)**

### **Scope**

This section covers the `AAlsCharacter` class, which is the primary C++ class representing a controllable or AI-driven character in the ALS system. It serves as the central orchestrator for character behavior, state management, action execution, input processing, and network synchronization, integrating closely with `UAlsCharacterMovementComponent` and `UAlsAnimationInstance`.

### **Purpose**

The `AAlsCharacter` class extends Unreal Engine's base `ACharacter` to provide a highly sophisticated and extensible foundation for third-person (and optionally first-person) character locomotion and action gameplay. Its primary goals are:
*   To manage and replicate a detailed set of character states (View Mode, Locomotion Mode, Rotation Mode, Stance, Gait, Overlay Mode, and active Locomotion Actions).
*   To process player/AI input and translate it into desired character behaviors.
*   To implement complex character actions like Mantling, Rolling, and Ragdolling with robust networking.
*   To serve as the data source for the animation system (`UAlsAnimationInstance`) and the custom movement component (`UAlsCharacterMovementComponent`).
*   To provide a clear and extensible C++ and Blueprint API for game-specific character logic.

### **Key Concepts/Components**

1.  **`UAlsCharacterSettings* Settings`**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character")`**
    *   **Significance**: A pointer to a `UAlsCharacterSettings` Data Asset. This asset centralizes many configurable parameters for the character's behavior, such as teleportation thresholds, in-air rotation modes, sprint priorities, and settings for Mantling, Ragdolling, and Rolling. This allows for easy tuning of character behavior without recompiling C++ code.

2.  **`UAlsMovementSettings* MovementSettings`**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Als Character")`**
    *   **Significance**: A pointer to a `UAlsMovementSettings` Data Asset. This asset defines the detailed movement parameters (speeds, accelerations, friction) based on the character's current Gait, Stance, and Rotation Mode. It's passed to the `UAlsCharacterMovementComponent`.

3.  **State Gameplay Tags (`ViewMode`, `LocomotionMode`, `RotationMode`, `Stance`, `Gait`, `OverlayMode`, `LocomotionAction`)**
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated...)`** (Note: Some are replicated with `ReplicatedUsing` for specific OnRep logic, others are just `Replicated`).
    *   **Significance**: These `FGameplayTag` properties represent the core high-level states of the character.
        *   **Desired States vs. Actual States**:
            *   `bDesiredAiming`, `DesiredRotationMode`, `DesiredStance`, `DesiredGait`: These represent what the player/AI *wants* the character to do. They are replicated and serve as input to the character's logic.
            *   `ViewMode`, `LocomotionMode`, `RotationMode`, `Stance`, `Gait`, `OverlayMode`, `LocomotionAction`: These are the *actual, current* states of the character, derived from desired states, input, and environmental conditions. They are used by the animation system and other game logic.
        *   **Replication**: Essential for multiplayer, ensuring all clients see a consistent character state. Some, like `bDesiredAiming` and `OverlayMode`, use `OnReplicated_...` functions to trigger logic when their value changes on clients.
    *   **Complete List of Possible States (from `AlsGameplayTags.h`)**:
        *   **`ViewMode`**: `Als.ViewMode.FirstPerson`, `Als.ViewMode.ThirdPerson`
        *   **`LocomotionMode`**: `Als.LocomotionMode.Grounded`, `Als.LocomotionMode.InAir`
        *   **`RotationMode` / `DesiredRotationMode`**: `Als.RotationMode.VelocityDirection`, `Als.RotationMode.ViewDirection`, `Als.RotationMode.Aiming`
        *   **`Stance` / `DesiredStance`**: `Als.Stance.Standing`, `Als.Stance.Crouching`
        *   **`Gait` / `DesiredGait`**: `Als.Gait.Walking`, `Als.Gait.Running`, `Als.Gait.Sprinting`
        *   **`OverlayMode`**: `Als.OverlayMode.Default`, `Masculine`, `Feminine`, `Injured`, `HandsTied`, `Rifle`, `PistolOneHanded`, `PistolTwoHanded`, `Bow`, `Torch`, `Binoculars`, `Box`, `Barrel`
        *   **`LocomotionAction`**: `Als.LocomotionAction.Mantling`, `Ragdolling`, `GettingUp`, `Rolling` (and potentially custom ones).
        *   **`GroundedEntryMode`**: `Als.GroundedEntryMode.FromRoll` (used by `UAlsAnimationInstance`).

4.  **State Structs (`MovementBase`, `ViewState`, `LocomotionState`, `MantlingState`, `RagdollingState`, `RollingState`)**
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient)`**
    *   **Significance**: These C++ structs hold detailed numerical and contextual data related to specific aspects of the character's state. They are updated every tick and provide the granular information needed by the animation and movement components.
    *   **Examples**:
        *   `FAlsMovementBaseState MovementBase`: Tracks the object the character is standing on.
        *   `FAlsViewState ViewState`: Manages camera view rotation and network smoothing.
        *   `FAlsLocomotionState LocomotionState`: Contains speed, velocity, input direction, target yaw, aiming limits, etc.
        *   `FAlsMantlingState MantlingState`: Holds the `RootMotionSourceId` for active mantling.
        *   `FAlsRagdollingState RagdollingState`: Stores ragdoll velocity, pull force, and speed limits.
        *   `FAlsRollingState RollingState`: Stores the `TargetYawAngle` for the roll.

5.  **`ReplicatedViewRotation` (`FRotator`)**
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, ReplicatedUsing = "OnReplicated_ReplicatedViewRotation")`**
    *   **Significance**: The raw view rotation of the character as replicated from the server or owning client. The `ViewState.Rotation` is derived from this, incorporating network smoothing. The `OnReplicated_ReplicatedViewRotation` function triggers view smoothing correction on clients.

6.  **`InputDirection` (`FVector_NetQuantizeNormal`)**
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated)`**
    *   **Significance**: The normalized direction of the character's current movement input. Replicated to allow servers and other clients to know the player's intended movement direction.

7.  **`DesiredVelocityYawAngle` (float)**
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated)`**
    *   **Significance**: The yaw angle of the character's *desired* velocity, which might differ from the actual velocity due to obstacles. Used in `VelocityDirection` rotation mode.

8.  **`RagdollTargetLocation` (`FVector_NetQuantize`)**
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, Replicated)`**
    *   **Significance**: The server-authoritative target location for the character's ragdoll pelvis on simulated clients. Used to apply a corrective pull force to keep client-side ragdolls somewhat synchronized with the server's simulation.

### **Methods & Functions**

1.  **State Setting Functions (`SetViewMode`, `SetDesiredAiming`, `SetDesiredRotationMode`, `SetDesiredStance`, `SetDesiredGait`, `SetOverlayMode`)**
    *   **Purpose**: Public C++ and Blueprint-callable functions to change the character's desired or actual states.
    *   **Parameters**: Typically take the new state value (e.g., a `FGameplayTag` or a `bool`).
    *   **Return Value**: Usually `void`.
    *   **What they accomplish**:
        *   Update the corresponding C++ property.
        *   Mark the property as dirty for replication (`MARK_PROPERTY_DIRTY_FROM_NAME`) if using push-based replication.
        *   Call server RPCs (e.g., `ServerSetDesiredAiming`) if initiated on an autonomous client to ensure server authority.
        *   Trigger internal logic or `OnRep_` functions/events to apply the state change (e.g., `ApplyDesiredStance`, `RefreshRotationMode`).
    *   **Remember to call `Super::`**: Not directly applicable here as these are new functions, but if you override an existing `ACharacter` function that ALS also overrides, always call `Super::`.

2.  **Action Functions (`StartRolling`, `StartMantlingGrounded`, `StartMantlingInAir`, `StartRagdolling`, `StopRagdolling`, `Jump`)**
    *   **Purpose**: Initiate complex character actions.
    *   **Parameters**: Vary by action (e.g., `StartRolling` takes `PlayRate` and `TargetYawAngle`).
    *   **Return Value**: Often `void` or `bool` indicating success.
    *   **What they accomplish**:
        *   Perform checks (e.g., `IsRollingAllowedToStart`, `CanSlide_Implementation`).
        *   Handle replication (Server and Multicast RPCs).
        *   Set the `LocomotionAction` Gameplay Tag.
        *   Play animation montages.
        *   Modify character movement state (e.g., set custom movement mode, block input).
        *   Interact with physics (e.g., enabling simulate physics for ragdoll).
    *   **_Implementation Suffixes**:
        *   `ServerStartRolling_Implementation`: Executed on the server when a client calls `ServerStartRolling`.
        *   `MulticastStartRolling_Implementation`: Executed on the server and all clients when the server calls `MulticastStartRolling`.

3.  **Internal Refresh Functions (`Refresh...`)**
    *   **Purpose**: Called every tick or on state changes to update the character's internal logic and state structs.
    *   **Examples**: `RefreshInput`, `RefreshLocomotionEarly/Late`, `RefreshView`, `RefreshGait`, `RefreshRotationMode`, `RefreshGroundedRotation`, `RefreshInAirRotation`, `RefreshMantling`, `RefreshRagdolling`, `RefreshRolling`.
    *   **What they accomplish**: Perform calculations, update state variables, make decisions about rotation, speed, and actions based on the current character context.

4.  **Replication Callbacks (`OnRep_...`)**
    *   **Purpose**: Functions automatically called on clients when a `ReplicatedUsing` property changes.
    *   **Examples**: `OnReplicated_DesiredAiming`, `OnReplicated_ReplicatedViewRotation`, `OnReplicated_OverlayMode`.
    *   **What they accomplish**: Allow clients to react to server-driven state changes, often by calling internal logic or Blueprint events (e.g., `OnDesiredAimingChanged_Implementation`).

5.  **Event Overrides (Blueprint Native Events / Blueprint Implementable Events)**
    *   **Purpose**: Provide hooks for Blueprint logic to extend or react to C++ events.
    *   **Examples**: `OnLocomotionModeChanged_Implementation`, `OnDesiredAimingChanged_Implementation`, `OnRotationModeChanged_Implementation`, `OnStanceChanged_Implementation`, `OnGaitChanged_Implementation`, `OnOverlayModeChanged_Implementation`, `OnMantlingStarted_Implementation`, `OnMantlingEnded_Implementation`, `OnRagdollingStarted_Implementation`, `OnRagdollingEnded_Implementation`, `SelectRollMontage_Implementation`, `SelectMantlingSettings_Implementation`, `SelectGetUpMontage_Implementation`, `IsMantlingAllowedToStart_Implementation`, `CanSlide_Implementation` (if you added it).
    *   **What they accomplish**: Enable designers to customize character behavior, select animations, or trigger visual/audio effects in Blueprints without modifying C++.
    *   **Remember to call Parent Function**: In Blueprint, if you override a Blueprint Native Event (which has a C++ implementation), make sure to call the "Parent: EventName" node if you want the base C++ logic to execute.

### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

#### **1. Adding a New View Mode (e.g., "OverTheShoulder Aiming")**

**C++:**

1.  **Define Gameplay Tag (`AlsGameplayTags.h` & `.cpp`):**
    ```cpp
    // AlsGameplayTags.h
    namespace AlsViewModeTags { ALS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(OverTheShoulderAim) }
    // AlsGameplayTags.cpp
    namespace AlsViewModeTags { UE_DEFINE_GAMEPLAY_TAG(OverTheShoulderAim, FName{TEXTVIEW("Als.ViewMode.OverTheShoulderAim")}) }
    ```
2.  **Update `AAlsCharacter` and `UAlsCameraComponent` logic:**
    *   Modify `AAlsCharacter::SetViewMode` if any special handling is needed (usually not for simple view modes).
    *   In `UAlsCameraComponent::TickCamera` (or helper functions like `CalculateCameraOffset`), add logic that checks for `ViewMode == AlsViewModeTags::OverTheShoulderAim` and applies different camera offsets, FOV, or behavior.
    *   In `UAlsCameraAnimationInstance::NativeUpdateAnimation`, update `ViewMode` based on the character's new view mode and create animation curves specific to this OTS view if needed (e.g., `CameraOffsetOTS_X`).
3.  **GAS Integration**:
    *   An ability (e.g., `GA_EnterOTSAim`) could call `Character->SetViewMode(AlsViewModeTags::OverTheShoulderAim)`.
    *   The ability might also apply a `GameplayEffect` that grants a tag like `State.Aiming.OTS`, which other systems or abilities could react to.

**Blueprint:**

1.  **Define Gameplay Tag**: You'd still do this in C++ as shown above for consistency and C++ access.
2.  **Triggering**: From a Blueprint (e.g., character input event), call `Set View Mode` on your `AAlsCharacter` instance, passing the `Als.ViewMode.OverTheShoulderAim` tag.
3.  **Reacting in Camera Blueprint**: If your `UAlsCameraComponent` is subclassed in Blueprint, you can override its Tick or relevant functions to check for the new `ViewMode` tag (get it from `CharacterOwner`) and adjust camera parameters.
4.  **Reacting in Animation Blueprint**: In your `ABP_AlsCharacter` (or custom AnimBP):
    *   Read `ViewMode` from the `UAlsAnimationInstance`.
    *   Use a `Select` node or blend logic based on `ViewMode` to choose different camera animation curve values or trigger different camera animation states.

#### **2. Adding a New Locomotion Action (e.g., "Parkour Wall Run")**

This is a more complex addition, often requiring custom movement physics.

**C++:**

1.  **Define Gameplay Tag (`AlsGameplayTags.h` & `.cpp`):**
    ```cpp
    // AlsGameplayTags.h
    namespace AlsLocomotionActionTags { ALS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(WallRunning) }
    // AlsGameplayTags.cpp
    namespace AlsLocomotionActionTags { UE_DEFINE_GAMEPLAY_TAG(WallRunning, FName{TEXTVIEW("Als.LocomotionAction.WallRunning")}) }
    ```
2.  **`AAlsCharacter` Implementation:**
    *   Add `StartWallRunning()`, `StopWallRunning()`, `ServerStartWallRunning_Implementation()`, `MulticastStartWallRunning_Implementation()`, etc., similar to the "Slide" example.
    *   `StartWallRunningImplementation()` would:
        *   Perform traces to detect a suitable wall.
        *   Set `LocomotionAction` to `Als.LocomotionAction.WallRunning`.
        *   Set `UCharacterMovementComponent::MovementMode` to `MOVE_Custom` and `AlsCharacterMovement->CustomMovementMode` to a new `EAlsCustomMovementMode::CM_WallRunning`.
        *   Lock movement mode: `AlsCharacterMovement->SetMovementModeLocked(true)`.
        *   Block input: `AlsCharacterMovement->SetInputBlocked(true)`.
        *   Play a wall run animation montage (often root motion driven).
3.  **`UAlsCharacterMovementComponent` Implementation:**
    *   Add `CM_WallRunning` to the `EAlsCustomMovementMode` enum.
    *   Override `PhysCustom()` to implement wall running physics: apply upward and forward forces, handle gravity, detect end of wall run (timer, no more wall, jump input).
4.  **GAS Integration**:
    *   Create `GA_WallRun`.
    *   `ActivateAbility`: Check if character can wall run (e.g., in air, near a wall, correct angle). If so, call `Character->StartWallRunning()`.
    *   The ability might end when the character's `LocomotionAction` is no longer `WallRunning` (handled by `PhysCustom` or player input like jumping off the wall).
    *   You could apply a `GameplayEffect` during wall running that grants `State.IsWallRunning` and perhaps provides a temporary speed boost or modified jump height.

**Blueprint:**

1.  **Animations**: Create wall run start, loop, and end animations, and corresponding montages.
2.  **AnimBP**: Add a "WallRunning" state to your locomotion state machine, triggered by `LocomotionAction == Als.LocomotionAction.WallRunning`. This state would play the wall run loop animation. The start/end montages might be played directly by the GAS ability.
3.  **Triggering**: The `GA_WallRun` ability would be triggered by player input (e.g., jump while facing a wall in air).

#### **3. Updating Existing Character Actions (e.g., Modify Mantling Reach)**

*   **C++**: Modify the `FAlsMantlingTraceSettings` within `UAlsCharacterSettings` (e.g., `GroundedTrace.ReachDistance`).
*   **Blueprint (via Data Asset)**:
    1.  Open your `DA_AlsCharacterSettings` asset in the editor.
    2.  Navigate to the `Mantling` section, then to `Grounded Trace` (or `InAir Trace`).
    3.  Adjust the `Reach Distance` property.
    *   **No code changes needed if you're just tweaking values in the Data Asset.**

### **Network Synchronization (Recap for `AAlsCharacter`)**

*   **Replicated Properties**: `bDesiredAiming`, `DesiredRotationMode`, `DesiredStance`, `DesiredGait`, `ViewMode`, `OverlayMode`, `ReplicatedViewRotation`, `InputDirection`, `DesiredVelocityYawAngle`, `RagdollTargetLocation`.
    *   These are declared with `UPROPERTY(..., Replicated...)` or `UPROPERTY(..., ReplicatedUsing = OnRep_...)`.
    *   Listed in `GetLifetimeReplicatedProps` with `DOREPLIFETIME_WITH_PARAMS_FAST` and `bIsPushBased = true`.
    *   Changes on the server (or autonomous client for client-owned properties before an RPC) require `MARK_PROPERTY_DIRTY_FROM_NAME`.
*   **RPCs (`_Implementation` Suffixes)**:
    *   `ServerSetViewMode`, `ServerSetDesiredAiming`, etc.: Client calls these, server executes `_Implementation`.
    *   `ClientSetViewMode`, `ClientSetDesiredAiming`, etc.: Server calls these, specific client executes `_Implementation`.
    *   `MulticastStartRolling`, `MulticastStartMantling`, etc.: Server calls these, server and all clients execute `_Implementation`.
*   **`OnRep_` Functions**:
    *   `OnReplicated_DesiredAiming`, `OnReplicated_ReplicatedViewRotation`, `OnReplicated_OverlayMode`: Called on clients when the corresponding replicated property changes value due to a server update. They typically call Blueprint Native Events or internal logic to react to the change.

**Example: Replicating a New `bIsClimbingLedge` boolean state:**

1.  **`AAlsCharacter.h`**:
    ```cpp
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient, ReplicatedUsing = OnRep_IsClimbingLedge)
    bool bIsClimbingLedge{false};

    UFUNCTION()
    void OnRep_IsClimbingLedge();

    UFUNCTION(BlueprintCallable, Category = "ALS|Character") // If you need to set it externally
    void SetIsClimbingLedge(bool bNewClimbing);

    UFUNCTION(Server, Reliable) // If clients can initiate
    void ServerSetIsClimbingLedge(bool bNewClimbing);
    ```
2.  **`AAlsCharacter.cpp`**:
    ```cpp
    void AAlsCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
    {
        Super::GetLifetimeReplicatedProps(OutLifetimeProps);
        // ... existing DOREPLIFETIME ...
        FDoRepLifetimeParams PushBasedParams;
        PushBasedParams.bIsPushBased = true;
        DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bIsClimbingLedge, PushBasedParams);
    }

    void AAlsCharacter::SetIsClimbingLedge(bool bNewClimbing)
    {
        if (bIsClimbingLedge != bNewClimbing)
        {
            bIsClimbingLedge = bNewClimbing;
            MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, bIsClimbingLedge, this); // Mark dirty for push model

            if (GetLocalRole() == ROLE_AutonomousProxy)
            {
                ServerSetIsClimbingLedge(bIsClimbingLedge);
            }
            // Call OnRep locally if server or autonomous client for immediate effect
            if (GetLocalRole() >= ROLE_AutonomousProxy)
            {
                OnRep_IsClimbingLedge();
            }
        }
    }

    void AAlsCharacter::ServerSetIsClimbingLedge_Implementation(bool bNewClimbing)
    {
        SetIsClimbingLedge(bNewClimbing); // Server updates its state and replicates
    }

    void AAlsCharacter::OnRep_IsClimbingLedge()
    {
        // Logic to execute on clients when bIsClimbingLedge changes
        // e.g., UE_LOG(LogTemp, Warning, TEXT("Client %s: bIsClimbingLedge changed to %s"), *GetName(), bIsClimbingLedge ? TEXT("True") : TEXT("False"));
        // Potentially update animation instance or other visual states
        // OnIsClimbingLedgeChanged(bIsClimbingLedge); // Call a Blueprint event
    }
    ```
*   **GAS Integration**: A `GA_ClimbLedge` ability would call `Character->SetIsClimbingLedge(true)` upon activation and `false` upon ending. It might also apply a `GameplayEffect` that grants a `State.ClimbingLedge` tag.

### **Debugging**

*   **Console Commands**:
    *   `ShowDebug Als.State`: Displays the current Gameplay Tag states of the selected character.
    *   `ShowDebug Als.Shapes`: Visualizes input, velocity, and target rotation vectors.
    *   `ShowDebug Als.Traces`: Shows environment interaction traces (ground prediction, footstep effects).
    *   `ShowDebug Als.Mantling`: Specific traces for the mantling system.
*   **Unreal Editor Tools**:
    *   **Details Panel**: Select your `AAlsCharacter` instance during PIE. You can inspect the live values of all its `UPROPERTY` members, including replicated states and state structs.
    *   **Network Profiler**: (`Window -> Developer Tools -> Network Profiler`) Can help diagnose replication issues, showing what properties are being replicated and how often.
    *   **Gameplay Debugger**: (`'`) Useful for a general overview of actor states, including Gameplay Tags if integrated with GAS.
*   **Logs**: `UE_LOG(LogAls, ...)` or `UE_LOG(LogTemp, ...)` can be inserted into C++ functions to print state information during runtime.
*   **Breakpoints**: Use your C++ IDE's debugger to set breakpoints in functions like `SetDesiredAiming`, RPC implementations (`ServerSetDesiredAiming_Implementation`), or `OnRep_` functions to step through the logic and inspect variable values.














------------------------------------------------------------------------------------------------------------



















## **1-B. `UAlsCharacterMovementComponent` (Custom Movement Logic)**

### **Scope**

This section covers the `UAlsCharacterMovementComponent` class, which is a custom extension of Unreal Engine's built-in `UCharacterMovementComponent`. It's a critical part of the ALS system responsible for handling the low-level physics, collision, and numerical aspects of character movement, as well as synchronizing these with the higher-level ALS character states and animation.

### **Purpose**

The primary purpose of `UAlsCharacterMovementComponent` is to tailor the generic character movement logic provided by Unreal Engine to fit the specific, advanced locomotion requirements of ALS. This includes:
*   Dynamically adjusting movement parameters (speed, acceleration, friction) based on ALS's gait, stance, and rotation modes.
*   Improving network prediction and synchronization for custom movement states.
*   Providing hooks for custom physics interactions, such as those used during rolling or advanced mantling.
*   Allowing temporary blocking of player input.

### **Key Concepts/Components**

1.  **`FAlsCharacterNetworkMoveData`, `FAlsSavedMove`, `FAlsNetworkPredictionData` (Network Prediction)**
    *   **Purpose**: These structs are fundamental for robust multiplayer movement. When a client (e.g., your player) moves, it generates "moves" that are sent to the server. These structs allow ALS to package *additional, custom information* about the character's state (like its current `RotationMode` or `Stance`) along with the standard movement data. This ensures the server, and then other clients, can accurately reconstruct the character's state and movement.
    *   **`FAlsCharacterNetworkMoveData`**: The data structure that defines the specific ALS-related information to be sent in each network move. It includes `RotationMode`, `Stance`, and `MaxAllowedGait`.
    *   **`FAlsSavedMove`**: A client-side struct that "saves" a particular movement frame's data. It stores the values of `RotationMode`, `Stance`, and `MaxAllowedGait` at the time of the move. When the server sends a correction or another client needs to simulate this move, `FAlsSavedMove` can be used to re-apply these states correctly.
    *   **`FAlsNetworkPredictionData`**: Manages the allocation and lifecycle of `FAlsSavedMove` objects for client-side prediction.
    *   **`bIsPushBased = true`**: (Applies to the `AAlsCharacter` properties that are replicated, but `UAlsCharacterMovementComponent` relies on them). This means the networking system only sends updates when properties are explicitly marked "dirty." The custom move data structs in `UAlsCharacterMovementComponent` are essential for this efficient data transfer within the movement system.

2.  **`UAlsMovementSettings` (Data Asset)**
    *   **Purpose**: This `UDataAsset` (configured in the Unreal Editor) is the central repository for defining all of the character's movement parameters based on various ALS states. It contains nested maps for different `RotationMode`s and `Stance`s, each leading to a `FAlsMovementGaitSettings` struct.
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")`**: Allows editing in the editor and read-only access in Blueprints.
    *   **`VelocityAngleToSpeedInterpolationRange`**: Defines a range of angles that influences how directional speed is calculated.
    *   **`RotationModes` (Map)**: A `TMap<FGameplayTag, FAlsMovementStanceSettings>` that organizes gait settings by `RotationMode` (e.g., Velocity Direction, View Direction, Aiming).

3.  **`FAlsMovementStanceSettings` (Struct)**
    *   **Purpose**: Contains a map of `FAlsMovementGaitSettings` for different stances (Standing, Crouching) within a given `RotationMode`.
    *   **`Stances` (Map)**: A `TMap<FGameplayTag, FAlsMovementGaitSettings>` that organizes gait settings by `Stance`.

4.  **`FAlsMovementGaitSettings` (Struct)**
    *   **Purpose**: Defines the actual numerical movement speeds and related physics parameters for a specific combination of `RotationMode` and `Stance`.
    *   **`bAllowDirectionDependentMovementSpeed`**: If true, `WalkBackwardSpeed` and `RunBackwardSpeed` will be used for backward movement, otherwise only forward speeds apply.
    *   **`WalkForwardSpeed`, `WalkBackwardSpeed`, `RunForwardSpeed`, `RunBackwardSpeed`, `SprintSpeed`**: Defines the target speeds for different gaits.
    *   **`AccelerationAndDecelerationAndGroundFrictionCurve`**: A `UCurveVector` that maps the character's `GaitAmount` (a value from 0 to 3) to `MaxAcceleration`, `BrakingDeceleration`, and `GroundFriction`. This allows fine-grained control over how the character feels at different speeds.
    *   **`RotationInterpolationSpeedCurve`**: A `UCurveFloat` that maps `GaitAmount` to the interpolation speed for character rotation.

5.  **`RotationMode`, `Stance`, `MaxAllowedGait` (Internal State Properties)**
    *   **Purpose**: These `FGameplayTag` properties store the *currently active* movement modes within the movement component itself. They are crucial because the movement component's physics calculations (like `GetMaxSpeed()`, `GetMaxAcceleration()`) depend on these values.
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)`**: Visible for debugging in the editor, read-only in Blueprints, and not saved to disk.

6.  **`GaitAmount` (float)**
    *   **Purpose**: A normalized float value (0-3) indicating the character's current movement speed relative to its configured gait speeds.
        *   0: Stopped
        *   0-1: Walking
        *   1-2: Running
        *   2-3: Sprinting
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient, Meta = (ClampMin = 0, ClampMax = 3))`**: Visible for debugging, read-only, clamped.

7.  **`MaxAccelerationWalking`, `BrakingDecelerationWalking`, `GroundFriction` (Internal State Properties)**
    *   **Purpose**: These floats are updated dynamically based on the current `GaitAmount` and the curves defined in `FAlsMovementGaitSettings`. They directly control the character's responsiveness on the ground.

8.  **`bMovementModeLocked` (boolean)**
    *   **Purpose**: A flag that, when true, prevents the `UCharacterMovementComponent` from changing its `MovementMode` (e.g., from `MOVE_Custom` back to `MOVE_Walking`). This is vital for actions like Mantling or Ragdolling, where ALS takes full control of movement.
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)`**: Visible for debugging, read-only.

9.  **`bInputBlocked` (boolean)**
    *   **Purpose**: When true, this flag causes the `UCharacterMovementComponent` to ignore any movement input received from the player or AI controller.
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)`**: Visible for debugging, read-only.

10. **`bAllowImprovedPenetrationAdjustment` (boolean)**
    *   **Purpose**: A setting that, when enabled, allows for potentially better collision resolution when the character starts slightly penetrating a surface, especially with kinematic objects.

### **Methods & Functions**

1.  **`SetMovementSettings(UAlsMovementSettings* NewMovementSettings)`**
    *   **Purpose**: Sets the primary movement settings data asset for this component. This is usually called once during character initialization (`AAlsCharacter::PostInitializeComponents`).
    *   **Usage**: Must be called from C++.
    *   **GAS Integration**: Typically, you wouldn't directly modify this asset during gameplay via GAS. Instead, GAS abilities would change *which* settings are active by altering the character's `DesiredGait`, `DesiredStance`, or `DesiredRotationMode`, which then causes the movement component to *select* appropriate settings from its `MovementSettings` asset.

2.  **`RefreshGaitSettings()`**
    *   **Purpose**: Internal helper function that re-evaluates the `GaitSettings` struct based on the current `RotationMode` and `Stance`. This effectively "selects" the correct set of speed/friction/acceleration curves for the current state.
    *   **Usage**: Called automatically when `RotationMode` or `Stance` changes.

3.  **`SetRotationMode(const FGameplayTag& NewRotationMode)`**
    *   **Purpose**: Updates the movement component's internal `RotationMode`. This automatically triggers `RefreshGaitSettings()`.
    *   **Usage**: Called by `AAlsCharacter` when its `RotationMode` changes. You should not directly call this from external code, but rather rely on `AAlsCharacter::SetRotationMode`.

4.  **`SetStance(const FGameplayTag& NewStance)`**
    *   **Purpose**: Updates the movement component's internal `Stance`. This automatically triggers `RefreshGaitSettings()`.
    *   **Usage**: Called by `AAlsCharacter` when its `Stance` changes (e.g., after `Crouch()` or `UnCrouch()`). You should not directly call this from external code, but rather rely on `AAlsCharacter::SetDesiredStance`.

5.  **`SetMaxAllowedGait(const FGameplayTag& NewMaxAllowedGait)`**
    *   **Purpose**: Sets the maximum gait the character is currently permitted to be in (e.g., a "walk only" ability might limit `MaxAllowedGait` to `AlsGaitTags::Walking`). This directly influences the `MaxWalkSpeed` calculated by the movement component.
    *   **Usage**: Called by `AAlsCharacter::RefreshGait()`.
    *   **GAS Integration**: This is a direct point of integration. See "Adding/Adapting Functionality" below.

6.  **`GetGaitAmount()`**
    *   **Purpose**: Returns the calculated `GaitAmount` (0-3 float), representing how fast the character is moving relative to its gait speeds. This is heavily used by the `UAlsAnimationInstance` to drive animation blends.
    *   **Usage**: Called by `UAlsAnimationInstance`.

7.  **`RefreshGroundedMovementSettings()`**
    *   **Purpose**: Updates `MaxWalkSpeed`, `MaxWalkSpeedCrouched`, `MaxAccelerationWalking`, `BrakingDecelerationWalking`, and `GroundFriction` based on the current `GaitAmount` and the active gait settings curves. It also handles direction-dependent speed if `bAllowDirectionDependentMovementSpeed` is true.
    *   **Usage**: Called internally during `PhysWalking` and `PhysNavWalking` to ensure movement parameters are always up-to-date.

8.  **`SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0)`**
    *   **Purpose**: The standard `UCharacterMovementComponent` function to change the character's movement mode (e.g., `MOVE_Walking`, `MOVE_Falling`, `MOVE_Custom`).
    *   **ALS Adaptation**: In `UAlsCharacterMovementComponent`, this function is *overridden* to respect the `bMovementModeLocked` flag. If `bMovementModeLocked` is true, calls to this function are ignored.
    *   **Usage**: Called by `AAlsCharacter` (e.g., when entering Mantling or Ragdolling, which sets `MOVE_Custom`).
    *   **Always call `Super::SetMovementMode()` in C++ when overriding this to preserve base functionality.**

9.  **`SetMovementModeLocked(bool bNewMovementModeLocked)`**
    *   **Purpose**: Toggles the `bMovementModeLocked` flag. When locked, the character's movement mode cannot be changed by the `SetMovementMode` function.
    *   **Usage**: Called by `AAlsCharacter` to temporarily restrict movement mode changes during specific actions (e.g., Mantling, Ragdolling).
    *   **GAS Integration**: An ability could call this directly if it needs to ensure the character's movement mode remains static for its duration (e.g., a "grapple" ability where the character cannot be interrupted into falling).

10. **`SetInputBlocked(bool bNewInputBlocked)`**
    *   **Purpose**: Toggles the `bInputBlocked` flag. When true, the movement component completely ignores character input, causing the character to stop moving (unless external forces or root motion are applied).
    *   **Usage**: Called by `AAlsCharacter` (e.g., when entering Mantling or Getting Up). `AAlsCharacter::SetLocomotionAction` automatically sets this based on the new action.
    *   **GAS Integration**: This is a direct and powerful point of integration. See "Adding/Adapting Functionality" below.

11. **`OnPhysicsRotation` (Delegate)**
    *   **Purpose**: A multicast delegate (`FAlsPhysicsRotationDelegate`) that is broadcast every physics tick. This provides a hook for other systems (like `AAlsCharacter`'s `RefreshRollingPhysics`) to execute logic *after* the movement component has updated its internal physics rotation, but before the character's final transform is set.
    *   **Usage**: Subscribers (`AAlsCharacter::CharacterMovement_OnPhysicsRotation`) can bind to this delegate.
    *   **GAS Integration**: An ability could potentially bind to this delegate for very precise, physics-tick-based modifications, although this is a more advanced use case.

12. **`TryConsumePrePenetrationAdjustmentVelocity(FVector& OutVelocity)`**
    *   **Purpose**: Used internally by `AAlsCharacter` to get the velocity the character had *before* any penetration adjustments were made by the movement component. This is important for "desired velocity" logic (`bRotateTowardsDesiredVelocityInVelocityDirectionRotationMode`), where ALS wants to rotate towards the intended movement direction, even if a small collision caused a temporary deviation.
    *   **Usage**: Called by `AAlsCharacter::RefreshLocomotion()`.

### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

#### **1. Adding a New Movement Trait / Condition through `UAlsMovementSettings`**

Let's say you want to introduce a "Wet" condition that affects how fast the character can move and how quickly they accelerate/decelerate.

**A. Define New Gameplay Tag (if applicable)**
*   For a simple "Wet" condition, you might add `Als.Condition.Wet` to `AlsGameplayTags.h` and `AlsGameplayTags.cpp`.

**B. Extend `UAlsMovementSettings`**
*   You'd need to add a new `TMap` to `UAlsMovementSettings` to store gait settings specific to this "Wet" condition. This requires C++ modification of `UAlsMovementSettings`.
    ```cpp
    // AlsMovementSettings.h
    // ...
    USTRUCT(BlueprintType)
    struct ALS_API FAlsWetConditionSettings // New struct to hold wet gait settings
    {
        GENERATED_BODY()
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceInlineRow))
        TMap<FGameplayTag, FAlsMovementStanceSettings> RotationModes // Re-use existing structure
        {
            {AlsRotationModeTags::VelocityDirection, {}},
            {AlsRotationModeTags::ViewDirection, {}},
            {AlsRotationModeTags::Aiming, {}}
        };
    };

    UCLASS(Blueprintable, BlueprintType)
    class ALS_API UAlsMovementSettings : public UDataAsset
    {
        GENERATED_BODY()
        // ... existing properties ...
        UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
        FAlsWetConditionSettings WetConditions; // New property
    };
    ```
*   **Compile C++**.
*   In the editor, open your `UAlsMovementSettings` data asset. You will now see a new `Wet Conditions` section where you can configure speeds/curves for when the character is wet.

**C. Adapt `UAlsCharacterMovementComponent` to use the New Setting**
*   Modify `UAlsCharacterMovementComponent::RefreshGaitSettings()` to check for the "Wet" condition and prioritize its settings.
    ```cpp
    // AlsCharacterMovementComponent.cpp
    void UAlsCharacterMovementComponent::RefreshGaitSettings()
    {
        // ... existing checks ...

        const auto* RotationModeSettings{MovementSettings->RotationModes.Find(RotationMode)};
        GaitSettings = ALS_ENSURE(RotationModeSettings != nullptr) ? *RotationModeSettings->Stances.Find(Stance) : FAlsMovementGaitSettings{};

        // --- NEW LOGIC FOR WET CONDITION ---
        AAlsCharacter* AlsCharacter = Cast<AAlsCharacter>(CharacterOwner);
        if (IsValid(AlsCharacter) && AlsCharacter->HasMatchingGameplayTag(AlsConditionTags::Wet)) // Assuming a GameplayTag on character
        {
            const auto* WetRotationModeSettings = MovementSettings->WetConditions.RotationModes.Find(RotationMode);
            if (WetRotationModeSettings)
            {
                const FAlsMovementGaitSettings* WetGaitSettings = WetRotationModeSettings->Stances.Find(Stance);
                if (WetGaitSettings)
                {
                    // Merge or override existing GaitSettings with wet-specific settings
                    // For simplicity, let's just override here. A more complex system might blend.
                    GaitSettings = *WetGaitSettings;
                }
            }
        }
        // --- END NEW LOGIC ---
    }
    ```
*   **Compile C++**.

**D. GAS Integration (Applying the "Wet" Condition)**
*   **C++ (Gameplay Ability)**:
    ```cpp
    // MyGameplayAbility_Wet.h
    UCLASS()
    class UMyGameplayAbility_Wet : public UGameplayAbility
    {
        GENERATED_BODY()
    public:
        UPROPERTY(EditDefaultsOnly, Category = "Wet")
        TSubclassOf<UGameplayEffect> WetEffectClass;

        virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
        virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bWasCancelled) override;
    };

    // MyGameplayAbility_Wet.cpp
    void UMyGameplayAbility_Wet::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
    {
        if (CommitAbility(Handle, ActorInfo, ActivationInfo))
        {
            FGameplayEffectContextHandle Context = ActorInfo->AbilitySystemComponent->Make )->MakeEffectContext();
            FGameplayEffectSpecHandle SpecHandle = ActorInfo->AbilitySystemComponent->MakeOutgoingSpec(WetEffectClass, GetAbilityLevel(), Context);
            if (SpecHandle.IsValid())
            {
                // Apply the GameplayEffect that grants the "Wet" GameplayTag
                FActiveGameplayEffectHandle ActiveGEHandle = ActorInfo->AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
                // Store ActiveGEHandle if you need to remove it later
            }
        }
    }
    void UMyGameplayAbility_Wet::EndAbility(...)
    {
        // Remove the GameplayEffect that granted the "Wet" GameplayTag
    }
    ```
*   **Blueprint (Gameplay Effect)**:
    *   Create a new Blueprint class derived from `UGameplayEffect` (e.g., `GE_Wet`).
    *   In `GE_Wet`, add `Als.Condition.Wet` to `GrantedTags` or `OngoingTagRequirements`.
    *   This Gameplay Effect would be applied by your `UMyGameplayAbility_Wet`. When the character gains this tag, `UAlsCharacterMovementComponent` will use the "Wet" gait settings.

#### **2. Using `SetInputBlocked()` for Ability Control**

This is an excellent way to temporarily halt player movement during channeled abilities, cinematic actions, or stuns.

*   **C++ (Gameplay Ability)**:
    ```cpp
    // GA_StunAbility.h
    UCLASS()
    class UGA_StunAbility : public UGameplayAbility
    {
        GENERATED_BODY()
    public:
        virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
        virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bWasCancelled) override;
    };

    // GA_StunAbility.cpp
    #include "AlsCharacter.h"
    #include "AlsCharacterMovementComponent.h"

    void UGA_StunAbility::ActivateAbility(...)
    {
        AAlsCharacter* Character = Cast<AAlsCharacter>(ActorInfo->AvatarActor.Get());
        if (Character && Character->GetLocalRole() == ROLE_Authority) // Only apply blocking on server
        {
            // Set input blocked on server, which will replicate
            Character->GetAlsCharacterMovementComponent()->SetInputBlocked(true);
            // Optionally: Play stun animation (e.g., by setting a GameplayTag that triggers an AnimBP state)
            // Or play a montage directly through ASC.
        }
        // Set a timer for duration of stun, then end ability
        K2_EndAbility(); // Or use a duration GE
    }

    void UGA_StunAbility::EndAbility(...)
    {
        AAlsCharacter* Character = Cast<AAlsCharacter>(ActorInfo->AvatarActor.Get());
        if (Character && Character->GetLocalRole() == ROLE_Authority)
        {
            Character->GetAlsCharacterMovementComponent()->SetInputBlocked(false);
        }
    }
    ```
*   **Blueprint (Gameplay Ability)**:
    *   Create a Blueprint Gameplay Ability (BGA).
    *   In the `ActivateAbility` event: Get `Character Owner` -> Cast to `AlsCharacter` -> Get `Als Character Movement Component` -> Call `Set Input Blocked (true)`.
    *   In the `EndAbility` event: Call `Set Input Blocked (false)`.
    *   You would typically apply a `GameplayEffect` that prevents movement as well (e.g., by adding a tag like `State.Stunned` that disables input in `CharacterMovementComponent`). The `SetInputBlocked` directly interacts with ALS's custom input consumption.

#### **3. Adding a Custom Movement Mode for Abilities (e.g., "Vaulting" or "Climbing")**

While ALS has Mantling, a simpler "Vaulting" action or a more continuous "Climbing" might use a custom movement mode.

*   **A. Define Custom Movement Mode**
    *   In `AlsCharacterMovementComponent.h`, add a new enum value to `EAlsCustomMovementMode::Type`:
        ```cpp
        // AlsCharacterMovementComponent.h
        UENUM()
        namespace EAlsCustomMovementMode
        {
            enum Type
            {
                CM_None = 0,
                CM_Sliding,
                CM_Vaulting, // New custom mode
                CM_MAX
            };
        }
        ```
*   **B. Implement `PhysCustom` Logic**
    *   Override `PhysCustom` in `UAlsCharacterMovementComponent.cpp` and add your vaulting/climbing physics logic under `case EAlsCustomMovementMode::CM_Vaulting:`.
        ```cpp
        // AlsCharacterMovementComponent.cpp
        void UAlsCharacterMovementComponent::PhysCustom(float DeltaTime, int32 Iterations)
        {
            Super::PhysCustom(DeltaTime, Iterations); // Always call Super!

            AAlsCharacter* AlsChar = Cast<AAlsCharacter>(CharacterOwner);
            if (!IsValid(AlsChar)) return;

            switch (CustomMovementMode)
            {
                case EAlsCustomMovementMode::CM_Vaulting:
                    // Your vaulting/climbing physics logic here.
                    // This might involve:
                    // 1. Moving the character along a predefined path (e.g., from a SplineComponent on the obstacle).
                    // 2. Applying forces or directly setting velocity.
                    // 3. Checking for the end of the vault and transitioning back to MOVE_Walking.
                    // 4. Updating a state struct on AAlsCharacter for animation.

                    // Example: Simple linear movement for a fixed duration
                    // AlsChar->VaultState.CurrentVaultTime += DeltaTime; // Assume AlsChar->VaultState exists
                    // if (AlsChar->VaultState.CurrentVaultTime < AlsChar->VaultState.VaultDuration)
                    // {
                    //     FVector CurrentVelocity = AlsChar->GetActorForwardVector() * 200.0f; // Example speed
                    //     MoveUpdatedComponent(CurrentVelocity * DeltaTime, UpdatedComponent->GetComponentQuat(), false);
                    // }
                    // else
                    // {
                    //     // End the vault, transition back to walking
                    //     AlsChar->SetLocomotionAction(FGameplayTag::EmptyTag); // Clear action
                    //     SetMovementModeLocked(false);
                    //     SetMovementMode(MOVE_Walking);
                    // }
                    break;
                // ... other custom modes
            }
        }
        ```
*   **C. GAS Integration (Activating/Deactivating Vaulting)**
    *   **Gameplay Ability (C++ or Blueprint)**:
        ```cpp
        // GA_Vault.h
        UCLASS()
        class UGA_Vault : public UGameplayAbility
        {
            GENERATED_BODY()
        public:
            virtual void ActivateAbility(...) override;
            virtual void EndAbility(...) override;

            UPROPERTY(EditDefaultsOnly, Category = "Vaulting")
            UAnimMontage* VaultMontage; // Reference to your vault animation

            UPROPERTY(EditDefaultsOnly, Category = "Vaulting")
            FGameplayTag VaultActionTag; // Als.LocomotionAction.Vaulting
        };

        // GA_Vault.cpp
        void UGA_Vault::ActivateAbility(...)
        {
            AAlsCharacter* Character = Cast<AAlsCharacter>(ActorInfo->AvatarActor.Get());
            if (Character && Character->GetLocalRole() == ROLE_Authority)
            {
                // Check if can vault (e.g., trace for obstacle)
                // if (!Character->CanVault()) { K2_EndAbility(); return; }

                // Lock movement mode and set custom mode
                Character->GetAlsCharacterMovementComponent()->SetMovementModeLocked(true);
                Character->GetCharacterMovement()->SetMovementMode(MOVE_Custom, EAlsCustomMovementMode::CM_Vaulting);

                // Set LocomotionAction for animation
                Character->SetLocomotionAction(VaultActionTag);

                // Play montage. Use AbilitySystemComponent to play it for replication.
                float Duration = PlayMontage(this, VaultMontage, 1.0f); // Plays and replicates
                if (Duration > 0.0f)
                {
                    // End ability after montage completes or set a timer
                    FTimerHandle TimerHandle;
                    Character->GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::K2_EndAbility), Duration, false);
                }
                else
                {
                    K2_EndAbility(); // Failed to play montage
                }
            }
            else { K2_EndAbility(); }
        }

        void UGA_Vault::EndAbility(...)
        {
            AAlsCharacter* Character = Cast<AAlsCharacter>(ActorInfo->AvatarActor.Get());
            if (Character && Character->GetLocalRole() == ROLE_Authority && Character->GetLocomotionAction() == VaultActionTag)
            {
                Character->SetLocomotionAction(FGameplayTag::EmptyTag); // Clear action
                Character->GetAlsCharacterMovementComponent()->SetMovementModeLocked(false);
                Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking); // Revert to normal
            }
            Super::EndAbility(...); // Always call super!
        }
        ```
    *   In the Vaulting `UAnimMontage`, you might use `Als Anim Notify State Set Locomotion Action` to set the `LocomotionAction` tag, ensuring the correct state is reflected for animation.

### **Network Synchronization**

`UAlsCharacterMovementComponent` plays a crucial role in multiplayer by extending Unreal's movement replication.

*   **`FAlsCharacterNetworkMoveData`, `FAlsSavedMove`**: These are the key structures. When a client performs a move, `FAlsSavedMove::SetMoveFor` collects the current `RotationMode`, `Stance`, and `MaxAllowedGait` from the movement component. This data is then packed into `FAlsCharacterNetworkMoveData` and sent to the server via `FAlsCharacterNetworkMoveData::Serialize`.
*   **Server Processing (`MoveAutonomous`)**: When the server receives a client's move, `UAlsCharacterMovementComponent::MoveAutonomous` is called. It extracts `RotationMode`, `Stance`, and `MaxAllowedGait` from the received `MoveData` and updates the server's local movement component state (`this->RotationMode = MoveData->RotationMode;` etc.). It then calls `RefreshGaitSettings()` to ensure the server's movement calculations are based on the correct parameters.
*   **Client Prediction Data (`GetPredictionData_Client`)**: `UAlsCharacterMovementComponent` overrides `GetPredictionData_Client` to return an `FAlsNetworkPredictionData` instance, which allocates `FAlsSavedMove` objects. This allows the client to locally predict its movement using the correct ALS states.
*   **`CombineWith`**: `FAlsSavedMove::CombineWith` is overridden. It's a subtle but important point for animation: ALS has custom rotation logic on `AAlsCharacter`. To prevent the engine's default `CombineWith` from overwriting the character's rotation with older, saved data, ALS temporarily "cheats" by making the old move's rotation match the current one, calls `Super::CombineWith`, and then restores the original old rotation. This ensures ALS's rotation logic remains dominant.

### **Debugging**

*   **Character Debug Displays**: The `AAlsCharacter` debug displays (`ShowDebug Als.State`, `ShowDebug Als.Shapes`) directly show the `RotationMode`, `Stance`, `Gait`, and the calculated movement vectors, which are all influenced by `UAlsCharacterMovementComponent`.
*   **Console Variables**:
    *   `p.NetShowCorrections 1`: Shows client-side movement corrections, which can help diagnose replication issues related to custom movement modes.
    *   `ai.debugpath 1`: Can show AI navigation paths, which interact with `PhysNavWalking`.
*   **Visualizing Curves**: The `ShowDebug Als.Curves` option in `AAlsCharacter` displays the `AccelerationAndDecelerationAndGroundFrictionCurve` and `RotationInterpolationSpeedCurve` values in real-time, allowing you to see how `GaitAmount` maps to these parameters.
*   **`UAlsCharacterMovementComponent` Details Panel**: During PIE, select your character and inspect the `Als Character Movement` section in the Details panel. You can see the live values of `RotationMode`, `Stance`, `MaxAllowedGait`, `GaitAmount`, `bMovementModeLocked`, `bInputBlocked`, and the dynamically updated `MaxAccelerationWalking`, `BrakingDecelerationWalking`, and `GroundFriction`. This is invaluable for seeing exactly what parameters are active.
*   **Logs**: Keep an eye on the Output Log for warnings or errors related to movement, especially if `ALS_ENSURE` macros are triggered.










---------------------------------------------------------------------------------------------------------
















## **1-C. `UAlsAnimationInstance` (Main Animation Blueprint Logic)**

### **Scope**

This section covers the `UAlsAnimationInstance` class, which is the direct C++ parent class for the main animation blueprint that drives the character's visual representation. It acts as an intermediary, taking the high-level `AAlsCharacter` state and translating it into granular animation parameters, blends, and event triggers. This section also addresses the broader topic of integrating character animations with the Gameplay Ability System (GAS).

### **Purpose**

`UAlsAnimationInstance` serves as the primary animation logic hub for ALS characters. Its responsibilities include:
*   Consuming and processing the `AAlsCharacter`'s current state (view mode, locomotion mode, stance, gait, actions).
*   Calculating and updating animation-specific state structs (`FAlsLayeringState`, `FAlsPoseState`, `FAlsFeetState`, etc.) that are then used by the animation graph (AnimGraph) to blend animations.
*   Managing complex animation features like foot IK, dynamic transitions, rotate-in-place, and turn-in-place.
*   Providing the necessary input data for Control Rig.
*   Handling animation notifies and related events.
*   Optimizing animation updates by leveraging multi-threading.

### **Key Concepts/Components**

1.  **Animation Update Phases (`Native...Animation` functions)**
    *   **`NativeInitializeAnimation()`**: Called once when the animation instance is initialized. Used to get initial references (e.g., to `AAlsCharacter`).
    *   **`NativeBeginPlay()`**: Called when the game starts. Used to validate settings and references.
    *   **`NativeUpdateAnimation(float DeltaTime)`**: This is the main **game thread** update function. It's responsible for fetching the latest character state from `AAlsCharacter` and updating the `UAlsAnimationInstance`'s internal properties that are **not thread-safe** (e.g., `Character` pointers, certain network-related states).
    *   **`NativeThreadSafeUpdateAnimation(float DeltaTime)`**: This is the main **worker thread** (multi-threaded) update function. It's designed to perform computationally intensive calculations that are safe to run in parallel with the game thread. This is where most of ALS's animation state calculations occur (`RefreshLayering`, `RefreshPose`, `RefreshLook`, `RefreshFeet`, `RefreshTransitions`).
    *   **`NativePostUpdateAnimation()`**: Called on the **game thread** after `NativeThreadSafeUpdateAnimation` and after the animation graph has evaluated. It's used for any final updates or to play montages (montages *must* be played on the game thread).

2.  **State Structs for Animation (`FAls...State`)**
    *   These are the internal data structures that `UAlsAnimationInstance` updates every tick. They are *read-only* for the Animation Graph and Blueprints.
    *   **`FAlsLayeringState`**: Controls blending amounts for different body parts to apply layered animations (e.g., full body, upper body, lower body).
    *   **`FAlsPoseState`**: Represents the character's general pose (e.g., `GroundedAmount`, `InAirAmount`, `GaitAmount`, `StandingAmount`, `CrouchingAmount`). These are often floats (0-1 or 0-3) used to blend between different base poses.
    *   **`FAlsViewAnimationState`**: Animation-specific view parameters (e.g., `YawAngle`, `PitchAngle`, `LookAmount`).
    *   **`FAlsSpineState`**: Manages spine rotation based on aiming or view modes.
    *   **`FAlsLookState`**: Manages head and eye look-at behavior.
    *   **`FAlsLocomotionAnimationState`**: Detailed locomotion data specifically formatted for animation (e.g., `VelocityBlend`, `Acceleration`).
    *   **`FAlsLeanState`**: Stores lean amounts for character body lean while moving.
    *   **`FAlsGroundedState`**: Contains states specific to grounded locomotion (e.g., `HipsDirection`, `VelocityBlend`, `MovementDirection`, `RotationYawOffsets`).
    *   **`FAlsStandingState`**: State for standing locomotion (e.g., `StrideBlendAmount`, `PlayRate`, `SprintAccelerationAmount`).
    *   **`FAlsCrouchingState`**: State for crouching locomotion (e.g., `StrideBlendAmount`, `PlayRate`).
    *   **`FAlsInAirState`**: State for in-air locomotion (e.g., `VerticalVelocity`, `GroundPredictionAmount`, `JumpPlayRate`).
    *   **`FAlsFeetState`**: Detailed state for foot placement, including target and locked locations/rotations, and `FootPlantedAmount`.
    *   **`FAlsTransitionsState`, `FAlsDynamicTransitionsState`**: Manage the state of various animation transitions like quick stops and dynamic foot adjustments.
    *   **`FAlsRotateInPlaceState`, `FAlsTurnInPlaceState`**: Manage the state of these specific idle rotation animations.
    *   **`FAlsRagdollingAnimationState`**: Manages animation aspects of ragdolling (e.g., `FinalRagdollPose`, `FlailPlayRate`).

3.  **Animation Curves (Constants)**
    *   ALS extensively uses animation curves (float curves) embedded within animation assets. These curves provide a powerful way for animators to precisely control blending, timing, and weighting without requiring programmer intervention for every tweak.
    *   `UAlsConstants.h` defines the `FName`s for all standard ALS animation curves (e.g., `LayerHead`, `PoseGait`, `FootLeftIk`).
    *   `UAlsAnimationModifier_CreateCurves` and `UAlsAnimationModifier_CreateLayeringCurves` are editor tools to easily add these curves to your animations.

4.  **Control Rig Input (`FAlsControlRigInput`)**
    *   **Purpose**: This struct is passed from `UAlsAnimationInstance` to the Control Rig Graph. It provides the Control Rig with all the necessary character and animation state data to perform procedural adjustments like foot IK and spine adjustments.
    *   **`UAlsAnimationInstance::GetControlRigInput()`**: A BlueprintCallable function that returns this struct.

### **Methods & Functions**

1.  **Core Update Pipeline**:
    *   `NativeInitializeAnimation()`, `NativeBeginPlay()`, `NativeUpdateAnimation()`, `NativeThreadSafeUpdateAnimation()`, `NativePostUpdateAnimation()`: Explained in "Key Concepts" above. These are fundamental overrides for the animation instance's lifecycle and update loop.
    *   **Always call `Super::` at the appropriate point in these overrides!** (e.g., `Super::NativeUpdateAnimation(DeltaTime);`)

2.  **State Refresh Functions (Internal)**:
    *   `RefreshMovementBaseOnGameThread()`: Updates movement base information.
    *   `RefreshLayering()`, `RefreshPose()`: Calculate blend amounts for various animation layers and poses based on character state and curves.
    *   `RefreshViewOnGameThread()`, `RefreshView(float DeltaTime)`: Updates view-related states and spine rotation.
    *   `RefreshLocomotionOnGameThread()`, `RefreshLocomotion()`: Updates locomotion states.
    *   `RefreshGrounded()`, `RefreshGroundedLean()`, `RefreshVelocityBlend()`, `RefreshGroundedMovement()`: Calculations for grounded locomotion.
    *   `RefreshInAir()`, `RefreshGroundPrediction()`, `RefreshInAirLean()`: Calculations for in-air locomotion.
    *   `RefreshFeetOnGameThread()`, `RefreshFeet()`, `RefreshFoot()`, `ProcessFootLockTeleport()`, `ProcessFootLockBaseChange()`, `RefreshFootLock()`: Detailed logic for foot placement, IK, and locking.
    *   `RefreshRagdollingOnGameThread()`: Updates ragdoll animation states.
    *   `RefreshTransitions()`, `RefreshDynamicTransitions()`: Manage dynamic animation transitions.
    *   `RefreshRotateInPlace()`, `RefreshTurnInPlace()`: Manage idle rotation animations.

3.  **Public API for Actions/Events (Called by `AAlsCharacter` or Abilities)**:
    *   **`MarkPendingUpdate()`**: Flags the animation instance as needing a full state refresh. Useful when a character appears in the world or teleports.
    *   **`MarkTeleported()`**: Sets a timestamp for the last teleportation. Used for foot lock glitches and camera lag.
    *   **`SetGroundedEntryMode(const FGameplayTag& NewGroundedEntryMode)`**: Sets the grounded entry mode for specific land/roll transitions.
    *   **`ResetGroundedEntryMode()`**: Clears the grounded entry mode.
    *   **`SetHipsDirection(EAlsHipsDirection NewHipsDirection)`**: Sets the hips direction for grounded locomotion.
    *   **`ResetPivot()`**: Resets the pivot active state.
    *   **`Jump()`**: Flags a jump request for in-air animation.
    *   **`PlayQuickStopAnimation()`**: Plays a quick stop montage.
    *   **`PlayTransitionAnimation(...)`, `PlayTransitionLeftAnimation(...)`, `PlayTransitionRightAnimation(...)`**: Functions to play specific transition montages. These are often used by `AAlsCharacter` but can be called by other systems.
    *   **`StopTransitionAndTurnInPlaceAnimations()`**: Stops currently playing transition or turn-in-place montages.
    *   **`SnapshotFinalRagdollPose()`**: Captures the current skeletal mesh pose for blending out of ragdoll.
    *   **`GetControlRigInput()`**: Returns the `FAlsControlRigInput` struct for Control Rig.
    *   **`GetCurveValueClamped01(const FName& CurveName)`**: Utility to get a float curve value clamped between 0 and 1.

### **Integration with GAS (Gameplay Ability System)**

GAS is a powerful framework for defining and executing gameplay mechanics. Integrating ALS animations with GAS involves using Gameplay Tags and Montages as the primary bridge.

#### **What is already accomplished in C++ relevant to GAS Integration**

1.  **Gameplay Tag-Driven States**:
    *   As discussed, `AAlsCharacter` already exposes many of its key states as `FGameplayTag` properties (e.g., `LocomotionMode`, `RotationMode`, `Stance`, `Gait`, `OverlayMode`, `LocomotionAction`).
    *   **How it helps GAS**:
        *   **Gameplay Effects (GEs)**: GEs can grant or apply tags to the `UAbilitySystemComponent` (ASC) on the `AAlsCharacter`. `UAlsAnimationInstance` reads these tags (indirectly, via `AAlsCharacter`) and uses them to drive animation logic.
        *   **Gameplay Abilities (GAs)**: GAs can check for or require these tags to activate, and they can apply GEs to grant/remove these tags.
    *   **Example**: A "Disarm" ability (GA) could apply a GE that grants the `Als.OverlayMode.HandsTied` tag. `UAlsAnimationInstance` detects this tag and blends to the "Hands Tied" overlay animation.

2.  **`LocomotionAction` Tag**:
    *   This is a special `FGameplayTag` property in `AAlsCharacter` that indicates a "special" character action (Mantling, Rolling, Ragdolling, Getting Up).
    *   **How it helps GAS**:
        *   **Ability Activation Conditions**: GAs can check `Character->GetLocomotionAction()` to prevent activation while the character is Mantling, Rolling, or Ragdolling (e.g., cannot attack while mantling).
        *   **Driving Animation Blueprints**: `UAlsAnimationInstance` directly uses `LocomotionAction` to transition to and blend specialized animation states (e.g., Mantling State Machine, Ragdoll physics blend).

3.  **Animation Notifies and Notify States**:
    *   **`UAlsAnimNotifyState_SetLocomotionAction`**: This Notify State is critical for GAS. When placed on an `UAnimMontage` (often played by a GA), it sets `AAlsCharacter::LocomotionAction` to a specified `GameplayTag` for the duration of the notify. This is a very clean way to signal to the entire ALS animation system that a custom action is active.
    *   **`UAlsAnimNotifyState_EarlyBlendOut`**: Also vital for GAS-driven montages. It allows an `UAnimMontage` to automatically blend out early if certain conditions (like `bHasInput`, `LocomotionMode`, `RotationMode`, `Stance`) change. This is essential for making GAS abilities feel responsive and interruptible.
    *   **How it helps GAS**: GAs often play montages. These Notify States ensure that the character's core state and animation blends correctly respond to the montage's playback, and that the montage can gracefully exit when gameplay conditions change.

#### **What Must Be Done in Blueprints to Add New Animations and Integrate with GAS**

The primary interface for animators and designers will be the Animation Blueprint, which uses `UAlsAnimationInstance` as its parent class.

1.  **Animation Assets (Sequences, Montages)**
    *   **Import Animations**: Import your new animation sequences (e.g., FBX files for a new "Punch" attack, a "Block" pose, a "Throw" action).
    *   **Create Animation Montages**: For any action that requires a specific, often interruptible, full-body animation (like attacks, rolls, interacts, reloads), create an `UAnimMontage` from your animation sequence. Montages are the preferred way for GAS abilities to play animations.
        *   **Setting up Slots**: Ensure your Montages are set to play in appropriate slots (e.g., "FullBody" slot, "UpperBody" slot). ALS already defines "Transition" slots, "TurnInPlaceStanding", "TurnInPlaceCrouching" slots. You might add "FullBodyAction" or "UpperBodyAttack" slots.

2.  **Animation Blueprint Setup (using `UAlsAnimationInstance` as parent)**
    *   **State Machines**: Use State Machines in your AnimGraph (e.g., "Locomotion" State Machine, "Combat" State Machine) to manage different animation sets. Transitions between states can be driven by `UAlsAnimationInstance` properties (e.g., `LocomotionMode`, `Gait`, `LocomotionAction`).
    *   **Blend Spaces**: Create Blend Spaces (e.g., `BS_WalkRunSprint`) to blend animations based on character speed and direction. `UAlsAnimationInstance` provides `GaitAmount` and `VelocityBlend` for this.
    *   **Layered Blends per Bone**: Use "Layered Blend Per Bone" nodes extensively to layer animations.
        *   **Lower Body**: Base locomotion (walking, running).
        *   **Upper Body**: Aiming, overlays (weapon holding), attacks.
        *   **ALS Layers**: `UAlsAnimationInstance` exposes blend amounts for various layers (e.g., `LayeringState.HeadBlendAmount`, `LayeringState.ArmRightBlendAmount`) that you can directly use in your AnimGraph. This allows granular control over how much an overlay animation affects different body parts.
    *   **Control Rig Integration**:
        *   **Purpose**: Control Rig allows for procedural adjustments to bones, perfect for dynamic IK. ALS's foot IK and hand IK retargeting are done here.
        *   **How to use**: In your AnimGraph, drag off a pose and add a "Control Rig" node. Set the Control Rig asset (e.g., `CR_ALS_Character`).
        *   **Connecting ALS Data**: The `FAlsControlRigInput` struct provided by `UAlsAnimationInstance::GetControlRigInput()` will be the primary input for your Control Rig. Connect the pins (e.g., `FootLeftLocation`, `FootLeftRotation`, `SpineYawAngle`) to corresponding input variables in your Control Rig graph.
        *   **Adjusting Current Animations (Foot IK)**: For *any* animation you add, the ALS Control Rig's foot IK units (`AlsRigUnit_FootOffsetTrace`, `AlsRigUnit_ApplyFootOffsetLocation`, `AlsRigUnit_ApplyFootOffsetRotation`) will automatically adjust the feet to the ground plane, preventing sliding and floating, *as long as you have the necessary foot IK curves* (`FootLeftIk`, `FootRightIk`, `FootLeftLock`, `FootRightLock`) set up in your animation sequences using the `UAlsAnimationModifier_CreateCurves` tool.
    *   **Anim Notifies (Blueprint)**:
        *   **Footstep Effects**: `Als Anim Notify Footstep Effects` is an essential Blueprint notify to trigger sounds, decals, and particles at footfall.
        *   **Custom Anim Notifies**: Add your own `AnimNotify` events (e.g., "AttackHit", "ReloadFinished"). These can then be used in Blueprints (Event Graph) to trigger GAS abilities (e.g., calling `TryActivateAbilityByClass`) or to tell an active ability to advance its logic (`AbilitySystemComponent->SendGameplayEventToActor`).
    *   **Blueprint Event Graph Logic**:
        *   Override `Event Blueprint Update Animation` (which corresponds to `NativeUpdateAnimation`).
        *   Override `Event Blueprint Thread Safe Update Animation` (corresponds to `NativeThreadSafeUpdateAnimation`). Use this for expensive, thread-safe calculations.
        *   Override `Event Blueprint Post Update Animation` (corresponds to `NativePostUpdateAnimation`).
        *   Use these events to read character state, perform complex conditional logic for blending, and trigger actions.

### **GAS Integration (Detailed)**

Here's a more in-depth look at how GAS directly interacts with ALS for new features and common usages.

#### **1. Driving Animation States with Gameplay Tags**

*   **Scenario**: A "Combat Mode" ability that changes the character's posture and weapon stance.
*   **GAS Implementation**:
    *   Create a `GameplayEffect` (e.g., `GE_CombatStance`) that grants `Als.OverlayMode.Rifle` and `Als.RotationMode.Aiming` tags to the character's ASC.
    *   Create a `GameplayAbility` (e.g., `GA_ToggleCombatMode`) that applies/removes `GE_CombatStance`.
*   **ALS Animation Blueprint**:
    *   The AnimBP, parented to `UAlsAnimationInstance`, will read `OverlayMode` and `RotationMode` properties.
    *   It will have a "Combat Mode" State Machine or Layered Blend per Bone set up that is activated when `OverlayMode` is `Als.OverlayMode.Rifle`.
    *   The aiming pose/logic will be activated when `RotationMode` is `Als.RotationMode.Aiming`.
*   **Outcome**: When `GA_ToggleCombatMode` is activated, the character seamlessly transitions to a combat-ready stance, holding the rifle, and aiming behavior is enabled, all without direct animation calls from the ability.

#### **2. Playing Montages via GAS**

*   **Core Principle**: GAS abilities should play animation montages through the `UAbilitySystemComponent` (`ASC`). This automatically handles replication of the montage and often provides built-in callbacks for when the montage ends or receives notifies.
    *   **C++ Example**: `UAbilitySystemComponent->PlayMontage(UGameplayAbility* InAbility, FGameplayAbilityActivationInfo ActivationInfo, UAnimMontage* MontageToPlay, float InPlayRate, FName StartSectionName = NAME_None, float InStartTime = 0.0f)`.
    *   **Blueprint Example**: `Play Montage` node available in Blueprint Gameplay Abilities.

*   **Scenario**: A "Heavy Attack" ability with a unique animation.
*   **GAS Implementation**:
    *   In your `GA_HeavyAttack` ability (C++ or Blueprint):
        *   On `ActivateAbility`, call `PlayMontage` on the ASC, referencing your `AM_HeavyAttack` montage.
        *   Use the `AnimNotifyState_SetLocomotionAction` on `AM_HeavyAttack` to set `LocomotionAction` to `Als.LocomotionAction.Attack` (a new tag you define). This informs the ALS system that the character is performing an attack.
        *   Use `AnimNotifyState_EarlyBlendOut` on `AM_HeavyAttack` to allow the attack animation to be interrupted if the character, for example, is `Stunned` (checks for a `Stunned` tag or `bInputBlocked`).
        *   Place a custom `AnimNotify` (e.g., "AttackDealDamage") at the point of impact in the montage. In the GA, use `Wait for Anim Notify` or `Wait for Montage End` (for simple abilities) to drive the ability's logic.
*   **ALS Animation Blueprint**:
    *   Have a "Combat" State Machine in your AnimBP. A transition to an "Attack" state can be triggered when `LocomotionAction` is `Als.LocomotionAction.Attack`.
    *   The "Attack" state would then play the `AM_HeavyAttack` montage via a `Slot` node.
*   **Outcome**: The ability triggers the animation. The `LocomotionAction` ensures the character blends smoothly into the attack animation, overriding other locomotion. If the character gets stunned mid-attack, `EarlyBlendOut` stops the animation gracefully, and the ability can react to this interruption.

#### **3. Adding New Features (Common Usages in Games)**

**A. Weapon Reloading**
*   **Feature**: Play a unique reload animation for a weapon.
*   **GAS**:
    *   Create `GA_ReloadWeapon`.
    *   `ActivateAbility`: Check ammo, play `AM_Reload` montage via ASC.
    *   `AM_Reload` contains `AnimNotifyState_SetLocomotionAction` to set `LocomotionAction` to `Als.LocomotionAction.Reloading`.
    *   `AM_Reload` contains an `AnimNotify` (e.g., "ReloadComplete") at the point where the ammo is actually reloaded. The GA waits for this notify before updating ammo count.
*   **ALS AnimBP**:
    *   A State Machine for `Reloading` is entered when `LocomotionAction` is `Als.LocomotionAction.Reloading`. This state plays the `AM_Reload` montage.

**B. Melee Attacks**
*   **Feature**: Series of melee attacks with different animations.
*   **GAS**:
    *   Create `GA_MeleeAttack`. This ability might be tagged `Ability.Attack.Melee`.
    *   `ActivateAbility`: Determine which attack in the combo chain to play. Play `AM_MeleeAttackCombo1` (or 2, 3...) via ASC.
    *   `AM_MeleeAttackComboX` contains `AnimNotifyState_SetLocomotionAction` to set `LocomotionAction` to `Als.LocomotionAction.MeleeAttack`.
    *   `AM_MeleeAttackComboX` contains an `AnimNotify` (e.g., "MeleeHit") at the point of impact. The GA handles hit detection and applies damage upon receiving this notify.
    *   Ability could then transition to the next attack in the combo based on input or a `Wait for Montage End` node.
*   **ALS AnimBP**:
    *   A State Machine for `MeleeAttack` is entered when `LocomotionAction` is `Als.LocomotionAction.MeleeAttack`. This state plays the attack montages via a slot.

**C. Interaction Animations (e.g., Opening Door, Picking Up Item)**
*   **Feature**: Character plays a specific animation when interacting with an object.
*   **GAS**:
    *   Create `GA_Interact`.
    *   `ActivateAbility`: Perform a trace to find an interactable object. If found, determine the correct `AM_Interact` montage (e.g., `AM_OpenDoor`, `AM_Pickup`).
    *   Play the chosen montage via ASC.
    *   `AM_Interact` contains `AnimNotifyState_SetLocomotionAction` to set `LocomotionAction` to `Als.LocomotionAction.Interacting`.
    *   `AM_Interact` contains an `AnimNotify` (e.g., "InteractPoint") when the interaction should actually occur (e.g., door opens, item disappears). The GA waits for this notify.
*   **ALS AnimBP**:
    *   An `Interacting` State Machine or a slot is triggered when `LocomotionAction` is `Als.LocomotionAction.Interacting`.

**D. Damage/Hit Reactions**
*   **Feature**: Character reacts to taking damage with a flinch/hit animation.
*   **GAS**:
    *   When damage is applied to a character via a `GameplayEffect`, the `UAbilitySystemComponent` often calls `HandleDamage` or an equivalent.
    *   Within this handling, you can use `ASC->PlayMontage` to trigger `AM_HitReaction` (e.g., `AM_HitReaction_Front`, `AM_HitReaction_Back`).
    *   These montages might have `AnimNotifyState_SetLocomotionAction` set to `Als.LocomotionAction.HitReact` to momentarily override other locomotion and ensure the full body reacts.
*   **ALS AnimBP**:
    *   A `HitReact` State Machine plays the reaction montages via a slot when `LocomotionAction` is `Als.LocomotionAction.HitReact`.

### **Network Synchronization**

`UAlsAnimationInstance` itself is not directly replicated in the same way `AAlsCharacter` properties are. Instead, it processes state that *is* replicated from `AAlsCharacter`. The animation system's replication is handled at the montage level by the `UAbilitySystemComponent`.

*   **Character State Replication**: All the `GameplayTag` properties in `AAlsCharacter` (like `LocomotionMode`, `RotationMode`, `Stance`, `Gait`, `OverlayMode`, `LocomotionAction`) are replicated. `UAlsAnimationInstance` simply reads these replicated values on each client to drive its animation logic locally.
*   **Montage Replication**: When a `UAnimMontage` is played via `UAbilitySystemComponent::PlayMontage()`, the `ASC` automatically handles its replication. This means:
    *   The server tells all clients to play the montage at the correct position and rate.
    *   Clients that join late or whose montage gets out of sync will receive corrections.
*   **AnimNotifyState Replication**: The `UAlsAnimNotifyState_SetLocomotionAction` (and other notifies) are typically placed on `UAnimMontage` assets. Since the montage playback is replicated, the events triggered by these notifies/notify states will occur at approximately the same time on all clients. The `SetLocomotionAction` itself is then replicated as an `AAlsCharacter` property.
*   **`_Implementation` and `OnRep_`**: These are primarily used in `AAlsCharacter` for its replicated properties and RPCs. `UAlsAnimationInstance` does not typically have its own `_Implementation` RPC functions or `OnRep_` functions because it reacts to state changes from `AAlsCharacter` rather than being the source of replicated state changes itself.

### **Debugging**

Debugging animation can be complex, especially with layered and procedural systems.

*   **`ShowDebug Als.Curves`**:
    *   **Console Command**: `ShowDebug Als.Curves` (or `Shift+1` if bound).
    *   **Purpose**: Displays the real-time values of all animation curves on the character's skeletal mesh. This is *invaluable* for seeing how the various blend amounts (layering, pose, foot IK) are being calculated and used.
*   **`ShowDebug Als.State`**:
    *   **Console Command**: `ShowDebug Als.State` (or `Shift+2`).
    *   **Purpose**: Shows the current `GameplayTag` states of the character (e.g., `ViewMode`, `LocomotionAction`), which are the primary inputs `UAlsAnimationInstance` uses.
*   **Animation Debugger**:
    *   **Unreal Editor Tool**: While playing in PIE, open the `Window -> Animation -> Animation Debugger`. Select your character.
    *   **Purpose**: This powerful tool allows you to:
        *   Inspect the entire animation graph execution, including node values, blend weights, and active states.
        *   View the values of all properties within your `UAlsAnimationInstance` and its state structs (`FAlsLayeringState`, `FAlsFeetState`, etc.).
        *   See active animation montages and their playback positions.
        *   Monitor Anim Notifies firing.
        *   Step through animation updates frame by frame.
*   **Control Rig Debugging**:
    *   **Unreal Editor Tool**: In the Animation Debugger, navigate to the `Control Rig` tab.
    *   **Purpose**: Debug your Control Rig graphs by inspecting node outputs, inputs, and the state of bones.
*   **Gameplay Ability Debugger**:
    *   **Unreal Editor Tool**: `Window -> Gameplay Ability System -> Gameplay Ability Debugger`.
    *   **Purpose**: Essential for GAS integration. Allows you to see:
        *   Which abilities are active.
        *   Which `GameplayEffects` are applied (and thus which `GameplayTags` are granted).
        *   Which `GameplayTags` are currently on the character's ASC.
        *   Events being sent and received.
        *   This helps verify if your abilities are correctly granting the tags that ALS's animation system is expecting.
*   **`UAlsAnimationInstance` Details Panel**: In PIE, select your character's skeletal mesh, and in the Details panel, find the `Anim Instance` section. Expand it to see the live values of all `UPROPERTY`s within `UAlsAnimationInstance` and its nested structs.
*   **Logging**: Custom log messages (`UE_LOG(LogAls, ...)` or `UE_LOG(LogTemp, ...)` can be added to your C++ functions within `UAlsAnimationInstance` to trace execution flow and variable values.
















------------------------------------------------------------------------------------------------------------














## **1-D. `UAlsLinkedAnimationInstance` (Linked Animation Instances)**

### **Scope**

This section covers the `UAlsLinkedAnimationInstance` class. This class is designed to be used as a parent class for Animation Blueprints that are "linked" to a main Animation Blueprint (which itself inherits from `UAlsAnimationInstance`).

### **Purpose**

The primary purpose of `UAlsLinkedAnimationInstance` is to facilitate a modular approach to animation logic in Unreal Engine. Instead of having one massive Animation Blueprint for the character, developers can break down complex animation behaviors into smaller, more manageable "sub" Animation Blueprints (Linked Anim Layers or Linked Anim Graphs). `UAlsLinkedAnimationInstance` provides a standardized way for these linked instances to access and utilize the core state and functionality of the main `UAlsAnimationInstance`.

This promotes:
*   **Organization**: Keeps animation logic for different features (e.g., facial animation, specific weapon handling) separate.
*   **Reusability**: Linked animation layers can potentially be reused across different characters or projects if designed generically.
*   **Collaboration**: Different animators or programmers can work on different linked layers simultaneously.

### **Key Concepts/Components**

1.  **`Parent` (TWeakObjectPtr<UAlsAnimationInstance>)**
    *   **`UPROPERTY(VisibleAnywhere, Category = "State", Transient)`**
    *   **Significance**: This is the most crucial member. It holds a weak pointer to the *main* `UAlsAnimationInstance` that owns the Skeletal Mesh Component. This `Parent` pointer is the gateway for the linked instance to access all the calculated states, curve values, and helper functions of the primary animation instance.
    *   **Initialization**: Set during `NativeInitializeAnimation()`. It casts the result of `GetSkelMeshComponent()->GetAnimInstance()` to `UAlsAnimationInstance`. In editor preview, it falls back to the default `UAlsAnimationInstance` object if a valid game world character isn't found.

2.  **`Character` (TObjectPtr<AAlsCharacter>)**
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)`**
    *   **Significance**: A direct pointer to the `AAlsCharacter` that owns the Skeletal Mesh. This provides convenient access to character-level states if needed directly within the linked instance, though it's generally recommended to go through the `Parent` `UAlsAnimationInstance` for animation-specific data.
    *   **Initialization**: Set during `NativeInitializeAnimation()`.

3.  **Inheritance from `UAnimInstance`**
    *   Like any animation instance, it inherits the core functionalities of `UAnimInstance`, allowing it to have its own AnimGraph, EventGraph, functions, and variables.

4.  **`bUseMainInstanceMontageEvaluationData`**
    *   **`bUseMainInstanceMontageEvaluationData = true;` (in constructor)**
    *   **Significance**: This boolean (inherited from `UAnimInstance`) is set to `true` by default in `UAlsLinkedAnimationInstance`. It ensures that when animation montages are played, their slot evaluation and curve data are taken from the *main* animation instance (`Parent`). This is vital for consistency, ensuring that montages play correctly regardless of which linked layer might be active or overriding a particular slot.

### **Methods & Functions**

1.  **`NativeInitializeAnimation()`**
    *   **Purpose**: Called once when the animation instance is initialized.
    *   **What it accomplishes**:
        *   Calls `Super::NativeInitializeAnimation()`. **Always call `Super::`!**
        *   Initializes the `Parent` and `Character` pointers.
        *   Handles editor preview setup by using default objects if necessary.

2.  **`NativeBeginPlay()`**
    *   **Purpose**: Called when the game begins play for this animation instance.
    *   **What it accomplishes**:
        *   Calls `Super::NativeBeginPlay()`. **Always call `Super::`!**
        *   Contains an `ALS_ENSURE_MESSAGE` to warn the developer if the `Parent` instance is not valid or not a `UAlsAnimationInstance`. This helps catch incorrect setup where `UAlsLinkedAnimationInstance` might be used without a proper ALS parent.

3.  **`CreateAnimInstanceProxy()`**
    *   **Purpose**: Overridden to return a new `FAlsAnimationInstanceProxy`.
    *   **What it accomplishes**: Ensures that the custom proxy logic (specifically the `PostUpdate` call to `NativePostUpdateAnimation`) is used for linked instances as well, if they were to be the "master" instance in some unusual setup (though typically they are not).
    *   **Remember to call `Super::`**: While not strictly necessary here because it *returns* a new object, if it were modifying a base class member, `Super` would be important.

4.  **Proxy Functions (e.g., `InitializeLook()`, `RefreshGrounded()`, `SetHipsDirection(...)`)**
    *   **Purpose**: These are public, Blueprint-callable functions that simply forward their calls to the corresponding function on the `Parent` (`UAlsAnimationInstance`).
    *   **Parameters & Return Values**: Match the functions they call on the `Parent`.
    *   **What they accomplish**: They provide a convenient and safe way for AnimGraphs or EventGraphs within the *linked* Animation Blueprint to trigger state initializations or refresh logic in the *main* `UAlsAnimationInstance`. This avoids direct, potentially unsafe, access to the parent's internal state or methods.
    *   **Example**:
        ```cpp
        // UAlsLinkedAnimationInstance.cpp
        void UAlsLinkedAnimationInstance::InitializeLook()
        {
            if (Parent.IsValid())
            {
                Parent->InitializeLook();
            }
        }
        ```
    *   **GAS Integration**: These functions are not typically called directly by GAS abilities. GAS abilities interact with `AAlsCharacter`, which then updates `UAlsAnimationInstance`. These proxy functions are for internal animation system communication between linked layers and the main anim instance.

5.  **`GetParent()` / `GetParentUnsafe()`**
    *   **Purpose**: Blueprint-callable functions to get a reference to the `Parent` (`UAlsAnimationInstance`).
    *   **`GetParent()`**: The recommended function.
    *   **`GetParentUnsafe()`**: A deprecated alias.
    *   **Return Value**: `UAlsAnimationInstance*`.
    *   **What it accomplishes**: Allows Blueprint graphs within the linked instance to access public properties and functions of the main `UAlsAnimationInstance`.
    *   **Usage Note**: The comment in the code warns about using this to read properties directly in thread-safe contexts if those properties are not themselves managed in a thread-safe way by the parent. For ALS, most of the parent's animation-critical state (like `FAlsFeetState`, `FAlsPoseState`) is updated in `NativeThreadSafeUpdateAnimation`, so reading them via `GetParent()` in a linked instance's `Event Blueprint Thread Safe Update Animation` is generally safe.

### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

The primary way to use `UAlsLinkedAnimationInstance` is to create a new Animation Blueprint and set its "Parent Class" to `AlsLinkedAnimationInstance`.

#### **1. Creating a Linked Animation Blueprint for Facial Animation**

**Scenario**: You want to manage facial expressions and lip-syncing in a separate Animation Blueprint.

**Steps**:

1.  **Create New Animation Blueprint**:
    *   In the Content Browser, right-click -> Animation -> Animation Blueprint.
    *   Select your character's skeleton.
    *   For "Parent Class," choose `AlsLinkedAnimationInstance`. Name it something like `ABP_AlsCharacter_Face`.

2.  **Setup in Main Animation Blueprint (`ABP_AlsCharacter`)**:
    *   Open your main character Animation Blueprint (e.g., `ABP_AlsCharacter`).
    *   In the AnimGraph, add a "Linked Anim Layer" node (or "Linked Anim Graph" for more complex setups).
    *   In the Details panel of the "Linked Anim Layer" node:
        *   Set the "Layer" property to the `ABP_AlsCharacter_Face` AnimBP you just created.
        *   Specify a "Slot Name" if this layer should only affect certain animation slots (e.g., a "Face" slot). If not, it can affect the whole pose passed into it.

3.  **Implement Facial Logic in `ABP_AlsCharacter_Face`**:
    *   **EventGraph**:
        *   In `Event Blueprint Initialize Animation`, you can call `Get Parent` and cast it to your main `UAlsAnimationInstance` if you need to store a more specific type or access custom properties.
        *   In `Event Blueprint Update Animation` or `Event Blueprint Thread Safe Update Animation`, get data from the `Parent` instance (e.g., `Character->GetOverlayMode()`, or a custom property you add to `UAlsAnimationInstance` like `CurrentSpeechPhoneme`).
            ```blueprint
            # Event Blueprint Thread Safe Update Animation (in ABP_AlsCharacter_Face)
            Sequence:
              - Get Parent (AlsLinkedAnimationInstance) -> IsValid? (Branch)
                - True: Get Parent -> Get Overlay Mode (UAlsAnimationInstance) -> Store as local variable "CurrentOverlayMode"
                - True: Get Parent -> (Cast to YourMainAnimBP) -> Get Custom Facial Data -> Store as local "FacialExpressionIntensity"
            ```
    *   **AnimGraph**:
        *   Use the stored variables (e.g., `CurrentOverlayMode`, `FacialExpressionIntensity`) to drive facial animation logic (e.g., blending between different facial expression poses, driving morph targets for lip-syncing).
        *   You might use "Layered Blend Per Bone" targeting only the head and facial bones.

**C++ (Extending `UAlsLinkedAnimationInstance` for specific needs - less common):**

If you needed very specific C++ logic within your linked instance that required more direct interaction or custom properties:

1.  **Create Custom Linked Instance Class**:
    ```cpp
    // AlsLinkedFacialAnimInstance.h
    #pragma once
    #include "AlsLinkedAnimationInstance.h"
    #include "AlsLinkedFacialAnimInstance.generated.h"

    UCLASS()
    class UAlsLinkedFacialAnimInstance : public UAlsLinkedAnimationInstance
    {
        GENERATED_BODY()
    protected:
        UPROPERTY(BlueprintReadOnly, Category = "Facial")
        float CurrentSmileIntensity{0.0f};

    public:
        virtual void NativeUpdateAnimation(float DeltaTime) override;
    };

    // AlsLinkedFacialAnimInstance.cpp
    #include "AlsCharacter.h" // If needed

    void UAlsLinkedFacialAnimInstance::NativeUpdateAnimation(float DeltaTime)
    {
        Super::NativeUpdateAnimation(DeltaTime); // Always call Super!

        if (Parent.IsValid() && Character.IsValid())
        {
            // Example: Set smile intensity based on a custom character property or GAS tag
            // if (Character->HasMatchingGameplayTag(AlsHappyTag))
            // {
            //    CurrentSmileIntensity = FMath::FInterpTo(CurrentSmileIntensity, 1.0f, DeltaTime, 5.0f);
            // }
            // else
            // {
            //    CurrentSmileIntensity = FMath::FInterpTo(CurrentSmileIntensity, 0.0f, DeltaTime, 5.0f);
            // }
        }
    }
    ```
2.  Then, in your `ABP_AlsCharacter_Face`, you would set its parent class to `AlsLinkedFacialAnimInstance` instead of the base `AlsLinkedAnimationInstance`.

**GAS Integration**:
*   Facial expressions driven by GAS would typically involve a `GameplayAbility` applying a `GameplayEffect` that grants a tag (e.g., `Emotion.Happy`, `Emotion.Angry`) to the character.
*   Your `UAlsLinkedFacialAnimInstance` (or its Blueprint counterpart) would read these tags from `Character->AbilitySystemComponent->GetActiveGameplayTags()` (or via a helper function on `AAlsCharacter`) in its update and set properties like `CurrentSmileIntensity`.
*   The AnimGraph in `ABP_AlsCharacter_Face` then uses `CurrentSmileIntensity` to drive the facial animation.

#### **2. Common New Features / Use Cases for Linked Layers**

*   **Weapon-Specific Handling**: A linked layer for each weapon type (rifle, pistol, bow) that overrides arm/spine animations and handles IK for weapon holding.
    *   The main `UAlsAnimationInstance` would have a property like `CurrentWeaponType` (an enum or tag).
    *   The "Linked Anim Layer" node in the main AnimBP could use a "Switch on Enum/Tag" to select which weapon-specific linked layer to activate.
*   **Detailed Hand Animations**: A layer dedicated to intricate finger movements, gestures, or interaction with small props.
*   **Physics-Based Secondary Animation**: A linked layer that uses Physical Animation Components or Control Rig to add jiggle to accessories, hair, or cloth, driven by the parent's motion.
*   **Status Effect Overlays**: Visual effects for being on fire, poisoned, or frozen could be managed in a linked layer that blends in overlay animations or material parameter changes.
    *   **GAS**: Abilities apply effects granting tags like `Status.OnFire`. The linked layer reads these tags.

### **Network Synchronization**

`UAlsLinkedAnimationInstance` itself does not introduce new network synchronization mechanisms. It relies entirely on the replication of state from:
1.  **`AAlsCharacter`**: As discussed, properties like `ViewMode`, `LocomotionMode`, `OverlayMode` are replicated. The linked instance (via its `Parent`) reads these replicated states.
2.  **`UAbilitySystemComponent`**: GAS handles replication of active abilities, gameplay effects, and gameplay tags. The linked instance (via `Parent->Character->GetAbilitySystemComponent()`) can read these.
3.  **Animation Montages**: If a montage is played on the main `UAlsAnimationInstance` (e.g., by a GAS ability), its playback state (current time, play rate, active section) is replicated by the engine. Since `bUseMainInstanceMontageEvaluationData` is true, linked layers correctly evaluate slots based on the main instance's replicated montage state.

The linked instance itself is a client-side visual representation. Its internal variables are typically not replicated directly; they are derived from the replicated state of the main character or ASC.

### **Debugging**

Debugging linked animation instances is very similar to debugging a main animation instance:

1.  **Animation Debugger (Unreal Editor)**:
    *   This is the primary tool. When you select your character in PIE and open the Animation Debugger, you can navigate into "Linked Anim Layer" nodes in the main AnimGraph.
    *   This will show you the AnimGraph of the linked instance (`ABP_AlsCharacter_Face` in our example) and allow you to inspect its internal variables, blend weights, and active states.
2.  **`UAlsLinkedAnimationInstance` Details Panel**:
    *   If your linked AnimBP inherits from a custom C++ class (like `UAlsLinkedFacialAnimInstance`), you can inspect its `UPROPERTY` members directly in the Details panel of the Skeletal Mesh Component during PIE, under the "Anim Instance" section for that specific linked layer.
3.  **`GetParent()` Access**:
    *   Within a linked AnimBP's EventGraph, you can use `Print String` nodes after getting values from `GetParent()` to see what data the linked instance is receiving.
4.  **Breakpoints**:
    *   In C++, you can set breakpoints in `UAlsLinkedAnimationInstance::NativeUpdateAnimation` or any custom C++ functions you add to a derived linked instance class.
    *   In Blueprint, set breakpoints in the EventGraph of your linked AnimBP.
5.  **Logging**: Use `UE_LOG` in C++ or `Print String` in Blueprint within the linked instance's update functions to trace values.
















----------------------------------------------------------------------------------------------------------

















## **1-E. Settings Data Assets (`UAlsCharacterSettings`, `UAlsMovementSettings`, `UAlsAnimationInstanceSettings`)**

### **Scope**

This section provides a thorough examination of the key Data Asset classes used by ALS to configure character behavior and animation:
*   `UAlsCharacterSettings`: General character attributes, action parameters (mantling, rolling, ragdolling), and view settings.
*   `UAlsMovementSettings`: Detailed locomotion parameters (speeds, acceleration, friction) organized by rotation mode and stance.
*   `UAlsAnimationInstanceSettings`: Fine-grained control over various animation instance behaviors, including IK, transitions, and procedural animation aspects.

### **Purpose**

Data Assets in Unreal Engine are a powerful way to separate configuration data from code and Blueprint logic. ALS leverages this extensively to:
*   **Centralize Configuration**: All key tunable parameters for character and animation behavior are located in these assets.
*   **Easy Iteration**: Designers and animators can tweak character feel and animation responses directly in the editor by modifying these Data Assets, without needing to change C++ code or complex Blueprints.
*   **Modularity**: Different characters can use different instances of these settings assets, allowing for varied behaviors (e.g., a heavy character vs. a nimble character).
*   **Data-Driven Design**: The system reads these settings at runtime to determine how the character should behave and animate.

### **Key Concepts/Components (Data Asset Structure)**

#### **A. `UAlsCharacterSettings` (`DataAsset_AlsCharacterSettings` by default)**

This asset governs the high-level behavior and action-specific parameters of `AAlsCharacter`.

1.  **General Settings**:
    *   **`TeleportDistanceThreshold` (float, cm)**: If the character moves further than this distance in a single frame, it's considered a teleport. This affects camera lag and foot IK to prevent visual glitches.
        *   **Example Configuration**: `200.0`. A lower value makes teleport detection more sensitive, potentially causing issues if the character has very high, legitimate speeds. A higher value might miss actual teleports.
    *   **`MovingSpeedThreshold` (float, cm/s)**: The speed above which the character is considered "moving" for various logic checks (e.g., deciding if it can turn in place).
        *   **Example Configuration**: `50.0`. Set this just above the speed where idle animations should stop and movement animations begin.

2.  **In-Air Rotation**:
    *   **`InAirRotationMode` (EAlsInAirRotationMode enum)**: Controls how the character rotates while in the air when not aiming.
        *   `RotateToVelocityOnJump`: Character rotates to face its velocity direction when jumping.
        *   `KeepRelativeRotation`: Character maintains its rotation relative to the camera's yaw.
        *   `KeepWorldRotation`: Character attempts to maintain its world rotation.
        *   **Example Configuration**: `RotateToVelocityOnJump` is common for platformers or action games where air control feels natural. `KeepRelativeRotation` might be used if the character should always face where the camera is pointing, even mid-air.
    *   **`bAllowAimingWhenInAir` (bool)**: If true, the character can enter aiming rotation mode while airborne.
        *   **Example Configuration**: `true`. Allows for mid-air aiming shots. Set to `false` if aiming should be restricted to grounded states.

3.  **Sprint & Aim Interaction**:
    *   **`bSprintHasPriorityOverAiming` (bool)**: If true, initiating a sprint will take precedence and potentially force the character out of aiming mode. If false, aiming can prevent sprinting.
        *   **Example Configuration**: `false`. Common setup where aiming is a deliberate action that overrides sprinting. Set to `true` if sprinting should always be possible even if the aim input is held.
    *   **`bRotateToVelocityWhenSprinting` (bool)**: If true, and the character is sprinting, it will automatically switch to `VelocityDirection` rotation mode, regardless of the `DesiredRotationMode`.
        *   **Example Configuration**: `true`. Makes sprinting feel more committed and directional. Set to `false` if you want sprint rotation to follow `DesiredRotationMode`.

4.  **Movement Base Rotation Inheritance**:
    *   **`bInheritMovementBaseRotationInVelocityDirectionRotationMode` (bool)**: When in `VelocityDirection` rotation mode and standing on a rotating platform, if true, the character will inherit the platform's rotation. If false, it will try to maintain its world rotation.
        *   **Example Configuration**: `false`. Usually, you want the character to maintain its orientation relative to the world, not spin with the platform if they are trying to run in a straight line *across* it. Set to `true` if the character should "stick" to the platform's rotation.
    *   **`bRotateTowardsDesiredVelocityInVelocityDirectionRotationMode` (bool)**: If true, when in `VelocityDirection` mode, the character rotates towards its *intended* movement direction (even if slightly blocked) rather than its *actual* current velocity.
        *   **Example Configuration**: `true`. Often feels more responsive as the character tries to go where the player wants, not just where physics currently allows.
    *   **`bAutoRotateOnAnyInputWhileNotMovingInViewDirectionRotationMode` (bool)**: If true, and in `ViewDirection` rotation mode, any movement input while stationary will cause the character to start rotating towards the view direction.
        *   **Example Configuration**: `true`. Makes the character feel more alive and ready to move in the camera's direction.

5.  **`View` (FAlsViewSettings)**
    *   **`bEnableNetworkSmoothing` (bool)**: Enables/disables network smoothing for the character's view rotation on simulated proxies.
        *   **Example Configuration**: `true`. Highly recommended for smoother visuals in multiplayer.
    *   **`bEnableListenServerNetworkSmoothing` (bool)**: Enables/disables view smoothing for the character when controlled by a remote client on a listen server.
        *   **Example Configuration**: `true`.

6.  **`Mantling` (FAlsGeneralMantlingSettings)**: This is a complex struct with many sub-properties.
    *   **`bAllowMantling` (bool)**: Master switch for enabling/disabling mantling.
    *   **`TraceAngleThreshold` (float, degrees)**: Max angle between character forward and obstacle normal for mantle detection.
        *   **Example Configuration**: `110.0`. Wider allows mantling more angled surfaces.
    *   **`MaxReachAngle` (float, degrees)**: Max angle character can reach sideways for mantling.
        *   **Example Configuration**: `50.0`.
    *   **`SlopeAngleThreshold` (float, degrees)**: Max slope of the surface the character can mantle onto.
        *   **Example Configuration**: `35.0`. Surfaces steeper than this won't be mantleable.
    *   **`TargetPrimitiveSpeedThreshold` (float, cm/s)**: If the object to be mantled is moving faster than this, mantling is disallowed.
        *   **Example Configuration**: `10.0`. Prevents mantling very fast-moving platforms.
    *   **`MantlingHighHeightThreshold` (float, cm)**: The height above which a mantle is considered "High" (using different animations/settings).
        *   **Example Configuration**: `125.0`.
    *   **`GroundedTrace` / `InAirTrace` (FAlsMantlingTraceSettings)**: Define parameters for traces used to detect mantleable objects.
        *   **`LedgeHeight` (Vector2D, Min/Max, cm)**: The vertical range the character can mantle. E.g., `GroundedTrace.LedgeHeight = (X=50.0, Y=225.0)` means character can mantle obstacles between 50cm and 225cm high when grounded.
        *   **`ReachDistance` (float, cm)**: How far forward the character can reach to initiate a mantle. E.g., `GroundedTrace.ReachDistance = 75.0`.
        *   **`TargetLocationOffset` (float, cm)**: How far *into* the mantle surface the character aims to land.
        *   **`StartLocationOffset` (float, cm)**: How far *back* from the mantle edge the character's starting position for the animation is considered.
        *   **`bDrawFailedTraces` (bool)**: If true, shows debug traces even if mantling fails. Essential for tuning.
    *   **`MantlingTraceChannel` (ECollisionChannel)**: The collision channel used for mantle detection traces.
        *   **Example Configuration**: `ECC_Visibility`. Often used, but you might create a custom "Mantleable" channel.
    *   **`MantlingTraceResponseChannels` (Array of ECollisionChannel)**: Which channels should block the mantle traces.
    *   **`BlendOutDuration` (float, seconds)**: How long it takes to blend out of the mantle animation if interrupted.
    *   **`bStartRagdollingOnTargetPrimitiveDestruction` (bool)**: If the object being mantled is destroyed, should the character ragdoll?
        *   **Example Configuration**: `true`. Adds realism.

7.  **`Ragdolling` (FAlsRagdollingSettings)**
    *   **`bStartRagdollingOnLand` (bool)**: If true, character will ragdoll if they land too hard.
    *   **`RagdollingOnLandSpeedThreshold` (float, cm/s)**: The downward velocity threshold for triggering ragdoll on land.
        *   **Example Configuration**: `1000.0`.
    *   **`bLimitInitialRagdollSpeed` (bool)**: If true, applies a temporary speed cap to the ragdoll right after activation to prevent extreme velocities from physics glitches.
    *   **`GetUpFrontMontage` / `GetUpBackMontage` (UAnimMontage*)**: Animation montages played when the character gets up from a ragdoll, depending on whether they are face-up or face-down.
        *   **Configuration**: Assign your "Get Up From Front" and "Get Up From Back" animation montages here.

8.  **`Rolling` (FAlsRollingSettings)**
    *   **`Montage` (UAnimMontage*)**: The animation montage to play for the rolling action.
        *   **Configuration**: Assign your character's roll animation montage.
    *   **`bCrouchOnStart` (bool)**: If true, the character will attempt to crouch at the start of a roll.
    *   **`bRotateToInputOnStart` (bool)**: If true, the roll will be directed towards the current movement input direction.
    *   **`RotationInterpolationSpeed` (float)**: How quickly the character interpolates its rotation to the target roll direction during the roll.
        *   **Example Configuration**: `10.0`. Higher is faster.
    *   **`bStartRollingOnLand` (bool)**: If true, character will roll if they land hard enough (but not hard enough to ragdoll).
    *   **`RollingOnLandSpeedThreshold` (float, cm/s)**: The downward velocity threshold for triggering a roll on land.
        *   **Example Configuration**: `700.0`. Must be less than `RagdollingOnLandSpeedThreshold`.
    *   **`bInterruptRollingWhenInAir` (bool)**: If true, and the character becomes airborne during a roll, the roll is interrupted (often transitioning to ragdoll if falling far).
        *   **Example Configuration**: `true`. Prevents air-rolling.

#### **B. `UAlsMovementSettings` (`DataAsset_AlsMovementSettings` by default)**

This asset is crucial for defining how the character *feels* to control in terms of speed, acceleration, and friction under different conditions.

1.  **`VelocityAngleToSpeedInterpolationRange` (Vector2D, degrees)**
    *   **Purpose**: When `bAllowDirectionDependentMovementSpeed` is true in `FAlsMovementGaitSettings`, this defines the range of angles (relative to character forward) over which the speed interpolates from forward speed to backward speed.
    *   **Example Configuration**: `(X=100.0, Y=125.0)`.
        *   If the character moves directly forward (0 degrees), they use `WalkForwardSpeed`/`RunForwardSpeed`.
        *   If they move directly backward (180 degrees), they use `WalkBackwardSpeed`/`RunBackwardSpeed`.
        *   If their velocity angle is between 0 and 100 degrees, they use forward speed.
        *   If their velocity angle is between 125 and 180 degrees, they use backward speed.
        *   Between 100 and 125 degrees, the speed is interpolated.

2.  **`RotationModes` (TMap<FGameplayTag, FAlsMovementStanceSettings>)**
    *   **Structure**: This is a map where the *key* is a `RotationMode` Gameplay Tag (e.g., `Als.RotationMode.ViewDirection`) and the *value* is an `FAlsMovementStanceSettings` struct.
    *   **Configuration**:
        *   You will typically have entries for `Als.RotationMode.VelocityDirection`, `Als.RotationMode.ViewDirection`, and `Als.RotationMode.Aiming`.
        *   For each of these, you configure the nested `FAlsMovementStanceSettings`.

3.  **`FAlsMovementStanceSettings` (nested within `RotationModes`)**
    *   **Structure**: Contains a `TMap<FGameplayTag, FAlsMovementGaitSettings>` where the *key* is a `Stance` Gameplay Tag (e.g., `Als.Stance.Standing`) and the *value* is an `FAlsMovementGaitSettings` struct.
    *   **Configuration**:
        *   Inside each `RotationMode` entry, you'll configure entries for `Als.Stance.Standing` and `Als.Stance.Crouching`.

4.  **`FAlsMovementGaitSettings` (nested within `Stances`)**
    *   This is where the core movement numbers are defined.
    *   **`bAllowDirectionDependentMovementSpeed` (bool)**: See above.
    *   **`WalkForwardSpeed` / `WalkBackwardSpeed` (float, cm/s)**: Max speed when walking.
        *   **Example (Standing, ViewDirection)**: `WalkForwardSpeed = 175.0`. `WalkBackwardSpeed = 150.0` (if direction dependent).
    *   **`RunForwardSpeed` / `RunBackwardSpeed` (float, cm/s)**: Max speed when running.
        *   **Example (Standing, ViewDirection)**: `RunForwardSpeed = 375.0`. `RunBackwardSpeed = 300.0`.
    *   **`SprintSpeed` (float, cm/s)**: Max speed when sprinting.
        *   **Example (Standing, ViewDirection)**: `SprintSpeed = 650.0`.
    *   **`AccelerationAndDecelerationAndGroundFrictionCurve` (UCurveVector*)**:
        *   **Purpose**: This is a *Vector Curve* asset. It maps the `GaitAmount` (X-axis of the curve, from 0 to 3) to three separate float values (Y-axis for each of its R, G, B curves, which correspond to MaxAcceleration, BrakingDeceleration, and GroundFriction respectively).
        *   **Configuration**:
            1.  Create a new `CurveVector` asset (e.g., `CV_Als_GroundedMovement_Default`).
            2.  Open it. You'll see three curves (R, G, B).
            3.  **R (X-component, Red) is Max Acceleration**:
                *   Add a key at Time `0.0` (Stopped), Value (e.g., `2000.0`).
                *   Add a key at Time `1.0` (Walking), Value (e.g., `1500.0`).
                *   Add a key at Time `2.0` (Running), Value (e.g., `2500.0`).
                *   Add a key at Time `3.0` (Sprinting), Value (e.g., `3500.0`).
            4.  **G (Y-component, Green) is Braking Deceleration**:
                *   Add keys similarly, e.g., Time `0.0` Value `1500.0`, Time `1.0` Value `1000.0`, etc.
            5.  **B (Z-component, Blue) is Ground Friction**:
                *   Add keys, e.g., Time `0.0` Value `8.0`, Time `1.0` Value `6.0`, Time `2.0` Value `4.0`, Time `3.0` Value `2.0`.
            6.  Assign this `CV_Als_GroundedMovement_Default` asset to the `AccelerationAndDecelerationAndGroundFrictionCurve` property in your `FAlsMovementGaitSettings`.
        *   **Result**: As the character's speed changes (thus changing `GaitAmount`), their acceleration, deceleration, and friction will dynamically update based on these curves, allowing for very nuanced movement feel.
    *   **`RotationInterpolationSpeedCurve` (UCurveFloat*)**:
        *   **Purpose**: This is a *Float Curve* asset. It maps `GaitAmount` (X-axis, 0-3) to the character's rotation interpolation speed (Y-axis).
        *   **Configuration**:
            1.  Create a `CurveFloat` asset (e.g., `CF_Als_RotationSpeed`).
            2.  Add keys:
                *   Time `0.0` (Stopped), Value (e.g., `8.0` - for turning in place).
                *   Time `1.0` (Walking), Value (e.g., `6.0`).
                *   Time `2.0` (Running), Value (e.g., `5.0`).
                *   Time `3.0` (Sprinting), Value (e.g., `4.0` - often slower rotation interp when sprinting fast).
            3.  Assign this asset.
        *   **Result**: Character rotation responsiveness changes with speed.

**GAS Integration for Movement Settings**:
*   Typically, GAS abilities will influence *which* set of these settings are active by changing `AAlsCharacter`'s `DesiredGait`, `DesiredStance`, or `DesiredRotationMode`. The `UAlsCharacterMovementComponent` then automatically picks the correct `FAlsMovementGaitSettings` from the `UAlsMovementSettings` asset.
*   **Example: "Slowed" Debuff**:
    *   An ability applies a `GameplayEffect` that grants a `Debuff.Slowed` tag.
    *   Another passive ability (or a C++ system listening to tag changes) could detect `Debuff.Slowed` and then:
        *   Call `Character->SetDesiredGait(AlsGaitTags::Walking)`.
        *   And/Or directly call `Character->GetAlsCharacterMovementComponent()->SetMaxAllowedGait(AlsGaitTags::Walking)` to cap the speed.
        *   This forces the character to use the "Walking" parameters from `UAlsMovementSettings`, effectively slowing them.
*   **Example: Temporary Speed Boost**:
    *   An ability grants a `GameplayEffect` that has a `StatMod` (Magnitude Modifier) on a custom `Movement.SpeedMultiplier` attribute.
    *   You would need to modify `UAlsCharacterMovementComponent::RefreshGroundedMovementSettings()` to read this attribute from the ASC and multiply the calculated `MaxWalkSpeed` by it.
        ```cpp
        // In UAlsCharacterMovementComponent::RefreshGroundedMovementSettings()
        // ... after MaxWalkSpeed is initially set ...
        if (IsValid(CharacterOwner) && CharacterOwner->GetAbilitySystemComponent())
        {
            float SpeedMultiplier = CharacterOwner->GetAbilitySystemComponent()->GetNumericAttribute(UMyAttributeSet::GetSpeedMultiplierAttribute());
            MaxWalkSpeed *= SpeedMultiplier; // Apply multiplier
        }
        ```

#### **C. `UAlsAnimationInstanceSettings` (`DataAsset_AlsAnimationInstanceSettings` by default)**

This asset controls various animation-specific behaviors within `UAlsAnimationInstance`.

1.  **`General` (FAlsGeneralAnimationSettings)**
    *   **`bUseHandIkBones` (bool)**: If true, Control Rig will use dedicated IK bones for hands (e.g., `ik_hand_l`, `ik_hand_r`). If false, it might use virtual bones or the hand bones themselves.
    *   **`bUseFootIkBones` (bool)**: Similar to above, but for feet (e.g., `ik_foot_l`, `ik_foot_r`). If false, it defaults to virtual bones (`VB foot_l/r`).
    *   **`MovingSmoothSpeedThreshold` (float, cm/s)**: Speed threshold used by `UAlsAnimationInstance` to determine if the character is "moving smoothly" for animation blending purposes.
        *   **Example Configuration**: `150.0`.
    *   **`LeanInterpolationSpeed` (float)**: How quickly the character's lean amount interpolates.
        *   **Example Configuration**: `4.0`. Higher is faster.

2.  **`View` (FAlsViewAnimationSettings)**
    *   **`LookTowardsCameraRotationInterpolationSpeed` (float)**: Interpolation speed for the character's head/eyes when looking towards the camera direction (in View Direction rotation mode).
    *   **`LookTowardsInputYawAngleInterpolationSpeed` (float)**: Interpolation speed for head/eyes when looking towards the input direction (in Velocity Direction rotation mode when stationary).

3.  **`Grounded` (FAlsGroundedSettings)**
    *   **`RotationYawOffsetForwardCurve`, `...BackwardCurve`, `...LeftCurve`, `...RightCurve` (UCurveFloat*)**: These curves map the *view-relative velocity yaw angle* to a rotation offset. This offset is added to the character's target rotation in `ViewDirection` mode to make movement look more natural (e.g., character slightly turns into the direction of strafing).
        *   **Configuration**: Create `UCurveFloat` assets. The X-axis is the angle between view and velocity (e.g., -180 to 180). The Y-axis is the desired yaw offset. For `RotationYawOffsetForwardCurve`, you'd typically have 0 offset at 0 degrees, and maybe small positive/negative offsets as the angle deviates slightly. For `RotationYawOffsetLeftCurve`, you'd have a significant negative offset when velocity is to the left of view.
    *   **`VelocityBlendInterpolationSpeed` (float)**: How quickly the `VelocityBlend` values (ForwardAmount, BackwardAmount, etc.) interpolate in `UAlsAnimationInstance`.
        *   **Example Configuration**: `12.0`.

4.  **`Standing` (FAlsStandingSettings)** & **`Crouching` (FAlsCrouchingSettings)**
    *   **`AnimatedWalkSpeed`, `AnimatedRunSpeed`, `AnimatedSprintSpeed` (float, cm/s)**: These are the speeds at which your *walk, run, and sprint animations were authored*. They are crucial for calculating the correct animation `PlayRate` to match the character's actual movement speed.
        *   **Example Configuration (Standing)**: `AnimatedWalkSpeed = 150.0`, `AnimatedRunSpeed = 350.0`, `AnimatedSprintSpeed = 600.0`. These should match the root motion speed of your cycle animations.
    *   **`StrideBlendAmountWalkCurve`, `StrideBlendAmountRunCurve` (UCurveFloat*)** (Standing) / **`StrideBlendAmountCurve` (UCurveFloat*)** (Crouching): Map character's current `Speed` to a `StrideBlendAmount`. This value scales the "distance feet travel" in blend spaces, allowing animations to look good across a range of speeds without needing separate animations for every minor speed variation.
        *   **Configuration**: Create `UCurveFloat` assets. X-axis is character speed. Y-axis is the stride blend (typically 0.0 to 1.0 or slightly higher). At lower speeds, stride blend might be lower (smaller steps), increasing as speed picks up.
    *   **`PivotActivationSpeedThreshold` (float, cm/s)** (Standing only): If character speed drops below this while turning sharply, a "pivot" animation might be triggered.

5.  **`InAir` (FAlsInAirSettings)**
    *   **`LeanAmountCurve` (UCurveFloat*)**: Maps *vertical velocity* to a lean multiplier. Used to reverse lean direction when transitioning from upward to downward movement in air.
    *   **`GroundPredictionAmountCurve` (UCurveFloat*)**: Maps *ground prediction sweep hit time* (0-1 range) to the `GroundPredictionAmount` (0-1). Allows a smooth blend into a "landing" pose as the character approaches the ground.
    *   **`GroundPredictionSweepChannel`, `GroundPredictionResponseChannels`**: Configure collision channels for the ground prediction trace.

6.  **`Feet` (FAlsFeetSettings)**
    *   **`bDisableFootLock` (bool)**: Master switch to disable foot locking IK.
    *   **`FootLockAngleLimit` (float, degrees)**: Max angle the foot can be locked relative to the thigh to prevent leg twisting.
        *   **Example Configuration**: `90.0`.

7.  **`Transitions` (FAlsTransitionsSettings)** & **`DynamicTransitions` (FAlsDynamicTransitionsSettings)**
    *   These control parameters for quick stop animations, dynamic foot adjustment animations, and provide references to the animation sequences used for these transitions (e.g., `StandingLeftSequence` for a quick stop while turning left).
    *   **Example `Transitions.QuickStopPlayRate = (X=1.75, Y=3.0)`**: The quick stop animation plays faster if the character needs to rotate more during the stop.
    *   **Example `DynamicTransitions.FootLockDistanceThreshold = 8.0`**: If a locked foot is more than 8cm away from its target animation position, a dynamic transition animation might play to adjust it.

8.  **`RotateInPlace` (FAlsRotateInPlaceSettings)** & **`TurnInPlace` (FAlsGeneralTurnInPlaceSettings)**
    *   Configure thresholds (view yaw angle, view yaw speed) and animation parameters (play rates, specific `UAlsTurnInPlaceSettings` assets for 90/180 degree turns, standing/crouching) for idle rotation animations.
    *   **`UAlsTurnInPlaceSettings` (sub-asset)**: Holds the `AnimSequenceBase`, `PlayRate`, and `AnimatedTurnAngle` for a specific turn animation (e.g., Standing Turn 90 Left).

**GAS Integration with Animation Instance Settings**:
*   GAS abilities generally do not directly modify `UAlsAnimationInstanceSettings` at runtime.
*   However, the *outcomes* of GAS abilities (changes in character state like `Gait`, `Stance`, `LocomotionAction`) will cause `UAlsAnimationInstance` to use different parts of these settings.
*   **Example**: An ability that forces the character to walk slowly (e.g., "OverEncumbered" effect) would change `AAlsCharacter::DesiredGait` to `Walking`. `UAlsAnimationInstance` then reads the "Walking" parameters from `FAlsStandingSettings` (like `AnimatedWalkSpeed` and `StrideBlendAmountWalkCurve`) to calculate appropriate animation play rates and blends.
*   If an ability needs to temporarily override a specific animation behavior controlled by these settings (e.g., force a specific `PlayRate` for an animation during a special move), it would typically do so by:
    1.  Playing a unique `UAnimMontage` for that move.
    2.  Within that montage, using animation curves to override default values (e.g., a curve that directly sets a "DesiredPlayRate" property in the AnimBP).
    3.  Or, less commonly, the ability could directly set a temporary override property on `AAlsCharacter` or `UAlsAnimationInstance` that the animation logic then reads.

### **Network Synchronization (for Settings Assets)**

*   Settings Data Assets (`UAlsCharacterSettings`, `UAlsMovementSettings`, `UAlsAnimationInstanceSettings`) are **not directly replicated** in the traditional sense of actor properties.
*   They are **loaded on all clients and the server** as part of the game's content.
*   The *character* (`AAlsCharacter`) holds a pointer to these assets. When the character is spawned, this pointer is part of its initial replicated data (or set deterministically).
*   All clients and the server then use their *local copy* of these Data Assets to configure behavior.
*   This means that for all players to have a consistent experience, they must all have the *same version* of these Data Assets. If a client has a modified Data Asset, their local character might behave or animate differently from how the server and other clients see it, leading to desynchronization.
*   Changes to these Data Assets are typically done in the editor and are part of a game patch/update. Dynamic, per-instance modifications at runtime are usually handled by changing the character's state tags or applying `GameplayEffects` that modify attributes, which then cause the system to interpret the existing settings differently.

### **Debugging (for Settings Assets)**

1.  **Editor Inspection**: The primary way to "debug" settings is to open the Data Assets themselves in the Unreal Editor and review their values.
2.  **`AAlsCharacter` Details Panel**: When your character is selected in PIE, you can see which `UAlsCharacterSettings` and `UAlsMovementSettings` assets it is currently using.
3.  **`UAlsAnimationInstance` Details Panel**: Similarly, you can see which `UAlsAnimationInstanceSettings` asset it's using.
4.  **`UAlsCharacterMovementComponent` Details Panel**: Shows the currently active `GaitSettings` (derived from `UAlsMovementSettings`), `MaxAccelerationWalking`, `GroundFriction`, etc., allowing you to verify if the correct settings are being applied based on character state.
5.  **`ShowDebug Als.State` / `ShowDebug Als.Curves`**: These character debug displays indirectly help debug settings by showing you the *outcome* of those settings (e.g., if `Gait` is `Walking`, are the animation curves behaving as expected for walking based on your `UAlsAnimationInstanceSettings`?).
6.  **Print Values in Blueprints/C++**:
    *   In `AAlsCharacter::BeginPlay` or `UAlsAnimationInstance::NativeInitializeAnimation`, you can print the values from these settings assets to the log to confirm they are being loaded correctly.
    *   If you suspect a setting is not having the desired effect, print the intermediate values in the relevant `Refresh...` functions in `AAlsCharacter` or `UAlsAnimationInstance` to see how the setting is being used in calculations.








---------------------------------------------------------------------------------------------------------------










## **1-F. State Structs (Data Containers for `AAlsCharacter`)**

### **Scope**

This section details the C++ `USTRUCT`s (structs) that are members of the `AAlsCharacter` class. These structs are responsible for holding granular, numerical, and contextual data related to specific aspects of the character's dynamic state. They are distinct from the `GameplayTag` states, which define the *type* or *mode*; these structs contain the *data values* for those states.

### **Purpose**

The state structs within `AAlsCharacter` serve to:
*   **Organize Data**: Group related variables into logical units (e.g., all locomotion-specific data in `FAlsLocomotionState`).
*   **Encapsulate State**: Provide a clear representation of the character's current condition for various systems.
*   **Facilitate Calculations**: Store intermediate and final values used in movement, rotation, and action logic.
*   **Data Source for Animation**: While `UAlsAnimationInstance` has its own state structs, it often derives its initial data from these `AAlsCharacter` structs.
*   **Replication (Indirectly)**: Some of these structs contain data that is directly replicated (like `ReplicatedViewRotation` within `FAlsViewState`'s context, or `RagdollTargetLocation`). Others are primarily updated locally on the server and autonomous proxy, with their effects being seen through the character's overall replicated movement and actions.

### **Key Concepts/Components (State Structs in `AAlsCharacter`)**

#### **1. `FAlsMovementBaseState MovementBase`**

*   **Purpose**: Tracks information about the physical object (primitive component) the character is currently standing on or interacting with as a movement base. This is crucial for characters to move correctly with moving platforms.
*   **Key `UPROPERTY` Members**:
    *   **`Primitive` (TObjectPtr<UPrimitiveComponent>)**:
        *   **`UPROPERTY(BlueprintReadWrite, Category = "ALS")`** (Note: In `AAlsCharacter.h`, it's `VisibleAnywhere, BlueprintReadOnly, Category = "State|Als Character", Transient`).
        *   **Significance**: A pointer to the `UPrimitiveComponent` the character is based on (e.g., a moving platform's mesh). `nullptr` if not based.
        *   **Functional Meaning**: This is the actual surface the character is "stuck" to. If this platform moves or rotates, the character should (conditionally) move and rotate with it.
    *   **`BoneName` (FName)**:
        *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")`** (In `AAlsCharacter.h`, it's `VisibleAnywhere, BlueprintReadOnly...`).
        *   **Significance**: If the character is based on a specific bone of a skeletal mesh (e.g., standing on the wing of a large animated creature), this stores the name of that bone.
        *   **Functional Meaning**: Specifies the exact attachment point on a complex, animated movement base.
    *   **`bBaseChanged` (uint8 : 1)**:
        *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")`** (In `AAlsCharacter.h`, it's `VisibleAnywhere, BlueprintReadOnly...`).
        *   **Significance**: A boolean flag (packed as a bitfield) that is true for one frame when the character's movement base changes (e.g., steps onto a new platform or steps off).
        *   **Functional Meaning**: Signals to other systems (like animation foot locking or camera) that the reference frame for relative movement has changed and they might need to reinitialize their relative offsets.
    *   **`bHasRelativeLocation` (uint8 : 1)**:
        *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")`** (In `AAlsCharacter.h`, it's `VisibleAnywhere, BlueprintReadOnly...`).
        *   **Significance**: True if the character's location is currently relative to the movement base.
        *   **Functional Meaning**: Indicates that the character's world position is affected by the base's translation.
    *   **`bHasRelativeRotation` (uint8 : 1)**:
        *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")`** (In `AAlsCharacter.h`, it's `VisibleAnywhere, BlueprintReadOnly...`).
        *   **Significance**: True if the character's rotation is currently relative to the movement base's rotation.
        *   **Functional Meaning**: Indicates that the character's world orientation is affected by the base's rotation. This is key for the character to turn with a rotating platform.
    *   **`Location` (FVector)**:
        *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")`** (In `AAlsCharacter.h`, it's `VisibleAnywhere, BlueprintReadOnly...`).
        *   **Significance**: The world-space location of the movement base (or specific bone).
        *   **Functional Meaning**: The current position of the surface the character is on.
    *   **`Rotation` (FQuat)**:
        *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")`** (In `AAlsCharacter.h`, it's `VisibleAnywhere, BlueprintReadOnly...`).
        *   **Significance**: The world-space rotation (as a quaternion) of the movement base.
        *   **Functional Meaning**: The current orientation of the surface the character is on.
    *   **`DeltaRotation` (FRotator)**:
        *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")`** (In `AAlsCharacter.h`, it's `VisibleAnywhere, BlueprintReadOnly...`).
        *   **Significance**: The change in the movement base's rotation since the last frame. Only valid if `bHasRelativeRotation` is true and `bBaseChanged` is false.
        *   **Functional Meaning**: How much the platform rotated this frame. This is used by `AAlsCharacter` to adjust its own rotation and view rotation to stay aligned with the moving base.

*   **Update Logic**: `AAlsCharacter::RefreshMovementBase()` updates this struct every tick by querying `GetBasedMovement()` from the `UCharacterMovementComponent`.

#### **2. `FAlsViewState ViewState`**

*   **Purpose**: Manages the character's view rotation, including network smoothing for a smoother visual experience on clients.
*   **Key `UPROPERTY` Members**:
    *   **`NetworkSmoothing` (FAlsViewNetworkSmoothingState)**:
        *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")`**.
        *   **Significance**: Contains all data for interpolating the view rotation on clients between server updates.
        *   **Functional Meaning**: This is what prevents the camera (and aim direction) from snapping or jittering on clients in a multiplayer game when network updates for view rotation arrive.
            *   `bEnabled`: Turns smoothing on/off.
            *   `ServerTime`, `ClientTime`, `Duration`: Timing variables for interpolation.
            *   `InitialRotation`, `TargetRotation`, `CurrentRotation`: Used to lerp from the last known rotation to the newly received server rotation.
    *   **`Rotation` (FRotator)**:
        *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")`**.
        *   **Significance**: The *current, smoothed* view rotation of the character (Roll, Pitch, Yaw). This is the rotation that should be used by the camera and aiming logic.
        *   **Functional Meaning**: This is "where the character is looking." On the server and autonomous client, it's very close to the raw controller input. On simulated clients, it's the interpolated result of `ReplicatedViewRotation`.
    *   **`YawSpeed` (float, deg/s)**:
        *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0, ForceUnits = "deg/s"))`**.
        *   **Significance**: The speed at which the view yaw is changing.
        *   **Functional Meaning**: How fast the player is turning the camera left or right. Used by `UAlsAnimationInstance` to influence rotation animations (e.g., faster turn-in-place if camera moves fast).
    *   **`PreviousYawAngle` (float, degrees)**:
        *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = -180, ClampMax = 180, ForceUnits = "deg"))`**.
        *   **Significance**: The view yaw angle from the previous frame.
        *   **Functional Meaning**: Used to calculate `YawSpeed`.

*   **Update Logic**: `AAlsCharacter::RefreshView()` and `RefreshViewNetworkSmoothing()` update this struct. It's influenced by `AAlsCharacter::ReplicatedViewRotation` and controller input.

#### **3. `FAlsLocomotionState LocomotionState`**

*   **Purpose**: Contains detailed, real-time data about the character's movement, input, and target orientation. This is one of the most frequently accessed structs by various ALS systems.
*   **Key `UPROPERTY` Members**:
    *   **`bHasInput` (uint8 : 1)**:
        *   **Significance**: True if the character currently has movement input (e.g., from joystick or keyboard).
        *   **Functional Meaning**: Indicates if the player is actively trying to move the character. Affects rotation logic, gait selection, and animation blending.
    *   **`InputYawAngle` (float, degrees)**:
        *   **Significance**: The world-space yaw angle of the current movement input vector.
        *   **Functional Meaning**: The direction the player *wants* to move, irrespective of current character facing or velocity.
    *   **`bHasVelocity` (uint8 : 1)**:
        *   **Significance**: True if the character's current 2D speed is above a small threshold.
        *   **Functional Meaning**: Indicates if the character is actually moving across the ground or through the air.
    *   **`Speed` (float, cm/s)**:
        *   **Significance**: The character's current speed in the XY plane (horizontal speed).
        *   **Functional Meaning**: How fast the character is moving horizontally. Drives gait calculation and animation play rates.
    *   **`Velocity` (FVector)**:
        *   **Significance**: The character's current 3D world-space velocity vector.
        *   **Functional Meaning**: The direction and magnitude of the character's movement in 3D space.
    *   **`VelocityYawAngle` (float, degrees)**:
        *   **Significance**: The world-space yaw angle of the character's current horizontal velocity.
        *   **Functional Meaning**: The direction the character is *actually* moving horizontally.
    *   **`bMoving` (uint8 : 1)**:
        *   **Significance**: True if the character `bHasInput` AND `bHasVelocity`, OR if `Speed` is greater than `Settings->MovingSpeedThreshold`.
        *   **Functional Meaning**: A general flag indicating active movement, often used to decide between idle and moving animation states or behaviors.
    *   **`bRotationTowardsLastInputDirectionBlocked` (uint8 : 1)**:
        *   **Significance**: A flag used to temporarily prevent the character from snapping its rotation towards the last input direction, especially after landing or when changing rotation modes.
        *   **Functional Meaning**: Prevents undesirable jerky rotations when the character stops or lands, allowing animations to play out more smoothly.
    *   **`TargetYawAngle` (float, degrees)**:
        *   **Significance**: The immediate target yaw angle the character's rotation logic is trying to achieve. This might be derived from velocity, view, or input.
        *   **Functional Meaning**: The "raw" direction the character wants to face *right now*.
    *   **`SmoothTargetYawAngle` (float, degrees)**:
        *   **Significance**: A smoothed version of `TargetYawAngle`. It interpolates towards `TargetYawAngle` over time, preventing instant snaps.
        *   **Functional Meaning**: This is the yaw angle that `AAlsCharacter`'s rotation functions (`SetRotationSmooth`, `SetRotationExtraSmooth`) typically use as their interpolation target, resulting in smoother character turns.
    *   **`ViewRelativeTargetYawAngle` (float, degrees)**:
        *   **Significance**: The character's `TargetYawAngle` expressed relative to the current `ViewState.Rotation.Yaw`.
        *   **Functional Meaning**: Represents how much the character needs to turn left or right to align with its target yaw, from the perspective of the camera. Useful for some animation blend calculations.
    *   **`AimingYawAngleLimit` (float, degrees)**:
        *   **Significance**: The current maximum allowed angle between the character's forward direction and the view direction when aiming. This limit is dynamically adjusted.
        *   **Functional Meaning**: Prevents the character's lower body from twisting unrealistically when the player rapidly changes aim direction. Starts wide (e.g., 180 degrees) and narrows as the character aligns.
    *   **`bAimingLimitAppliedThisFrame` (uint8 : 1)**:
        *   **Significance**: True if the aiming yaw limit was actively constraining the character's rotation during the current frame.
        *   **Functional Meaning**: Internal flag used by `RefreshLocomotionLate` to decide if `AimingYawAngleLimit` should be reset.
    *   **`bResetAimingLimit` (uint8 : 1)**:
        *   **Significance**: If true, `AimingYawAngleLimit` will be reset to its maximum value (e.g., 180 degrees) at the end of the frame if it wasn't applied.
        *   **Functional Meaning**: Ensures the aiming limit doesn't stay constricted if the player stops aiming or rotating quickly.

*   **Update Logic**: Updated primarily in `AAlsCharacter::RefreshInput()`, `RefreshLocomotionEarly()`, `RefreshLocomotion()`, and `RefreshLocomotionLate()`, as well as by rotation functions.

#### **4. `FAlsMantlingState MantlingState`**

*   **Purpose**: Stores state specifically for an active mantling action.
*   **Key `UPROPERTY` Members**:
    *   **`RootMotionSourceId` (int32)**:
        *   **Significance**: The ID of the active `FAlsRootMotionSource_Mantling` applied to the `UCharacterMovementComponent`. A value of `0` or less means no mantling root motion source is active.
        *   **Functional Meaning**: Links the character's mantling state to the specific root motion source that is controlling its precise movement during the mantle animation. This is how ALS ensures the character follows the animated path over the obstacle.

*   **Update Logic**: Set by `AAlsCharacter::StartMantlingImplementation()` and cleared by `StopMantling()`. Checked in `RefreshMantling()`.

#### **5. `FAlsRagdollingState RagdollingState`**

*   **Purpose**: Holds physics-related data for when the character is in a ragdoll state.
*   **Key `UPROPERTY` Members**:
    *   **`Velocity` (FVector)**:
        *   **Significance**: The last known velocity of the character's pelvis bone while ragdolling.
        *   **Functional Meaning**: Used to potentially transfer momentum if the character transitions out of ragdoll (e.g., into falling) and for animation (flail rate).
    *   **`PullForce` (float, Newtons)**:
        *   **Significance**: The magnitude of the corrective force applied to the ragdoll on simulated clients to pull it towards the server's `RagdollTargetLocation`.
        *   **Functional Meaning**: Helps keep client-side ragdolls from deviating too drastically from the server's simulation, reducing visual discrepancies in multiplayer.
    *   **`SpeedLimitFrameTimeRemaining` (int32)**:
        *   **Significance**: A countdown (in frames) during which the ragdoll's initial speed is capped.
        *   **Functional Meaning**: A temporary hack/fix to prevent the ragdoll from launching at extreme speeds due to physics instabilities when transitioning into ragdoll, especially at low frame rates or during complex interactions.
    *   **`SpeedLimit` (float, cm/s)**:
        *   **Significance**: The actual speed cap applied during `SpeedLimitFrameTimeRemaining`.
        *   **Functional Meaning**: The maximum speed individual ragdoll bones are allowed to have during the initial frames of ragdolling.

*   **Update Logic**: Updated during `AAlsCharacter::StartRagdollingImplementation()` and `RefreshRagdolling()`.

#### **6. `FAlsRollingState RollingState`**

*   **Purpose**: Stores data for an active rolling action.
*   **Key `UPROPERTY` Members**:
    *   **`TargetYawAngle` (float, degrees)**:
        *   **Significance**: The desired world-space yaw angle the character should face by the end of the roll.
        *   **Functional Meaning**: Controls the final orientation of the character after the roll animation completes. The character will interpolate its rotation towards this angle during the roll.

*   **Update Logic**: Set in `AAlsCharacter::StartRollingImplementation()`. Used by `RefreshRollingPhysics()`.

### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

Since these state structs are primarily internal data containers managed by `AAlsCharacter`'s core logic, direct modification from external systems (especially Blueprints or GAS abilities) is generally **not recommended** and can lead to unpredictable behavior or desynchronization.

Instead, you influence these structs by:
1.  **Setting Desired States**: Call functions like `SetDesiredGait`, `SetDesiredStance`, `SetDesiredAiming`. `AAlsCharacter`'s internal logic will then update the relevant state structs based on these inputs and other conditions.
2.  **Initiating Actions**: Call functions like `StartMantling`, `StartRolling`. These functions will manage the relevant state structs (e.g., `FAlsMantlingState`, `FAlsRollingState`) internally.
3.  **Modifying Input**: Providing movement input naturally updates parts of `FAlsLocomotionState`.
4.  **Through Gameplay Effects (GAS)**: Apply `GameplayEffects` that grant `GameplayTags` or modify `Attributes`. `AAlsCharacter` can then be extended to read these tags/attributes and adjust its internal state struct calculations accordingly.

**Example: GAS Ability that makes the character "Slip" and temporarily lose input control, affecting `FAlsLocomotionState`**

**1. Define Gameplay Tag (Optional but good for clarity):**
   `Als.Status.Slipping`

**2. Create a Gameplay Ability (e.g., `GA_ApplySlipStatus`)**
   *   This ability could be triggered by walking on an "ice" `GameplayCue` or a specific `GameplayEffect`.
   *   **C++ (`GA_ApplySlipStatus.cpp`):**
     ```cpp
     void UGA_ApplySlipStatus::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
     {
         if (CommitAbility(Handle, ActorInfo, ActivationInfo))
         {
             AAlsCharacter* Character = Cast<AAlsCharacter>(ActorInfo->AvatarActor.Get());
             if (Character)
             {
                 // Option A: Apply a GameplayEffect that blocks input via an attribute or tag
                 // For example, GE_SlipDebuff could grant a "Input.Blocked.Slip" tag
                 // ActorInfo->AbilitySystemComponent->ApplyGameplayEffectToSelf(SlipDebuffEffect, ...);

                 // Option B: Directly (if authority) or via RPC, modify a custom bool on Character
                 // This is less ideal for pure GAS, but shows interaction.
                 // Character->SetTemporarilySlipping(true); // A new function you'd add to AAlsCharacter

                 // For visual feedback, you might play a "slipping" montage
                 PlayMontage(this, SlippingMontage, 1.0f);

                 // End ability after a duration
                 FTimerHandle TimerHandle_EndSlip;
                 GetWorld()->GetTimerManager().SetTimer(TimerHandle_EndSlip, FTimerDelegate::CreateLambda([this, ActorInfo]() {
                     AAlsCharacter* Char = Cast<AAlsCharacter>(ActorInfo->AvatarActor.Get());
                     // if (Char) Char->SetTemporarilySlipping(false);
                     // Remove the "Input.Blocked.Slip" tag if using Option A
                     K2_EndAbility();
                 }), 2.0f, false); // Slip for 2 seconds
             }
         }
         else
         {
             EndAbility(Handle, ActorInfo, ActivationInfo, true);
         }
     }
     ```

**3. Modify `AAlsCharacter::RefreshInput()` (Example of reacting to a custom state/tag)**
   ```cpp
   // AAlsCharacter.cpp
   void AAlsCharacter::RefreshInput(const float DeltaTime)
   {
       if (GetLocalRole() >= ROLE_AutonomousProxy)
       {
           // Assume GetCharacterMovement()->GetCurrentAcceleration() gives raw input
           FVector CurrentInput = GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal();

           // --- MODIFICATION FOR SLIP ---
           // Check for a Gameplay Tag from ASC or a custom bool like bIsTemporarilySlipping
           bool bIsSlipping = false;
           if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Als.Status.Slipping"))))
           {
               bIsSlipping = true;
           }
           // else if (bIsTemporarilySlipping) { bIsSlipping = true; }


           if (bIsSlipping)
           {
               // During slip, maybe only allow minimal control or specific "counter-slip" input
               // For this example, let's just drastically reduce input influence
               CurrentInput *= 0.1f;
           }
           // --- END MODIFICATION ---

           SetInputDirection(CurrentInput);
       }

       LocomotionState.bHasInput = InputDirection.SizeSquared() > UE_KINDA_SMALL_NUMBER;

       if (LocomotionState.bHasInput)
       {
           LocomotionState.InputYawAngle = UE_REAL_TO_FLOAT(UAlsVector::DirectionToAngleXY(InputDirection));
       }
   }
   ```
*   **Functional Impact**: When the "Slip" ability is active, the `LocomotionState.bHasInput` might still become true if there's *some* input, but `LocomotionState.InputYawAngle` would be based on a significantly dampened input vector. The character's movement would become less responsive, simulating a slip. The actual *sliding physics* would ideally be handled in `UAlsCharacterMovementComponent::PhysWalking` or `PhysCustom` by checking for the same `Als.Status.Slipping` tag and applying different friction or forces.

### **Network Synchronization**

*   **Direct Replication**:
    *   `AAlsCharacter::ReplicatedViewRotation` is directly replicated and used to derive `FAlsViewState::Rotation` with smoothing.
    *   `AAlsCharacter::InputDirection` is directly replicated.
    *   `AAlsCharacter::DesiredVelocityYawAngle` is directly replicated.
    *   `AAlsCharacter::RagdollTargetLocation` is directly replicated.
*   **Indirect Replication**: Most other data within these state structs (e.g., `FAlsLocomotionState::Speed`, `FAlsLocomotionState::VelocityYawAngle`) are *not* directly replicated member by member. Instead:
    *   They are calculated on the **server** based on its authoritative state and replicated input.
    *   They are calculated on the **autonomous client** (the owning player) based on local input for client-side prediction.
    *   **Simulated clients** derive these values by observing the character's replicated movement (handled by `UCharacterMovementComponent`) and the replicated high-level Gameplay Tag states from `AAlsCharacter`. The `UAlsAnimationInstance` on simulated clients then uses these locally derived values to drive animations.
*   **Consistency**: The goal is that the *calculations* performed on the server, autonomous client, and simulated clients, when fed the same replicated base data (like transform, velocity, input direction, Gameplay Tag states), should result in very similar values for these internal state structs. This is key to keeping animations synchronized.
*   **`AAlsCharacter::MovementBase`**: The core `BasedMovement` info from `UCharacterMovementComponent` *is* replicated. `AAlsCharacter::RefreshMovementBase` just copies and processes this.

### **Debugging**

*   **`ShowDebug Als.State`**: Will show the high-level Gameplay Tag states.
*   **`ShowDebug Als.Shapes`**: Visualizes `InputYawAngle`, `VelocityYawAngle`, `SmoothTargetYawAngle` from `FAlsLocomotionState`, and the `ViewState.Rotation`.
*   **Details Panel**:
    *   Select your `AAlsCharacter` instance in PIE.
    *   In the "Details" panel, navigate to the "State|Als Character" category.
    *   You can expand `MovementBase`, `ViewState`, `LocomotionState`, `MantlingState`, `RagdollingState`, and `RollingState` to see all their member variables updating in real-time. This is extremely powerful for understanding the current numerical state of the character.
    *   For example, you can watch `LocomotionState.Speed` change as you move, or see `MovementBase.DeltaRotation` when standing on a rotating platform.
*   **Logging and Breakpoints**: Adding `UE_LOG` statements or breakpoints in the `Refresh...` functions within `AAlsCharacter.cpp` that update these structs (e.g., `RefreshLocomotion`, `RefreshView`) is very effective for tracing how these values are being calculated.












---------------------------------------------------------------------------------------------------------













## **II. Camera System**

This section details the custom camera implementation provided by ALS. While it ships with robust First-Person and Third-Person perspective support, its modular design allows for extension to other camera styles, such as a Top-Down view.

### **A. `UAlsCameraComponent` (Camera Logic)**

#### **Scope**

This section covers the `UAlsCameraComponent` class. This component, derived from `USkeletalMeshComponent`, is responsible for calculating and providing the final view information (location, rotation, FOV) to the engine. It's designed to be driven by animation curves and character state.

#### **Purpose**

The `UAlsCameraComponent` handles the dynamic positioning and orientation of the game camera. Its core responsibilities are:
*   Calculating camera properties based on character state (`AAlsCharacter`) and animation curves produced by `UAlsCameraAnimationInstance`.
*   Supporting different camera perspectives (First-Person, Third-Person by default).
*   Implementing features like camera lag, collision avoidance, and dynamic Field of View (FOV) adjustments.
*   Providing a clean interface for the character to obtain its view information.

#### **Key Concepts/Components**

1.  **`Settings` (TObjectPtr<UAlsCameraSettings>)**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")`**
    *   **Significance**: A pointer to a `UAlsCameraSettings` Data Asset. This asset holds all configurable parameters for the camera's behavior across different modes (e.g., FOV, socket names, trace parameters).

2.  **`bOverrideFieldOfView` (uint8 : 1) & `FieldOfViewOverride` (float)**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (InlineEditConditionToggle))`**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (EditCondition = "bOverrideFieldOfView"))`**
    *   **Significance**: Allows for a manual override of the camera's Field of View, bypassing the values defined in `UAlsCameraSettings` or calculated by animation curves.
    *   **Functional Meaning**: Useful for temporary FOV changes, like an "aim down sights" zoom or a special ability effect.

3.  **`PostProcessWeight` (float)**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ClampMax = 1))`**
    *   **Significance**: Controls the blend weight of the post-process settings defined in `UAlsCameraSettings`.
    *   **Functional Meaning**: Allows for dynamic application or blending of camera-specific post-processing effects (e.g., depth of field when aiming).

4.  **Internal State Variables (`PivotTargetLocation`, `PivotLagLocation`, `PivotLocation`, `CameraLocation`, `CameraRotation`, `CameraFieldOfView`)**
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)`**
    *   **Significance**: These store the intermediate and final calculated values for the camera's position and orientation.
        *   `PivotTargetLocation`: The ideal, unlagged position around which the third-person camera should orbit.
        *   `PivotLagLocation`: The `PivotTargetLocation` after applying lag.
        *   `PivotLocation`: The `PivotLagLocation` after applying offsets from animation curves.
        *   `CameraLocation`, `CameraRotation`, `CameraFieldOfView`: The final calculated camera properties passed to the engine.
    *   **Functional Meaning**: These represent the step-by-step calculation of the camera's view, incorporating lag, animation-driven offsets, and collision adjustments.

5.  **Movement Base Tracking (`MovementBasePrimitive`, `MovementBaseBoneName`, `PivotMovementBaseRelativeLagLocation`, `CameraMovementBaseRelativeRotation`)**
    *   **Significance**: These variables allow the camera to correctly follow characters standing on moving platforms, maintaining its relative position and orientation to the base if configured.
    *   **Functional Meaning**: Ensures the camera doesn't "float away" or jitter when the character is on a dynamic surface.

6.  **`TraceDistanceRatio` (float)**
    *   **Significance**: Used for camera collision. It's a ratio (0-1) representing how far along the ideal camera vector (from pivot to target) the camera is actually placed due to collision. 1 means no collision, <1 means collision pushed the camera closer.
    *   **Functional Meaning**: Smoothly interpolates the camera's distance from the character when obstacles are encountered or cleared.

7.  **`bRightShoulder` (uint8 : 1)**
    *   **Significance**: Determines if the third-person camera is positioned over the character's right or left shoulder.
    *   **Functional Meaning**: Allows for dynamic shoulder swapping, common in third-person shooters.

#### **Methods & Functions**

1.  **`TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)`**
    *   **Purpose**: The main update function for the component.
    *   **What it accomplishes**:
        *   Handles time dilation adjustments.
        *   Calls `TickCamera()` to perform the core camera logic if parallel animation evaluation is not running.
        *   **Always call `Super::TickComponent()`!**

2.  **`CompleteParallelAnimationEvaluation(bool bDoPostAnimationEvaluation)`**
    *   **Purpose**: Called when parallel animation evaluation finishes.
    *   **What it accomplishes**: Calls `TickCamera()` to ensure camera logic is updated with the latest animation data.
    *   **Always call `Super::CompleteParallelAnimationEvaluation()`!**

3.  **`TickCamera(float DeltaTime, bool bAllowLag = true)`**
    *   **Purpose**: The core private function where all camera calculations happen.
    *   **What it accomplishes**:
        *   Refreshes movement base information.
        *   Calculates `PivotTargetLocation`.
        *   Checks for `FirstPersonOverride` curve from the animation instance.
        *   Calculates camera rotation (`CalculateCameraRotation`).
        *   Calculates pivot lag location (`CalculatePivotLagLocation`).
        *   Calculates pivot offset from animation curves (`CalculatePivotOffset`).
        *   Calculates camera offset from animation curves (`CalculateCameraOffset`).
        *   Performs camera collision trace (`CalculateCameraTrace`).
        *   Interpolates between first-person and third-person views if `FirstPersonOverride` is active.
        *   Applies FOV override and FOV offset curve.

4.  **`GetViewInfo(FMinimalViewInfo& ViewInfo) const`**
    *   **Purpose**: Public function called by `AAlsCharacter::CalcCamera` (or directly by the PlayerCameraManager if this component were the ViewTarget) to provide the final camera parameters to the engine.
    *   **What it accomplishes**: Populates `ViewInfo.Location`, `ViewInfo.Rotation`, `ViewInfo.FOV`, and post-process settings.

5.  **Calculation Helpers (Private: `CalculateCameraRotation`, `CalculatePivotLagLocation`, `CalculatePivotOffset`, `CalculateCameraOffset`, `CalculateFovOffset`, `CalculateCameraTrace`)**
    *   **Purpose**: These internal functions break down the `TickCamera` logic into manageable pieces. They handle specific calculations like rotation interpolation, lag application, reading animation curves for offsets, and collision sweeping.

6.  **Location Getters (`GetFirstPersonCameraLocation`, `GetThirdPersonPivotLocation`, `GetThirdPersonTraceStartLocation`)**
    *   **Purpose**: Provide specific world-space locations derived from character mesh sockets, used as starting points for camera calculations.
    *   **`GetFirstPersonCameraLocation()`**: Returns the location of the `Settings->FirstPerson.CameraSocketName` (e.g., "FPPCamera" socket on the head).
    *   **`GetThirdPersonPivotLocation()`**: Returns a point often between the character's root and head (`Settings->ThirdPerson.FirstPivotSocketName`, `Settings->ThirdPerson.SecondPivotSocketName`), serving as the camera's orbit center.
    *   **`GetThirdPersonTraceStartLocation()`**: Returns the start location for the camera collision trace, typically from a shoulder socket (`Settings->ThirdPerson.TraceShoulderRightSocketName` or `...LeftSocketName`).

7.  **Setters (`SetFieldOfViewOverriden`, `SetFieldOfViewOverride`, `SetPostProcessWeight`, `SetRightShoulder`)**
    *   **Purpose**: Public Blueprint-callable functions to modify camera settings at runtime.
    *   **GAS Integration**: Abilities can call these to, for example, zoom the FOV (`SetFieldOfViewOverride`), apply a special visual effect (`SetPostProcessWeight`), or change the camera shoulder.

### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

#### **Implementing a Top-Down View**

A top-down view is significantly different from the existing first/third-person modes. We'll need to modify `UAlsCameraComponent` and potentially `UAlsCameraSettings`.

**Method 1: Adding a New "TopDown" View Mode (More Integrated)**

This involves adding a new state to the existing system.

**1. Define New Gameplay Tag for View Mode (in `AAlsCharacter`)**
   *   Follow the steps in section 1-A to add `Als.ViewMode.TopDown` to `AlsGameplayTags.h` and `.cpp`.

**2. Modify `UAlsCameraSettings` (Optional, for dedicated settings)**
   *   **C++ (`AlsCameraSettings.h`)**:
     ```cpp
     // AlsCameraSettings.h
     USTRUCT(BlueprintType)
     struct ALSCAMERA_API FAlsTopDownCameraSettings
     {
         GENERATED_BODY()

         UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 5, ClampMax = 175, ForceUnits = "deg"))
         float FieldOfView{60.0f};

         UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceUnits = "cm"))
         float DesiredDistance{1500.0f}; // Distance from character

         UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ForceUnits = "deg"))
         float CameraPitchAngle{-75.0f}; // Angle looking down

         UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (ClampMin = 0))
         float LocationLagSpeed{5.0f}; // How quickly camera follows character

         UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")
         uint8 bUseFixedYaw : 1 {false}; // Lock camera yaw or let it follow character?

         UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS", Meta = (EditCondition = "bUseFixedYaw", ForceUnits = "deg"))
         float FixedYawAngle{0.0f};
     };

     UCLASS(Blueprintable, BlueprintType)
     class ALSCAMERA_API UAlsCameraSettings : public UDataAsset
     {
         // ... existing properties ...
         UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
         FAlsTopDownCameraSettings TopDown;
     };
     ```
   *   **Editor**: Create/Open your `DA_AlsCameraSettings` and configure the new `TopDown` section.

**3. Modify `UAlsCameraComponent::TickCamera()`**
   *   **C++ (`AlsCameraComponent.cpp`)**:
     ```cpp
     // AlsCameraComponent.cpp (inside TickCamera, after getting Character and Settings)
     #include "Utility/AlsGameplayTags.h" // For AlsViewModeTags
     #include "AlsCharacter.h" // To get ViewMode from character

     // ... inside TickCamera ...
     AAlsCharacter* AlsCharacterOwner = Cast<AAlsCharacter>(Character);
     if (!IsValid(AlsCharacterOwner))
     {
         // Fallback or error
         return;
     }

     const FGameplayTag CurrentViewMode = AlsCharacterOwner->GetViewMode();

     if (CurrentViewMode == AlsViewModeTags::TopDown)
     {
         // --- TOP-DOWN LOGIC ---
         const FVector CharacterHeadLocation = Character->GetMesh()->GetSocketLocation(UAlsConstants::HeadBoneName()); // Or GetActorLocation()

         // Calculate Target Location
         FVector TargetCameraLocation = CharacterHeadLocation - (FRotator(Settings->TopDown.CameraPitchAngle, 0.0f, 0.0f).Vector() * Settings->TopDown.DesiredDistance);

         // Apply Lag
         if (bAllowLag && Settings->TopDown.LocationLagSpeed > 0.0f)
         {
             CameraLocation = FMath::VInterpTo(CameraLocation, TargetCameraLocation, DeltaTime, Settings->TopDown.LocationLagSpeed);
         }
         else
         {
             CameraLocation = TargetCameraLocation;
         }

         // Calculate Rotation
         if (Settings->TopDown.bUseFixedYaw)
         {
             CameraRotation = FRotator(Settings->TopDown.CameraPitchAngle, Settings->TopDown.FixedYawAngle, 0.0f);
         }
         else
         {
             // Option: Follow character's smoothed yaw
             CameraRotation = FRotator(Settings->TopDown.CameraPitchAngle, AlsCharacterOwner->GetLocomotionState().SmoothTargetYawAngle, 0.0f);
             // Option: Follow character's actual yaw
             // CameraRotation = FRotator(Settings->TopDown.CameraPitchAngle, Character->GetActorRotation().Yaw, 0.0f);
         }

         CameraFieldOfView = bOverrideFieldOfView ? FieldOfViewOverride : Settings->TopDown.FieldOfView;
         // PostProcessWeight = Settings->TopDown.PostProcessBlendWeight; // If you add this to FAlsTopDownCameraSettings

         // Ensure the camera component itself is not visible if it's a skeletal mesh
         SetVisibility(false); 
         return; // Skip other camera logic
         // --- END TOP-DOWN LOGIC ---
     }
     else if (FAnimWeight::IsFullWeight(FirstPersonOverride))
     {
         // ... existing first-person logic ...
     }
     // ... existing third-person logic ...
     ```

**4. Modify `UAlsCameraAnimationInstance` (if using animation-driven top-down effects)**
   *   If your top-down view needs dynamic offsets or FOV changes driven by character animation (e.g., zooming out when sprinting), you'd add new curves to `UAlsCameraConstants` (e.g., `TopDownDistanceOffsetCurve`) and read them in `UAlsCameraAnimationInstance`.
   *   Then, in `UAlsCameraComponent::TickCamera`'s top-down block, you would read these curve values from `AnimationInstance->GetCurveValue(...)` and apply them. For a simple top-down view, this might not be necessary.

**5. Triggering the Top-Down View**
   *   **C++ or Blueprint**: Call `AlsCharacterOwner->SetViewMode(AlsViewModeTags::TopDown);`

**GAS Integration for Top-Down View Switch:**
*   Create a `GameplayAbility` (e.g., `GA_SetTopDownView`).
*   **`ActivateAbility`**:
    ```cpp
    AAlsCharacter* Character = Cast<AAlsCharacter>(ActorInfo->AvatarActor.Get());
    if (Character)
    {
        Character->SetViewMode(AlsViewModeTags::TopDown);
    }
    K2_EndAbility(); // End immediately as it's a state change
    ```
*   This ability could be triggered by player input or a game event.
*   To switch back, another ability or the same ability toggling would call `SetViewMode` with `AlsViewModeTags::ThirdPerson`.

**Method 2: Creating a Separate Top-Down Camera Actor/Component (Less Integrated, More Decoupled)**

If the top-down view is very different and doesn't need to leverage ALS camera's lag/collision in the same way, or if you want to switch to a completely different camera actor:

1.  **Create a New Camera Actor or Component**:
    *   Create a new Blueprint Actor (e.g., `BP_TopDownCameraRig`) with a `CameraComponent`.
    *   Implement its logic to follow the `AAlsCharacter` from above.
        ```blueprint
        // Event Tick in BP_TopDownCameraRig
        TargetCharacter (AAlsCharacter variable, set externally) -> IsValid?
          - True: TargetCharacter -> GetActorLocation -> (Add Z offset for height) -> SetActorLocation (of BP_TopDownCameraRig)
                  CameraComponent (Self) -> SetWorldRotation (Pitch = -90, Yaw = 0, Roll = 0)
        ```
2.  **Switching View Targets**:
    *   When you want to switch to top-down, get the `PlayerController` and call `SetViewTargetWithBlend`, passing your `BP_TopDownCameraRig` as the new view target.
    *   To switch back, call `SetViewTargetWithBlend` again, passing the `AAlsCharacter` instance.
3.  **Disabling `UAlsCameraComponent`**:
    *   When the `BP_TopDownCameraRig` is active, you should deactivate the `UAlsCameraComponent` on the `AAlsCharacter` to prevent it from doing unnecessary work or trying to control the view.
        ```cpp
        // In AAlsCharacter, when switching TO TopDownCameraRig
        if (AlsCamera) // Assuming UAlsCameraComponent is named AlsCamera
        {
            AlsCamera->Deactivate();
        }

        // When switching BACK FROM TopDownCameraRig
        if (AlsCamera)
        {
            AlsCamera->Activate(true); // true to reset/reinitialize
        }
        ```
4.  **GAS Integration**:
    *   A `GameplayAbility` (`GA_ToggleTopDownCameraActor`) would handle the `SetViewTargetWithBlend` calls and the activation/deactivation of `UAlsCameraComponent`.

**Advantages of Method 1 (Integrated)**:
*   Leverages existing ALS camera settings and animation curve system if needed.
*   Keeps camera logic somewhat centralized within `UAlsCameraComponent`.
*   The character itself remains the view target.

**Advantages of Method 2 (Decoupled)**:
*   Cleaner separation if the top-down camera has vastly different requirements.
*   Easier if you don't want to modify the core `UAlsCameraComponent` C++ code.
*   More aligned with Unreal's typical approach of having dedicated camera actors for distinct views.

For most cases where you want a top-down view that still feels connected to the character (e.g., RTS-style control of a single unit), **Method 1 is often preferable** as it keeps things within the ALS framework. Method 2 is better for completely cinematic or detached camera systems.

### **B. `UAlsCameraAnimationInstance` (Camera Animation Logic)**

#### **Scope**

This section covers the `UAlsCameraAnimationInstance` class. It is an `UAnimInstance` specifically designed to be used by the `UAlsCameraComponent` (which is a `USkeletalMeshComponent`).

#### **Purpose**

The `UAlsCameraAnimationInstance`'s primary role is to process the `AAlsCharacter`'s state and translate it into animation curve values that the `UAlsCameraComponent` can then use to dynamically adjust its behavior. Think of it as the "brain" that tells the camera *how much* to offset, lag, or change FOV based on what the character is doing.

#### **Key Concepts/Components**

1.  **`Character` (TObjectPtr<AAlsCharacter>)** & **`Camera` (TObjectPtr<UAlsCameraComponent>)**
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)`**
    *   **Significance**: Pointers to the owning character and the camera component itself. These are initialized in `NativeInitializeAnimation`.

2.  **State Properties (`ViewMode`, `LocomotionMode`, `RotationMode`, `Stance`, `Gait`, `LocomotionAction`, `bRightShoulder`)**
    *   **`UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", Transient)`**
    *   **Significance**: These properties mirror the key states of the `AAlsCharacter`. They are updated every frame in `NativeUpdateAnimation`.
    *   **Functional Meaning**: These allow the AnimGraph of `UAlsCameraAnimationInstance` to react to character states and output different curve values.

3.  **Animation Curves (Output)**
    *   **Significance**: This class doesn't *store* curve values as properties, but its AnimGraph *produces* them. The `UAlsCameraComponent` reads these curves.
    *   **Key Curves Defined in `UAlsCameraConstants.h`**:
        *   `CameraOffsetXCurve`, `CameraOffsetYCurve`, `CameraOffsetZCurve`: Control the XYZ offset of the camera from its pivot.
        *   `FovOffsetCurve`: Adds a dynamic offset to the camera's FOV.
        *   `PivotOffsetXCurve`, `PivotOffsetYCurve`, `PivotOffsetZCurve`: Control the XYZ offset of the camera *pivot* from the character's base pivot location.
        *   `LocationLagXCurve`, `LocationLagYCurve`, `LocationLagZCurve`: Control the smoothing/lag speed for the camera's location on each axis independently.
        *   `RotationLagCurve`: Controls the smoothing/lag speed for the camera's rotation.
        *   `FirstPersonOverrideCurve`: A value from 0 to 1. When 1, forces a full first-person view. Allows smooth blending.
        *   `TraceOverrideCurve`: A value from 0 to 1. When 1, the camera collision trace starts from an overridden location (defined in `UAlsCameraSettings`) rather than the character's shoulder.

#### **Methods & Functions**

1.  **`NativeInitializeAnimation()`**
    *   **Purpose**: Initializes `Character` and `Camera` pointers.
    *   **Always call `Super::NativeInitializeAnimation()`!**

2.  **`NativeUpdateAnimation(float DeltaTime)`**
    *   **Purpose**: The main update loop.
    *   **What it accomplishes**:
        *   Calls `Super::NativeUpdateAnimation(DeltaTime)`. **Always call `Super::`!**
        *   Synchronizes its internal state properties (`ViewMode`, `LocomotionMode`, etc.) with the current state of the `Character`.
        *   **Crucially, the AnimGraph attached to this instance evaluates *after* this function, producing the output animation curve values.**

#### **Adding/Adapting Functionality (Example: Top-Down View Animation Control)**

If you wanted your Top-Down view (from Method 1 above) to have its distance or pitch dynamically adjusted by character animation (e.g., zoom out when sprinting, tilt up when looking far):

1.  **Define New Animation Curves (C++ in `UAlsCameraConstants.h`)**:
    ```cpp
    // UAlsCameraConstants.h
    // ...
    inline static const FName TopDownDistanceOffsetCurve{TEXTVIEW("TopDownDistanceOffset")};
    inline static const FName TopDownPitchOffsetCurve{TEXTVIEW("TopDownPitchOffset")};
    // ...
    // Add UFUNCTION wrappers:
    UFUNCTION(BlueprintPure, Category = "ALS|Camera Constants|Animation Curves", Meta = (ReturnDisplayName = "Curve Name"))
    static const FName& TopDownDistanceOffsetCurveName();
    // ... and for PitchOffset
    ```
    ```cpp
    // UAlsCameraConstants.cpp
    // ...
    const FName& UAlsCameraConstants::TopDownDistanceOffsetCurveName() { return TopDownDistanceOffsetCurve; }
    // ...
    ```
2.  **Add Curves to Camera Animations**:
    *   Use an `Animation Modifier` (like `AlsAnimationModifier_CreateCurves`) on any relevant camera animation sequences (or even a base "camera idle" animation used by `UAlsCameraAnimationInstance`) to add these new curves.
    *   Set their default values (e.g., 0).

3.  **Modify `UAlsCameraAnimationInstance`'s AnimGraph (`ABP_AlsCamera`)**:
    *   In the AnimGraph of `ABP_AlsCamera` (or your custom camera AnimBP):
        *   When `ViewMode == AlsViewModeTags::TopDown`:
            *   If `Gait == AlsGaitTags::Sprinting`, drive the `TopDownDistanceOffsetCurve` to a positive value (e.g., 200cm) to make the camera move further away.
            *   If the character is "aiming" far (you'd need a way to signal this from `AAlsCharacter` to `UAlsCameraAnimationInstance`, perhaps via a custom float property like `LookDistance`), drive `TopDownPitchOffsetCurve` to a positive value (e.g., 10 degrees) to tilt the camera up.
        *   Otherwise, ensure these curves are at their default (0).

4.  **Modify `UAlsCameraComponent::TickCamera()` to Use New Curves**:
    *   **C++ (`AlsCameraComponent.cpp`)** in the Top-Down logic block:
      ```cpp
      // ... inside TopDown logic block in TickCamera ...
      float DistanceOffset = AnimationInstance.IsValid() ? AnimationInstance->GetCurveValue(UAlsCameraConstants::TopDownDistanceOffsetCurveName()) : 0.0f;
      float PitchOffset = AnimationInstance.IsValid() ? AnimationInstance->GetCurveValue(UAlsCameraConstants::TopDownPitchOffsetCurveName()) : 0.0f;

      float FinalDesiredDistance = Settings->TopDown.DesiredDistance + DistanceOffset;
      float FinalCameraPitchAngle = Settings->TopDown.CameraPitchAngle + PitchOffset;

      FVector TargetCameraLocation = CharacterHeadLocation - (FRotator(FinalCameraPitchAngle, 0.0f, 0.0f).Vector() * FinalDesiredDistance);
      // ... rest of the lag and rotation logic using FinalCameraPitchAngle ...
      ```

**GAS Integration**:
*   The `UAlsCameraAnimationInstance` doesn't directly interact with GAS. It reacts to character states.
*   If a GAS ability changes the character's `Gait` to `Sprinting` (e.g., `GA_Sprint` applies an effect that sets `AAlsCharacter::DesiredGait`), then `UAlsCameraAnimationInstance` will detect this state change, its AnimGraph will output a new `TopDownDistanceOffsetCurve` value, and `UAlsCameraComponent` will use it to adjust the camera.

### **C. Settings Data Asset (`UAlsCameraSettings`)**

#### **Scope**

This section covers the `UAlsCameraSettings` Data Asset class.

#### **Purpose**

`UAlsCameraSettings` centralizes all the configurable parameters that define the behavior of the `UAlsCameraComponent` across its different supported modes (First-Person, Third-Person, and any custom modes like the Top-Down example).

#### **Key Concepts/Components (Structure of `UAlsCameraSettings`)**

1.  **`bIgnoreTimeDilation` (bool)**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")`**
    *   **Significance**: If true, the camera's `DeltaTime` for lag and interpolation calculations will be based on real-time, ignoring game time dilation (slow-motion effects).
    *   **Functional Meaning**: Keeps camera movement smooth and responsive even if the game world is in slow motion.
    *   **Example Configuration**: `true`.

2.  **`TeleportDistanceThreshold` (float, cm)**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ForceUnits = "cm"))`**
    *   **Significance**: If the camera's *pivot target* moves further than this distance in one frame, camera lag is temporarily disabled to prevent a long, slow "catch-up" pan.
    *   **Functional Meaning**: Ensures the camera snaps quickly to the character after a teleportation or very large, sudden movement.
    *   **Example Configuration**: `200.0`.

3.  **`FirstPerson` (FAlsFirstPersonCameraSettings)**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")`**
    *   **Structure**:
        *   **`FieldOfView` (float, degrees)**: Default FOV for first-person mode. Example: `90.0`.
        *   **`CameraSocketName` (FName)**: The name of the socket on the character's mesh where the first-person camera should be positioned. Example: `TEXTVIEW("FirstPersonCamera")` (often on the head bone).
    *   **Functional Meaning**: Defines the camera's properties when `AlsCharacterOwner->GetViewMode() == AlsViewModeTags::FirstPerson` or when the `FirstPersonOverrideCurve` is active.

4.  **`ThirdPerson` (FAlsThirdPersonCameraSettings)**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")`**
    *   **Structure**:
        *   **`FieldOfView` (float, degrees)**: Default FOV for third-person mode. Example: `90.0`.
        *   **`FirstPivotSocketName` (FName)** & **`SecondPivotSocketName` (FName)**: Names of two sockets on the character mesh. The `UAlsCameraComponent` calculates the third-person camera pivot as the midpoint between these two sockets. Examples: `UAlsConstants::RootBoneName()` and `UAlsConstants::HeadBoneName()`.
        *   **`TraceRadius` (float, cm)**: Radius of the sphere sweep used for camera collision. Example: `15.0`.
        *   **`TraceChannel` (ECollisionChannel)**: Collision channel for the camera collision trace. Example: `ECC_Visibility`.
        *   **`TraceShoulderLeftSocketName` / `TraceShoulderRightSocketName` (FName)**: Sockets used as the *start* point for the camera collision trace, depending on the `bRightShoulder` state. Examples: `TEXTVIEW("ThirdPersonTraceShoulderLeft")`, `TEXTVIEW("ThirdPersonTraceShoulderRight")`.
        *   **`TraceOverrideOffset` (FVector3f)**: An offset applied to the camera trace start location if the `TraceOverrideCurve` is active. Example: `(X=0, Y=0, Z=40)`.
        *   **`bEnableTraceDistanceSmoothing` (bool)**: Enables smooth interpolation of the camera's distance when it's pushed by collision.
        *   **`TraceDistanceSmoothing` (FAlsTraceDistanceSmoothingSettings)**:
            *   **`InterpolationSpeed` (float)**: How quickly the camera interpolates back to its desired distance after a collision is cleared. Example: `3.0`.
    *   **Functional Meaning**: Defines all parameters for the standard third-person follow camera, including its pivot, collision properties, and smoothing.

5.  **`TopDown` (FAlsTopDownCameraSettings)** (If added as per "Method 1" in `UAlsCameraComponent` extension)
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")`**
    *   **Structure (as defined in earlier example)**:
        *   `FieldOfView`, `DesiredDistance`, `CameraPitchAngle`, `LocationLagSpeed`, `bUseFixedYaw`, `FixedYawAngle`.
    *   **Functional Meaning**: Defines all parameters for the top-down camera view.
        *   **Example Configuration**: `FieldOfView=70.0`, `DesiredDistance=1200.0`, `CameraPitchAngle=-80.0` (very steep), `LocationLagSpeed=8.0`, `bUseFixedYaw=true`, `FixedYawAngle=0.0` (fixed north-facing, for example).

6.  **`PostProcess` (FPostProcessSettings)**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")`**
    *   **Significance**: A standard Unreal Engine `FPostProcessSettings` struct.
    *   **Functional Meaning**: Allows you to define a set of default post-processing effects (bloom, depth of field, color grading, etc.) that can be applied by the `UAlsCameraComponent` based on its `PostProcessWeight`.
    *   **Example Configuration**: You might set up subtle depth of field here. Then, in `UAlsCameraComponent`, when the player aims, you could increase `PostProcessWeight` to make the DoF more pronounced.

#### **Adding/Adapting Functionality (How to Configure)**

*   **Create/Modify Data Asset**:
    1.  In your project's Content Browser, find or create your `DA_AlsCameraSettings` asset (or a copy if you want variations).
    2.  Open it. You'll see all the properties listed above.
*   **Configuring Third-Person**:
    *   **Pivot**: Choose `FirstPivotSocketName` (e.g., `pelvis` or `root`) and `SecondPivotSocketName` (e.g., `head`). The camera will try to orbit the midpoint. A lower first pivot makes the camera feel more grounded with the character's hips.
    *   **Collision**: Adjust `TraceRadius` based on your character size. A smaller radius allows the camera to get closer to walls before being pushed. Select an appropriate `TraceChannel`.
    *   **Shoulder Sockets**: Ensure sockets like `ThirdPersonTraceShoulderLeft` exist on your character's skeleton, positioned appropriately on the shoulders. These are the origin points for the camera collision probe.
*   **Configuring First-Person**:
    *   Set `FieldOfView`.
    *   Ensure a socket named whatever you put in `CameraSocketName` (e.g., `FirstPersonCamera`) exists on your character's head bone, positioned where the eyes would be.
*   **Configuring Top-Down (if added via Method 1)**:
    *   Set `DesiredDistance` to how far above the character the camera should be.
    *   Set `CameraPitchAngle` (e.g., -70 to -85 degrees for a steep top-down view, or -45 for an isometric-like view).
    *   Decide if `bUseFixedYaw` is needed. If true, the camera's world yaw is locked. If false, it will follow the character's yaw (or smoothed target yaw).
    *   Adjust `LocationLagSpeed` for follow responsiveness.
*   **Post-Processing**: Expand the `PostProcess` section and enable/tweak various effects (Bloom, Exposure, Lens Flares, Color Grading, etc.). The `UAlsCameraComponent::PostProcessWeight` property will control how much of these settings are applied.

**GAS Integration with Camera Settings**:
*   GAS abilities generally **do not directly modify `UAlsCameraSettings` at runtime.** The settings asset provides the *ruleset* for the camera.
*   Abilities influence *which part* of the ruleset is used by changing `AAlsCameraCharacter::ViewMode`.
*   For dynamic changes not covered by `ViewMode` switching:
    *   **FOV Zoom for Aiming**: A `GA_AimDownSights` ability could directly call `CameraComponent->SetFieldOfViewOverride(ZoomedFOV)` and `CameraComponent->SetFieldOfViewOverriden(true)`. On ending the ability, it calls `SetFieldOfViewOverriden(false)`.
    *   **Temporary Post-Process Effect**: A `GA_SpecialVision` ability could apply a `GameplayEffect` that grants a tag. The `UAlsCameraComponent` could be modified to check for this tag in its `TickCamera` and, if present, Lerp its `PostProcessWeight` towards 1.0 (or set it directly), and potentially even blend to a *different* set of `FPostProcessSettings` (you might add another `FPostProcessSettings` property to `UAlsCameraSettings` for this special vision mode).

### **Network Synchronization (for Camera Settings)**

*   Like other Data Assets, `UAlsCameraSettings` is **not replicated per frame**.
*   It's loaded by all clients and the server. The `UAlsCameraComponent` on each machine holds a pointer to its configured `UAlsCameraSettings` asset.
*   Consistency relies on all players having the same version of the Data Asset.
*   The *results* of these settings (the camera's final transform) are what matter for the player's view. The character's view rotation (`ReplicatedViewRotation`) *is* replicated, and each client's `UAlsCameraComponent` uses its local `UAlsCameraSettings` to interpret this and calculate the final camera position relative to the (replicated) character.

### **Debugging (for Camera System)**

*   **`ShowDebug Als.CameraShapes`**: (Bound to `Shift + 7` in `AAlsCharacterExample`)
    *   Visualizes: `PivotTargetLocation` (Green), `PivotLagLocation` (Orange), `PivotLocation` (Blue-Cyan). Helps understand camera lag and offsets.
    *   Also shows text display of `CameraFieldOfView` and `bRightShoulder`.
*   **`ShowDebug Als.CameraTraces`**: (Bound to `Shift + 8` in `AAlsCharacterExample`)
    *   Visualizes: The main camera collision sweep (Green if no hit, Red if hit), and the adjusted hit location. Also shows traces for geometry adjustment if the camera starts penetrated.
*   **`UAlsCameraComponent` Details Panel**:
    *   Select the `UAlsCameraComponent` (it's a child of your character's mesh) during PIE.
    *   Inspect "State" variables like `CameraLocation`, `CameraRotation`, `PivotLocation`, `TraceDistanceRatio` in real-time.
*   **`UAlsCameraAnimationInstance` Details Panel / Animation Debugger**:
    *   Inspect the output values of animation curves like `CameraOffsetXCurve`, `RotationLagCurve`, etc., to see how animations are influencing the camera.
*   **Modifying Settings Live**: Changes to the `DA_AlsCameraSettings` Data Asset in the editor *while PIE is running* will often take effect immediately, allowing for rapid iteration of camera feel.








--------------------------------------------------------------------------------------------------------------















## **III. Locomotion Features (Detailed Breakdown)**

This section elaborates on specific locomotion behaviors implemented in the ALS system. These features work together to create a responsive, visually appealing, and adaptable character movement system.

### **A. State Management (Gameplay Tags)**

#### **Scope**

This subsection focuses on how ALS utilizes Unreal Engine's Gameplay Tags to define and manage the character's various locomotion-related states. This covers the tags themselves and how they are used by `AAlsCharacter`, `UAlsCharacterMovementComponent`, and `UAlsAnimationInstance`.

#### **Purpose**

Gameplay Tags are used in ALS to provide a robust, hierarchical, and easily extensible system for categorizing and querying the character's state. This is fundamental to how ALS achieves its complex, state-driven locomotion and animation logic. The main purposes are:
*   **Clear State Definition**: Provides human-readable and organized names for different character conditions.
*   **Conditional Logic**: Allows game logic, animation blueprints, and GAS abilities to easily check for specific states or combinations of states.
*   **Data Driving**: Gameplay Tags often drive which settings (from `UAlsCharacterSettings` or `UAlsMovementSettings`) or animation behaviors are active.
*   **Network Replication**: Gameplay Tags (especially those on `AAlsCharacter`) are replicated, ensuring state consistency in multiplayer.

#### **Key Concepts/Components (Gameplay Tag Categories)**

The primary Gameplay Tags relevant to locomotion are defined in `AlsGameplayTags.h` and used as properties in `AAlsCharacter`.

1.  **`AlsLocomotionModeTags`**:
    *   **Property in `AAlsCharacter`**: `LocomotionMode` (FGameplayTag)
    *   **Tags**:
        *   `Als.LocomotionMode.Grounded`: Character is physically on a walkable surface.
            *   **Functional Meaning**: Enables grounded movement physics, foot IK, landing behaviors, and grounded-specific animations (walking, running, crouching).
        *   `Als.LocomotionMode.InAir`: Character is airborne.
            *   **Functional Meaning**: Enables falling physics, air control, jump/fall animations, and disables most grounded behaviors like foot locking.
    *   **Control**: Primarily set by `AAlsCharacter::OnMovementModeChanged` based on the `UCharacterMovementComponent`'s current `MovementMode`.

2.  **`AlsRotationModeTags`**:
    *   **Properties in `AAlsCharacter`**: `DesiredRotationMode` (FGameplayTag), `RotationMode` (FGameplayTag)
    *   **Tags**:
        *   `Als.RotationMode.VelocityDirection`: Character's body orients towards its current movement velocity.
            *   **Functional Meaning**: Character automatically faces the direction they are running. Often used for sprinting or less combat-focused movement.
        *   `Als.RotationMode.ViewDirection`: Character's body orients towards the camera's view direction (yaw).
            *   **Functional Meaning**: Allows for independent movement and camera aiming, common in third-person action games. Animations often include look offsets and body leaning.
        *   `Als.RotationMode.Aiming`: Character's upper body directly follows the camera's view for precise aiming, while the lower body attempts to keep up within limits.
            *   **Functional Meaning**: Used for aiming weapons. The spine and arms align with the camera, and specific aiming animations and IK are usually active.
    *   **Control**: `DesiredRotationMode` is set by player input or AI. `RotationMode` is then derived by `AAlsCharacter::RefreshRotationMode` based on `DesiredRotationMode`, `bDesiredAiming`, `ViewMode`, and current `LocomotionMode` and `Gait`.

3.  **`AlsStanceTags`**:
    *   **Properties in `AAlsCharacter`**: `DesiredStance` (FGameplayTag), `Stance` (FGameplayTag)
    *   **Tags**:
        *   `Als.Stance.Standing`: Character is in an upright, standing posture.
            *   **Functional Meaning**: Uses standing height collision, standard movement speeds, and standing animations.
        *   `Als.Stance.Crouching`: Character is in a lowered, crouched posture.
            *   **Functional Meaning**: Uses crouched height collision, reduced movement speeds, and crouching animations. Affects line of sight and stealth.
    *   **Control**: `DesiredStance` is set by player input or AI. `Stance` is updated by `AAlsCharacter::OnStartCrouch`/`OnEndCrouch` and `ApplyDesiredStance`.

4.  **`AlsGaitTags`**:
    *   **Properties in `AAlsCharacter`**: `DesiredGait` (FGameplayTag), `Gait` (FGameplayTag)
    *   **Tags**:
        *   `Als.Gait.Walking`: Slowest grounded movement speed.
            *   **Functional Meaning**: Activates walking animations, uses walking speed/acceleration parameters from `UAlsMovementSettings`.
        *   `Als.Gait.Running`: Medium grounded movement speed.
            *   **Functional Meaning**: Activates running animations, uses running speed/acceleration parameters.
        *   `Als.Gait.Sprinting`: Fastest grounded movement speed.
            *   **Functional Meaning**: Activates sprinting animations, uses sprinting speed/acceleration parameters. Often locks `RotationMode` to `VelocityDirection`.
    *   **Control**: `DesiredGait` is set by player input (e.g., sprint key). `Gait` is calculated by `AAlsCharacter::RefreshGait` based on current speed and `MaxAllowedGait` (which itself is derived from `DesiredGait` and other conditions like aiming or stance).

5.  **`AlsOverlayModeTags`**:
    *   **Property in `AAlsCharacter`**: `OverlayMode` (FGameplayTag)
    *   **Tags (Examples)**: `Als.OverlayMode.Default`, `Als.OverlayMode.Rifle`, `Als.OverlayMode.PistolOneHanded`, `Als.OverlayMode.Injured`, etc.
    *   **Purpose**: Primarily affects upper body animation and how the character holds items or expresses a condition, layering on top of base locomotion.
    *   **Functional Meaning**: `Default` means no specific overlay. `Rifle` would make the character adopt a rifle-holding pose. `Injured` might make the character limp or hold a wounded arm. This tag is heavily used by `UAlsAnimationInstance` to select and blend appropriate overlay animations.
    *   **Control**: Set by game logic, often via `AAlsCharacter::SetOverlayMode`.

6.  **`AlsLocomotionActionTags`**:
    *   **Property in `AAlsCharacter`**: `LocomotionAction` (FGameplayTag)
    *   **Tags (Examples)**: `Als.LocomotionAction.Mantling`, `Als.LocomotionAction.Rolling`, `Als.LocomotionAction.Ragdolling`, `Als.LocomotionAction.GettingUp`.
    *   **Purpose**: Indicates that the character is currently performing a special, often uninterruptible or state-overriding, action.
    *   **Functional Meaning**: When one of these tags is active, normal locomotion logic (input processing, rotation) might be suppressed or modified. The animation system switches to dedicated animation states or plays specific montages.
    *   **Control**: Set by the `Start...` functions for each action (e.g., `AAlsCharacter::StartMantlingImplementation` sets it to `Mantling`). Cleared when the action finishes.

7.  **`AlsGroundedEntryModeTags`**:
    *   **Property in `UAlsAnimationInstance`**: `GroundedEntryMode` (FGameplayTag) (Note: While defined in `AlsGameplayTags`, it's primarily used and managed within `UAlsAnimationInstance` based on `AAlsCharacter`'s actions).
    *   **Tags (Example)**: `Als.GroundedEntryMode.FromRoll`.
    *   **Purpose**: Provides context to the animation system about *how* the character just became grounded.
    *   **Functional Meaning**: Allows the animation blueprint to play a specific landing or transition animation. For example, if `LocomotionMode` changes to `Grounded` and `GroundedEntryMode` is `FromRoll`, a "roll landing" animation can be played instead of a generic jump land.
    *   **Control**: Set by `UAlsAnimationInstance::SetGroundedEntryMode`, often called by `AAlsCharacter` when an action completes (e.g., after a roll that ends on the ground).

#### **Methods & Functions (Related to Gameplay Tags in `AAlsCharacter`)**

*   **`SetViewMode(const FGameplayTag& NewViewMode)`**: Changes the character's view mode.
*   **`SetDesiredAiming(bool bNewDesiredAiming)`**: Sets the desire to aim.
*   **`SetDesiredRotationMode(const FGameplayTag& NewDesiredRotationMode)`**: Sets the preferred rotation mode.
*   **`SetDesiredStance(const FGameplayTag& NewDesiredStance)`**: Sets the preferred stance.
*   **`SetDesiredGait(const FGameplayTag& NewDesiredGait)`**: Sets the preferred gait.
*   **`SetOverlayMode(const FGameplayTag& NewOverlayMode)`**: Changes the character's overlay mode.
*   **`SetLocomotionAction(const FGameplayTag& NewLocomotionAction)`**: Sets the active special locomotion action.
*   **`On...Changed_Implementation()` Events**: (e.g., `OnRotationModeChanged_Implementation`, `OnOverlayModeChanged_Implementation`) Blueprint Native Events in `AAlsCharacter` that are triggered when the corresponding actual state tag changes. These allow Blueprint logic to react to state transitions.
*   **`RefreshRotationMode()`, `ApplyDesiredStance()`, `RefreshGait()`**: Internal functions in `AAlsCharacter` that use desired state tags and other conditions to determine and set the actual state tags.

#### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

**1. Adding a New `OverlayMode` (e.g., "CarryingHeavyObject")**

*   **C++ (`AlsGameplayTags.h` & `.cpp`)**:
    ```cpp
    // AlsGameplayTags.h
    namespace AlsOverlayModeTags { ALS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(CarryingHeavyObject) }
    // AlsGameplayTags.cpp
    namespace AlsOverlayModeTags { UE_DEFINE_GAMEPLAY_TAG(CarryingHeavyObject, FName{TEXTVIEW("Als.OverlayMode.CarryingHeavyObject")}) }
    ```
    Compile C++.
*   **Animation Blueprint (`ABP_AlsCharacter`)**:
    *   In your AnimGraph, add logic to your upper body layering to blend in a "carrying heavy object" pose animation when `OverlayMode == Als.OverlayMode.CarryingHeavyObject`. This typically involves a `Layered Blend Per Bone` node targeting the spine and arms.
    *   You might also want this overlay to affect movement speed.
*   **`AAlsCharacter` (or derived class)**:
    *   Create a function to set this state:
      ```cpp
      // MyAlsCharacter.h
      UFUNCTION(BlueprintCallable, Category = "MyCharacter|Actions")
      void StartCarryingHeavyObject();

      UFUNCTION(BlueprintCallable, Category = "MyCharacter|Actions")
      void StopCarryingHeavyObject();

      // MyAlsCharacter.cpp
      void AMyAlsCharacter::StartCarryingHeavyObject()
      {
          SetOverlayMode(AlsOverlayModeTags::CarryingHeavyObject);
          // Optionally, also cap the DesiredGait or MaxAllowedGait
          SetDesiredGait(AlsGaitTags::Walking);
          if (GetAlsCharacterMovementComponent())
          {
              GetAlsCharacterMovementComponent()->SetMaxAllowedGait(AlsGaitTags::Walking);
          }
      }
      void AMyAlsCharacter::StopCarryingHeavyObject()
      {
          SetOverlayMode(AlsOverlayModeTags::Default); // Revert to default
          // Optionally, restore default gait allowance
          SetDesiredGait(AlsGaitTags::Running); // Or whatever your default is
          if (GetAlsCharacterMovementComponent())
          {
              GetAlsCharacterMovementComponent()->SetMaxAllowedGait(AlsGaitTags::Sprinting); // Or default
          }
      }
      ```
*   **GAS Integration**:
    *   A `GameplayAbility` (`GA_PickupHeavyObject`) would be responsible for:
        1.  Checking if the character can pick up the object.
        2.  Calling `Character->StartCarryingHeavyObject()`.
        3.  Possibly applying a `GameplayEffect` that grants `State.IsCarryingHeavy` and perhaps a passive ability that periodically drains stamina while carrying.
    *   Another ability, `GA_DropHeavyObject`, would call `Character->StopCarryingHeavyObject()` and remove the associated `GameplayEffect`.

**2. Adding a New `LocomotionAction` (e.g., "Taunt")**

*   **C++ (`AlsGameplayTags.h` & `.cpp`)**:
    ```cpp
    // AlsGameplayTags.h
    namespace AlsLocomotionActionTags { ALS_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Taunting) }
    // AlsGameplayTags.cpp
    namespace AlsLocomotionActionTags { UE_DEFINE_GAMEPLAY_TAG(Taunting, FName{TEXTVIEW("Als.LocomotionAction.Taunting")}) }
    ```
*   **Animation Montage (`AM_Taunt`)**: Create a montage for the taunt animation.
    *   Inside `AM_Taunt`, add an `Als Anim Notify State Set Locomotion Action` that sets the `LocomotionAction` property to `Als.LocomotionAction.Taunting`. This ensures that while the montage plays, the character's state reflects "Taunting".
    *   Optionally, add an `Als Anim Notify State Early Blend Out` if the taunt should be interruptible by movement input or damage.
*   **GAS Integration**:
    *   Create a `GameplayAbility` (`GA_Taunt`).
    *   In `ActivateAbility` of `GA_Taunt`, use `PlayMontageAndWait` (Blueprint) or `AbilitySystemComponent->PlayMontage()` (C++) to play `AM_Taunt`.
    *   The ability will end when the montage finishes or is interrupted.
*   **Animation Blueprint (`ABP_AlsCharacter`)**:
    *   The `LocomotionAction` being `Taunting` (due to the notify state in the montage) can be used in the AnimGraph to:
        *   Potentially blend out or modify base locomotion if the taunt is not full-body root motion.
        *   Drive other layered effects (e.g., facial expression).

#### **Network Synchronization**

*   As covered in `AAlsCharacter` (Section 1-A), the `GameplayTag` properties like `DesiredGait`, `OverlayMode`, `LocomotionAction` (indirectly via actions) are replicated.
*   This ensures that when a state changes on the server (or an autonomous client initiates a change that's confirmed by the server), all clients will eventually have the same tag active, leading to synchronized animation states.
*   **RPCs (`_Implementation`) and `OnRep_` functions in `AAlsCharacter` are the primary mechanisms for synchronizing these tags.**
    *   Example: `AAlsCharacter::SetOverlayMode` calls `ServerSetOverlayMode`, which updates the server's `OverlayMode` and marks it dirty. This then replicates to clients, triggering `OnReplicated_OverlayMode`, which in turn calls `OnOverlayModeChanged_Implementation`.

#### **Debugging**

*   **`ShowDebug Als.State`**: This console command is invaluable. When your character is selected in PIE, it will display the current active `GameplayTag` for `ViewMode`, `LocomotionMode`, `RotationMode`, `Stance`, `Gait`, `OverlayMode`, and `LocomotionAction`.
*   **Gameplay Debugger (`'` key by default)**: If you have GAS integrated, the Gameplay Debugger can show active `GameplayTags` on the `AbilitySystemComponent`, which can be useful for verifying if GAS-driven state changes are applying the correct tags that ALS systems expect.
*   **Animation Debugger**: In the Animation Debugger, you can inspect the values of the `GameplayTag` properties within your `UAlsAnimationInstance` (which get them from `AAlsCharacter`) to see how they are driving your AnimGraph logic.













------------------------------------------------------------------------------------------------------------














### **B. Animation Layering & Blending**

#### **Scope**

This subsection details ALS's system for layering animations, allowing different body parts to be controlled by different animation sources simultaneously. This is primarily managed within the `UAlsAnimationInstance` and driven by animation curves.

#### **Purpose**

Animation layering provides a flexible way to combine animations for a more dynamic and nuanced character performance. Instead of playing monolithic, full-body animations for every state, ALS allows:
*   **Independent Upper/Lower Body**: The character can run (lower body) while aiming or holding a weapon (upper body).
*   **Additive Animations**: Small, localized animations (like recoil or a head nod) can be added on top of base animations.
*   **Slot-Based Overrides**: Montages can play in specific slots, temporarily overriding only parts of the body (e.g., an upper-body attack montage).
*   **Smooth Transitions**: Layer blend weights can be animated via curves for smooth transitions between different layered states.

#### **Key Concepts/Components**

1.  **`FAlsLayeringState` (in `UAlsAnimationInstance`)**
    *   **Purpose**: This struct holds the current blend weights (alpha values, typically 0-1) for various body parts or animation layers.
    *   **Key Members (Examples)**:
        *   `HeadBlendAmount`, `HeadAdditiveBlendAmount`, `HeadSlotBlendAmount`
        *   `ArmLeftBlendAmount`, `ArmLeftAdditiveBlendAmount`, `ArmLeftSlotBlendAmount`, `ArmLeftLocalSpaceBlendAmount`, `ArmLeftMeshSpaceBlendAmount` (similar for ArmRight)
        *   `SpineBlendAmount`, `SpineAdditiveBlendAmount`, `SpineSlotBlendAmount`
        *   `PelvisBlendAmount`, `PelvisSlotBlendAmount`
        *   `LegsBlendAmount`, `LegsSlotBlendAmount`
    *   **Functional Meaning**: These float values directly control the "Alpha" input of "Layered Blend Per Bone" nodes (or similar blending nodes) in the Animation Blueprint's AnimGraph. A value of `1.0` means the layer is fully active for that part; `0.0` means it's inactive.
    *   **Update Logic**: These values are updated in `UAlsAnimationInstance::RefreshLayering()` by reading animation curves.

2.  **Layering Animation Curves (Defined in `UAlsConstants.h`)**
    *   **Purpose**: These are float curves authored directly into animation sequences (e.g., an "Idle_Rifle" animation). They tell the ALS system how much a particular layer should be active when that animation is playing.
    *   **Examples**:
        *   `LayerHeadCurveName()` (`LayerHead`): Controls the blend weight for the head layer.
        *   `LayerArmLeftAdditiveCurveName()` (`LayerArmLeftAdditive`): Controls additive blending for the left arm.
        *   `LayerArmLeftLocalSpaceCurveName()` (`LayerArmLeftLocalSpace`): Controls if the left arm animation is in local or mesh space.
        *   `LayerArmLeftSlotCurveName()` (`LayerArmLeftSlot`): Typically set to 1.0 in animations that are meant to be played in an arm slot, and 0.0 in full-body animations that should *not* be overridden by arm slot animations unless explicitly desired.
    *   **Functional Meaning**: An "Idle_Rifle" animation would have `LayerArmLeftCurve`, `LayerArmRightCurve`, and `LayerSpineCurve` set to `1.0` for its duration, indicating that it should fully control these body parts. A base locomotion animation might have these set to `0.0` if it expects an overlay to provide the arm/spine pose.

3.  **"Layered Blend Per Bone" Node (AnimGraph)**
    *   **Purpose**: This standard Unreal Engine AnimGraph node is the workhorse for ALS's layering. It blends between a "Base Pose" and multiple "Blend Poses" based on per-bone weight.
    *   **ALS Usage**:
        *   You typically have a chain of these nodes.
        *   The "Base Pose" for the first one might be the full-body locomotion.
        *   "Blend Pose 0" might be an upper-body overlay (e.g., aiming), blended using `SpineBlendAmount` on the spine bones and `ArmLeft/RightBlendAmount` on arm bones. The "Blend Weight" input of the node for this layer would be driven by these properties from `FAlsLayeringState`.
        *   "Blend Pose 1" could be an additive recoil animation, blended using `HeadAdditiveBlendAmount` or `ArmAdditiveBlendAmount`.

4.  **Animation Slots (`UAlsConstants.h`)**
    *   **Examples**: `TransitionSlotName()`, `TurnInPlaceStandingSlotName()`, `TurnInPlaceCrouchingSlotName()`. You would add more like `FullBodyActionSlot`, `UpperBodySlot`.
    *   **Purpose**: Montages play in specific slots. "Slot" nodes in the AnimGraph allow these montages to override the pose coming into them.
    *   **Functional Meaning**: `LayerXYZSlotCurveName()` curves in animations determine if the base animation should "allow" a slot montage to play over it for that body part. If `LayerArmLeftSlotCurve` is `1.0` in a montage played in an "UpperBody" slot, and `0.0` in the base locomotion animation, the montage will fully override the left arm of the locomotion.

#### **Methods & Functions (in `UAlsAnimationInstance`)**

*   **`RefreshLayering()`**:
    *   **Purpose**: Called in `NativeThreadSafeUpdateAnimation`.
    *   **What it accomplishes**: Reads all the `Layer...CurveName()` values from the currently evaluated animation pose (using `GetProxyOnAnyThread<FAnimInstanceProxy>()->GetAnimationCurves()`). It then populates the `LayeringState` struct with these values.
    *   The logic for `ArmLeftMeshSpaceBlendAmount` (and Right) is: `!FAnimWeight::IsFullWeight(LayeringState.ArmLeftLocalSpaceBlendAmount)`. This means mesh space blending is fully active *unless* local space blending is also fully active.

#### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

**1. Adding a New Overlay Layer (e.g., "Holding Shield" on Left Arm)**

*   **A. Define New Animation Curves (Optional but Recommended for Control)**
    *   If not already present, you might consider if you need more granular curves than just `LayerArmLeft`. For a shield, `LayerArmLeftCurve` and `LayerArmLeftSlotCurve` are likely sufficient.
    *   Use `UAlsAnimationModifier_CreateLayeringCurves` to add these curves to your "Shield_Idle", "Shield_Block_Pose" animations. Set `LayerArmLeftCurve` to `1.0` in these animations.

*   **B. Animation Blueprint (`ABP_AlsCharacter`) - AnimGraph Setup**
    1.  **Get Layering State**: In your AnimBP's EventGraph (e.g., `Event Blueprint Thread Safe Update Animation`), make sure you are getting the `LayeringState` from the `UAlsAnimationInstance` parent and promoting its members (like `ArmLeftBlendAmount`) to local variables if needed for easier access in the AnimGraph. (ALS `UAlsAnimationInstance` already makes `LayeringState` a `BlueprintReadOnly` UPROPERTY, so you can access it directly).
    2.  **Add Layered Blend**: Find an appropriate place in your AnimGraph, likely after base locomotion and before any full-body additives.
        *   Add a `Layered Blend Per Bone` node.
        *   Connect the pose from the previous node to its `Base Pose` input.
        *   For the `Blend Poses 0` input, connect your "Shield Holding" pose (this could be a state in a state machine, a direct animation sequence, or another cached pose).
        *   Click on the `Layered Blend Per Bone` node. In the Details panel:
            *   Add a "Branch Filter". Set `Bone Name` to `clavicle_l` (or your character's left clavicle/shoulder bone).
            *   Set `Blend Depth` to a value that includes all arm bones (e.g., 5 or 6). Set `Mesh Space Rotation Blend` and `Mesh Space Scale Blend` to true if needed.
        *   Connect `LayeringState.ArmLeftBlendAmount` (from your `UAlsAnimationInstance` reference) to the `Blend Weights 0` input of the node.

*   **C. Triggering the Layer (via `OverlayMode`)**
    1.  In `ABP_AlsCharacter`'s EventGraph (e.g., `Event Blueprint Thread Safe Update Animation`), after getting the `OverlayMode` from `UAlsAnimationInstance`:
        ```blueprint
        # Event Blueprint Thread Safe Update Animation
        Get Overlay Mode (from AlsAnimInstance) == Als.OverlayMode.Shield (your new tag) ? (Branch)
          - True: Set local variable "TargetArmLeftBlend" to 1.0
          - False: Set local variable "TargetArmLeftBlend" to 0.0 (or to value from another active overlay)

        # Then, smoothly interpolate the actual blend amount used in AnimGraph:
        LayeringState.ArmLeftBlendAmount = FInterpTo(LayeringState.ArmLeftBlendAmount, TargetArmLeftBlend, DeltaTime, BlendSpeed)
        ```
        *(Note: `UAlsAnimationInstance::RefreshLayering` actually reads curves to set these. For an overlay driven by a tag like `OverlayMode`, you'd typically have an animation state (e.g., "Overlay_Shield_Idle") that itself has the `LayerArmLeftCurve` set to 1.0. The AnimBP then just needs to play this "Overlay_Shield_Idle" state when the `OverlayMode` tag is active, and the curve system handles the rest.)*

        **Corrected approach for `OverlayMode` driven layers:**
        *   In `ABP_AlsCharacter` AnimGraph, have a state machine for Overlays.
        *   This state machine transitions to "Shield_Idle_State" when `OverlayMode == Als.OverlayMode.Shield`.
        *   The "Shield_Idle_State" plays an animation (e.g., `A_Shield_Idle`) which has `LayerArmLeftCurve` set to 1.0.
        *   `UAlsAnimationInstance::RefreshLayering()` will automatically pick up this `LayerArmLeftCurve` value when `A_Shield_Idle` is active, and set `LayeringState.ArmLeftBlendAmount` to 1.0.
        *   Your `Layered Blend Per Bone` node in the AnimGraph then just uses `LayeringState.ArmLeftBlendAmount`.

*   **GAS Integration**:
    *   A `GA_EquipShield` ability would call `Character->SetOverlayMode(Als.OverlayMode.Shield)`.
    *   The character's `OverlayMode` replicates.
    *   `UAlsAnimationInstance` updates its `OverlayMode` property.
    *   The AnimBP transitions to the shield overlay animation state.
    *   `RefreshLayering()` reads the curves from the shield animation, setting `LayeringState.ArmLeftBlendAmount` (and others) correctly.
    *   The `Layered Blend Per Bone` node applies the shield arm animation.

**2. Adding an Additive Recoil Animation**

*   **Animation**: Create a short, additive animation for weapon recoil (e.g., `A_Recoil_Rifle_Additive`). Ensure it's marked as "Additive" in its asset details (e.g., "Mesh Space", "Local Space" based on reference pose).
*   **Curve**: In `A_Recoil_Rifle_Additive`, you might have `LayerArmRightAdditiveCurve` set to `1.0` for its duration (or keyed to fade in/out).
*   **AnimBP AnimGraph**:
    *   After your main upper body/aiming logic, add an "Apply Additive" node.
    *   Connect the main pose to "Base".
    *   Connect your `A_Recoil_Rifle_Additive` (perhaps played as a dynamic montage or a single frame pose) to "Additive".
    *   Connect `LayeringState.ArmRightAdditiveBlendAmount` to the "Alpha" of the "Apply Additive" node.
*   **Triggering**:
    *   **GAS**: A `GA_FireWeapon` ability, after firing, could play a very short, non-looping montage of the recoil animation in a dedicated additive slot, or it could trigger a `GameplayCue` (e.g., `GameplayCue.Weapon.Recoil`).
    *   **GameplayCue (Blueprint)**: The `GameplayCueNotify_Actor` Blueprint for `GameplayCue.Weapon.Recoil` could get the `UAlsAnimationInstance` and directly set a temporary variable like `RecoilAlpha` to `1.0`, then use a timeline or delay to set it back to `0.0`. The `LayeringState.ArmRightAdditiveBlendAmount` would then be interpolated towards this `RecoilAlpha`.
    *   **Alternatively (and often better for recoil)**: The weapon itself might have an `AnimInstance` and play its own recoil animation, which is then linked or copied to the character's arms if they are IK-ed to the weapon. Or, the weapon firing directly drives procedural recoil on the character's arms in Control Rig. For simple additive recoil on character, the `LayeringState.ArmXYZAdditiveBlendAmount` is the way to go.

#### **Network Synchronization**

*   The `FAlsLayeringState` struct in `UAlsAnimationInstance` is **not directly replicated**.
*   It's calculated locally on each client and the server based on:
    1.  The currently playing animations (base locomotion, overlays, montages).
    2.  The animation curves embedded within those animations.
*   Since the *triggering* of these animations (e.g., changing `OverlayMode` which is replicated, or playing a GAS-replicated montage) is synchronized, the resulting curve values and thus the `LayeringState` should be consistent enough across machines for visual coherency.
*   Discrepancies can arise if clients have different animation assets or if there's significant lag causing one client to be in a different part of an animation than another.

#### **Debugging**

*   **`ShowDebug Als.Curves`**: Essential for seeing the live float values of all `Layer...` curves. This tells you what `RefreshLayering()` is reading.
*   **Animation Debugger**:
    *   Inspect the AnimGraph: Check the inputs to your `Layered Blend Per Bone` nodes and `Apply Additive` nodes. Ensure the correct alpha values from `LayeringState` are being piped in.
    *   Verify bone weights: Select a `Layered Blend Per Bone` node and look at its "Bone Blend Weights" to ensure you're targeting the correct bones with the correct influence.
*   **`UAlsAnimationInstance` Details Panel**: In PIE, inspect the `LayeringState` struct directly to see the calculated blend amounts.
*   **Visual Inspection**: Simply observe the character. If an arm isn't blending correctly, or an overlay isn't appearing, it's likely an issue with:
    *   The `OverlayMode` tag not being set correctly on `AAlsCharacter`.
    *   The animation for that overlay not having the correct `Layer...Curve` values set to `1.0`.
    *   The `Layered Blend Per Bone` node in the AnimGraph not being configured with the right bones or blend weight input.















---------------------------------------------------------------------------------------------------------


















### **C. Foot IK & Placement**

#### **Scope**

This subsection details ALS's Foot Inverse Kinematics (IK) and placement system. This system is responsible for making the character's feet realistically interact with the ground geometry, preventing floating feet or feet clipping through slopes and steps. It's a combination of C++ logic in `UAlsAnimationInstance`, animation curves, and Control Rig.

#### **Purpose**

The main goals of the Foot IK & Placement system are:
*   **Ground Adaptation**: Adjust foot positions and orientations to match the terrain underneath them.
*   **Foot Locking**: Keep feet planted on the ground during certain phases of movement (e.g., when a foot should be bearing weight) to prevent sliding.
*   **Visual Realism**: Enhance the believability of character movement by making foot interactions with the environment look natural.

#### **Key Concepts/Components**

1.  **`FAlsFeetState` (in `UAlsAnimationInstance`)**
    *   **Purpose**: This struct is the central data container for all foot IK related information.
    *   **Key Members**:
        *   **`PelvisRotation` (FQuat4f)**: Current rotation of the character's pelvis bone. Used as a reference for foot locking angle limits.
        *   **`FootPlantedAmount` (float, -1 to 1)**: A value derived from the `FootPlantedCurveName()`.
            *   `< 0.5` absolute value: Foot is considered planted or about to plant.
            *   `> 0.5` absolute value: Foot is in the air.
            *   Sign: Positive for right foot, negative for left foot.
            *   **Functional Meaning**: Helps determine which foot is currently supporting weight and should be prioritized for locking or IK.
        *   **`FeetCrossingAmount` (float, 0 to 1)**: Derived from `FeetCrossingCurveName()`. Indicates if the feet are in a "crossed over" state (e.g., during some turns).
        *   **`Left` / `Right` (FAlsFootState)**: Two instances of `FAlsFootState`, one for each foot.
            *   **`FAlsFootState` Members**:
                *   **`LockAmount` (float, 0-1)**: How much the foot is "locked" to its `LockLocation` and `LockRotation`. Driven by `FootLeft/RightLockCurveName()`.
                *   **`ThighAxis` (FVector3f)**: The normalized vector from the pelvis to the thigh, in component space of the thigh bone. Calculated once at initialization. Used for foot lock angle limiting.
                *   **`TargetLocation` / `TargetRotation` (FVector / FQuat)**: The desired world-space location/rotation of the foot *from the animation data* (e.g., from the `ik_foot_l/r` or `VB foot_l/r` bone sockets) *before* any IK adjustments.
                *   **`LockLocation` / `LockRotation` (FVector / FQuat)**: The world-space location/rotation where the foot is currently locked. This is updated when `LockAmount` becomes 1.
                *   **`LockComponentRelativeLocation` / `...Rotation`**: The `LockLocation`/`Rotation` expressed relative to the character's capsule component.
                *   **`LockMovementBaseRelativeLocation` / `...Rotation`**: The `LockLocation`/`Rotation` expressed relative to the character's current movement base (if any). Important for keeping feet planted on moving platforms.
                *   **`FinalLocation` / `FinalRotation` (FVector3f / FQuat4f)**: The *final* calculated location/rotation of the foot in *component space*, after all IK, locking, and offsets have been applied. This is what's fed into Control Rig.
    *   **Update Logic**: `FAlsFeetState` is updated in `UAlsAnimationInstance::RefreshFeetOnGameThread()` (for `TargetLocation/Rotation` from sockets) and `UAlsAnimationInstance::RefreshFeet()` (for IK calculations, locking, and final positions).

2.  **Foot IK Animation Curves (Defined in `UAlsConstants.h`)**
    *   **`FootLeftIkCurveName()` / `FootRightIkCurveName()` (`FootLeftIk`, `FootRightIk`)**:
        *   **Values**: 0 to 1.
        *   **Functional Meaning**: Controls the blend weight for applying foot IK adjustments (from ground traces) to each foot. `1.0` means full IK adjustment, `0.0` means the foot follows the raw animation. Typically `1.0` when the foot is near/on the ground, and `0.0` when it's high in the air (e.g., during a kick or high step).
    *   **`FootLeftLockCurveName()` / `FootRightLockCurveName()` (`FootLeftLock`, `FootRightLock`)**:
        *   **Values**: 0 to 1.
        *   **Functional Meaning**: Controls the `LockAmount` for each foot. When this curve hits `1.0` in an animation (e.g., at the point of full foot plant in a walk cycle), the foot's current traced ground position (`TargetLocation`/`TargetRotation` *after* ground trace offset) is captured as the `LockLocation`/`LockRotation`. As long as this curve stays at `1.0` and `LockAmount` is `1.0`, the foot will try to stay at that locked spot, even if the raw animation would move it. When the curve drops below `1.0`, `LockAmount` starts to decrease, and the foot blends back towards the raw animation / IK target.
    *   **`FootPlantedCurveName()` (`FootPlanted`)**: See `FAlsFeetState::FootPlantedAmount`.
    *   **`FeetCrossingCurveName()` (`FeetCrossing`)**: See `FAlsFeetState::FeetCrossingAmount`.

3.  **Control Rig (`CR_Als_Character` or similar)**
    *   **Purpose**: Control Rig performs the final procedural adjustments to the foot bones based on the data calculated by `UAlsAnimationInstance`.
    *   **Key ALS Rig Units Used**:
        *   **`FAlsRigUnit_FootOffsetTrace`**: Performs a line trace downwards from the animated foot position to find the actual ground. Outputs `OffsetLocationZ` (how much to raise/lower the foot) and `OffsetNormal` (the surface normal).
        *   **`FAlsRigUnit_ApplyFootOffsetLocation`**: Takes the `OffsetLocationZ` from the trace and the `PelvisOffset` (how much the pelvis itself has been IK-adjusted downwards), and applies a smoothed and constrained vertical offset to the foot. It also considers leg length and pelvis distance to prevent overstretching or compression.
        *   **`FAlsRigUnit_ApplyFootOffsetRotation`**: Takes the `OffsetNormal` from the trace and applies a rotation to the foot to align it with the ground slope, respecting joint limits (`Swing1/2LimitAngle`, `TwistLimitAngle`).
    *   **Input**: Receives `FAlsControlRigInput` from `UAlsAnimationInstance`, which includes `FootLeftLocation`, `FootLeftRotation` (these are the `FinalLocation`/`FinalRotation` from `FAlsFeetState`), and other parameters like `bFootOffsetAllowed`.

4.  **Skeleton Sockets/Virtual Bones (`UAlsConstants.h`)**
    *   **`FootLeftIkBoneName()` (`ik_foot_l`) / `FootRightIkBoneName()` (`ik_foot_r`)**: Actual IK effector bones in the skeleton, often positioned at the ball or heel of the foot. Control Rig directly manipulates these.
    *   **`FootLeftVirtualBoneName()` (`VB foot_l`) / `FootRightVirtualBoneName()` (`VB foot_r`)**: Virtual bones created in the skeleton that follow the regular `foot_l`/`foot_r` bones. If `Settings->General.bUseFootIkBones` is `false`, ALS uses these virtual bones as the *source* for the `TargetLocation`/`TargetRotation` of the feet before IK. This provides a clean animation target that isn't itself affected by IK.
    *   **Configuration**: If using `ik_foot_l/r` bones, they should be part of your character's skeleton. If using virtual bones, they need to be set up in the Skeleton asset (Source: `foot_l`, Target: `foot_l` - yes, same bone to create a child VB that just copies transform).

#### **Methods & Functions (in `UAlsAnimationInstance`)**

*   **`RefreshFeetOnGameThread()`**:
    *   **Purpose**: Called from `NativeUpdateAnimation` (Game Thread).
    *   **What it accomplishes**: Gets the *initial* `TargetLocation` and `TargetRotation` for each foot by reading the socket transforms of either the IK bones (e.g., `ik_foot_l`) or virtual bones (e.g., `VB foot_l`) from the `SkelMeshComponent`. This provides the raw animated position of the feet for the current frame. Also updates `FeetState.PelvisRotation`.

*   **`RefreshFeet(float DeltaTime)`**:
    *   **Purpose**: Called from `NativeThreadSafeUpdateAnimation` (Worker Thread). This is where most of the IK logic happens.
    *   **What it accomplishes**:
        *   Reads `FootPlantedCurveName()` and `FeetCrossingCurveName()`.
        *   Calls `RefreshFoot()` for each foot.

*   **`RefreshFoot(FAlsFootState& FootState, const FName& IkCurveName, const FName& LockCurveName, const FTransform& ComponentTransformInverse, float DeltaTime)`**:
    *   **Purpose**: Handles IK logic for a single foot.
    *   **What it accomplishes**:
        *   Reads the `IkCurveName` (e.g., `FootLeftIkCurveName()`) and `LockCurveName` for the current foot.
        *   Calls `ProcessFootLockTeleport()` to adjust lock location if character teleported.
        *   Calls `ProcessFootLockBaseChange()` to adjust lock location if movement base changed.
        *   Calls `RefreshFootLock()` to update the `LockAmount` and potentially capture a new `LockLocation`/`Rotation` or blend out of an existing lock.
        *   The `FinalLocation` and `FinalRotation` (which will be passed to Control Rig) are calculated here by blending between the animated `TargetLocation/Rotation` and the `LockLocation/Rotation` based on `LockAmount`.
        *   **Important**: The actual ground tracing and application of vertical/rotational offsets to `FinalLocation/Rotation` happens *inside Control Rig* using the `FAlsRigUnit_FootOffsetTrace`, `FAlsRigUnit_ApplyFootOffsetLocation`, and `FAlsRigUnit_ApplyFootOffsetRotation` units. `UAlsAnimationInstance` prepares the *target* for these Control Rig units.

*   **`ProcessFootLockTeleport(...)` & `ProcessFootLockBaseChange(...)`**:
    *   Handle adjustments to the `LockLocation`/`Rotation` when the character teleports or its movement base changes, to prevent the locked foot from being left behind or incorrectly positioned.

*   **`RefreshFootLock(...)`**:
    *   **Purpose**: Core logic for foot locking.
    *   **What it accomplishes**:
        *   Reads the `LockCurveName`.
        *   Decreases `LockAmount` if the character is moving fast, in the air, or if `bDisableFootLock` (from `UAlsAnimationInstanceSettings`) is true.
        *   If the `LockCurve` value is 1 and `LockAmount` was previously less than 1, it captures the current foot's `TargetLocation` and `TargetRotation` (which should already be adjusted by Control Rig's ground trace for that frame via the `FAlsControlRigInput` being based on *last frame's* IK solution) as the new `LockLocation` and `LockRotation`.
        *   If `LockAmount` > 0, it keeps the `LockLocation`/`Rotation` relative to the movement base if applicable.
        *   It applies an angle limit (`Settings->Feet.FootLockAngleLimit`) to prevent leg twisting when the character rotates while a foot is locked.
        *   Finally, it calculates `FootState.FinalLocation` and `FootState.FinalRotation` by interpolating between the raw animation target (`FootState.TargetLocation/Rotation`) and the `FootState.LockLocation/Rotation` using `FootState.LockAmount`. These `FinalLocation/Rotation` values are then passed to Control Rig.

#### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

**1. Adjusting Foot IK Strength or Behavior**

*   **Animation Curves (Primary Method)**:
    *   **To make feet stick more/less**: Modify the `FootLeftLockCurve` and `FootRightLockCurve` in your walk/run/idle animations.
        *   A longer duration where the curve is `1.0` will keep the foot locked longer.
        *   A faster transition from `1.0` to `0.0` will unlock the foot more quickly.
    *   **To enable/disable IK for certain actions**: Modify the `FootLeftIkCurve` and `FootRightIkCurve`.
        *   For a jump animation where feet should leave the ground freely, set these curves to `0.0`.
        *   For a very specific "foot slide" attack, you might temporarily set these to `0.0`.
    *   **Tool**: Use `UAlsAnimationModifier_CreateCurves` to add/modify these curves in your animation sequences.

*   **`UAlsAnimationInstanceSettings` (`DA_AlsAnimationInstanceSettings`)**:
    *   **`Feet.bDisableFootLock` (bool)**: Globally disables all foot locking if checked. Useful for debugging or specific gameplay scenarios.
    *   **`Feet.FootLockAngleLimit` (float)**: Adjust this to control how much the character's leg can twist when a foot is locked and the body rotates. A smaller value prevents more twisting but might cause earlier unlocking.
        *   **Example Configuration**: Default is `90.0`. Try `70.0` for less twist tolerance, or `110.0` for more.

*   **Control Rig (`CR_Als_Character`)**:
    *   **Trace Parameters**: Inside the Control Rig graph, find the `AlsFootOffsetTrace` nodes for each foot. You can adjust:
        *   `TraceDistanceUpward`/`Downward`: How far the trace looks for ground.
        *   `WalkableFloorAngle`: Max slope the foot will consider "ground".
        *   `FootHeight`: The offset from the bone to the bottom of the foot.
    *   **Interpolation**: In `AlsApplyFootOffsetLocation` and `AlsApplyFootOffsetRotation` nodes:
        *   `OffsetInterpolationFrequency`/`DampingRatio` (Location): Controls the springiness of vertical foot adjustment.
        *   `OffsetInterpolationSpeed` (Rotation): Controls how quickly the foot orients to the slope.
    *   **Limits**: In `AlsApplyFootOffsetRotation`:
        *   `Swing1/2LimitAngle`, `TwistLimitAngle`: Define how much the foot can rotate relative to the calf to match the ground slope. These prevent unnatural ankle breaks.

**2. Integrating Foot IK with a New Action (e.g., "Stomp" Attack)**

*   **Animation (`AM_Stomp`)**: Create a stomp animation montage.
    *   **Curves**:
        *   During the wind-up and actual stomp, you might want `FootLeftIkCurve` (or right) to be `0.0` to allow the foot to follow the authored animation precisely without IK interference.
        *   Just before impact and during impact, `FootLeftLockCurve` should be `0.0` (not locked).
        *   After impact, as the character recovers, you might quickly ramp `FootLeftIkCurve` to `1.0` and `FootLeftLockCurve` to `1.0` if the foot should plant firmly.
*   **GAS Implementation**:
    *   `GA_Stomp` plays `AM_Stomp`.
    *   The animation curves will naturally control the IK and locking behavior during the montage.
*   **Control Rig**: No specific changes needed in Control Rig unless the stomp requires unique IK behavior not covered by existing units (e.g., deforming soft ground, which is much more advanced).

**3. Common New Feature: Adjusting Foot IK for Different Shoe Types (e.g., High Heels)**

*   **Problem**: Default `FootHeight` in Control Rig might be for flat shoes. High heels require the "bottom" of the foot to be effectively lower.
*   **Solution A (Simplest - Global Change)**:
    *   Modify the `FootHeight` input on the `AlsFootOffsetTrace` nodes in `CR_Als_Character` to match the heel height. This affects all animations.
*   **Solution B (More Flexible - Per-Outfit/Shoe)**:
    1.  **C++ (`UAlsAnimationInstance` or `AAlsCharacter`)**: Add a new replicated property or Gameplay Tag.
        ```cpp
        // AAlsCharacter.h
        // UPROPERTY(Replicated, BlueprintReadOnly) FGameplayTag CurrentFootwearType;
        // OR
        UPROPERTY(Replicated, BlueprintReadOnly) float CurrentHeelHeightOffset = 0.0f;
        ```
    2.  **GAS**: When equipping different shoes, an ability/effect sets `CurrentFootwearType` or `CurrentHeelHeightOffset`.
    3.  **`UAlsAnimationInstance`**:
        *   In `GetControlRigInput()`, add `HeelHeightOffset` as a new member to `FAlsControlRigInput`.
        *   Populate `ControlRigInput.HeelHeightOffset` from `Character->CurrentHeelHeightOffset`.
    4.  **Control Rig (`CR_Als_Character`)**:
        *   Add a `HeelHeightOffset` input variable.
        *   In the graph, subtract this `HeelHeightOffset` from the `FootHeight` pin of the `AlsFootOffsetTrace` node (or add it to the `FootTargetLocation.Z` *before* the trace if that makes more sense for your setup).
        ```rigVM
        // Inside Control Rig Graph, before AlsFootOffsetTrace for Left Foot
        RigVM.FootTargetLocationL.Z = RigVM.Input.FootLeftLocation.Z - RigVM.HeelHeightOffset;
        // ... then use RigVM.FootTargetLocationL as input to AlsFootOffsetTrace.FootTargetLocation
        ```

#### **Network Synchronization**

*   **`FAlsFeetState`**: This struct and its members are **not directly replicated**. It's calculated locally on each client and the server.
*   **Consistency Source**:
    1.  **Replicated Character Transform/Velocity**: The base position and movement of the character are replicated.
    2.  **Replicated Animation State**: Animation montages (and their playback time/curves) are replicated if played via GAS/ASC. The character's high-level state tags (`OverlayMode`, `LocomotionMode`, etc.) that influence which base animations play are also replicated.
    3.  **World Geometry**: Assumed to be (mostly) static and identical for all clients for ground traces.
    4.  **Control Rig Logic**: The Control Rig graph is deterministic. Given the same inputs, it produces the same outputs.
*   **Inputs to Foot IK that ARE Replicated (Indirectly or Directly)**:
    *   `AAlsCharacter::MovementBase` information (for foot locking on moving platforms).
    *   The overall character location/rotation.
    *   The animation pose itself (driven by replicated states/montages).
*   **Process**:
    1.  `UAlsAnimationInstance` calculates `TargetLocation`/`TargetRotation` from animated socket positions (which are derived from replicated animation states).
    2.  It calculates `LockAmount` based on animation curves (from replicated animation states).
    3.  It calculates `FinalLocation`/`FinalRotation` (the data for Control Rig) based on these.
    4.  Control Rig (`AlsFootOffsetTrace`, etc.) runs locally on each machine, tracing against the local world geometry.
*   Because the inputs (animated foot position, character transform, world geometry) are largely synchronized, the outputs of the foot IK system (final foot positions) should be visually very similar across clients, even if not bit-for-bit identical due to floating-point precision or slight differences in physics ticks for traces.
*   **No `OnRep_` or RPCs for `FAlsFeetState`**: It's a derived, calculated state.

#### **Debugging**

*   **`ShowDebug Als.Curves`**: Essential for checking `FootLeftIkCurve`, `FootRightIkCurve`, `FootLeftLockCurve`, `FootRightLockCurve`, `FootPlantedCurve`. If these curves are incorrect in your animations, foot IK will behave unexpectedly.
*   **Control Rig Debugger**:
    *   Open your `CR_Als_Character` asset and enable debugging.
    *   In PIE, with the Animation Debugger open and your character selected, you can step into the Control Rig graph.
    *   Inspect the inputs and outputs of `AlsFootOffsetTrace` (see trace start/end, hit location, normal), `AlsApplyFootOffsetLocation` (see calculated offset), and `AlsApplyFootOffsetRotation`. This is the BEST way to see exactly why a foot is being placed where it is.
*   **`UAlsAnimationInstance` Details Panel**: In PIE, inspect the `FeetState` property and its sub-members (`Left.FinalLocation`, `Left.LockAmount`, `Right.OffsetLocationZ` etc.) to see the data being fed *into* Control Rig.
*   **Visualizers in Control Rig Editor**: When editing `CR_Als_Character`, you can enable "Draw Hierarchy" and other debug drawing options in the preview viewport to visualize bone transforms.
*   **ALS Debug Traces (`ShowDebug Als.Traces` or the `bDrawDebug` on `AlsFootOffsetTrace` unit)**:
    *   The `AlsFootOffsetTrace` unit in Control Rig has a `bDrawDebug` input pin. If you connect a boolean 'true' to this (e.g., via a global debug variable accessible in Control Rig), it will draw the line trace it performs for each foot. The color is usually blue/purple as defined in the unit. This helps see if the trace is hitting the ground correctly.






















--------------------------------------------------------------------------------------------------------




















## **III-D. Dynamic Transitions**

### **Scope**

This subsection details the systems within `UAlsAnimationInstance` responsible for managing automatic and contextual animation transitions. These are not player-initiated actions like jumping or rolling, but rather subtle, procedural animations that enhance the fluidity and responsiveness of character movement, particularly during starts, stops, and idle rotations.

### **Purpose**

Dynamic transitions aim to:
*   **Improve Visual Quality**: Make character movement look less robotic by adding small, anticipatory, or follow-through motions.
*   **Enhance Responsiveness**: Allow the character to react more naturally to changes in input or camera orientation.
*   **Reduce Foot Sliding**: Address situations where foot locking might cause visual stiffness by playing small adjustment animations.
*   **Automate Idle Rotations**: Provide natural-looking turning animations when the character is stationary but the player is changing the view.

### **Key Concepts/Components (and related State/Settings structs)**

The dynamic transitions are primarily managed within `UAlsAnimationInstance` and configured via `UAlsAnimationInstanceSettings`.

#### **1. Quick Stop**

*   **Purpose**: Plays a specific animation when the character rapidly decelerates to a stop, making the stop look more weighted and less abrupt.
*   **Driven by**:
    *   **`UAlsAnimationInstance::PlayQuickStopAnimation()`**: This function is called, typically from Blueprint (e.g., in the AnimGraph's state machine when transitioning from moving to idle under certain conditions).
    *   **`FAlsTransitionsSettings` (within `UAlsAnimationInstanceSettings`)**:
        *   **`QuickStopBlendInDuration`, `QuickStopBlendOutDuration` (float, seconds)**: Control how quickly the quick stop montage blends in and out.
            *   **Example Configuration**: `BlendInDuration = 0.1`, `BlendOutDuration = 0.2`.
        *   **`QuickStopPlayRate` (Vector2D, Min/Max PlayRate)**: The play rate of the quick stop animation can be scaled based on how much the character needs to rotate to face its final direction.
            *   **Example Configuration**: `(X=1.75, Y=3.0)`. If the character hardly rotates, play rate is 1.75x. If it rotates 180 degrees, play rate is 3.0x.
        *   **`QuickStopStartTime` (float, seconds)**: Allows starting the quick stop animation from a specific point.
            *   **Example Configuration**: `0.3`. Skips the initial part of the animation.
        *   **`StandingLeftSequence`, `StandingRightSequence`, `CrouchingLeftSequence`, `CrouchingRightSequence` (UAnimSequenceBase*)**: Pointers to the actual animation sequences/montages to play for quick stops, depending on stance and the direction of the stop/turn.
            *   **Configuration**: Assign your "QuickStop_L", "QuickStop_R" animations for standing and crouching.
*   **`FAlsTransitionsState` (in `UAlsAnimationInstance`)**:
    *   **`QueuedTransitionSequence`, `QueuedTransitionBlendInDuration`, etc.**: These members are used to queue up the selected quick stop (or other transition) animation to be played on the game thread in `NativePostUpdateAnimation` via `PlayQueuedTransitionAnimation()`.
*   **Functional Meaning**: When the character is moving and then input stops (or velocity rapidly drops), the animation system can detect this and trigger `PlayQuickStopAnimation()`. This function selects the appropriate animation (left/right, standing/crouching) based on the character's state and remaining yaw to target, then plays it as a dynamic montage in the `UAlsConstants::TransitionSlotName()` slot.

#### **2. Dynamic Transitions (Foot Adjustments)**

*   **Purpose**: Plays small, additive-like animations to adjust a foot's position when it's locked but the character's body has rotated or shifted slightly, causing the locked foot to look unnatural or overstretched. This is common during slow in-place rotations while aiming.
*   **Driven by**:
    *   **`UAlsAnimationInstance::RefreshDynamicTransitions()`**: Called in the animation update. It checks the distance between a foot's current `LockLocation` and its animated `TargetLocation`. If the distance exceeds a threshold, it triggers a transition.
    *   **`FAlsDynamicTransitionsSettings` (within `UAlsAnimationInstanceSettings`)**:
        *   **`FootLockDistanceThreshold` (float, cm)**: If the distance between a locked foot's current position and its desired animated position exceeds this, a dynamic transition may play.
            *   **Example Configuration**: `8.0`.
        *   **`BlendDuration` (float, seconds)**: Blend in/out time for the foot adjustment animation.
            *   **Example Configuration**: `0.2`.
        *   **`PlayRate` (float)**: Play rate for the foot adjustment animation.
            *   **Example Configuration**: `1.5`.
        *   **`StandingLeftSequence`, `StandingRightSequence`, `CrouchingLeftSequence`, `CrouchingRightSequence` (UAnimSequenceBase*)**: Animations specifically designed to be short "foot shuffle" or "step adjust" animations. Often, these might be the second half of a two-foot transition animation, so only one foot moves.
            *   **Configuration**: Assign short foot adjustment animations here.
    *   **`FAlsDynamicTransitionsState` (in `UAlsAnimationInstance`)**:
        *   **`FrameDelay` (int32)**: Prevents dynamic transitions from firing too rapidly in consecutive frames.
*   **Functional Meaning**: If a character is aiming and slowly rotating, their feet might be locked. As the body rotates, the locked foot's position becomes increasingly "wrong" relative to the new body orientation. `RefreshDynamicTransitions` detects this. If `FeetState.Left.LockAmount` is high and `FVector::DistSquared(FeetState.Left.TargetLocation, FeetState.Left.LockLocation)` is large, it queues one of the specified adjustment animations (e.g., `StandingLeftSequence`) to play in the `UAlsConstants::TransitionSlotName()`, subtly moving the foot to a more natural position.

#### **3. Rotate In Place**

*   **Purpose**: When the character is stationary and in an aiming-like rotation mode (`RotationMode == AlsRotationModeTags::Aiming` or `ViewMode == AlsViewModeTags::FirstPerson`), this feature allows the character's upper body/spine to smoothly rotate towards the camera's view direction if the view deviates significantly from the character's forward facing. This is a continuous, procedural rotation driven by animation curves in the AnimGraph, not a distinct montage.
*   **Driven by**:
    *   **`UAlsAnimationInstance::RefreshRotateInPlace()`**: Called in the animation update.
    *   **`UAlsAnimationInstance::IsRotateInPlaceAllowed()`**: Virtual function determining if this behavior is currently allowed.
    *   **`FAlsRotateInPlaceSettings` (within `UAlsAnimationInstanceSettings`)**:
        *   **`ViewYawAngleThreshold` (float, degrees)**: If the absolute difference between camera yaw and character yaw exceeds this, rotate-in-place becomes active.
            *   **Example Configuration**: `50.0`.
        *   **`ReferenceViewYawSpeed` (Vector2D, Min/Max deg/s)**: A reference range for camera rotation speed.
        *   **`PlayRate` (Vector2D, Min/Max PlayRate)**: The "play rate" here isn't for a montage, but rather a multiplier for how quickly the procedural rotation (often controlled by `RotationYawSpeedCurveName()` in an animation state) should occur. This play rate is scaled based on how fast the camera is moving, using `ReferenceViewYawSpeed`.
            *   **Example Configuration**: `ReferenceViewYawSpeed=(X=180, Y=460)`, `PlayRate=(X=1.15, Y=3.0)`. If camera yaw speed is 180 deg/s, rotation speed multiplier is 1.15x. If 460 deg/s, it's 3.0x.
    *   **`FAlsRotateInPlaceState` (in `UAlsAnimationInstance`)**:
        *   **`bRotatingLeft`, `bRotatingRight` (bool)**: Flags indicating if the conditions for rotating left/right are met.
        *   **`PlayRate` (float)**: The calculated rotation speed multiplier.
*   **Functional Meaning**: In the Animation Blueprint's AnimGraph, there will be a state or blend that uses `RotateInPlaceState.PlayRate` to drive the speed of a "RotateInPlace" animation (which itself might just be an animation that turns the `RotationYawSpeedCurveName()` on or off). When `bRotatingLeft` or `bRotatingRight` is true, this animation state becomes active, and the character procedurally rotates its upper body/spine to follow the camera.

#### **4. Turn In Place**

*   **Purpose**: When the character is stationary, in `ViewDirection` rotation mode (and not First Person), and the camera is turned significantly beyond a threshold for a certain duration, this system plays a full-body turn animation montage to realign the character with the camera.
*   **Driven by**:
    *   **`UAlsAnimationInstance::RefreshTurnInPlace()`**: Called in the animation update.
    *   **`UAlsAnimationInstance::IsTurnInPlaceAllowed()`**: Virtual function.
    *   **`FAlsGeneralTurnInPlaceSettings` (within `UAlsAnimationInstanceSettings`)**:
        *   **`ViewYawAngleThreshold` (float, degrees)**: If absolute camera yaw relative to character forward exceeds this, the activation delay starts.
            *   **Example Configuration**: `45.0`.
        *   **`ViewYawSpeedThreshold` (float, deg/s)**: If camera yaw speed is *above* this, turn-in-place is inhibited (player is likely actively looking around quickly, not settling on a new direction).
            *   **Example Configuration**: `50.0`.
        *   **`ViewYawAngleToActivationDelay` (Vector2D, Min/Max seconds)**: The time the `ViewYawAngleThreshold` must be exceeded. This delay is mapped from the `ViewYawAngleThreshold` to 180 degrees. A larger view deviation results in a shorter delay before turning.
            *   **Example Configuration**: `(X=0.0, Y=0.75)`. At `ViewYawAngleThreshold` (e.g., 45 deg), delay is 0.75s. At 180 deg, delay is 0s (instant turn).
        *   **`Turn180AngleThreshold` (float, degrees)**: If the view yaw angle exceeds this, a 180-degree turn animation is chosen over a 90-degree one.
            *   **Example Configuration**: `130.0`.
        *   **`BlendDuration` (float, seconds)**: Blend in/out time for the turn animation montages.
        *   **`StandingTurn90Left`, `...Right`, `...180Left`, `...180Right` (UAlsTurnInPlaceSettings*)**: Pointers to specific settings objects for each turn type/direction when standing.
        *   **`CrouchingTurn90Left`, etc. (UAlsTurnInPlaceSettings*)**: Similar for crouching.
    *   **`UAlsTurnInPlaceSettings` (instanced objects within `FAlsGeneralTurnInPlaceSettings`)**:
        *   **`Sequence` (UAnimSequenceBase*)**: The actual turn animation montage.
        *   **`PlayRate` (float)**: Base play rate for this specific turn animation.
        *   **`bScalePlayRateByAnimatedTurnAngle` (bool)**: If true, the `PlayRate` is further scaled based on how much the character *actually* needs to turn versus how much the animation *is authored* to turn (`AnimatedTurnAngle`). This helps match animation speed to the required rotation.
        *   **`AnimatedTurnAngle` (float, degrees)**: The amount of rotation the `Sequence` animation performs by default.
            *   **Configuration**: For a "Turn90_L" animation that rotates the character 90 degrees, set `AnimatedTurnAngle = 90.0`.
    *   **`FAlsTurnInPlaceState` (in `UAlsAnimationInstance`)**:
        *   **`ActivationDelay` (float)**: Accumulates time while turn conditions are met.
        *   **`QueuedSettings`, `QueuedSlotName`, `QueuedTurnYawAngle`**: Store parameters for the turn animation selected by `RefreshTurnInPlace`, to be played by `PlayQueuedTurnInPlaceAnimation()` on the game thread.
        *   **`PlayRate` (float)**: The final calculated play rate for the currently playing turn-in-place montage.
*   **Functional Meaning**: If the player stands still and rotates the camera far enough to one side (e.g., > 45 degrees) and holds it there for a short duration (e.g., 0.5s), `RefreshTurnInPlace` will select the appropriate 90 or 180-degree turn montage (based on stance and angle) and queue it. `PlayQueuedTurnInPlaceAnimation` then plays this montage in either `UAlsConstants::TurnInPlaceStandingSlotName()` or `UAlsConstants::TurnInPlaceCrouchingSlotName()`. The character's actual rotation is smoothly driven by the animation's root motion or by curves like `RotationYawSpeedCurveName()`.

#### **Methods & Functions (in `UAlsAnimationInstance`)**

*   **`PlayQuickStopAnimation()`**: Public, BlueprintCallable. Determines direction and calls `PlayTransitionLeft/RightAnimation`.
*   **`PlayTransitionAnimation(...)`, `PlayTransitionLeftAnimation(...)`, `PlayTransitionRightAnimation(...)`**: Public, BlueprintCallable. These queue a transition montage. `bFromStandingIdleOnly` allows restricting transitions to only play if starting from a standing idle pose.
*   **`StopTransitionAndTurnInPlaceAnimations(float BlendOutDuration)`**: Public, BlueprintCallable. Stops any active montages in the transition or turn-in-place slots.
*   **`RefreshTransitions()`**: Internal. Updates `TransitionsState.bTransitionsAllowed` based on the `AllowTransitionsCurveName()`. Dynamic transitions, quick stops, and turn-in-place are generally only allowed if this curve is 1.0.
*   **`RefreshDynamicTransitions()`**: Protected, BlueprintCallable. Implements the logic for foot adjustment dynamic transitions.
*   **`PlayQueuedTransitionAnimation()`**: Internal. Plays the montage queued in `TransitionsState`.
*   **`StopQueuedTransitionAndTurnInPlaceAnimations()`**: Internal. Stops montages based on `TransitionsState.bStopTransitionsQueued`.
*   **`IsRotateInPlaceAllowed()`**: Virtual. Determines if rotate in place logic should run.
*   **`RefreshRotateInPlace()`**: Protected, BlueprintCallable. Implements rotate in place logic.
*   **`IsTurnInPlaceAllowed()`**: Virtual. Determines if turn in place logic should run.
*   **`InitializeTurnInPlace()`**: Protected, BlueprintCallable. Resets `TurnInPlaceState.ActivationDelay`.
*   **`RefreshTurnInPlace()`**: Protected, BlueprintCallable. Implements turn in place logic.
*   **`PlayQueuedTurnInPlaceAnimation()`**: Internal. Plays the montage queued in `TurnInPlaceState`.

#### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

**1. Customizing Turn-In-Place Animations and Behavior**

*   **Change Animations**:
    *   In your `DA_AlsAnimationInstanceSettings` asset, navigate to the `TurnInPlace` section.
    *   For each entry (e.g., `Standing Turn 90 Left`), expand it. You'll see a `Sequence` property. Assign your custom turn animation montage here.
    *   Adjust `PlayRate` and `AnimatedTurnAngle` to match your new animation.
*   **Adjust Thresholds**:
    *   In `DA_AlsAnimationInstanceSettings -> TurnInPlace`:
        *   Modify `ViewYawAngleThreshold` to make turns trigger sooner (lower value) or later (higher value).
        *   Adjust `ViewYawAngleToActivationDelay` to change how long the player needs to hold the camera offset before a turn starts.
*   **GAS Integration**: Turn-In-Place is generally an automatic system. GAS abilities wouldn't typically trigger it directly. However, an ability that *roots* the character or forces a specific facing might temporarily disable Turn-In-Place by:
    *   Applying a `GameplayEffect` that grants a tag (e.g., `State.CannotTurnInPlace`).
    *   Overriding `UAlsAnimationInstance::IsTurnInPlaceAllowed()` in a C++ subclass or checking this tag in the Blueprint AnimBP's logic that calls `RefreshTurnInPlace`.
      ```cpp
      // MyAlsAnimationInstance.h
      // virtual bool IsTurnInPlaceAllowed() override;
      // MyAlsAnimationInstance.cpp
      // bool UMyAlsAnimationInstance::IsTurnInPlaceAllowed()
      // {
      //     if (Character.IsValid() && Character->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.CannotTurnInPlace"))))
      //     {
      //         return false;
      //     }
      //     return Super::IsTurnInPlaceAllowed();
      // }
      ```

**2. Adding a New Type of Dynamic Transition (e.g., "Stumble Recovery")**

*   **Scenario**: If the character is lightly bumped (e.g., by a weak physics impulse or a minor attack), play a small, quick stumble/recovery animation instead of a full hit react or ragdoll.
*   **A. Define Gameplay Tag (for triggering, optional but good for GAS)**:
    `Als.LocomotionAction.Stumble` (or use a `GameplayEvent`).
*   **B. Create Stumble Animations**: Short animations for stumbling forward, backward, left, right.
*   **C. `UAlsAnimationInstanceSettings`**:
    *   You could add a new struct `FAlsStumbleSettings` similar to `FAlsDynamicTransitionsSettings`, with properties for stumble animations and blend durations.
*   **D. `UAlsAnimationInstance`**:
    *   Add a new state struct `FAlsStumbleState`.
    *   Add a public function `PlayStumbleAnimation(EAlsMovementDirection StumbleDirection)`. This function would select the appropriate stumble animation from your new settings based on `StumbleDirection` and queue it using the `TransitionsState` (or a new dedicated queue if stumbles can overlap with other transitions).
    *   You might add a `RefreshStumble()` function called in the update loop if stumbling has an ongoing effect or conditions to check.
*   **E. GAS Integration**:
    *   Create a `GameplayAbility` `GA_StumbleReact`.
    *   This ability could be triggered by:
        *   A `GameplayEffect` that applies minimal damage and also sends a `GameplayEvent` with payload data indicating the hit direction.
        *   A passive ability listening for these damage/hit events.
    *   `GA_StumbleReact::ActivateAbility` would:
        1.  Determine stumble direction from the event payload.
        2.  Call `AlsAnimationInstance->PlayStumbleAnimation(CalculatedDirection)`.
        3.  The stumble animation montage itself could use `AlsAnimNotifyStateSetLocomotionAction` to set `LocomotionAction` to `Als.LocomotionAction.Stumble` for its duration.
        4.  The ability might end when the montage completes.

#### **Network Synchronization**

*   **State Structs (`FAlsTransitionsState`, `FAlsDynamicTransitionsState`, etc.)**: These are **not directly replicated**. They manage the *local* queuing and state of these transitions on each client and the server.
*   **Triggering**:
    *   **Quick Stop, Rotate/Turn In Place**: These are generally triggered by local conditions (velocity changes, camera angles) that are themselves derived from replicated character state (transform, input, view rotation). So, all clients should trigger them at roughly the same time. The played montages (`TurnInPlace`) are not explicitly replicated as "play this montage now" RPCs from these systems, but rather the *conditions* for playing them become true on all machines. The animation slot system handles the visual playback.
    *   **Dynamic Transitions (Foot Adjustments)**: Also locally triggered based on foot lock state and animation curves.
*   **Montage Playback**: When `UAlsAnimationInstance` calls `PlaySlotAnimationAsDynamicMontage` (for quick stops, dynamic transitions, turn-in-place), it's playing a montage locally. There isn't a separate RPC sent for these specific dynamic transitions because they are intended to be emergent from the replicated base character state.
    *   This differs from GAS-triggered actions (like Rolling) where the ability uses `ASC->PlayMontage()` which *does* handle explicit montage replication.
*   **Consistency**: Relies on the base replicated character state (`AAlsCharacter`'s tags, transform, velocity) being consistent enough for all clients to evaluate the conditions for these dynamic transitions similarly. Minor timing differences are usually acceptable for these subtle effects.

#### **Debugging**

*   **`ShowDebug Als.State`**: Check `LocomotionAction`, `RotationMode`, `ViewState.YawAngle`, `LocomotionState.Speed` to understand if conditions for these transitions are being met.
*   **`ShowDebug Als.Curves`**: Verify `AllowTransitionsCurveName()` if transitions aren't playing. Check curves like `RotationYawSpeedCurveName()` if rotate/turn in place speeds are off.
*   **Animation Debugger**:
    *   Step through the AnimGraph to see if the transition/turn states are being entered.
    *   Inspect the properties of `UAlsAnimationInstance` (e.g., `TransitionsState.QueuedTransitionSequence`, `TurnInPlaceState.ActivationDelay`) to see if transitions are being queued or if delays are accumulating correctly.
*   **`UAlsAnimationInstanceSettings`**: Double-check all thresholds, durations, play rates, and assigned animation sequences in your `DA_AlsAnimationInstanceSettings` asset. A common issue is a missing animation sequence assignment.
*   **Visual Inspection**: Observe the character closely.
    *   *Quick Stop not playing?* Is the character stopping fast enough? Is `AllowTransitionsCurveName()` active? Is the correct sequence assigned?
    *   *Foot shuffle (Dynamic Transition) not happening?* Is `FootLockDistanceThreshold` too high? Is the foot actually locked (`FootLockCurve` at 1)?
    *   *Rotate/Turn In Place not working?* Check `ViewYawAngleThreshold`, `ViewYawSpeedThreshold`, and `ActivationDelay` values. Is `IsRotateInPlaceAllowed()` or `IsTurnInPlaceAllowed()` returning true?

















----------------------------------------------------------------------------------------------------------















## **III-E. Advanced Locomotion Actions**

### **Scope**

This subsection details the complex, often multi-stage, character actions provided by `AAlsCharacter`: Mantling, Rolling, and Ragdolling. These go beyond simple state changes and involve significant C++ logic, animation montages, physics interactions, and network synchronization.

### **Purpose**

These advanced actions provide core gameplay mechanics found in many modern action games, enhancing character mobility, environmental interaction, and physical reactivity.
*   **Mantling**: Allows characters to traverse vertical obstacles smoothly.
*   **Rolling**: Provides an evasive maneuver.
*   **Ragdolling**: Enables realistic reactions to strong impacts or player/AI control for physics-based scenarios.

---

### **1. Mantling**

*   **Purpose**: Enables the character to detect and smoothly climb or vault over obstacles of varying heights.
*   **Key Concepts**:
    *   **Trace-Based Detection**: `AAlsCharacter::StartMantling()` uses a series of capsule and sphere sweeps (`FAlsMantlingTraceSettings` in `UAlsCharacterSettings`) to find a valid ledge:
        1.  **Forward Trace**: Sweeps forward from the character to find an obstacle wall.
        2.  **Downward Trace**: Sweeps downward from the top of the detected obstacle to find a walkable surface (the ledge).
        3.  **Overlap Checks**: Verifies if there's enough space for the character at the target landing spot and if the path isn't blocked (e.g., by a low ceiling).
    *   **Mantling Types (`EAlsMantlingType`)**:
        *   `High`: For taller obstacles, often using a more involved climbing animation.
        *   `Low`: For shorter obstacles, typically a quicker vault.
        *   `InAir`: For mantles initiated while the character is airborne.
        *   Determined by `AAlsCharacter::StartMantlingImplementation()` based on `MantlingHeight` and `Settings->Mantling.MantlingHighHeightThreshold`.
    *   **`FAlsMantlingParameters` (Struct)**: Packages all necessary information about a detected mantle opportunity (target primitive, relative location/rotation to target, mantle height, mantle type) for replication and processing.
    *   **Root Motion Source (`FAlsRootMotionSource_Mantling`)**: This is a custom `FRootMotionSource` applied to the `UCharacterMovementComponent`.
        *   **Crucial Role**: It precisely controls the character's movement and rotation during the mantle animation, ensuring the character follows the animated path relative to the mantled object, even if that object is moving.
        *   It interpolates the character from their starting offset to the animation's intended end position relative to the target.
    *   **Animation Montages**: Different montages are selected based on `EAlsMantlingType` (via `AAlsCharacter::SelectMantlingSettings_Implementation`, which returns a `UAlsMantlingSettings` object containing the montage).
    *   **Settings**:
        *   **`UAlsCharacterSettings->Mantling` (FAlsGeneralMantlingSettings)**: Contains general trace parameters, thresholds, and fallback behaviors.
        *   **`UAlsMantlingSettings` (Data Asset, pointed to by `FAlsGeneralMantlingSettings` or selected by `SelectMantlingSettings_Implementation`)**: Contains the specific `UAnimMontage`, correction curves, and start time calculation parameters for a particular type of mantle (e.g., `DA_AlsMantling_High`, `DA_AlsMantling_Low`).
            *   `bAutoCalculateStartTime`: If true, `AAlsCharacter::CalculateMantlingStartTime` dynamically finds the best start time in the montage to match the character's height relative to the ledge.
            *   `HorizontalCorrectionCurve`, `VerticalCorrectionCurve`: Optional curves to fine-tune the character's path during the root motion.
*   **Key Functions in `AAlsCharacter`**:
    *   **`StartMantlingGrounded()` / `StartMantlingInAir()`**: Public entry points.
    *   **`StartMantling(const FAlsMantlingTraceSettings& TraceSettings)`**: Performs the trace logic.
    *   **`IsMantlingAllowedToStart_Implementation()`**: BlueprintNativeEvent to check if mantling can begin.
    *   **`ServerStartMantling_Implementation()`, `MulticastStartMantling_Implementation()`, `StartMantlingImplementation()`**: Handle replication and the core logic of initiating the mantle (setting `LocomotionAction` to `Mantling`, changing movement mode to `MOVE_Custom`, applying `FAlsRootMotionSource_Mantling`, playing the montage).
    *   **`SelectMantlingSettings_Implementation(EAlsMantlingType MantlingType)`**: BlueprintNativeEvent to choose the `UAlsMantlingSettings` asset for the given mantle type.
    *   **`CalculateMantlingStartTime(...)`**: Determines the optimal start frame for the montage.
    *   **`RefreshMantling()`**: Tick function to monitor an active mantle (e.g., checks if the target primitive is still valid).
    *   **`StopMantling(bool bStopMontage)`**: Ends the mantling action, removes the root motion source, and restores normal movement.
    *   **`OnMantlingStarted_Implementation()`, `OnMantlingEnded_Implementation()`**: BlueprintNativeEvents for game-specific logic.

*   **Adding/Adapting Functionality**:
    *   **Tune Parameters**: The primary way to customize mantling is by adjusting values in `DA_AlsCharacterSettings->Mantling` and your various `DA_AlsMantling_[Type]` assets (e.g., trace distances, ledge height ranges, animation start times, correction curves).
    *   **Custom Mantle Types**:
        1.  Define a new `EAlsMantlingType` value (requires C++ enum modification).
        2.  Create new `UAlsMantlingSettings` Data Assets for this type (e.g., `DA_AlsMantling_CornerGrab`).
        3.  Override `SelectMantlingSettings_Implementation` in your `AAlsCharacter` C++ or Blueprint subclass to return your new settings asset when your custom mantle type is detected.
        4.  Modify `StartMantling` trace logic if your new mantle type requires different detection (e.g., corner grab needs to check for two perpendicular surfaces).
        5.  Create new animation montages for this mantle type.
    *   **GAS Integration**:
        *   Mantling is usually triggered by player input (e.g., pressing jump near an edge). The existing `AAlsCharacter::Jump_Implementation` checks `StartMantlingGrounded()`.
        *   If you want a dedicated "Mantle" ability (`GA_Mantle`):
            *   `ActivateAbility`: Call `Character->StartMantlingGrounded()` or `Character->StartMantlingInAir()`.
            *   The ability would likely end itself quickly, as the mantling action itself is self-contained and time-limited by the root motion source and montage.
            *   The `LocomotionAction` tag `Als.LocomotionAction.Mantling` (set internally by `StartMantlingImplementation`) is key for other GAS abilities to know the character is busy (e.g., an attack ability could have `SourceBlockedTags` include `Mantling`).
        *   A `GameplayEffect` could be applied by `GA_Mantle` to grant a `State.Mantling` tag for the duration, which could provide immunity to certain effects or modify attributes (e.g., reduced damage taken).

*   **Network Synchronization**:
    *   Mantling is initiated by an autonomous client or AI (server-side).
    *   `AAlsCharacter::StartMantling` calls `ServerStartMantling` (if client).
    *   `ServerStartMantling_Implementation` calls `MulticastStartMantling`.
    *   `MulticastStartMantling_Implementation` calls `StartMantlingImplementation` on the server and all clients.
    *   `StartMantlingImplementation` applies the `FAlsRootMotionSource_Mantling`. Root Motion Sources are replicated by Unreal's `UCharacterMovementComponent`. The parameters within `FAlsRootMotionSource_Mantling` (like `TargetRelativeLocation`, `ActorFeetLocationOffset`, `MontageStartTime`) are serialized in its `NetSerialize` function.
    *   The animation montage itself is typically played via `Montage_Play` directly in `StartMantlingImplementation`. For better explicit replication, especially if GAS is managing it, you'd have the GA use `ASC->PlayMontage()`. ALS's current mantling setup plays it locally on each machine once the multicast to start mantling is received, relying on the root motion source and replicated character state to keep things aligned.
*   **Debugging**:
    *   **`ShowDebug Als.Mantling`**: Crucial. Visualizes all the traces (Forward, Downward, Overlaps) performed by `StartMantling`. Shows green for successful segments, red/orange for failed ones. Essential for understanding why a mantle might not be triggering.
    *   **`DA_AlsCharacterSettings->Mantling.GroundedTrace.bDrawFailedTraces` / `InAirTrace.bDrawFailedTraces`**: Set these to `true` to see traces even if they don't result in a successful mantle detection.
    *   **Animation Debugger**: Check if the correct mantle montage is playing and if root motion is being extracted.
    *   **Details Panel (`AAlsCharacter`)**: Inspect `LocomotionAction` to see if it's `Mantling`. Inspect `MantlingState.RootMotionSourceId`.
    *   **Details Panel (`UCharacterMovementComponent`)**: Under "Debug" or "Root Motion", you can sometimes see information about active root motion sources.

---

### **2. Rolling**

*   **Purpose**: Allows the character to perform a directional evasive roll, typically driven by root motion.
*   **Key Concepts**:
    *   **Animation Montage**: Rolling is executed by playing a specific animation montage (defined in `UAlsCharacterSettings->Rolling.Montage`).
    *   **Root Motion**: The roll's movement and rotation are primarily driven by the root motion extracted from this montage.
    *   **Targeted Rotation**: The character smoothly interpolates its yaw towards a `TargetYawAngle` during the roll, allowing players to direct the roll.
    *   **State Integration**: Sets `LocomotionAction` to `Als.LocomotionAction.Rolling`. Can be configured to crouch on start or be interrupted if airborne.
    *   **Settings (`UAlsCharacterSettings->Rolling` - `FAlsRollingSettings`)**:
        *   `Montage` (UAnimMontage*): The roll animation.
        *   `bCrouchOnStart` (bool): If true, character attempts to enter crouch stance at the start of the roll.
        *   `bRotateToInputOnStart` (bool): If true and input is present, the roll's `TargetYawAngle` is set to the input direction. Otherwise, it's the current character facing.
        *   `RotationInterpolationSpeed` (float): How quickly the character's yaw interpolates towards `RollingState.TargetYawAngle` during the roll's physics update.
        *   `bStartRollingOnLand` (bool) & `RollingOnLandSpeedThreshold` (float): If true, and character lands with vertical speed exceeding threshold, a roll is initiated.
        *   `bInterruptRollingWhenInAir` (bool): If true, rolling stops (often transitioning to ragdoll) if the character becomes airborne.
*   **Key Functions in `AAlsCharacter`**:
    *   **`StartRolling(float PlayRate)` / `StartRolling(float PlayRate, float TargetYawAngle)`**: Public entry points.
    *   **`SelectRollMontage_Implementation()`**: BlueprintNativeEvent to choose the roll montage.
    *   **`IsRollingAllowedToStart(const UAnimMontage* Montage) const`**: Checks if a roll can currently be initiated.
    *   **`ServerStartRolling_Implementation()`, `MulticastStartRolling_Implementation()`, `StartRollingImplementation()`**: Handle replication and the core logic (setting `LocomotionAction`, `RollingState.TargetYawAngle`, playing montage, setting initial rotation).
    *   **`RefreshRolling(float DeltaTime)`**: Called every tick. If not on server/autonomous proxy with root motion, it calls `RefreshRollingPhysics`.
    *   **`CharacterMovement_OnPhysicsRotation(float DeltaTime)`** (subscribed by `AAlsCharacter` to `UAlsCharacterMovementComponent::OnPhysicsRotation`): Calls `RefreshRollingPhysics`.
    *   **`RefreshRollingPhysics(float DeltaTime)`**: Manually interpolates the character's yaw towards `RollingState.TargetYawAngle` using `Settings->Rolling.RotationInterpolationSpeed`. This is done because `bAllowPhysicsRotationDuringAnimRootMotion` is true in `UAlsCharacterMovementComponent`, allowing ALS to override root motion rotation if needed.
*   **Adding/Adapting Functionality**:
    *   **Change Roll Animation**: Assign a new montage to `DA_AlsCharacterSettings->Rolling.Montage`.
    *   **Adjust Parameters**: Tune `RotationInterpolationSpeed`, `bCrouchOnStart`, etc., in `DA_AlsCharacterSettings`.
    *   **Different Roll Types (e.g., Dodge Step vs. Full Roll)**:
        1.  Create new `UAnimMontage` assets for each roll type.
        2.  In your `AAlsCharacter` subclass, override `SelectRollMontage_Implementation()`:
            ```cpp
            // MyAlsCharacter.h
            // UPROPERTY(EditDefaultsOnly, Category="MyRolls") UAnimMontage* DodgeStepMontage;
            // UPROPERTY(EditDefaultsOnly, Category="MyRolls") UAnimMontage* FullRollMontage;
            // virtual UAnimMontage* SelectRollMontage_Implementation() override;

            // MyAlsCharacter.cpp
            // UAnimMontage* AMyAlsCharacter::SelectRollMontage_Implementation()
            // {
            //     // Based on some condition, e.g., current weapon or a Gameplay Tag
            //     if (HasMatchingGameplayTag(AlsCombatTags::IsInCloseCombat))
            //     {
            //         return DodgeStepMontage;
            //     }
            //     return FullRollMontage; // Or Super::SelectRollMontage_Implementation();
            // }
            ```
    *   **GAS Integration**:
        *   A `GA_Roll` ability would:
            *   `ActivateAbility`: Call `Character->StartRolling(PlayRate, TargetYawAngle)`.
            *   The `LocomotionAction` `Als.LocomotionAction.Rolling` (set by `StartRollingImplementation`) blocks other actions.
            *   The ability often ends when the montage finishes. `UAbilityTask_PlayMontageAndWait` is perfect for this.
            *   **Invulnerability during Roll**: Apply a `GameplayEffect` that grants a `State.Invulnerable` tag for the duration of the roll (e.g., by using the montage duration or specific notifies within the roll montage to start/stop the effect).
            *   **Stamina Cost**: Apply a `GameplayEffect` that deducts stamina (cost GE).

*   **Network Synchronization**:
    *   Similar to Mantling: Initiated by client/AI, `ServerStartRolling` RPC, then `MulticastStartRolling`.
    *   `StartRollingImplementation` plays the montage locally on all machines.
    *   `RollingState.TargetYawAngle` is implicitly synchronized because the initial `TargetYawAngle` is passed in the multicast. Subsequent rotation is client-predictable physics interpolation based on this target.
    *   Root motion from the montage is the primary driver of movement.

*   **Debugging**:
    *   **`ShowDebug Als.State`**: Check if `LocomotionAction` becomes `Rolling`.
    *   **Animation Debugger**: Ensure the correct roll montage is playing and root motion is active.
    *   **Details Panel (`AAlsCharacter`)**: Inspect `RollingState.TargetYawAngle`.
    *   **Visual Inspection**: Observe if the character rotates smoothly towards the intended direction during the roll. If not, `RotationInterpolationSpeed` might be too low/high, or the `RefreshRollingPhysics` logic might have issues.

---

### **3. Ragdolling & Getting Up**

*   **Purpose**: Allows the character to transition into a full physics-driven ragdoll state, and then potentially recover back into an animated state.
*   **Key Concepts**:
    *   **Physics Simulation**: Disables character movement and enables `Simulate Physics` on the `USkeletalMeshComponent`.
    *   **Collision Changes**: Capsule collision is disabled; mesh collision is enabled for physics interaction.
    *   **Networked Target Location**: For simulated proxies, `RagdollTargetLocation` (pelvis position from server/owning client) is replicated, and a pull force is applied to keep client ragdolls from diverging too much.
    *   **Flail Animation**: A subtle animation can be played on the ragdolled mesh (using `GetMesh()->bUpdateJointsFromAnimation = true` temporarily during `StartRagdollingImplementation`) to make it look less static. The play rate of this is driven by `RagdollingAnimationState.FlailPlayRate` in `UAlsAnimationInstance`, which is based on ragdoll velocity.
    *   **Speed Limiting**: `RagdollingState.SpeedLimit` helps prevent extreme initial velocities.
    *   **Pose Snapshot (`FPoseSnapshot`)**: Before exiting ragdoll, `UAlsAnimationInstance::SnapshotFinalRagdollPose()` captures the current skeletal mesh bone transforms. This snapshot is then used in the AnimGraph to smoothly blend from the ragdoll pose back to an animated "get up" pose.
    *   **Get Up Animations**: Different "get up" montages (`GetUpFrontMontage`, `GetUpBackMontage` from `FAlsRagdollingSettings`) are selected based on whether the character's pelvis was facing upwards or downwards when ragdolling stopped.
    *   **Settings (`UAlsCharacterSettings->Ragdolling` - `FAlsRagdollingSettings`)**:
        *   `bStartRagdollingOnLand` & `RagdollingOnLandSpeedThreshold`: For automatic ragdoll on hard landings.
        *   `bLimitInitialRagdollSpeed`: Enables the initial speed cap.
        *   `GetUpFrontMontage`, `GetUpBackMontage`: Montages for recovery.
*   **Key Functions in `AAlsCharacter`**:
    *   **`StartRagdolling()`**: Public entry point.
    *   **`IsRagdollingAllowedToStart() const`**: Checks if conditions permit starting ragdoll.
    *   **`ServerStartRagdolling_Implementation()`, `MulticastStartRagdolling_Implementation()`, `StartRagdollingImplementation()`**: Replication and core logic for entering ragdoll (sets `LocomotionAction` to `Ragdolling`, handles physics/collision setup, sets `RagdollTargetLocation`).
    *   **`SetRagdollTargetLocation(...)`, `ServerSetRagdollTargetLocation_Implementation(...)`**: For replicating the target pelvis location.
    *   **`RefreshRagdolling(float DeltaTime)`**: Tick function to manage active ragdoll (applies pull force on clients, updates ground trace for capsule, scales joint strength, limits speed).
    *   **`RagdollTraceGround(bool& bGrounded) const`**: Traces to find ground beneath the ragdoll's pelvis to position the (nocollide) capsule correctly.
    *   **`StopRagdolling()`**: Public entry point to attempt to exit ragdoll.
    *   **`IsRagdollingAllowedToStop() const`**: Checks conditions (e.g., ragdoll velocity below a threshold).
    *   **`ServerStopRagdolling_Implementation()`, `MulticastStopRagdolling_Implementation()`, `StopRagdollingImplementation()`**: Replication and core logic for exiting ragdoll (captures pose snapshot, restores physics/collision, reattaches mesh, sets movement mode, plays get up montage, sets `LocomotionAction` to `GettingUp` or empty).
    *   **`SelectGetUpMontage_Implementation(bool bRagdollFacingUpward)`**: BlueprintNativeEvent to choose the get up montage.
    *   **`OnRagdollingStarted_Implementation()`, `OnRagdollingEnded_Implementation()`**: BlueprintNativeEvents.
*   **Adding/Adapting Functionality**:
    *   **Tune Settings**: Adjust `RagdollingOnLandSpeedThreshold`, `PullForce` characteristics (indirectly via `RefreshRagdolling` interpolation), and assign appropriate Get Up montages in `DA_AlsCharacterSettings`.
    *   **Custom Get Up Conditions/Animations**: Override `SelectGetUpMontage_Implementation` to choose different get up animations based on more complex conditions (e.g., nearby walls, injuries). You might need to modify `StopRagdollingImplementation` if the recovery logic changes significantly.
    *   **GAS Integration**:
        *   **Triggering Ragdoll**:
            *   A `GA_ForceRagdoll` ability could call `Character->StartRagdolling()`. This could be triggered by a powerful attack's `GameplayEffect`.
            *   A `GameplayEffect` from an explosion could grant `State.MustRagdoll`. A passive ability or system check could then call `StartRagdolling()`.
        *   **Exiting Ragdoll**:
            *   A `GA_RecoverFromRagdoll` ability could be player-activated. Its `CanActivateAbility` would check `Character->IsRagdollingAllowedToStop()`. If so, `ActivateAbility` calls `Character->StopRagdolling()`.
            *   The `LocomotionAction` tags `Als.LocomotionAction.Ragdolling` and `Als.LocomotionAction.GettingUp` are crucial for GAS. Abilities that require normal character control (movement, attacks) should be blocked by these tags (e.g., in `SourceBlockedTags` or `TargetBlockedTags`).
        *   **Damage during Ragdoll**: Characters in ragdoll are often immune to certain types of follow-up damage or control effects. Apply a `GameplayEffect` when `StartRagdolling` is called that grants `State.IsRagdolling` (and perhaps `State.DamageImmune.Control`). Remove it in `StopRagdolling`.

*   **Network Synchronization**:
    *   Entry/Exit: `StartRagdolling` and `StopRagdolling` use Server/Multicast RPCs.
    *   `RagdollTargetLocation`: Replicated from server/owner to simulated clients to guide their local ragdoll.
    *   The actual physics simulation of the ragdoll is client-side. The server runs its own simulation. The `RagdollTargetLocation` and pull force help to keep them from diverging too wildly, but they won't be perfectly identical. This is a common approach for networked ragdolls to balance responsiveness and visual consistency.
    *   The "Get Up" montage, once triggered by `StopRagdollingImplementation` (which runs on all machines via multicast), plays locally. Its root motion (if any) helps align the character.

*   **Debugging**:
    *   **`ShowDebug Als.State`**: Check `LocomotionAction` for `Ragdolling` or `GettingUp`.
    *   **Physics Debugging (Unreal Editor)**:
        *   `ShowFlag.Collision 1` (in console) to see collision shapes.
        *   `pxvis collision` (NVIDIA PhysX Visual Debugger, if using PhysX and enabled) or Chaos equivalent.
        *   Verify that the mesh's physics asset is correctly configured (constraints, body sizes).
    *   **Animation Debugger**: When getting up, check the "Pose Snapshot" node in the AnimGraph and ensure the blend from ragdoll to the get up animation is smooth.
    *   **Details Panel (`AAlsCharacter`)**: Inspect `RagdollTargetLocation` on clients to see if it's updating. Inspect `RagdollingState` members.
    *   **Visual Inspection**: Is the ragdoll behaving erratically? Is it flying off at high speeds (check `bLimitInitialRagdollSpeed`)? Is the get up animation playing correctly based on orientation?


















--------------------------------------------------------------------------------------------------------
















## **IV. Core Utilities**

This section details the various utility classes and modules that provide essential helper functions, mathematical operations, constants, and debugging tools used throughout the Advanced Locomotion System. These utilities promote code reuse, clarity, and maintainability.

### **A. Math Utilities (`UAlsMath`)**

#### **Scope**

This covers the `UAlsMath` Blueprint Function Library, which contains general-purpose mathematical helper functions, primarily for float and spring-damper calculations.

#### **Purpose**

To provide a centralized location for common mathematical operations that are frequently needed in locomotion and animation logic, ensuring consistency and avoiding redundant implementations.

#### **Key Concepts/Components**

1.  **`FAlsSpringFloatState` (USTRUCT)**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")`**
    *   **Purpose**: Stores the persistent state required for a critically damped spring calculation for a single float value.
    *   **Key Members**:
        *   `Velocity` (float): The current velocity of the spring.
        *   `PreviousTarget` (float): The target value from the previous frame, used to calculate target velocity.
        *   `bStateValid` (bool): Flag indicating if the spring state has been initialized.
    *   **Methods**:
        *   `Reset()`: Resets the velocity to 0, and `bStateValid` to false.
    *   **Functional Meaning**: Allows for smooth, frame-rate independent interpolation of a float towards a target value with spring-like behavior (oscillation control via damping ratio).

#### **Methods & Functions (Static BlueprintCallable in `UAlsMath`)**

1.  **`Clamp01(float Value)`**
    *   **Purpose**: Clamps a float value to the range \[0, 1].
    *   **Parameters**: `Value` (float).
    *   **Return Value**: `float` - The clamped value.
    *   **Functional Meaning**: Commonly used for normalizing blend weights or alpha values.

2.  **`LerpClamped(float From, float To, float Ratio)`**
    *   **Purpose**: Performs a linear interpolation between `From` and `To` using `Ratio`, but clamps `Ratio` to \[0, 1] before interpolating.
    *   **Parameters**: `From` (float), `To` (float), `Ratio` (float).
    *   **Return Value**: `float` - The interpolated value.
    *   **Functional Meaning**: Safe linear interpolation that won't extrapolate beyond the target.

3.  **`Damp(const ValueType& Current, const ValueType& Target, float DeltaTime, float Smoothing)` (Template)**
    *   **`Damp(float DeltaTime, float Smoothing)` (BlueprintPure overload for Ratio)**
    *   **Purpose**: Smoothly interpolates `Current` towards `Target` using a frame-rate independent damping method. `Smoothing` is a value typically between 0 (instant) and 1 (very slow).
    *   **Parameters**: `Current` value, `Target` value, `DeltaTime` (float), `Smoothing` (float, 0-1 range effectively, though higher values are clamped internally by FMath::Pow).
    *   **Return Value**: The interpolated value.
    *   **Functional Meaning**: A common way to achieve smooth transitions for values that change over time (e.g., camera lag, blend weights). The closer `Smoothing` is to 0, the faster the interpolation.

4.  **`ExponentialDecay(const ValueType& Current, const ValueType& Target, float DeltaTime, float Lambda)` (Template)**
    *   **`ExponentialDecay(float DeltaTime, float Lambda)` (BlueprintPure overload for Ratio)**
    *   **Purpose**: Another frame-rate independent smoothing method. `Lambda` is the decay rate; higher values mean faster decay/interpolation.
    *   **Parameters**: `Current` value, `Target` value, `DeltaTime` (float), `Lambda` (float, typically > 0).
    *   **Return Value**: The interpolated value.
    *   **Functional Meaning**: Similar to `Damp`, often used for smoothing. `Lambda` offers a different way to control the "stiffness" of the interpolation.

5.  **`SpringDamp(StateType& SpringState, const ValueType& Current, const ValueType& Target, float DeltaTime, float Frequency, float DampingRatio, float TargetVelocityAmount = 1.0f)` (Template)**
    *   **`SpringDampFloat(UPARAM(ref) FAlsSpringFloatState& SpringState, float Current, float Target, float DeltaTime, float Frequency, float DampingRatio, float TargetVelocityAmount = 1.0f)` (BlueprintPure specialization for floats)**
    *   **Purpose**: Implements a critically damped spring interpolation. This provides a very natural and smooth way to move a value towards a target, with control over overshoot and oscillation.
    *   **Parameters**:
        *   `SpringState`: A reference to a state struct (like `FAlsSpringFloatState` or `FAlsSpringVectorState`) to store persistent velocity.
        *   `Current`: The current value.
        *   `Target`: The value to interpolate towards.
        *   `DeltaTime`: Frame delta time.
        *   `Frequency`: The speed of the spring; higher frequency means faster response. Represents oscillations per second if no damping.
        *   `DampingRatio`: Controls how the spring settles.
            *   `< 1`: Underdamped (will oscillate/overshoot).
            *   `= 1`: Critically damped (fastest settling without overshoot).
            *   `> 1`: Overdamped (settles slowly without overshoot).
        *   `TargetVelocityAmount`: How much of the target's implicit velocity (change from last frame) should be fed into the spring. `1.0` makes it more responsive to moving targets.
    *   **Return Value**: The interpolated value for the current frame.
    *   **Functional Meaning**: Used in ALS for smoothing things like Control Rig foot offsets (`OffsetLocationZ` in `FAlsRigUnit_ApplyFootOffsetLocation`), providing a physically-based feel to the interpolation.

6.  **`CalculateMovementDirection(float Angle, float ForwardHalfAngle, float AngleThreshold)`**
    *   **Purpose**: Determines a cardinal movement direction (Forward, Backward, Left, Right - `EAlsMovementDirection`) based on an input angle (typically character's velocity yaw relative to view yaw) and configurable thresholds.
    *   **Parameters**:
        *   `Angle`: The input angle in degrees (e.g., -180 to 180).
        *   `ForwardHalfAngle`: Defines the cone for "Forward" movement (e.g., 70 degrees means -70 to +70 is Forward).
        *   `AngleThreshold`: A small tolerance for angle comparisons.
    *   **Return Value**: `EAlsMovementDirection`.
    *   **Functional Meaning**: Used by `UAlsAnimationInstance::RefreshMovementDirection` to determine which set of directional animations (e.g., Strafe Left vs. Run Forward) should be blended in.

7.  **`TryCalculatePoleVector(const FVector& ALocation, const FVector& BLocation, const FVector& CLocation, FVector& ProjectionLocation, FVector& PoleDirection)`**
    *   **Purpose**: Calculates the pole vector for a three-point chain (A-B-C), which is essential for IK solvers like two-bone IK to determine the bending direction of a joint (like an elbow or knee).
    *   **Parameters**: Locations of points A (e.g., shoulder), B (e.g., elbow), C (e.g., hand).
    *   **Output Parameters**: `ProjectionLocation` (B projected onto the AC line), `PoleDirection` (vector from `ProjectionLocation` to B, normalized).
    *   **Return Value**: `bool` - True if a valid pole vector could be calculated (i.e., points are not collinear in a problematic way).
    *   **Functional Meaning**: Used by Control Rig (e.g., `FAlsRigUnit_CalculatePoleVector`) to guide IK solvers.

#### **Adding/Adapting Functionality**

*   **New Math Functions (C++)**: If you have a common mathematical utility you need, add a new static function to `UAlsMath.h` and implement it in `UAlsMath.cpp`. Make it `BlueprintPure` if it should be accessible from Blueprints and has no side effects.
*   **Using in Blueprints**: Drag off a relevant variable type (e.g., float) and search for the function name (e.g., "Damp", "Spring Damp Float").
*   **GAS Integration**: These are low-level math utilities. GAS abilities wouldn't typically call them directly. However, attributes modified by GAS `GameplayEffects` might be fed into these functions by other systems (e.g., a "slowness" attribute could modify the `Lambda` or `Smoothing` parameters passed to `ExponentialDecay` or `Damp` functions when calculating character speed interpolation).

---

### **B. Vector Utilities (`UAlsVector`)**

#### **Scope**

This covers the `UAlsVector` Blueprint Function Library, which provides helper functions specifically for `FVector` and `FVector2D` operations, often related to 2D direction and angle conversions in the XY plane.

#### **Purpose**

To offer convenient and optimized vector math operations commonly used in character locomotion, particularly for converting between angles and directional vectors, and for specific 2D calculations relevant to top-down or planar movement.

#### **Key Concepts/Components**

1.  **`FAlsSpringVectorState` (USTRUCT)**
    *   **`UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS")`**
    *   **Purpose**: Stores the persistent state for spring-damping an `FVector`.
    *   **Key Members**: Similar to `FAlsSpringFloatState` but with `FVector Velocity` and `FVector PreviousTarget`.
    *   **Functional Meaning**: Allows for smooth, spring-like interpolation of a 3D vector.

#### **Methods & Functions (Static BlueprintCallable in `UAlsVector`)**

1.  **`ClampMagnitude01(const FVector& Vector)` / `ClampMagnitude01(const FVector3f& Vector)` / `ClampMagnitude012D(const FVector2D& Vector)`**
    *   **Purpose**: Clamps the magnitude (length) of a vector to a maximum of 1.0 while preserving its direction. If magnitude is already <= 1.0, returns the original vector.
    *   **Functional Meaning**: Normalizes a vector only if its length exceeds 1. Useful for input vectors or calculated directions that should represent a normalized intention.

2.  **`RadianToDirection(float Radian)` (FVector2D)** / **`RadianToDirectionXY(float Radian)` (FVector)**
    *   **Purpose**: Converts an angle in radians into a 2D unit direction vector (XY plane for the FVector version, Z is 0).
    *   **Functional Meaning**: Core utility for converting angular representations (like yaw) into directional vectors for movement.

3.  **`AngleToDirection(float Angle)` (FVector2D)** / **`AngleToDirectionXY(float Angle)` (FVector)**
    *   **Purpose**: Converts an angle in degrees into a 2D unit direction vector.
    *   **Functional Meaning**: Same as above, but takes degrees, which is often more convenient for designers.

4.  **`DirectionToAngle(const FVector2D& Direction)` (double)** / **`DirectionToAngleXY(const FVector& Direction)` (double)**
    *   **Purpose**: Converts a 2D direction vector into an angle in degrees (-180 to 180).
    *   **Functional Meaning**: Used to get the yaw angle from a movement input vector or velocity vector.

5.  **`PerpendicularClockwiseXY(const FVector& Vector)`** / **`PerpendicularCounterClockwiseXY(const FVector& Vector)`**
    *   **Purpose**: Calculates a vector that is perpendicular (90 degrees) to the input vector in the XY plane, rotated either clockwise or counter-clockwise. Z component is preserved.
    *   **Functional Meaning**: Useful for calculating strafe directions (right/left) relative to a forward vector.

6.  **`AngleBetweenSkipNormalization(const FVector& From, const FVector& To)` (double)**
    *   **Purpose**: Calculates the angle (in degrees) between two vectors *without* normalizing them first. Assumes `From` and `To` are already (or nearly) unit vectors. Slightly more performant if normalization is guaranteed elsewhere.
    *   **Functional Meaning**: Optimized angle calculation for specific cases.

7.  **`AngleBetweenSignedXY(const FVector3f& From, const FVector3f& To)` (float)**
    *   **Purpose**: Calculates the *signed* angle (in degrees, -180 to 180) between two 3D vectors, projected onto the XY plane. The sign indicates clockwise or counter-clockwise rotation from `From` to `To`.
    *   **Functional Meaning**: Essential for determining not just *how much* to turn, but *in which direction* (left or right) in 2D. Used for foot locking angle limits.

8.  **`SlerpSkipNormalization(const FVector& From, const FVector& To, float Ratio)`**
    *   **Purpose**: Spherical Linear Interpolation between two direction vectors. Assumes `From` and `To` are already normalized. Preserves magnitude if inputs are unit vectors.
    *   **Functional Meaning**: Interpolates a direction vector along the shortest arc on a sphere. Useful for smoothly changing facing directions.

9.  **`SpringDampVector(UPARAM(ref) FAlsSpringVectorState& SpringState, ...)`**
    *   **Purpose**: Specialization of the generic spring-damper for `FVector`.
    *   **Functional Meaning**: Used for smoothly interpolating vector quantities like locations or velocities with spring dynamics.

#### **Adding/Adapting Functionality**

*   **New Vector Functions (C++)**: Add new static BlueprintPure functions to `UAlsVector` for any common vector operations you need.
*   **GAS Integration**: Similar to `UAlsMath`, these are low-level. GAS might modify attributes that are then used as *inputs* to these vector functions by core ALS logic (e.g., a "WindForce" vector attribute from GAS could be added to character velocity, and then `DirectionToAngleXY` would be used on the new velocity).

---

### **C. Rotation Utilities (`UAlsRotation`)**

#### **Scope**

This covers the `UAlsRotation` Blueprint Function Library, providing helper functions for `FRotator` and `FQuat` operations, often dealing with angle normalization, interpolation, and decomposition.

#### **Purpose**

To centralize common rotation and orientation calculations, especially those involving smooth interpolation of angles and handling the complexities of 360-degree wrapping and quaternion math.

#### **Key Concepts/Components**

*   **`CounterClockwiseRotationAngleThreshold` (static constexpr float)**
    *   Value: `5.0f`
    *   **Significance**: A small angle threshold used by `RemapAngleForCounterClockwiseRotation`. If an angle is very close to 180 degrees (e.g., 176 degrees), it's remapped to its negative equivalent (e.g., -184 degrees) to encourage counter-clockwise rotation when taking the shortest path.

#### **Methods & Functions (Static BlueprintCallable in `UAlsRotation`)**

1.  **`RemapAngleForCounterClockwiseRotation(ValueType Angle)` (Template & float overload)**
    *   **Purpose**: If an angle is slightly less than 180 degrees (e.g., 175 to 180), it subtracts 360 from it. This ensures that when interpolating towards such an angle using the shortest path, the rotation will be counter-clockwise rather than a long clockwise turn.
    *   **Functional Meaning**: Improves the visual naturalness of turns, preventing the character from always taking the "positive delta" path if a slightly negative delta is shorter.

2.  **`LerpAngle(float From, float To, float Ratio)`**
    *   **Purpose**: Linear interpolation between two angles (degrees), correctly handling wrapping around 360 degrees and using the shortest path (influenced by `RemapAngleForCounterClockwiseRotation`).
    *   **Functional Meaning**: Standard way to smoothly interpolate a single yaw, pitch, or roll angle.

3.  **`LerpRotation(const FRotator& From, const FRotator& To, float Ratio)`**
    *   **Purpose**: Linearly interpolates each component (Pitch, Yaw, Roll) of an `FRotator` using `LerpAngle`.
    *   **Functional Meaning**: Smoothly transitions an entire `FRotator`.

4.  **`InterpolateAngleConstant(float Current, float Target, float DeltaTime, float Speed)`**
    *   **Purpose**: Interpolates `Current` angle towards `Target` angle at a constant `Speed` (degrees per second), respecting `DeltaTime` and shortest path. Will not overshoot.
    *   **Functional Meaning**: Useful when you want a rotation to occur at a fixed angular rate.

5.  **`DampAngle(float Current, float Target, float DeltaTime, float Smoothing)`**
    *   **Purpose**: Smoothly interpolates `Current` angle towards `Target` using `UAlsMath::Damp` logic and `LerpAngle`.
    *   **Functional Meaning**: Frame-rate independent damped smoothing for angles.

6.  **`DampRotation(const FRotator& Current, const FRotator& Target, float DeltaTime, float Smoothing)`**
    *   **Purpose**: Applies `DampAngle` to each component of an `FRotator`.
    *   **Functional Meaning**: Damped smoothing for full rotators.

7.  **`ExponentialDecayAngle(float Current, float Target, float DeltaTime, float Lambda)`**
    *   **Purpose**: Smoothly interpolates `Current` angle towards `Target` using `UAlsMath::ExponentialDecay` logic and `LerpAngle`.
    *   **Functional Meaning**: Frame-rate independent exponential decay smoothing for angles.

8.  **`ExponentialDecayRotation(const FRotator& Current, const FRotator& Target, float DeltaTime, float Lambda)`**
    *   **Purpose**: Applies `ExponentialDecayAngle` to each component of an `FRotator`.
    *   **Functional Meaning**: Exponential decay smoothing for full rotators. This is heavily used in `AAlsCharacter` for its rotation updates (e.g., `SetRotationExtraSmooth`).

9.  **`InterpolateQuaternionFast(const FQuat& Current, const FQuat& Target, float DeltaTime, float Speed)`**
    *   **Purpose**: Interpolates `Current` quaternion towards `Target` at a constant angular `Speed`, using `FQuat::FastLerp` (which is an Nlerp - Normalized Linear Interpolation) instead of Slerp for performance.
    *   **Functional Meaning**: Faster quaternion interpolation than Slerp, often visually acceptable for game character rotations.

10. **`GetTwist(const FQuat& Quaternion, const FVector& TwistAxis = FVector::UpVector)`**
    *   **Purpose**: Decomposes a quaternion into its "twist" component around a specified `TwistAxis`. The twist is the rotation around that axis.
    *   **Functional Meaning**: Useful for isolating parts of a rotation, e.g., getting only the yaw component of a camera's rotation if `TwistAxis` is Z-up. Used by `AAlsCharacterMovementComponent` to isolate camera yaw for movement base relative rotation.

#### **Adding/Adapting Functionality**

*   **New Rotation Functions (C++)**: Add static BlueprintPure functions to `UAlsRotation` for any new common rotation/quaternion utilities.
*   **GAS Integration**: These are core math functions. An ability wanting to force a character to quickly face a target might calculate the target rotator and then, instead of directly setting it, it might use a `GameplayEffect` to drastically increase a (hypothetical) "ForcedRotationSpeed" attribute, which `AAlsCharacter`'s rotation logic then uses with `ExponentialDecayRotation`.

---

### **D. General Utilities (`UAlsUtility`)**

#### **Scope**

This covers the `UAlsUtility` Blueprint Function Library, containing miscellaneous helper functions that don't fit neatly into the more specialized math libraries but are useful across the ALS system.

#### **Purpose**

To provide a collection of general-purpose helper functions for common tasks like string conversion, accessing animation data, Gameplay Tag manipulation, and network information.

#### **Methods & Functions (Static BlueprintCallable in `UAlsUtility`)**

1.  **`NameToDisplayString(const FName& Name, bool bNameIsBool)` (FString)**
    *   **Purpose**: Converts an `FName` to a more human-readable `FString`, with an option to treat the name as if it were representing a boolean (e.g., for display in debug).
    *   **Functional Meaning**: Used by `AAlsCharacter::DisplayDebugState` to show Gameplay Tag names and boolean states cleanly.

2.  **`GetAnimationCurveValueFromCharacter(const ACharacter* Character, const FName& CurveName)` (float)**
    *   **Purpose**: Safely gets a float animation curve value from a character's animation instance.
    *   **Parameters**: Target `ACharacter`, `CurveName` to retrieve.
    *   **Return Value**: The curve value, or `0.0f` if character/mesh/anim instance is invalid.
    *   **Functional Meaning**: A utility for easily accessing animation curve data from game logic code outside the animation instance itself.

3.  **`GetChildTags(const FGameplayTag& Tag)` (FGameplayTagContainer)**
    *   **Purpose**: Returns a container of all direct child Gameplay Tags for a given parent tag.
    *   **Functional Meaning**: Useful for iterating or checking against a group of related tags (e.g., get all `Als.OverlayMode.*` tags if you pass `Als.OverlayMode`).

4.  **`GetSimpleTagName(const FGameplayTag& Tag)` (FName)**
    *   **Purpose**: Extracts the "simple" name of a Gameplay Tag (the part after the last dot). E.g., for `Als.LocomotionMode.Grounded`, it returns `Grounded`.
    *   **Functional Meaning**: Useful for display purposes or when you only need the final part of a tag's hierarchy.

5.  **`GetFirstPlayerPingSeconds(const UObject* WorldContext)` (float)**
    *   **Purpose**: Gets the network ping (in seconds) of the first local player.
    *   **Functional Meaning**: Can be used for debugging network conditions or potentially for adjusting latency-sensitive parameters (though this is advanced).

6.  **`TryGetMovementBaseRotationSpeed(const FBasedMovementInfo& BasedMovement, FRotator& RotationSpeed)` (bool)**
    *   **Purpose**: Attempts to get the angular velocity (rotation speed in deg/s) of the character's current movement base (if it's a dynamic physics object).
    *   **Parameters**: `BasedMovement` info from `Character->GetBasedMovement()`.
    *   **Output Parameter**: `RotationSpeed`.
    *   **Return Value**: `bool` - True if a valid rotation speed could be obtained.
    *   **Functional Meaning**: Used by `UAlsCharacterMovementComponent` and `AAlsCharacter` to know how fast a platform is rotating, so the character and camera can rotate with it correctly.

#### **Adding/Adapting Functionality**

*   **New Utility Functions (C++)**: Add more static BlueprintCallable functions to `UAlsUtility` for any general-purpose helpers you find yourself needing repeatedly.
*   **GAS Integration**: GAS abilities might use `GetAnimationCurveValueFromCharacter` if an ability's effect needs to be scaled by an animation curve (e.g., an ability that enhances foot IK strength might read the current `FootLeftIkCurve` to determine a base multiplier). `GetSimpleTagName` could be used in debug logs from abilities.

---

### **E. Constants (`UAlsConstants`, `UAlsCameraConstants`)**

#### **Scope**

Covers `UAlsConstants` and `UAlsCameraConstants`. These are Blueprint Function Libraries that serve only to expose `FName` constants to both C++ and Blueprints.

#### **Purpose**

To provide a single, authoritative source for frequently used FNames, such as:
*   Bone names (`RootBoneName`, `PelvisBoneName`, `FootLeftIkBoneName`).
*   Animation Slot names (`TransitionSlotName`, `TurnInPlaceStandingSlotName`).
*   Animation Curve names (`LayerHeadCurveName`, `PoseGaitCurveName`, `FootLeftLockCurveName`).
*   Debug Display names (`CurvesDebugDisplayName`, `CameraShapesDebugDisplayName`).

This prevents errors from typos when using these names as strings and makes refactoring easier (change it in one place, and it's updated everywhere).

#### **Methods & Functions (Static BlueprintPure in these classes)**

Each constant `FName` has a corresponding static `BlueprintPure` function that returns it.
*   **Example (`UAlsConstants.h`)**:
    ```cpp
    inline static const FName RootBone{TEXTVIEW("root")};
    // ...
    UFUNCTION(BlueprintPure, Category = "ALS|Constants|Bones", Meta = (ReturnDisplayName = "Bone Name"))
    static const FName& RootBoneName();
    ```
*   **Example (`UAlsConstants.cpp`)**:
    ```cpp
    const FName& UAlsConstants::RootBoneName() { return RootBone; }
    ```

#### **Adding/Adapting Functionality**

*   **Adding New Constants (C++)**:
    1.  In the `.h` file (e.g., `UAlsConstants.h`), declare a new `inline static const FName NewConstantName{TEXTVIEW("My.New.Constant")};`.
    2.  Add a new `UFUNCTION(BlueprintPure, ...)` declaration for it.
    3.  In the `.cpp` file, implement the simple getter function: `const FName& UAlsConstants::NewConstantName() { return NewConstantName; }`.
*   **Using in C++**: `UAlsConstants::PelvisBoneName()`
*   **Using in Blueprints**: Find the "Constants" category in the Blueprint function search, or search for the specific name (e.g., "Pelvis Bone Name").
*   **GAS Integration**: GAS abilities or effects might need to reference animation slot names (for `PlayMontageInSlot`) or curve names (if directly manipulating curves, though less common). Using these constants ensures correctness.

---

### **F. Logging (`AlsLog`)**

#### **Scope**

This covers the custom logging setup for ALS, primarily the `LogAls` log category.

#### **Purpose**

To provide a dedicated logging channel for messages originating from the ALS system. This allows developers to:
*   Filter ALS-specific messages in the Output Log.
*   Control the verbosity of ALS logs independently of other engine logs.
*   Send important ALS warnings or errors to a dedicated Message Log in the editor.

#### **Key Concepts/Components**

1.  **`AlsLog::MessageLogName` (FName)**
    *   Defined in `AlsLog.cpp`: `const FName AlsLog::MessageLogName{TEXTVIEW("Als")};`
    *   **Significance**: The name used to register and identify the ALS message log panel in the Unreal Editor's "Message Log" window.

2.  **`DEFINE_LOG_CATEGORY_EXTERN(LogAls, Log, All)` (`AlsLog.h`)**
    *   **Significance**: Declares the `LogAls` category so it can be used in header files.
    *   `Log`: Default verbosity if not overridden.
    *   `All`: Compile-time maximum verbosity.

3.  **`DEFINE_LOG_CATEGORY(LogAls)` (`AlsLog.cpp`)**
    *   **Significance**: Defines the actual log category instance.

4.  **`FALSModule::StartupModule()` (in `ALSModule.cpp`)**
    *   Registers the "ALS" message log listing with the editor's `FMessageLogModule`. This creates the "ALS" tab in the Message Log window.

#### **Methods & Functions (Usage)**

*   **Logging in C++**:
    ```cpp
    #include "Utility/AlsLog.h"

    UE_LOG(LogAls, Log, TEXT("This is a regular ALS log message. Value: %f"), MyFloatValue);
    UE_LOG(LogAls, Warning, TEXT("This is an ALS warning!"));
    UE_LOG(LogAls, Error, TEXT("This is an ALS error!"));
    ```
*   **Sending to Editor Message Log (C++)**:
    ```cpp
    #include "Utility/AlsLog.h"
    #include "Logging/MessageLog.h"
    #include "Misc/UObjectToken.h" // For linking to UObjects in messages

    // ...
    FMessageLog MessageLogWriter{AlsLog::MessageLogName};
    MessageLogWriter.Warning(FText::Format(LOCTEXT("MyWarningKey", "Something important happened with object: {0}"), FText::FromString(MyObject->GetName())));
    // Optional: Add a token to click and focus the object
    // MessageLogWriter.AddToken(FUObjectToken::Create(MyObject));
    MessageLogWriter.Notify(); // Or MessageLogWriter.Open() to force open the log
    ```

#### **Adding/Adapting Functionality**

*   This is a foundational system. You primarily *use* it by adding `UE_LOG(LogAls, ...)` calls in your ALS-related C++ code for debugging or information.
*   **GAS Integration**: Your custom Gameplay Abilities or Ability Tasks written in C++ can use `UE_LOG(LogAls, ...)` to output debug information specific to their interaction with ALS.

---

### **G. Macros (`AlsMacros.h`)**

#### **Scope**

This covers the custom preprocessor macros defined in `AlsMacros.h`, specifically the `ALS_ENSURE` family.

#### **Purpose**

To provide a lightweight assertion mechanism that is less intrusive than the standard `ensure()` macro, particularly in terms of performance impact during development and avoiding automatic crash report submissions for non-critical checks.

#### **Key Concepts/Components**

1.  **`ALS_ENSURE(Expression)`**
2.  **`ALS_ENSURE_MESSAGE(Expression, Format, ...)`**
3.  **`ALS_ENSURE_ALWAYS(Expression)`**
4.  **`ALS_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...)`**

*   **Similar to `ensure()`**: They check if `Expression` is true. If false, they log an error/warning and potentially break into the debugger.
*   **Key Differences from standard `ensure()`**:
    *   **No C++ Call Stack by Default**: They don't generate a full C++ call stack in the log, which can make logs cleaner and the ensure itself faster.
    *   **No Automatic Crash Report**: They are less likely to trigger the UE crash reporter for simple ensures.
    *   **Atomic Execution Flag**: Each `ALS_ENSURE` site uses a `static std::atomic<bool> bExecuted` to ensure it only fires once (unless it's an `_ALWAYS` version or `core.EnsureAlwaysEnabled` cvar is true). This prevents log spam from repeated failures of the same check.
*   **`_ALWAYS` versions**: Will trigger every time the condition is false, even if `bExecuted` was true, if `core.EnsureAlwaysEnabled` cvar is true.
*   **`_MESSAGE` versions**: Allow providing a custom formatted message.
*   **`AlsEnsure::Execute(...)`**: The C++ function that implements the logic for these macros.
*   **`PLATFORM_BREAK()`**: Called if the ensure fails and a debugger is attached, causing a breakpoint.

#### **Adding/Adapting Functionality**

*   **Usage**: Replace standard `ensure()` calls with `ALS_ENSURE()` or `ALS_ENSURE_MESSAGE()` in your ALS C++ code for checks where a full crash report or call stack is not strictly necessary for diagnosing the issue.
    ```cpp
    // Instead of: ensure(MyPointer != nullptr);
    ALS_ENSURE(IsValid(MyPointer));

    // Instead of: ensureMsgf(Value > 0, TEXT("Value must be positive: %d"), Value);
    ALS_ENSURE_MESSAGE(Value > 0, TEXT("Value must be positive: %d"), Value);
    ```
*   **GAS Integration**: When writing C++ Gameplay Abilities that interact heavily with ALS components, use `ALS_ENSURE` to validate pointers or states obtained from ALS to catch integration errors early.

---

### **H. Enum Utilities (`AlsEnumUtility.h`)**

#### **Scope**

This covers the `AlsEnumUtility` namespace, providing template helper functions for working with C++ `enum class` types.

#### **Purpose**

To offer convenient ways to get the string name or index of an enum value, which can be useful for logging, debugging, or serialization.

#### **Methods & Functions (Templated in `AlsEnumUtility` namespace)**

1.  **`GetIndexByValue<EnumType>(const EnumType Value)` (int32)**
    *   **Purpose**: Gets the integer index of an enum value within its `UEnum` definition.
    *   **Example**: `int32 Index = AlsEnumUtility::GetIndexByValue(EAlsMovementDirection::Forward);`

2.  **`GetNameStringByValue<EnumType>(const EnumType Value)` (FString)**
    *   **Purpose**: Gets the string representation of an enum value's name (e.g., "Forward" for `EAlsMovementDirection::Forward`).
    *   **Example**: `FString Name = AlsEnumUtility::GetNameStringByValue(MyMovementDirectionEnum); UE_LOG(LogAls, Log, TEXT("Current Direction: %s"), *Name);`

#### **Adding/Adapting Functionality**

*   This is a utility to be used, not typically extended unless you need more advanced enum reflection capabilities.
*   **GAS Integration**: Useful for logging the state of custom enums that might be used by your Gameplay Abilities or Effects for more descriptive debug messages.














-----------------------------------------------------------------------------------------













## **V. Editor & Developer Tools**

This section covers the tools and extensions provided by ALS to assist developers and animators in using and extending the system within the Unreal Editor. These tools streamline workflows, enable custom animation logic, and help in debugging.

### **A. Animation Modifiers**

#### **Scope**

This subsection details the Animation Modifier classes provided by ALS. Animation Modifiers are editor-time utilities that can be applied to `UAnimSequence` assets to programmatically alter them, such as adding or modifying animation curves.

#### **Purpose**

ALS Animation Modifiers automate repetitive tasks and ensure consistency when preparing animation assets for the ALS system. They primarily focus on creating and setting up the various animation curves that ALS relies on to drive its blending and procedural animation logic. This saves animators and designers significant manual work and reduces the chance of errors.

#### **Key Concepts/Components (Animation Modifier Classes)**

These classes are typically found in the `ALSEditor` module, as they are editor-only tools.

1.  **`UAlsAnimationModifier_CalculateRotationYawSpeed`**
    *   **Display Name**: "Als Calculate Rotation Yaw Speed Animation Modifier"
    *   **Purpose**: This modifier analyzes an animation sequence to determine the root bone's rotation yaw speed between frames. It then creates (or overwrites) a float curve named `UAlsConstants::RotationYawSpeedCurveName()` (typically "RotationYawSpeed") on the sequence.
    *   **Functional Meaning**: The "RotationYawSpeed" curve is used by `UAlsAnimationInstance` during grounded rotation logic (specifically `AAlsCharacter::ApplyRotationYawSpeedAnimationCurve`) to allow animations (like turn-in-place or start/stop transitions) to contribute to the character's actual yaw rotation, effectively mimicking root motion rotation without needing the root bone to be explicitly animated for rotation. Each key on the curve represents the calculated yaw speed from that frame to the next.
    *   **Key Steps in `OnApply_Implementation`**:
        *   Checks if the "RotationYawSpeed" curve exists; if so, removes it.
        *   Adds a new float curve named "RotationYawSpeed".
        *   Iterates through the animation frames.
        *   For each frame, it gets the root bone's transform at the current frame (`i-1` or `i`) and the next frame (`i` or `i+1`, depending on `RateScale` direction).
        *   Calculates the delta in yaw rotation between these two frames.
        *   Converts this delta yaw to a speed (delta yaw * frame rate * `RateScale`).
        *   Adds a key to the "RotationYawSpeed" curve at the current frame's time with the calculated speed.
    *   **Always call `Super::OnApply_Implementation()` when overriding this function in a custom C++ modifier.**

2.  **`UAlsAnimationModifier_CopyCurves`**
    *   **Display Name**: "Als Copy Curves Animation Modifier"
    *   **Purpose**: Copies float animation curves from a specified source `UAnimSequence` to the `UAnimSequence` the modifier is applied to.
    *   **`UPROPERTY` Members**:
        *   **`SourceSequence` (TSoftObjectPtr<UAnimSequence>)**: A soft object pointer to the animation sequence from which to copy curves.
            *   **`EditAnywhere, BlueprintReadOnly, Category = "Settings"`**
        *   **`bCopyAllCurves` (bool)**: If true, all float curves from the `SourceSequence` are copied.
            *   **`EditAnywhere, BlueprintReadOnly, Category = "Settings"`**
        *   **`CurveNames` (TArray<FName>)**: If `bCopyAllCurves` is false, only curves with names in this array are copied.
            *   **`EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (EditCondition = "!bCopyAllCurves")`**
    *   **Functional Meaning**: Useful for transferring existing curve data (e.g., perfectly tuned foot IK curves) from one animation to another similar animation, or from a template animation.
    *   **Key Steps in `OnApply_Implementation`**:
        *   Loads the `SourceSequence`.
        *   If `bCopyAllCurves` is true, iterates through all float curves in the source.
        *   Otherwise, iterates through the specified `CurveNames`.
        *   For each curve to copy, it calls the internal `CopyCurve` static function.
    *   **`CopyCurve(UAnimSequence* SourceSequence, UAnimSequence* TargetSequence, const FName& CurveName)` (static private)**:
        *   Removes the curve from `TargetSequence` if it already exists.
        *   Adds a new curve with `CurveName` to `TargetSequence`.
        *   Gets all float keys (times and values) from the curve in `SourceSequence`.
        *   Adds these keys to the newly created curve in `TargetSequence`.
    *   **Always call `Super::OnApply_Implementation()`!**

3.  **`UAlsAnimationModifier_CreateCurves`**
    *   **Display Name**: "Als Create Curves Animation Modifier"
    *   **Purpose**: Creates one or more new float animation curves on an animation sequence and populates them with specified keyframe data. This is the primary tool for adding the standard ALS curves (like IK, layering, pose state curves) to animations.
    *   **`UPROPERTY` Members**:
        *   **`bOverrideExistingCurves` (bool)**: If true, and a curve to be created already exists, the existing curve will be removed and replaced. If false, existing curves are skipped.
            *   **`EditAnywhere, BlueprintReadOnly, Category = "Settings"`**
        *   **`Curves` (TArray<FAlsAnimationCurve>)**: An array defining the curves to create.
            *   **`EditAnywhere, BlueprintReadOnly, Category = "Settings"`**
            *   **`FAlsAnimationCurve` (USTRUCT)**:
                *   `Name` (FName): The name of the curve to create.
                *   `bAddKeyOnEachFrame` (bool): If true, a key with value `0.0f` (by default, but see next point) is added to every frame of the animation. Useful for curves that need to exist but might be modified later manually or by another process. The default `Value` of `0.0f` for each frame is hardcoded in this specific modifier. If you need other values, you'd use the `Keys` array.
                *   `Keys` (TArray<FAlsAnimationCurveKey>): An array of keyframes to add to the curve. If `bAddKeyOnEachFrame` is false, these keys define the curve.
                    *   **`FAlsAnimationCurveKey` (USTRUCT)**:
                        *   `Frame` (int32): The frame number for this key.
                        *   `Value` (float): The float value of the curve at this key.
    *   **Functional Meaning**: This is how you ensure your animations have all the necessary ALS curves. For example, to set up basic foot IK, you'd add entries for `FootLeftIkCurveName()`, `FootRightIkCurveName()`, `FootLeftLockCurveName()`, and `FootRightLockCurveName()`. You would then set their `Keys`. For instance, for `FootLeftIkCurve`, you might set a key at frame 0 with value 0.0, a key where the foot is planted with value 1.0, and another key where the foot lifts off with value 0.0.
    *   **Default `Curves`**: The modifier comes pre-populated with a default list of commonly used ALS curves (Pose, Feet, general control curves), usually with `bAddKeyOnEachFrame = false` and a single key at frame 0 with value 0.0, acting as a template.
    *   **Key Steps in `OnApply_Implementation`**:
        *   Iterates through the `Curves` array.
        *   For each `FAlsAnimationCurve`:
            *   Checks if the curve already exists. If it does and `bOverrideExistingCurves` is false, skips it. Otherwise, removes the existing curve.
            *   Adds a new float curve with `Curve.Name`.
            *   If `Curve.bAddKeyOnEachFrame` is true, loops through all animation frames and adds a key with value `0` at each frame.
            *   Else, loops through `Curve.Keys` and adds a key to the animation curve at `Sequence->GetTimeAtFrame(CurveKey.Frame)` with `CurveKey.Value`.
    *   **Always call `Super::OnApply_Implementation()`!**

4.  **`UAlsAnimationModifier_CreateLayeringCurves`**
    *   **Display Name**: "Als Create Layering Curves Animation Modifier"
    *   **Purpose**: A specialized modifier for quickly adding a predefined set of ALS layering curves (e.g., `LayerHead`, `LayerArmLeft`, `LayerSpineSlot`) to an animation sequence, all with a single configurable value.
    *   **`UPROPERTY` Members**:
        *   **`bOverrideExistingCurves` (bool)**: Same as in `UAlsAnimationModifier_CreateCurves`.
        *   **`bAddKeyOnEachFrame` (bool)**: If true, a key is added at every frame with `CurveValue`. If false, only one key is added at frame 0 with `CurveValue`.
        *   **`CurveValue` (float)**: The float value to set for all the primary layering curves.
            *   **Example Configuration**: For an upper-body rifle aiming animation, you might set `CurveValue = 1.0`. For a base locomotion cycle that should *not* affect the upper body (expecting an overlay), you might set `CurveValue = 0.0` for upper body related curves in its `CurveNames` list, or more simply, ensure that the animation that *should* control the upper body has those curves set to 1.0.
        *   **`CurveNames` (TArray<FName>)**: A predefined list of common layering curves.
        *   **`bAddSlotCurves` (bool)**: If true, also creates/updates "Slot" curves (e.g., `LayerHeadSlotCurveName()`).
        *   **`SlotCurveValue` (float)**: The value to set for all the slot curves if `bAddSlotCurves` is true.
            *   **Example Configuration**: `SlotCurveValue = 1.0`. Slot curves typically control whether a montage playing in a specific slot can affect that body part. `1.0` means the slot is "open" for that part in this animation.
        *   **`SlotCurveNames` (TArray<FName>)**: A predefined list of common slot-related layering curves.
    *   **Functional Meaning**: Streamlines the process of setting up animations for ALS's layering system. For instance, if you have an animation meant to be a full-body pose that should take full control of layering, you'd apply this modifier with `CurveValue = 1.0` and `SlotCurveValue = 1.0`. If it's a base locomotion that expects overlays, many of these curves might be 0.0 in the base, and 1.0 in the overlay animation.
    *   **Key Steps in `OnApply_Implementation`**:
        *   Calls `CreateCurves(Sequence, CurveNames, CurveValue)`.
        *   If `bAddSlotCurves` is true, calls `CreateCurves(Sequence, SlotCurveNames, SlotCurveValue)`.
    *   **`CreateCurves(...)` (private helper)**: Similar logic to `UAlsAnimationModifier_CreateCurves`'s loop, but applies a single `Value` to all specified `Names`.
    *   **Always call `Super::OnApply_Implementation()`!**

#### **How to Use Animation Modifiers**

1.  **Open Animation Sequence**: In the Content Browser, double-click an `UAnimSequence` asset to open it in the Animation Editor.
2.  **Animation Data Tab**: Go to the "Window" menu and ensure "Animation Data" is checked. This panel usually appears on the right.
3.  **Add Modifier**: In the "Animation Data" panel, find the "Animation Modifiers" section. Click the "+" (Add) button.
4.  **Select Modifier**: A dropdown will appear. Choose the ALS Animation Modifier you want to apply (e.g., "Als Create Curves Animation Modifier").
5.  **Configure Modifier**: The selected modifier will appear in the list with its editable properties (e.g., for `UAlsAnimationModifier_CreateCurves`, you can edit the `Curves` array).
6.  **Apply Modifier**: Right-click on the modifier instance in the list and select "Apply Modifier".
7.  **Save Asset**: Save your `UAnimSequence` asset. The changes (e.g., new curves) are now part of the animation.

#### **Adding/Adapting Functionality**

*   **Creating a New Custom Animation Modifier (C++)**:
    1.  Create a new C++ class inheriting from `UAnimationModifier`. Place it in an Editor module (like `ALSEditor`).
    2.  Override the `OnApply_Implementation(UAnimSequence* Sequence)` function. **Remember to call `Super::OnApply_Implementation(Sequence);`**.
    3.  Inside your override, use `UAnimationBlueprintLibrary` functions to manipulate the `Sequence`:
        *   `UAnimationBlueprintLibrary::AddCurve(Sequence, CurveName, ERawCurveTrackTypes::RCT_Float)`
        *   `UAnimationBlueprintLibrary::RemoveCurve(Sequence, CurveName)`
        *   `UAnimationBlueprintLibrary::AddFloatCurveKey(Sequence, CurveName, Time, Value)`
        *   `UAnimationBlueprintLibrary::GetFloatKeys(Sequence, CurveName, Times, Values)`
    4.  Expose any necessary parameters as `UPROPERTY(EditAnywhere, ...)` in your modifier's header file.
*   **Example: Modifier to Set a Specific Curve to Sine Wave**:
    ```cpp
    // AlsAnimationModifier_SetSineCurve.h
    #pragma once
    #include "AnimationModifier.h"
    #include "AlsAnimationModifier_SetSineCurve.generated.h"

    UCLASS()
    class ALSEDITOR_API UAlsAnimationModifier_SetSineCurve : public UAnimationModifier
    {
        GENERATED_BODY()
    protected:
        UPROPERTY(EditAnywhere, Category = "Settings")
        FName TargetCurveName{"MySineCurve"};
        UPROPERTY(EditAnywhere, Category = "Settings")
        float Frequency = 1.0f; // Cycles per second
        UPROPERTY(EditAnywhere, Category = "Settings")
        float Amplitude = 1.0f;
    public:
        virtual void OnApply_Implementation(UAnimSequence* Sequence) override;
    };

    // AlsAnimationModifier_SetSineCurve.cpp
    #include "Animation/AnimSequence.h" // For UAnimSequence
    #include "AnimationBlueprintLibrary.h" // For curve functions

    void UAlsAnimationModifier_SetSineCurve::OnApply_Implementation(UAnimSequence* Sequence)
    {
        Super::OnApply_Implementation(Sequence);
        if (!Sequence) return;

        UAnimationBlueprintLibrary::RemoveCurve(Sequence, TargetCurveName, false); // Remove if exists
        UAnimationBlueprintLibrary::AddCurve(Sequence, TargetCurveName, ERawCurveTrackTypes::RCT_Float, false);

        const float SequenceLength = Sequence->GetPlayLength();
        const int32 NumFrames = Sequence->GetNumberOfSampledKeys();

        for (int32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
        {
            const float TimeAtFrame = Sequence->GetTimeAtFrame(FrameIndex);
            const float SineValue = FMath::Sin(TimeAtFrame * UE_TWO_PI * Frequency) * Amplitude;
            UAnimationBlueprintLibrary::AddFloatCurveKey(Sequence, TargetCurveName, TimeAtFrame, SineValue);
        }
        // Trigger a recompile of the sequence to see changes
        // Sequence->Modify(); // Handled by applying modifier
    }
    ```
*   **GAS Integration**: Animation Modifiers are editor-time tools. They don't directly interact with GAS at runtime. However, the curves they create *are* used by `UAlsAnimationInstance`, which in turn is influenced by character states that GAS abilities can modify. For example, if a GAS ability makes the character "energized," and you have an "EnergyLevel" curve on animations (perhaps set by a modifier based on some animation property), the animation system could read this curve to make animations play faster or stronger.







---------------------------------------------------------------------------
















### **B. AnimGraph Nodes**

#### **Scope**

This subsection details the custom Animation Graph nodes provided by ALS. These nodes can be used within the AnimGraph of an Animation Blueprint to implement specialized blending logic or access ALS-specific data in a visual way.

#### **Purpose**

Custom AnimGraph nodes extend the capabilities of the Animation Blueprint editor, allowing for:
*   **Tailored Blending**: Implementing blend logic that is specific to ALS's needs (e.g., blending based on Gameplay Tags).
*   **Simplified Graphing**: Encapsulating complex logic into a single node, making AnimGraphs cleaner and easier to understand.
*   **Direct Access to ALS Features**: Providing nodes that directly consume or manipulate ALS-specific states or curve types.

#### **Key Concepts/Components (AnimGraph Node Classes)**

These nodes are typically defined with two classes:
*   **`FAlsAnimNode_...` (e.g., `FAlsAnimNode_CurvesBlend`)**: The runtime struct (derived from `FAnimNode_Base` or a more specific base like `FAnimNode_BlendListBase`) that performs the actual animation processing on the worker thread. Contains `UPROPERTY` members for inputs (like poses, alpha values) and internal state.
*   **`UAlsAnimGraphNode_...` (e.g., `UAlsAnimGraphNode_CurvesBlend`)**: The editor-time representation of the node (derived from `UAnimGraphNode_Base`). Handles the visual appearance in the AnimGraph editor, pin creation, and node titles/tooltips. It contains an instance of the runtime node struct.

1.  **`FAlsAnimNode_CurvesBlend` / `UAlsAnimGraphNode_CurvesBlend`**
    *   **Display Name**: "Blend Curves"
    *   **Category**: "ALS"
    *   **Purpose**: Allows blending of animation *curves* from a "Curves Pose" onto a "Source Pose" without affecting the bone transforms of the "Source Pose". This is useful for applying curve data from one animation (like a utility animation that only contains curves) to another.
    *   **`FAlsAnimNode_CurvesBlend` `UPROPERTY` Members (Runtime Node)**:
        *   **`SourcePose` (FPoseLink)**: Input pose whose bone transforms will be passed through.
        *   **`CurvesPose` (FPoseLink)**: Input pose from which animation curves will be taken and blended/applied.
        *   **`BlendAmount` (float, 0-1, EditAnywhere on GraphNode, exposed as pin by default)**: The alpha for blending curves when `BlendMode` is `BlendByAmount`.
        *   **`BlendMode` (EAlsCurvesBlendMode enum, EditAnywhere on GraphNode)**: Defines how curves from `CurvesPose` are combined with curves from `SourcePose`.
            *   `BlendByAmount`: `OutputCurve = SourceCurve * (1-Amount) + CurvesPoseCurve * Amount`.
            *   `Combine`: `OutputCurve = CurvesPoseCurve` if it exists, else `SourceCurve`. (Overrides `SourceCurve`).
            *   `CombinePreserved`: `OutputCurve = SourceCurve` if it exists, else `CurvesPoseCurve`. (Does not override `SourceCurve`).
            *   `UseMaxValue`: `OutputCurve = Max(SourceCurve, CurvesPoseCurve)`.
            *   `UseMinValue`: `OutputCurve = Min(SourceCurve, CurvesPoseCurve)`.
            *   `Override`: `OutputCurve = CurvesPoseCurve`. (Ignores `SourceCurve` completely).
    *   **Functional Meaning**:
        *   Imagine you have a base locomotion animation. You also have a separate, short animation that *only* contains a curve to make the character blink. You can use "Blend Curves" with `BlendMode = Combine` to take the blink curve from the blink animation and apply it to your locomotion without affecting the movement itself.
        *   Another use: if `SourcePose` has a base "IKAlpha" curve and `CurvesPose` has a temporary override "IKAlpha_Weapon" curve, you could use `UseMaxValue` to ensure IK is active if either curve says it should be.
    *   **`Evaluate_AnyThread` Logic**: Evaluates `SourcePose`. If `BlendAmount` is relevant, evaluates `CurvesPose`. Then, based on `BlendMode`, it uses `Output.Curve.Accumulate()`, `Combine()`, `CombinePreserved()`, `UseMaxValue()`, `UseMinValue()`, or `Override()` to merge the curve data from `CurvesPoseContext.Curve` into `Output.Curve`.

2.  **`FAlsAnimNode_GameplayTagsBlend` / `UAlsAnimGraphNode_GameplayTagsBlend`**
    *   **Display Name**: "Blend Poses by Gameplay Tag"
    *   **Category**: "ALS"
    *   **Purpose**: A dynamic blend node that selects one of its input poses based on an active `FGameplayTag`. It's similar to Unreal's "Blend Poses by Enum" or "Blend Poses by Int," but uses Gameplay Tags for selection.
    *   **`FAlsAnimNode_GameplayTagsBlend` `UPROPERTY` Members (Runtime Node)**:
        *   **`ActiveTag` (FGameplayTag, EditAnywhere on GraphNode, exposed as pin by default)**: The input Gameplay Tag that determines which pose to activate.
        *   **`Tags` (TArray<FGameplayTag>, EditAnywhere on GraphNode)**: An array of Gameplay Tags defined in the node's details. Each tag in this array corresponds to an input pose pin on the node.
        *   **`BlendPose` (TArray<FPoseLink>)**: Inherited from `FAnimNode_BlendListBase`. This array holds the actual input pose links. `BlendPose[0]` is the default. `BlendPose[i+1]` corresponds to `Tags[i]`.
    *   **`UAlsAnimGraphNode_GameplayTagsBlend` Logic**:
        *   `PostEditChangeProperty`: If the `Tags` array is changed in the Details panel, it calls `ReconstructNode()` to update the pins.
        *   `ReallocatePinsDuringReconstruction`: Calls `Node.RefreshPosePins()` on the runtime node, which adds/removes `BlendPose` entries to match the number of `Tags`.
        *   `CustomizePinData`: Sets the friendly name of the input pose pins to "Default" for the first pin, and the simple tag name (e.g., "Grounded" from `Als.LocomotionMode.Grounded`) for subsequent pins.
    *   **Functional Meaning**: Allows you to create a switch in your AnimGraph based on Gameplay Tags.
        *   **Example**:
            *   Input `ActiveTag`: Connect the character's current `LocomotionMode` tag (from `UAlsAnimationInstance`).
            *   `Tags` array (in node details):
                *   Index 0: `Als.LocomotionMode.Grounded`
                *   Index 1: `Als.LocomotionMode.InAir`
            *   Pins:
                *   `Default Pose`: Fallback pose (can be anything, or even an empty pose).
                *   `"Grounded" Pose`: Connect your grounded locomotion state machine output here.
                *   `"InAir" Pose`: Connect your in-air locomotion state machine output here.
            *   The node will output the "Grounded" pose if `ActiveTag` is `Als.LocomotionMode.Grounded`, the "InAir" pose if it's `Als.LocomotionMode.InAir`, and the `Default Pose` otherwise (or if `ActiveTag` is invalid/empty).
    *   **`GetActiveChildIndex()` Logic (Runtime Node)**: Finds the `ActiveTag` in its internal `Tags` array. Returns the index + 1 (because index 0 is the `Default` pose). If not found or `ActiveTag` is invalid, returns 0.

#### **Adding/Adapting Functionality**

*   **Using Existing Nodes in AnimGraph**:
    1.  In your Animation Blueprint's AnimGraph, right-click and search for "Blend Curves" or "Blend Poses by Gameplay Tag" under the "ALS" category.
    2.  Connect pose links and input pins as needed.
    3.  Configure properties in the Details panel of the selected node (e.g., `BlendMode` for CurvesBlend, `Tags` array for GameplayTagsBlend).
*   **Creating a New Custom AnimGraph Node (C++)**: This is an advanced task.
    1.  **Runtime Node (`FMyCustomAnimNode`)**:
        *   Create a struct inheriting from `FAnimNode_Base` (or a more specific base like `FAnimNode_SkeletalControlBase` if it modifies bones, or `FAnimNode_BlendListBase` if it's a multi-pose blender).
        *   Add `UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", Meta=(PinShownByDefault))` for input pose links (`FPoseLink`) and other data pins (floats, bools, tags).
        *   Override virtual functions:
            *   `Initialize_AnyThread`, `CacheBones_AnyThread`, `Update_AnyThread`, `Evaluate_AnyThread` (for pose generation/blending) or `EvaluateSkeletalControl_AnyThread` (for bone manipulation).
            *   `GatherDebugData` (optional, for Animation Debugger).
    2.  **Editor Node (`UMyCustomAnimGraphNode`)**:
        *   Create a class inheriting from `UAnimGraphNode_Base`.
        *   Add a `UPROPERTY(EditAnywhere, Category="Settings") FMyCustomAnimNode Node;` member to hold the runtime node instance.
        *   Override:
            *   `GetNodeTitle()`, `GetTooltipText()`, `GetNodeCategory()`.
            *   `Create kilkuPins()` (if you have complex pin setups not handled by `UPROPERTY` meta).
            *   `CustomizePinData()` (to rename pins).
            *   `PostEditChangeProperty()` (if changing a property needs to reconstruct pins).
    3.  Compile. Your new node should appear in the AnimGraph palette.
*   **GAS Integration**:
    *   **`FAlsAnimNode_GameplayTagsBlend`**: This is highly useful for GAS.
        *   **Scenario**: You have different idle animations based on whether a `State.WeaponEquipped` tag or `State.Relaxed` tag is active (granted by GAS abilities/effects).
        *   **AnimGraph**: Use "Blend Poses by Gameplay Tag".
            *   Input `ActiveTag`: A variable in your `UAlsAnimationInstance` that you set based on `Character->GetAbilitySystemComponent()->GetActiveGameplayTags().HasTag(MyTag)`.
            *   `Tags` array: Add `State.WeaponEquipped`, `State.Relaxed`.
            *   Connect your "Weapon Equipped Idle" pose and "Relaxed Idle" pose to the corresponding pins.
    *   **`FAlsAnimNode_CurvesBlend`**:
        *   **Scenario**: A GAS ability applies a temporary effect that should make all IK less effective (e.g., character is "dazed").
        *   You could have a utility animation that contains only IK-related curves (like `FootLeftIkCurve`) all set to a low value (e.g., 0.2).
        *   A variable in `UAlsAnimationInstance`, `DazedIKAlpha` (float), is set to 1.0 when a "Dazed" tag is active (from GAS).
        *   **AnimGraph**:
            ```
            BaseLocoPose -> [FAlsAnimNode_CurvesBlend (SourcePose)]
                                 [Utility_Dazed_IK_Curves_Pose (CurvesPose)]
                                 BlendAmount = DazedIKAlpha
                                 BlendMode = UseMinValue (or BlendByAmount carefully)
                           -> OutputPose
            ```
            This would effectively reduce the IK curve values from the base pose if `DazedIKAlpha` is high and the utility pose has lower IK curve values.

#### **Network Synchronization**

*   AnimGraph nodes themselves and their internal runtime structs (`FAlsAnimNode_...`) are **not directly replicated**. They execute locally on each client and the server.
*   **Consistency**: Their synchronized behavior relies on their *inputs* being synchronized.
    *   **Pose Inputs (`FPoseLink`)**: The poses fed into these nodes are the result of other nodes or state machines, which are ultimately driven by replicated character state (`AAlsCharacter` Gameplay Tags) or replicated montage states.
    *   **Data Inputs (Floats, Tags)**: Pins like `BlendAmount` on `FAlsAnimNode_CurvesBlend` or `ActiveTag` on `FAlsAnimNode_GameplayTagsBlend` should be connected to properties within your `UAlsAnimationInstance`. These properties, in turn, should be updated based on the replicated state from `AAlsCharacter` or data derived from GAS (which also has its own replication mechanisms for tags and attributes).
*   If the inputs are consistent across all machines, the deterministic logic within the AnimGraph nodes will produce visually consistent animation outputs.

#### **Debugging**

*   **Animation Debugger**: This is the primary tool.
    *   **Node Inspection**: Select your custom ALS AnimGraph node instance in the debugger. You can see the live values of its input pins and internal properties (if exposed).
    *   **Pose Watch**: You can "watch" the bone transforms of poses flowing into and out of these nodes to see exactly how they are being blended or modified.
    *   **Curve Watch**: If debugging `FAlsAnimNode_CurvesBlend`, you can monitor the animation curves on the input and output poses.
*   **Visual Graph**: Set breakpoints on nodes in the Animation Blueprint editor to pause execution and inspect pin values.
*   **`UAlsAnimationInstance` Details Panel**: Check the values of the `UAlsAnimationInstance` properties that are feeding into these AnimGraph nodes.
*   **`ShowDebug Als.State` / `ShowDebug Als.Curves`**: Useful to see the high-level character state and curve values that might be inputs to these nodes or that these nodes are intended to affect.


















-----------------------------------------------------------------------------














Okay, let's proceed with practical examples for Animation Notifies, Animation Notify States, and Control Rig / RigVM Units within the ALS framework, keeping GAS integration in mind.

---

## **V-C. Animation Notifies**

#### **Scope**

This subsection covers the custom `UAnimNotify` classes provided by ALS. Animation Notifies are events that can be placed on an animation timeline (in `UAnimSequence` or `UAnimMontage` assets) to trigger C++ or Blueprint logic at a specific point during the animation's playback.

#### **Purpose**

ALS Animation Notifies are used to:
*   Synchronize gameplay events with animations (e.g., spawning footstep effects when a foot touches the ground).
*   Trigger non-animation related game logic from an animation (e.g., causing a camera shake during an impact animation).

#### **Key Concepts/Components (AnimNotify Classes)**

1.  **`UAlsAnimNotify_CameraShake`**
    *   **Display Name**: "Als Camera Shake Animation Notify"
    *   **Purpose**: Triggers a specified `UCameraShakeBase` class when the notify is encountered in an animation.
    *   **`UPROPERTY` Members**:
        *   **`CameraShakeClass` (TSubclassOf<UCameraShakeBase>)**: The class of camera shake to play.
            *   **`EditAnywhere, BlueprintReadOnly, Category = "Settings"`**
        *   **`CameraShakeScale` (float)**: A multiplier for the intensity of the camera shake.
            *   **`EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ForceUnits = "x")`**
    *   **Functional Meaning**: Allows animators to easily add screen shake effects to animations (e.g., during a heavy landing, explosion impact, or a powerful attack) directly on the animation timeline.
    *   **`Notify(USkeletalMeshComponent* Mesh, ...)` Logic**:
        *   Gets the `APawn` owner of the `Mesh`.
        *   Gets the `APlayerController` controlling the pawn.
        *   Gets the `PlayerCameraManager` from the controller.
        *   If valid, calls `PlayerCameraManager->StartCameraShake(CameraShakeClass, CameraShakeScale)`.
        *   **Note**: `bShouldFireInEditor` is `false` (won't play in Persona editor preview), `bTriggerOnDedicatedServer` is `false` (camera shakes are client-side effects).
    *   **Always call `Super::Notify()` when overriding this function in a custom C++ notify.**

2.  **`UAlsAnimNotify_FootstepEffects`**
    *   **Display Name**: "Als Footstep Effects Animation Notify"
    *   **Purpose**: Spawns footstep-related visual and audio effects (sound, decal, particle system) based on the surface type the character's foot lands on.
    *   **`UPROPERTY` Members**:
        *   **`FootstepEffectsSettings` (TObjectPtr<UAlsFootstepEffectsSettings>)**: A pointer to a `UAlsFootstepEffectsSettings` Data Asset that defines all the effects for different surface types.
            *   **`EditAnywhere, BlueprintReadOnly, Category = "Settings"`**
        *   **`FootBone` (EAlsFootBone enum: Left, Right)**: Specifies which foot this notify is for.
            *   **`EditAnywhere, BlueprintReadOnly, Category = "Settings"`**
        *   **`bSkipEffectsWhenInAir` (bool)**: If true, effects won't spawn if the character is in `LocomotionMode.InAir`.
        *   **`bSpawnSound`, `SoundVolumeMultiplier`, `SoundPitchMultiplier`, `SoundType` (EAlsFootstepSoundType), `bIgnoreFootstepSoundBlockCurve`**: Settings for spawning sound. `SoundType` (Step, WalkRun, Land) can be used by the sound cue to play different variations.
        *   **`bSpawnDecal`**: Settings for spawning decals.
        *   **`bSpawnParticleSystem`**: Settings for spawning Niagara particle effects.
    *   **Functional Meaning**: Placed on walk/run/jump land animations at the moment a foot makes contact with the ground. It performs a trace downwards from the specified foot bone to detect the surface type and then spawns the configured effects.
    *   **`Notify(USkeletalMeshComponent* Mesh, ...)` Logic**:
        1.  Checks if effects should be skipped (e.g., `bSkipEffectsWhenInAir`).
        2.  Determines foot bone name (`UAlsConstants::FootLeftBoneName()` or `...RightBoneName()`).
        3.  Gets foot transform and configured Z-axis for tracing.
        4.  Performs a line trace downwards from the foot to find the ground and its `UPhysicalMaterial`.
        5.  Determines `SurfaceType` (from `EPhysicalSurface`) from the hit physical material.
        6.  Finds the corresponding `FAlsFootstepEffectSettings` from the `FootstepEffectsSettings` Data Asset for that `SurfaceType`.
        7.  Calculates footstep location and rotation based on impact normal.
        8.  Calls helper functions `SpawnSound()`, `SpawnDecal()`, `SpawnParticleSystem()` based on the bool flags and retrieved settings.
        *   `SpawnSound`: Plays sound using `UGameplayStatics::PlaySoundAtLocation` or `SpawnSoundAttached`. Considers `FootstepSoundBlockCurveName()`.
        *   `SpawnDecal`: Spawns decal using `UGameplayStatics::SpawnDecalAtLocation` or `SpawnDecalAttached`. Considers `DecalSpawnAngleThresholdCos`.
        *   `SpawnParticleSystem`: Spawns Niagara system using `UNiagaraFunctionLibrary::SpawnSystemAtLocation` or `SpawnSystemAttached`.
    *   **`UAlsFootstepEffectsSettings` (Data Asset)**:
        *   Contains trace parameters (`SurfaceTraceChannel`, `SurfaceTraceDistance`), foot axis definitions, and a `TMap<TEnumAsByte<EPhysicalSurface>, FAlsFootstepEffectSettings> Effects`.
        *   Each `FAlsFootstepEffectSettings` entry has `Sound`, `Decal`, and `ParticleSystem` sub-structs defining assets and spawn parameters.
    *   **Always call `Super::Notify()`!**

3.  **`UAlsAnimNotify_SetGroundedEntryMode`**
    *   **Display Name**: "Als Set Grounded Entry Mode Animation Notify"
    *   **Purpose**: Sets the `GroundedEntryMode` Gameplay Tag on the `UAlsAnimationInstance`.
    *   **`UPROPERTY` Members**:
        *   **`GroundedEntryMode` (FGameplayTag)**: The tag to set (e.g., `Als.GroundedEntryMode.FromRoll`).
            *   **`EditAnywhere, BlueprintReadOnly, Category = "Settings"`**
    *   **Functional Meaning**: Used at the end of an action montage (like a roll) that transitions the character into a grounded state. This tag allows the grounded state machine in the AnimBP to play a specific entry/settle animation.
    *   **`Notify(USkeletalMeshComponent* Mesh, ...)` Logic**:
        *   Casts `Mesh->GetAnimInstance()` to `UAlsAnimationInstance`.
        *   If valid, calls `AnimationInstance->SetGroundedEntryMode(GroundedEntryMode)`.
    *   **Always call `Super::Notify()`!**

#### **How to Use Animation Notifies**

1.  **Open Animation Sequence/Montage**: In the Animation Editor.
2.  **Notifies Track**: Find the "Notifies" track below the animation timeline. If not visible, right-click on the track area -> Add Notify Track.
3.  **Add Notify**: Right-click on the desired frame in the Notifies track -> Add Notify... -> Select the ALS Notify (e.g., "Als Footstep Effects Animation Notify").
4.  **Configure Notify**: Select the newly added notify instance on the timeline. Its properties (e.g., `FootBone` for footsteps, `CameraShakeClass` for camera shake) will appear in the Details panel. Configure them as needed.
    *   **Footsteps Example**:
        *   Place `Als Footstep Effects Animation Notify` at the frame where the left foot hits the ground. Set `FootBone` to `Left`. Assign your `DA_AlsFootstepEffectsSettings` asset.
        *   Place another one for the right foot, setting `FootBone` to `Right`.
    *   **Camera Shake Example**:
        *   Place `Als Camera Shake Animation Notify` on a landing animation at the impact frame. Select a `CameraShakeClass` (e.g., `CS_LandHeavy`) and set a `CameraShakeScale`.
    *   **Grounded Entry Example**:
        *   At the end of your Roll montage, add `Als Set Grounded Entry Mode Animation Notify`. Set `GroundedEntryMode` to `Als.GroundedEntryMode.FromRoll`.
5.  **Save Asset**.

#### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

**1. Creating a New Custom AnimNotify (e.g., "SpawnImpactDecalAtLocation")**

*   **Scenario**: You want a generic notify to spawn a decal (like a bullet hole) at a specific bone/socket location when an animation hits a certain point.
*   **C++ Implementation**:
    1.  Create a new C++ class inheriting from `UAnimNotify`.
        ```cpp
        // AlsAnimNotify_SpawnDecalAtSocket.h
        #pragma once
        #include "Animation/AnimNotifies/AnimNotify.h"
        #include "AlsAnimNotify_SpawnDecalAtSocket.generated.h"

        class UMaterialInterface;

        UCLASS(DisplayName = "Als Spawn Decal at Socket")
        class ALSEDITOR_API UAlsAnimNotify_SpawnDecalAtSocket : public UAnimNotify // Or your game module if not editor
        {
            GENERATED_BODY()
        protected:
            UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
            FName SocketName; // Socket on the mesh where the decal should originate

            UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
            TSoftObjectPtr<UMaterialInterface> DecalMaterial;

            UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (AllowPreserveRatio))
            FVector DecalSize{10.0f, 10.0f, 10.0f};

            UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings", Meta = (ClampMin = 0, ForceUnits = "s"))
            float DecalLifetime{5.0f};
        public:
            UAlsAnimNotify_SpawnDecalAtSocket();
            virtual FString GetNotifyName_Implementation() const override;
            virtual void Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence, const FAnimNotifyEventReference& NotifyEventReference) override;
        };

        // AlsAnimNotify_SpawnDecalAtSocket.cpp
        #include "Kismet/GameplayStatics.h"
        #include "Materials/MaterialInterface.h"
        #include "Components/DecalComponent.h"
        #include "Components/SkeletalMeshComponent.h" // For Mesh->GetSocketLocation

        UAlsAnimNotify_SpawnDecalAtSocket::UAlsAnimNotify_SpawnDecalAtSocket() { bShouldFireInEditor = false; }
        FString UAlsAnimNotify_SpawnDecalAtSocket::GetNotifyName_Implementation() const { return TEXT("Als Spawn Decal at Socket"); }

        void UAlsAnimNotify_SpawnDecalAtSocket::Notify(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence, const FAnimNotifyEventReference& NotifyEventReference)
        {
            Super::Notify(Mesh, Sequence, NotifyEventReference); // Important!

            if (!Mesh || !DecalMaterial.LoadSynchronous()) return;

            FVector SpawnLocation = Mesh->GetSocketLocation(SocketName);
            FRotator SpawnRotation = Mesh->GetSocketRotation(SocketName); // Or calculate based on impact normal if it's a hit

            // For impact decals, you'd typically trace from socket forward and use hit location/normal
            // For simplicity here, just spawning at socket with socket rotation
            UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(Mesh->GetWorld(), DecalMaterial.Get(), DecalSize, SpawnLocation, SpawnRotation, DecalLifetime);
            // if (Decal) { /* Further setup */ }
        }
        ```
*   **Usage**: Add this notify to an animation (e.g., a weapon firing animation, place it at `SocketName`="MuzzleFlashSocket"). Configure `DecalMaterial`, `DecalSize`, etc.

**GAS Integration**:
*   **Sending Gameplay Events**: An `AnimNotify` (Blueprint or C++) can send a `GameplayEvent` to the character's `AbilitySystemComponent`.
    *   **Example**: In `AM_HeavyAttack` montage, at the impact frame, add a Blueprint AnimNotify `AN_MeleeImpact`.
    *   **`AN_MeleeImpact` Blueprint**: Get Owning Actor -> Cast to `AAlsCharacter` -> Get AbilitySystemComponent -> `Send Gameplay Event to Actor`. Event Tag: `Event.MeleeHit`. Payload: (Optional) Hit location, hit actor.
    *   **`GA_HeavyAttack`**: Use an `AbilityTask_WaitGameplayEvent` node listening for `Event.MeleeHit`. When the event is received, the ability performs damage calculation and application.
*   **Triggering Abilities**: An `AnimNotify` can call `TryActivateAbilityByTag` or `TryActivateAbilityByClass` on the character's ASC.
    *   **Example**: An `AnimNotify` "AN_ComboWindowOpen" at the end of an attack animation could attempt to activate the next attack ability in a combo chain if input is buffered.














-----------------------------------------------------------------------------------

















### **V-D. Animation Notify States**

#### **Scope**

This subsection details the custom `UAnimNotifyState` classes provided by ALS. AnimNotifyStates define a region within an animation timeline, triggering logic on `NotifyBegin`, `NotifyEnd`, and optionally `NotifyTick` (if it's a Branching Point).

#### **Purpose**

ALS AnimNotifyStates are used to:
*   Manage character states for the duration of an animation segment (e.g., setting `LocomotionAction`).
*   Allow animations to be blended out early based on gameplay conditions.
*   Modify character properties (like root motion scale) for a specific animation duration.

#### **Key Concepts/Components (AnimNotifyState Classes)**

1.  **`UAlsAnimNotifyState_EarlyBlendOut`**
    *   **Display Name**: "Als Early Blend Out Animation Notify State"
    *   **Purpose**: Allows an animation montage to be interrupted and blended out prematurely if certain character state conditions are met.
    *   **`UPROPERTY` Members**:
        *   `BlendOutDuration` (float, seconds): How long the blend out should take if interrupted.
        *   `bCheckInput` (bool): If true, montage blends out if character receives movement input.
        *   `bCheckLocomotionMode`, `LocomotionModeEquals` (FGameplayTag): If `bCheckLocomotionMode` is true, blends out if character's `LocomotionMode` becomes equal to `LocomotionModeEquals`.
        *   `bCheckRotationMode`, `RotationModeEquals` (FGameplayTag): Similar check for `RotationMode`.
        *   `bCheckStance`, `StanceEquals` (FGameplayTag): Similar check for `Stance`.
    *   **Functional Meaning**: Essential for making long actions (like "getting up" or a channeled ability played via montage) feel responsive. If the player provides input while getting up, this notify state can stop the get up montage and let the character move.
    *   **`BranchingPointNotifyTick(...)` Logic**:
        *   This function is called every frame while the notify state is active because `bIsNativeBranchingPoint = true`.
        *   It checks the configured conditions against the `AAlsCharacter`'s current state.
        *   If any condition is met, it gets the `MontageInstance` and calls `MontageInstance->Stop()` with the specified `BlendOutDuration`.
    *   **`CanBePlaced(UAnimSequenceBase* Sequence) const`**: Ensures this notify state can only be placed on `UAnimMontage` assets.
    *   **Always call `Super::BranchingPointNotifyTick()` when overriding.**

2.  **`UAlsAnimNotifyState_SetLocomotionAction`**
    *   **Display Name**: "Als Set Locomotion Action Animation Notify State"
    *   **Purpose**: Sets the `AAlsCharacter::LocomotionAction` Gameplay Tag for the duration of this notify state. Automatically clears the tag when the notify state ends.
    *   **`UPROPERTY` Members**:
        *   `LocomotionAction` (FGameplayTag): The Gameplay Tag to set on the character.
    *   **Functional Meaning**: This is a cornerstone of integrating custom actions played via montages with the ALS state system. For example, a "HeavyAttack" montage would have this notify state spanning its duration, setting `LocomotionAction` to `Als.LocomotionAction.Attack` (a custom tag you define). The `UAlsAnimationInstance` then uses this tag to blend into appropriate attacking animation states or layers.
    *   **`NotifyBegin(...)` Logic**: Gets `AAlsCharacter` and calls `Character->SetLocomotionAction(LocomotionAction)`.
    *   **`NotifyEnd(...)` Logic**: Gets `AAlsCharacter`. If `Character->GetLocomotionAction()` is still equal to *this notify's* `LocomotionAction`, it clears it by calling `Character->SetLocomotionAction(FGameplayTag::EmptyTag)`. This prevents accidentally clearing an action set by a different, overlapping notify state.
    *   **Always call `Super::NotifyBegin()` and `Super::NotifyEnd()`!**

3.  **`UAlsAnimNotifyState_SetRootMotionScale`**
    *   **Display Name**: "Als Set Root Motion Scale Animation Notify State"
    *   **Purpose**: Temporarily scales the character's root motion translation for the duration of the notify state.
    *   **`UPROPERTY` Members**:
        *   `TranslationScale` (float): The scale factor to apply to root motion translation (e.g., `0.5` for half speed, `2.0` for double speed).
    *   **Functional Meaning**: Useful for fine-tuning the distance covered by a root motion animation without re-authoring the animation itself. For example, if a roll animation covers too much distance, you can add this notify state with `TranslationScale = 0.8`.
    *   **`NotifyBegin(...)` Logic**: Gets `ACharacter` and calls `Character->SetAnimRootMotionTranslationScale(TranslationScale)`. Only applies if `LocalRole >= ROLE_AutonomousProxy`.
    *   **`NotifyEnd(...)` Logic**: Gets `ACharacter`. If the current scale still matches `TranslationScale`, it resets it to `1.0f`. Warns if the scale was changed by something else externally.
    *   **Always call `Super::NotifyBegin()` and `Super::NotifyEnd()`!**

#### **How to Use Animation Notify States**

1.  **Open Animation Sequence/Montage**.
2.  **Notifies Track**: Right-click -> Add Notify State... -> Select the ALS Notify State (e.g., "Als Set Locomotion Action Animation Notify State").
3.  **Adjust Duration**: Drag the ends of the notify state instance on the timeline to cover the desired animation segment.
4.  **Configure**: Select the notify state instance. Its properties will appear in the Details panel.
    *   **Set Locomotion Action Example**:
        *   On your `AM_Roll` montage, add `Als Set Locomotion Action`. Set its `LocomotionAction` property to `Als.LocomotionAction.Rolling`. Drag its duration to cover the entire roll.
    *   **Early Blend Out Example**:
        *   On your `AM_GetUpFromBack` montage, add `Als Early Blend Out`. Configure its conditions (e.g., `bCheckInput = true`). If the player moves while getting up, the montage will stop.
    *   **Root Motion Scale Example**:
        *   On an attack montage that moves too far, add `Als Set Root Motion Scale`. Set `TranslationScale` to `0.75`.
5.  **Save Asset**.

#### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

**1. Creating a New Custom AnimNotifyState (e.g., "ApplyGameplayEffectForDuration")**

*   **Scenario**: You want to apply a `GameplayEffect` (like a temporary speed boost or damage resistance) for the duration of a specific part of an animation.
*   **C++ Implementation**:
    1.  Create a new C++ class inheriting from `UAnimNotifyState`.
        ```cpp
        // AlsAnimNotifyState_ApplyEffect.h
        #pragma once
        #include "Animation/AnimNotifies/AnimNotifyState.h"
        #include "GameplayEffect.h" // For TSubclassOf<UGameplayEffect>
        #include "AlsAnimNotifyState_ApplyEffect.generated.h"

        UCLASS(DisplayName = "Als Apply GameplayEffect for Duration")
        class ALSEDITOR_API UAlsAnimNotifyState_ApplyEffect : public UAnimNotifyState
        {
            GENERATED_BODY()
        protected:
            UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
            TSubclassOf<UGameplayEffect> EffectToApply;

            // Store the active GE handle to remove it correctly
            UPROPERTY(Transient)
            FActiveGameplayEffectHandle AppliedEffectHandle;
        public:
            UAlsAnimNotifyState_ApplyEffect();
            virtual FString GetNotifyName_Implementation() const override;
            virtual void NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence, float TotalDuration, const FAnimNotifyEventReference& NotifyEventReference) override;
            virtual void NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence, const FAnimNotifyEventReference& NotifyEventReference) override;
        };

        // AlsAnimNotifyState_ApplyEffect.cpp
        #include "AbilitySystemComponent.h"
        #include "AlsCharacter.h" // Or your base character that has ASC

        UAlsAnimNotifyState_ApplyEffect::UAlsAnimNotifyState_ApplyEffect() {}
        FString UAlsAnimNotifyState_ApplyEffect::GetNotifyName_Implementation() const { return TEXT("Als Apply GE for Duration"); }

        void UAlsAnimNotifyState_ApplyEffect::NotifyBegin(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence, float TotalDuration, const FAnimNotifyEventReference& NotifyEventReference)
        {
            Super::NotifyBegin(Mesh, Sequence, TotalDuration, NotifyEventReference);
            if (!EffectToApply || !Mesh || !Mesh->GetOwner()) return;

            AActor* OwnerActor = Mesh->GetOwner();
            UAbilitySystemComponent* ASC = OwnerActor->FindComponentByClass<UAbilitySystemComponent>();
            // AAlsCharacter* AlsChar = Cast<AAlsCharacter>(OwnerActor); // If ASC is on AAlsCharacter
            // UAbilitySystemComponent* ASC = AlsChar ? AlsChar->GetAbilitySystemComponent() : nullptr;


            if (ASC)
            {
                FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
                ContextHandle.AddSourceObject(OwnerActor); // Optional: set source object
                FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectToApply, 1.0f, ContextHandle);

                if (SpecHandle.IsValid())
                {
                    AppliedEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
                }
            }
        }

        void UAlsAnimNotifyState_ApplyEffect::NotifyEnd(USkeletalMeshComponent* Mesh, UAnimSequenceBase* Sequence, const FAnimNotifyEventReference& NotifyEventReference)
        {
            Super::NotifyEnd(Mesh, Sequence, NotifyEventReference);
            if (!Mesh || !Mesh->GetOwner() || !AppliedEffectHandle.IsValid()) return;

            AActor* OwnerActor = Mesh->GetOwner();
            UAbilitySystemComponent* ASC = OwnerActor->FindComponentByClass<UAbilitySystemComponent>();
            // AAlsCharacter* AlsChar = Cast<AAlsCharacter>(OwnerActor);
            // UAbilitySystemComponent* ASC = AlsChar ? AlsChar->GetAbilitySystemComponent() : nullptr;

            if (ASC)
            {
                ASC->RemoveActiveGameplayEffect(AppliedEffectHandle);
            }
            AppliedEffectHandle.Invalidate(); // Clear the handle
        }
        ```
*   **Usage**: Place this notify state on a montage (e.g., a "power-up" animation). Select your `GameplayEffect` (e.g., `GE_SpeedBoost`) in the notify's details. For the duration of the notify state, the character will have the speed boost.

**GAS Integration**:
*   **`UAlsAnimNotifyState_SetLocomotionAction`**: As described, this is key for GAS abilities that play montages to correctly inform the ALS animation system about the character's action state. A `GA_Attack` playing `AM_Attack` would rely on this notify within `AM_Attack` to set `LocomotionAction = Als.LocomotionAction.Attack`.
*   **`UAlsAnimNotifyState_EarlyBlendOut`**: GAS abilities playing long montages (e.g., a channeled spell) can use this. If the ability is cancelled (e.g., by `AbilitySystemComponent->CancelAbility`), the montage might stop. If the character is stunned (another ability applies a "Stun" tag), this notify state can check for the "Stun" tag via `bCheckCharacterTags` (a hypothetical extension to the notify) and blend out the channeled spell animation.
*   **Custom Notify States for GAS**: Your custom notify states (like `AlsAnimNotifyState_ApplyEffect`) are powerful tools for abilities to manage timed effects that are perfectly synchronized with animation segments.










-------------------------------------------------------------------------------










### **V-E. Control Rig / RigVM Units**

#### **Scope**

This subsection details the custom Control Rig units (`FRigUnit_...`) provided by ALS and how they are used within a Control Rig graph (typically `CR_Als_Character`) to achieve procedural animation adjustments, primarily for Foot IK and Hand IK Retargeting.

#### **Purpose**

Control Rig allows for graph-based procedural animation and rigging directly within Unreal Engine. ALS leverages it for:
*   **Dynamic Foot Placement (IK)**: Adjusting foot and leg bone transforms to make feet conform to uneven terrain.
*   **Hand IK Retargeting**: Adjusting hand positions to correctly align with props or weapon attachments, especially when animations might not perfectly match.
*   **Procedural Spine/Pelvis Adjustments**: Modifying spine and pelvis bones for aiming or other procedural effects.
*   **Extensibility**: Providing a framework where users can add more complex procedural animation behaviors.

#### **Key Concepts/Components (RigVM Units & Control Rig Graph)**

1.  **Control Rig Asset (`CR_Als_Character`)**:
    *   This is an asset in your project that defines a Control Rig graph. It's assigned to the character's Skeletal Mesh Component.
    *   The AnimGraph in `ABP_AlsCharacter` has a "Control Rig" node that executes this graph, feeding it pose data and input parameters.

2.  **`FAlsControlRigInput` (USTRUCT in `UAlsAnimationInstance`)**:
    *   **Purpose**: This struct is prepared by `UAlsAnimationInstance::GetControlRigInput()` and passed as the primary input to the `CR_Als_Character` graph.
    *   **Key Members**: `bUseHandIkBones`, `bUseFootIkBones`, `bFootOffsetAllowed`, `VelocityBlendForwardAmount`, `VelocityBlendBackwardAmount`, `FootLeftLocation`, `FootLeftRotation`, `FootRightLocation`, `FootRightRotation`, `SpineYawAngle`.
    *   **Functional Meaning**: It provides Control Rig with all the necessary real-time data from the animation instance to perform its calculations (e.g., where the animation *wants* the feet to be, whether IK is allowed, how much the spine should twist).

3.  **ALS RigVM Units (C++ Structs deriving from `FRigUnitMutable` or `FRigUnit`)**:
    These are the custom nodes you can use within the Control Rig editor.

    *   **`FAlsRigUnit_FootOffsetTrace`**:
        *   **Display Name**: "Foot Offset Trace"
        *   **Purpose**: Performs a line trace downwards from the animated foot position to find the actual ground geometry.
        *   **Inputs**: `FootTargetLocation` (from `FAlsControlRigInput`), `TraceChannel`, `TraceDistanceUpward`/`Downward`, `WalkableFloorAngle`, `FootHeight`, `bEnabled`, `bDrawDebug`.
        *   **Outputs**: `OffsetLocationZ` (the vertical distance needed to move the foot to hit the ground), `OffsetNormal` (the surface normal at the hit point).
        *   **Functional Meaning**: This is the "sensing" part of foot IK. It figures out where the ground is relative to the animated foot.

    *   **`FAlsRigUnit_ApplyFootOffsetLocation`**:
        *   **Display Name**: "Apply Foot Offset Location"
        *   **Purpose**: Takes the `OffsetLocationZ` from the trace and other parameters to calculate and apply a smoothed and constrained vertical offset to the foot bone's location.
        *   **Inputs**: `PelvisItem`, `ThighItem` (for leg length/stretch calculations), `FootTargetLocation`, `FootOffsetLocationZ` (from trace), `PelvisOffset` (how much pelvis IK has lowered the pelvis), `LegLength`, `MinPelvisToFootDistanceZ`, `MaxLegStretchRatio`, interpolation parameters (`OffsetInterpolationFrequency`, etc.).
        *   **Output**: `FootLocation` (the new, IK-adjusted foot location).
        *   **FunctionalMeaning**: This unit applies the vertical correction. The spring dynamics make the foot settle naturally. Constraints prevent the leg from overstretching or the foot from going too high relative to the pelvis. It ensures the foot bone is placed at the correct height.

    *   **`FAlsRigUnit_ApplyFootOffsetRotation`**:
        *   **Display Name**: "Apply Foot Offset Rotation"
        *   **Purpose**: Takes the `OffsetNormal` from the trace and adjusts the foot bone's rotation to align with the ground slope, respecting anatomical joint limits.
        *   **Inputs**: `CalfItem` (for local space calculations), `FootTargetRotation` (from `FAlsControlRigInput`), `FootOffsetNormal` (from trace), `LimitOffset` (a base offset for the foot, e.g., slight toe-up), `Swing1/2LimitAngle`, `TwistLimitAngle`, `OffsetInterpolationSpeed`.
        *   **Output**: `FootRotation` (the new, IK-adjusted foot rotation).
        *   **Functional Meaning**: This unit orients the foot to match the ground. The swing/twist limits prevent the ankle from bending in unnatural ways.

    *   **`FAlsRigUnit_HandIkRetargeting`**:
        *   **Display Name**: "Hand Ik Retargeting"
        *   **Purpose**: Adjusts the positions of a set of "ItemsToMove" (typically spine bones, clavicles, and the IK hand itself) based on the difference between an animated hand position and a target IK hand position. It can favor either the left or right hand's IK target.
        *   **Inputs**: `LeftHandItem`, `LeftHandIkItem`, `RightHandItem`, `RightHandIkItem`, `ItemsToMove` (array of bones), `RetargetingWeight` (0 for left, 1 for right, 0.5 for average), `Weight` (overall alpha), `bPropagateToChildren`.
        *   **Functional Meaning**: Useful for two-handed weapon holding. If the weapon is an attachment and has `ik_hand_gun` sockets, this unit can adjust the character's arms and spine so the animated hands (`Left/RightHandItem`) align better with these IK sockets (`Left/RightHandIkItem`), making the character grip the weapon correctly even if the base animation isn't perfectly matched.
        *   **Logic**: Calculates an `RetargetingOffset` vector (the difference between where the animation *places* the hand(s) and where the IK target *wants* them to be). This offset is then applied to all `ItemsToMove`.

    *   **`FAlsRigUnit_ChainLength`**:
        *   **Display Name**: "Chain Length"
        *   **Purpose**: Calculates the current or initial length of a kinematic chain between two bones (Ancestor and Descendant).
        *   **Functional Meaning**: Can be used for things like leg length calculation (for `FAlsRigUnit_ApplyFootOffsetLocation`) or for other procedural effects that depend on bone chain distances.

    *   **`FAlsRigUnit_DistributeRotationSimple`**:
        *   **Display Name**: "Distribute Rotation Simple"
        *   **Purpose**: Takes an input `Rotation` (FQuat) and distributes it evenly among an array of `Items` (bones).
        *   **Functional Meaning**: Can be used to procedurally twist a chain of spine bones. For example, if `FAlsControlRigInput::SpineYawAngle` is 30 degrees, this unit could apply 10 degrees of yaw to `spine_01`, `spine_02`, and `spine_03`.

    *   **General RigVM Functions (`AlsRigUnits.h`)**:
        *   `FAlsRigVMFunction_Clamp01Float`: Clamps a float.
        *   `FAlsRigVMFunction_ExponentialDecayVector` / `...Quaternion`: Smoothly interpolates vectors/quaternions.
        *   `FAlsRigUnit_CalculatePoleVector`: Calculates IK pole vector.
        *   `FAlsRigVMFunction_IsGameWorld`: A control flow node that branches execution based on whether the rig is running in a game world or editor preview. Useful for enabling/disabling complex IK only in-game.

#### **How to Use Control Rig and ALS RigVM Units**

1.  **Assign Control Rig Asset**:
    *   Select your character's `SkeletalMeshComponent`.
    *   In the Details panel, under "Animation -> Control Rig", assign your `CR_Als_Character` (or custom derived) asset to the `Control Rig Class` slot.
2.  **AnimGraph Setup (`ABP_AlsCharacter`)**:
    *   In the AnimGraph, after your main state machine and layering logic, add a "Control Rig" node.
    *   Connect the final character pose to its "In Pose" pin.
    *   For the input pins of the "Control Rig" node (which correspond to the "Input" variables defined in your `CR_Als_Character` graph's "RigVM" graph):
        *   Get a reference to your `UAlsAnimationInstance`.
        *   Call `GetControlRigInput()`.
        *   Break the `FAlsControlRigInput` struct and connect its members to the corresponding pins on the "ControlRig" node (e.g., `FootLeftLocation` from struct to `FootLeftLocation` pin on node).
3.  **Control Rig Graph (`CR_Als_Character`)**:
    *   Open your `CR_Als_Character` asset.
    *   Go to the "RigVM" graph.
    *   **Input Variables**: Create input variables that match the members of `FAlsControlRigInput` (e.g., `FootLeftLocation` of type Vector, `SpineYawAngle` of type Float). Mark them as "Input" in their details. These will appear as pins on the "Control Rig" node in the AnimBP.
    *   **Foot IK Example Flow (for one foot, e.g., Left)**:
        1.  Get `FootLeftLocation` and `FootLeftRotation` from the input variables (which came from `FAlsControlRigInput`).
        2.  Use `AlsFootOffsetTrace`:
            *   Input `FootTargetLocation`: from `FootLeftLocation` input.
            *   Configure other trace params (channels, distances, `FootHeight`).
            *   `bEnabled`: from `FAlsControlRigInput.bFootOffsetAllowed` AND `FAlsControlRigInput.bUseFootIkBones`.
        3.  Use `AlsApplyFootOffsetLocation`:
            *   Input `FootTargetLocation`: from `FootLeftLocation` input.
            *   Input `FootOffsetLocationZ`: from `AlsFootOffsetTrace.OffsetLocationZ`.
            *   Input `PelvisOffset`: (This is more complex, often comes from a separate pelvis IK adjustment in Control Rig that tries to keep pelvis level or lower it based on foot positions).
            *   Configure `LegLength`, limits, interpolation.
        4.  Use `AlsApplyFootOffsetRotation`:
            *   Input `FootTargetRotation`: from `FootLeftRotation` input.
            *   Input `FootOffsetNormal`: from `AlsFootOffsetTrace.OffsetNormal`.
            *   Configure limits, interpolation.
        5.  **Set Bone Transforms**: Use "Set Transform" nodes (or "Set Translation", "Set Rotation") to apply the `FootLocation` (from `AlsApplyFootOffsetLocation`) and `FootRotation` (from `AlsApplyFootOffsetRotation`) to the actual foot IK bone (e.g., `ik_foot_l`). Ensure "Propagate to Children" is usually true for IK effectors.
    *   **Spine Rotation Example Flow**:
        1.  Get `SpineYawAngle` from input variables.
        2.  Create an `FQuat` from this yaw angle (e.g., `MakeQuatFromEuler` with Yaw set, Pitch/Roll 0).
        3.  Use `AlsDistributeRotationSimple`:
            *   Input `Rotation`: The FQuat from step 2.
            *   Input `Items`: An array of bone keys for your spine bones (e.g., `spine_01`, `spine_02`, `spine_03`).
            *   This will apply a portion of the `SpineYawAngle` to each spine bone.

#### **Adding/Adapting Functionality (C++ & Blueprint Examples)**

**1. Customizing Foot IK Behavior (e.g., Different `FootHeight` for Water Wading)**

*   **Scenario**: When the character is wading in shallow water, you want their feet to appear slightly submerged, meaning the IK should target a point *below* the actual water surface.
*   **A. Add State Detection**:
    *   `AAlsCharacter`: Add a `bIsWadingInWater` (bool, replicated) or a Gameplay Tag `State.Wading`.
    *   Update this state based on physics overlaps or volume detection.
*   **B. Pass State to Control Rig**:
    *   `UAlsAnimationInstance`: Add `bIsWading` to `FAlsControlRigInput`. Update it in `GetControlRigInput()` from `Character->bIsWadingInWater`.
*   **C. Modify Control Rig Graph (`CR_Als_Character`)**:
    *   Add an `IsWading` (bool) input variable.
    *   Before the `AlsFootOffsetTrace` node:
        ```rigVM
        // Get original FootHeight from a variable or direct input
        RigVM.CurrentFootHeight = RigVM.DefaultFootHeight;
        Branch (Condition = RigVM.Input.IsWading)
        {
            True: RigVM.CurrentFootHeight = RigVM.DefaultFootHeight - RigVM.WadingDepthOffset; // e.g., WadingDepthOffset = 5.0 cm
            False: // CurrentFootHeight remains DefaultFootHeight
        }
        // Connect RigVM.CurrentFootHeight to the FootHeight pin of AlsFootOffsetTrace
        ```

**2. Adding a New Procedural Adjustment (e.g., Head Look-At using Control Rig IK)**

*   **Scenario**: You want the character's head to procedurally look at a target point using Control Rig's IK capabilities, blending with the base animation.
*   **A. Define Target in `AAlsCharacter`**:
    ```cpp
    // AAlsCharacter.h
    // UPROPERTY(BlueprintReadWrite, Replicated) FVector LookAtTargetWorldLocation;
    // UPROPERTY(BlueprintReadWrite, Replicated) float HeadLookAtAlpha; // 0-1
    ```
*   **B. Pass to Control Rig via `UAlsAnimationInstance`**:
    *   Add `LookAtTargetLocation` (Vector) and `HeadLookAtAlpha` (float) to `FAlsControlRigInput`.
    *   Populate them in `UAlsAnimationInstance::GetControlRigInput()`.
*   **C. Control Rig Graph (`CR_Als_Character`)**:
    1.  Add `LookAtTargetLocation` and `HeadLookAtAlpha` input variables.
    2.  After all other spine/body adjustments, get the current global transform of the `head` bone.
    3.  Use a "Aim" constraint node (or "Aim Math" -> "Set Rotation") or a "Basic IK" node for the head:
        *   **Bone**: `head`
        *   **Target Location**: `Input.LookAtTargetLocation`
        *   **Effector/Primary Axis**: The head bone's forward axis (e.g., X+).
        *   **Pole Vector/Up Axis**: (Optional, for more stability) Could use character's up vector.
    4.  Get the *resulting* head transform from the IK/Aim node.
    5.  Use a "Blend Transform" node (or "Lerp Transform"):
        *   **A**: Original animated head transform (from before IK).
        *   **B**: IK-solved head transform.
        *   **Alpha**: `Input.HeadLookAtAlpha`.
    6.  Use "Set Transform" to apply the blended transform to the `head` bone.
*   **GAS Integration**:
    *   A `GameplayAbility` (`GA_FocusOnTarget`) could:
        *   Identify a target actor.
        *   Every tick (using an `AbilityTask_Tick`), update `Character->LookAtTargetWorldLocation` to the target actor's location.
        *   Set `Character->HeadLookAtAlpha` to `1.0`.
    *   When the ability ends, set `Character->HeadLookAtAlpha` to `0.0`.

#### **Network Synchronization**

*   **Control Rig Executes Locally**: The Control Rig graph runs on the server and on each client independently.
*   **Input Synchronization**: The consistency of Control Rig's output relies on its inputs from `FAlsControlRigInput` being reasonably synchronized. `UAlsAnimationInstance` calculates this struct based on character state, which *is* replicated (`AAlsCharacter` tags, transform) or derived from replicated animation states.
*   **No Direct Replication of Rig Outputs**: The bone transforms modified by Control Rig are not typically replicated bone by bone. Instead, the overall character animation and state that *leads* to those Control Rig inputs is what's synchronized.
*   **Visual Consistency**: Because the inputs and the Control Rig graph itself are deterministic, all clients should see very similar procedural adjustments, leading to visually consistent foot IK, hand placement, etc. Minor deviations are possible due to floating-point precision and varying `DeltaTime` on clients if not perfectly synced, but usually not noticeable.

#### **Debugging**

*   **Control Rig Editor & Debugger**: This is your primary tool.
    *   **Graph Execution**: Set breakpoints on RigVM nodes, step through, and inspect input/output values of each pin.
    *   **Viewport Preview**: Observe the character preview in the Control Rig editor as you make changes or scrub the timeline.
    *   **Debug Drawing**: Many RigVM nodes (especially IK and constraint nodes) have options to draw debug visualizations in the preview viewport. You can also use "Draw Transform" or "Draw Point/Line/Sphere" nodes in your graph for custom debug visuals.
*   **Animation Debugger (In PIE)**:
    *   Select your character. In the Animation Debugger, navigate to the "Control Rig" tab.
    *   You can see a list of all executable nodes and inspect their "cooked" (runtime) values for all exposed pins.
    *   This is invaluable for seeing what data your `ABP_AlsCharacter` is actually feeding into the "Control Rig" node.
*   **`UAlsAnimationInstance` Details Panel**: Check the `GetControlRigInput()` result or the `FAlsControlRigInput` member (if you cache it) to see what data is being prepared for Control Rig.
*   **Visual Inspection in Game**:
    *   *Feet floating/clipping?* Check `AlsFootOffsetTrace` outputs, `AlsApplyFootOffsetLocation` constraints, and `FootHeight` parameter.
    *   *Hands not aligned with weapon?* Check `AlsRigUnit_HandIkRetargeting` inputs (animated hand positions vs. IK target socket positions) and the `RetargetingWeight`.
    *   *Spine not twisting correctly for aim?* Check the input `SpineYawAngle` and the logic in `AlsDistributeRotationSimple`.
















--------------------------------------------------------------------------------------














## **IV. Core Utilities (Continued)**

### **F. Logging (`AlsLog`)**

#### **Scope**

This subsection covers the custom logging setup for the Advanced Locomotion System, specifically the `LogAls` category and its integration with the Unreal Engine editor's Message Log.

#### **Purpose**

The ALS logging system provides a dedicated channel for outputting informational messages, warnings, and errors generated by the ALS codebase. This offers several advantages:
*   **Clarity**: Separates ALS-specific logs from general engine or game logs, making it easier to find relevant information.
*   **Filtering**: Allows developers to filter the Output Log specifically for `LogAls` messages.
*   **Verbosity Control**: The verbosity of `LogAls` can be adjusted independently.
*   **Editor Integration**: Critical warnings and errors can be directed to the "ALS" tab in the editor's Message Log window, providing a more prominent and persistent notification than just the Output Log.

#### **Key Concepts/Components**

1.  **Log Category Definition**:
    *   **`AlsLog.h`**: `ALS_API DECLARE_LOG_CATEGORY_EXTERN(LogAls, Log, All)`
        *   Declares the `LogAls` category so it can be used throughout the ALS module and by other modules that depend on ALS.
        *   `Log`: Default runtime verbosity (can be changed via console commands or config files).
        *   `All`: Compile-time maximum verbosity (ensures all log levels are compiled in, even if not active at runtime).
    *   **`AlsLog.cpp`**: `DEFINE_LOG_CATEGORY(LogAls)`
        *   Actually defines the log category instance.

2.  **Message Log Name**:
    *   **`AlsLog.cpp`**: `const FName AlsLog::MessageLogName{TEXTVIEW("Als")};`
    *   **Significance**: This `FName` is used to register a custom tab in the Unreal Editor's "Message Log" window (Window -> Developer Tools -> Message Log).

3.  **Module Integration (`FALSModule` in `ALSModule.cpp`)**:
    *   **`FALSModule::StartupModule()`**:
        *   `FMessageLogModule& MessageLog = FModuleManager::LoadModuleChecked<FMessageLogModule>(FName(TEXTVIEW("MessageLog")));`
        *   `MessageLog.RegisterLogListing(AlsLog::MessageLogName, LOCTEXT("MessageLogLabel", "ALS"), MessageLogOptions);`
        *   This code, executed when the ALS module starts up in the editor, creates the "ALS" tab in the Message Log window. `MessageLogOptions` can configure if it shows filters, allows clearing, etc.

#### **Methods & Functions (How to Use Logging)**

*   **Standard `UE_LOG` Macro**:
    *   **Syntax**: `UE_LOG(LogCategory, Verbosity, Format, ...Args)`
    *   **Usage with `LogAls`**:
        ```cpp
        #include "Utility/AlsLog.h" // Ensure this is included

        // Informational message
        UE_LOG(LogAls, Log, TEXT("Character %s initialized successfully."), *MyCharacter->GetName());

        // Warning message
        UE_LOG(LogAls, Warning, TEXT("Potential issue: Foot IK trace failed for %s."), *FootBoneName.ToString());

        // Error message
        UE_LOG(LogAls, Error, TEXT("Critical error: AlsCharacterSettings asset is missing!"));
        ```
    *   **Verbosity Levels**: `Fatal`, `Error`, `Warning`, `Display`, `Log`, `Verbose`, `VeryVerbose`.

*   **Sending Messages to the Editor's "ALS" Message Log Tab**:
    *   This is typically used for more persistent warnings or errors that the developer should be clearly notified about.
    ```cpp
    #include "Utility/AlsLog.h"
    #include "Logging/MessageLog.h" // Required for FMessageLog
    #include "Misc/UObjectToken.h"   // Optional: To create clickable links to UObjects in the log

    void AMyAlsRelatedActor::SomeImportantCheck()
    {
        if (CrucialSetting == nullptr)
        {
            // Create a message log writer targeting the "Als" log
            FMessageLog MessageLogWriter(AlsLog::MessageLogName);

            // Construct the message text (using FText for potential localization)
            FText WarningMessage = FText::Format(
                NSLOCTEXT("MyAlsRelatedActor", "MissingCrucialSettingWarning", "CrucialSetting is not set for {0}! This might cause issues."),
                FText::FromString(this->GetName())
            );

            // Add the message as a warning
            TSharedRef<FTokenizedMessage> MessageToken = MessageLogWriter.Warning(WarningMessage);

            // Optional: Add a token that allows clicking the message to select the actor in the editor
            MessageToken->AddToken(FUObjectToken::Create(this));

            // Notify/Open the message log. Notify just adds it, Open forces the window to pop up.
            MessageLogWriter.Notify();
            // Or for more severe issues: MessageLogWriter.Open(EMessageSeverity::Warning);
        }
    }
    ```

#### **Adding/Adapting Functionality**

*   **Adding Log Messages**: The primary way to "adapt" this is to add more `UE_LOG(LogAls, ...)` statements throughout your ALS-derived C++ code or new ALS-related systems to aid in debugging and understanding runtime behavior.
*   **Custom Log Categories**: If you build a significant subsystem on top of ALS, you might consider defining your own log category (e.g., `LogMyAlsPlugin`) for even finer-grained filtering, following the same pattern as `LogAls`.
*   **GAS Integration**:
    *   **C++ Gameplay Abilities/Tasks**: Your custom C++ GAS components can use `UE_LOG(LogAls, ...)` to log information about ability activation, effects being applied, or interactions with ALS states.
        ```cpp
        // In a UGameplayAbility C++ class
        #include "Utility/AlsLog.h"

        void UMyAlsGasAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
        {
            Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
            UE_LOG(LogAls, Log, TEXT("Ability %s activated for %s."), *GetName(), *ActorInfo->AvatarActor->GetName());
            // ... ability logic ...
        }
        ```
    *   This helps trace how GAS is interacting with or driving ALS behaviors.

#### **Network Synchronization**

*   Logging itself is a local client/server operation. `UE_LOG` messages will appear in the Output Log of the machine where the code is executing.
*   When debugging multiplayer issues, it's common to check logs on both the server and the relevant clients to compare states and event timings.
*   Prefixing log messages with `UWorld::GetNetMode()` or `Actor->GetLocalRole()` can be very helpful to distinguish server logs from client logs.
    ```cpp
    // Example:
    // FString RoleString = UEnum::GetValueAsString(GetLocalRole());
    // UE_LOG(LogAls, Log, TEXT("[%s] My log message on actor %s"), *RoleString, *GetName());
    ```

#### **Debugging**

*   **Output Log**: The primary window in the Unreal Editor (Window -> Developer Tools -> Output Log) to view `UE_LOG` messages.
    *   **Filtering**: Use the "Categories" filter in the Output Log to show only `LogAls` messages.
*   **Message Log Window**: (Window -> Developer Tools -> Message Log). Select the "ALS" tab to see messages sent via `FMessageLog(AlsLog::MessageLogName)`.
*   **Console Commands for Log Verbosity**:
    *   `Log LogAls All`: Sets `LogAls` to display all messages up to `VeryVerbose`.
    *   `Log LogAls Warning`: Sets `LogAls` to display only `Warning`, `Error`, and `Fatal` messages.
    *   `Log LogAls None`: Disables `LogAls` output.
    These can be typed into the in-game console or editor console.

---

### **G. Macros (`AlsMacros.h`)**

#### **Scope**

This subsection details the custom C++ preprocessor macros defined in `AlsMacros.h`, focusing on the `ALS_ENSURE` family of macros.

#### **Purpose**

The `ALS_ENSURE` macros provide a lightweight, developer-friendly assertion mechanism. They are designed to:
*   Verify assumptions in the code during development.
*   Alert developers to unexpected conditions or states without necessarily crashing the editor or game (unlike `check()`).
*   Reduce log spam by typically firing only once per ensure site.
*   Offer a slightly more performant alternative to the full `ensure()` macro in some contexts, as they don't generate a C++ call stack by default.

#### **Key Concepts/Components**

1.  **`ALS_ENSURE(Expression)`**:
    *   Checks if `Expression` is true. If false, logs an error/warning and may break into the debugger. Fires once by default.
2.  **`ALS_ENSURE_MESSAGE(Expression, Format, ...)`**:
    *   Same as `ALS_ENSURE`, but allows a custom formatted message to be logged.
3.  **`ALS_ENSURE_ALWAYS(Expression)`**:
    *   Checks `Expression`. If false, logs and may break. *Always* fires if the condition is false and the cvar `core.EnsureAlwaysEnabled` is true, regardless of previous executions at that site.
4.  **`ALS_ENSURE_ALWAYS_MESSAGE(Expression, Format, ...)`**:
    *   Same as `ALS_ENSURE_ALWAYS`, but with a custom message.

*   **Internal Logic (`AlsEnsure::Execute`)**:
    *   A `static std::atomic<bool> bExecuted` is used at each macro call site to track if it has already fired.
    *   It respects `FPlatformMisc::IsEnsureAllowed()`.
    *   It checks the `core.EnsureAlwaysEnabled` console variable for `_ALWAYS` versions.
    *   If it determines it should fire, it logs using `LogOutputDevice` (which routes to `UE_LOG`) and attempts to `PLATFORM_BREAK()` if a debugger is present.
    *   It does *not* use the standard engine ensure handler that generates crash reports by default.

*   **`ALS_GET_TYPE_STRING(Type)`**:
    *   A utility macro (likely used internally or for other debugging) to get the string representation of a C++ type name.

#### **Methods & Functions (How to Use Macros)**

These are preprocessor macros, used directly in C++ code.

```cpp
#include "Utility/AlsMacros.h"
#include "AlsCharacter.h" // For example

void AMyAlsLogicComponent::ProcessCharacter(AAlsCharacter* Character)
{
    // Ensure the character pointer is valid before using it.
    if (!ALS_ENSURE_MESSAGE(IsValid(Character), TEXT("ProcessCharacter called with an invalid Character pointer!")))
    {
        return; // Don't proceed if ensure failed
    }

    // Example: Ensure character is in a specific state
    const float Speed = Character->GetLocomotionState().Speed;
    ALS_ENSURE_MESSAGE(Speed >= 0.0f, TEXT("Character speed %f is negative! This should not happen."), Speed);

    // For a condition that should *always* be checked if problematic, even if it fired before:
    // This might be for a very critical invariant that, if broken repeatedly, needs constant attention.
    // bool bCriticalInvariant = CheckMyCriticalThing();
    // ALS_ENSURE_ALWAYS_MESSAGE(bCriticalInvariant, TEXT("Critical invariant failed!"));
}
```

#### **Adding/Adapting Functionality**

*   **Primary Use**: Integrate these macros into your custom C++ code that extends or interacts with ALS. Use them for validating preconditions, postconditions, and invariants in your functions.
*   **Choosing `ALS_ENSURE` vs. `ensure()` vs. `check()`**:
    *   `check()`: For conditions that *must absolutely never* be false. If they are, it's a critical programming error, and the program should crash (even in shipping for some checks).
    *   `ensure()`: For conditions that *should* always be true, but if false, might not be fatal to the program. Generates a call stack and potentially a crash report. Can be spammy if the same ensure fails repeatedly.
    *   `ALS_ENSURE()`: For conditions similar to `ensure()`, but where you want less log spam (fires once by default), no automatic crash report, and potentially slightly better performance due to no call stack generation. Good for common checks in frequently called code.
*   **GAS Integration**:
    *   When writing C++ `UGameplayAbility` or `UGameplayAbilityTask` classes that interact with `AAlsCharacter` or `UAlsAnimationInstance`, use `ALS_ENSURE` to validate assumptions about the character's state or the validity of pointers.
    ```cpp
    // In a Gameplay Ability Task
    // UMyAlsAbilityTask_WaitForGait.h
    // AAlsCharacter* AlsCharacterOwner;

    // UMyAlsAbilityTask_WaitForGait.cpp
    // void UMyAlsAbilityTask_WaitForGait::TickTask(float DeltaTime)
    // {
    //    if (!ALS_ENSURE_MESSAGE(IsValid(AlsCharacterOwner), TEXT("AlsCharacterOwner became invalid during task %s"), *GetName()))
    //    {
    //        EndTask();
    //        return;
    //    }
    //    if (AlsCharacterOwner->GetGait() == TargetGait)
    //    {
    //        OnGaitReached.Broadcast();
    //        EndTask();
    //    }
    // }
    ```

#### **Network Synchronization**

*   Macros are compile-time constructs and have no direct network synchronization aspect. They execute on the machine where the code is running (client or server).
*   Ensure checks related to replicated data should consider that clients might have slightly stale data.

#### **Debugging**

*   **Debugger Breakpoint**: If an `ALS_ENSURE` fails and a debugger is attached, `PLATFORM_BREAK()` will trigger a breakpoint at the location of the failed ensure, allowing you to inspect the call stack (in your IDE) and variable values.
*   **Output Log**: Messages from failed `ALS_ENSURE` macros (including custom messages) are printed to the Output Log using `LogOutputDevice`, typically appearing as `Error` or `Warning` severity prefixed with "Ensure failed:".
*   **Console Variable `core.EnsureAlwaysEnabled`**:
    *   `core.EnsureAlwaysEnabled 0` (default): `ALS_ENSURE` fires once. `ALS_ENSURE_ALWAYS` fires once.
    *   `core.EnsureAlwaysEnabled 1`: `ALS_ENSURE` fires once. `ALS_ENSURE_ALWAYS` fires *every time* its condition is false. Useful for repeatedly hitting a problematic ensure during a debugging session.

---

### **H. Enum Utilities (`AlsEnumUtility.h`)**

#### **Scope**

This subsection covers the utility functions within the `AlsEnumUtility` namespace, designed to assist with common operations related to C++ `enum class` types that are registered with Unreal Engine's reflection system (UEnums).

#### **Purpose**

To provide convenient, type-safe helper functions for:
*   Converting an enum value to its integer index within the `UEnum` definition.
*   Retrieving the string name of an enum value.
These are primarily useful for debugging, logging, or when interfacing with systems that might require string or integer representations of enum values.

#### **Key Concepts/Components**

The utilities are template functions, meaning they can work with any `enum class` that has been exposed to Unreal's reflection system (e.g., by using `UENUM(BlueprintType)`).

#### **Methods & Functions (Templated in `AlsEnumUtility` namespace)**

1.  **`template <typename EnumType> int32 GetIndexByValue(const EnumType Value)`**
    *   **Purpose**: Given an enum value, this function returns its integer index as defined in the `UEnum` metadata.
    *   **Parameters**: `Value` (an instance of the `EnumType`).
    *   **Return Value**: `int32` - The index of the enum value.
    *   **Functional Meaning**: For an enum like:
        ```cpp
        UENUM(BlueprintType)
        enum class EMyEnum : uint8 { ValueA, ValueB, ValueC };
        ```
        `GetIndexByValue(EMyEnum::ValueB)` would return `1`.
    *   **Usage Example**:
        ```cpp
        #include "Utility/AlsEnumUtility.h"
        // Assuming EAlsMovementDirection is a UENUM
        EAlsMovementDirection CurrentDir = EAlsMovementDirection::Forward;
        int32 DirIndex = AlsEnumUtility::GetIndexByValue(CurrentDir); // DirIndex would be 0
        UE_LOG(LogAls, Log, TEXT("MovementDirection %s has index %d"), *StaticEnum<EAlsMovementDirection>()->GetNameStringByValue(static_cast<int64>(CurrentDir)), DirIndex);
        ```

2.  **`template <typename EnumType> FString GetNameStringByValue(const EnumType Value)`**
    *   **Purpose**: Given an enum value, this function returns its string name as defined in the `UEnum` metadata (the part after the `EMyEnum::` prefix).
    *   **Parameters**: `Value` (an instance of the `EnumType`).
    *   **Return Value**: `FString` - The string name of the enum value.
    *   **Functional Meaning**: For `EMyEnum::ValueB`, this would return the FString `"ValueB"`.
    *   **Usage Example**:
        ```cpp
        #include "Utility/AlsEnumUtility.h"
        EAlsGait CurrentGait = AlsGaitTags::Sprinting.GetTag().IsValid() ? EAlsGait::Sprinting : EAlsGait::Walking; // Hypothetical EAlsGait enum
        // Assuming EAlsGait exists and is a UENUM
        // FString GaitName = AlsEnumUtility::GetNameStringByValue(CurrentGait);
        // UE_LOG(LogAls, Log, TEXT("Current Gait: %s"), *GaitName);
        // For GameplayTags, UAlsUtility::GetSimpleTagName is usually preferred:
        FString GaitTagName = UAlsUtility::GetSimpleTagName(ACharacter->GetGait()).ToString();
        UE_LOG(LogAls, Log, TEXT("Current Gait Tag: %s"), *GaitTagName);
        ```
        *(Note: ALS primarily uses Gameplay Tags for states like Gait, Stance, etc. `AlsEnumUtility` would be more for custom C++ enums you define and expose as `UENUM`s, if any.)*

#### **Adding/Adapting Functionality**

*   These are generic utility functions. You don't typically "add" to them unless you discover a new common enum operation that would be widely useful.
*   **Usage**: Simply call these functions with your `UENUM` `enum class` type and value when you need its index or string name, typically for logging or creating debug UIs.
*   **GAS Integration**:
    *   If your Gameplay Abilities or Effects use custom C++ `UENUM`s for internal state tracking or parameterization, you can use `AlsEnumUtility::GetNameStringByValue` to easily log the current state of these enums for debugging GAS logic.
    ```cpp
    // In a custom Gameplay Effect Execution Calculation
    // UENUM() enum class EMyDamageType : uint8 { Slash, Pierce, Blunt };
    // EMyDamageType IncomingDamageType = GetDamageTypeFromSpec(Spec); // Hypothetical
    // FString DamageTypeName = AlsEnumUtility::GetNameStringByValue(IncomingDamageType);
    // UE_LOG(LogAls, Log, TEXT("Applying %s damage."), *DamageTypeName);
    ```

#### **Network Synchronization**

*   Enum utilities are purely local functions; they don't involve networking.
*   If an enum value itself is replicated (e.g., as a `UPROPERTY(Replicated)` on an actor), then each machine can independently use these utilities on its local copy of the enum value.

#### **Debugging**

*   The primary use of these utilities *is* for debugging and logging. By converting enum values to strings, you can make your log output much more readable than just printing integer values of enums.
*   When stepping through code in a debugger, you can often see the string name of an enum value directly, but these utilities are useful for programmatic logging or display.

---
