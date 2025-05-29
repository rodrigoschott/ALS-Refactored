**Attribute Map for a Complex Simulation Game**

---

**I. Core Physical & Vitality**
*(The fundamental state of being for a character)*
    *   **Health:** Current physical integrity.
    *   **MaxHealth:** Maximum physical integrity.
    *   **Consciousness:** (0-100%) Ability to perceive and interact. Affects most actions.
    *   **PainLevel:** (0-100) Current level of physical pain. High pain impacts Consciousness, Mood, Manipulation.
    *   **BleedingRate:** Rate at which character is losing blood (if applicable).
    *   **BloodLevel:** (0-100%) Current blood volume. Low levels cause debuffs.
    *   **Toxicity:** (0-100%) Level of toxins in the system (from pollution, poison, drugs).
    *   **HealingRate:** (Multiplier) Base speed of natural wound healing.
    *   **Age_Biological:** Actual biological age, might affect health, learning.
    *   **Age_Chronological:** Time since birth.
    *   **OrganEfficiency_[OrganName]:** (e.g., `OrganEfficiency_Lungs`, `OrganEfficiency_Kidneys`) (0-100%) Functionality of individual organs.
    *NEW* **LimbHealth_[LimbName]:** (e.g., `LimbHealth_Arm`, `LimbHealth_Leg`) (0-100%) Current integrity of limbs.
    *NEW* **Stamina**: For physical exertion, special moves, sprinting.
    *NEW* **MaxStamina**: Maximum stamina.
    *NEW* **StaminaRegenRate**: How quickly stamina recovers.
    *NEW* **StaminaEfficiency**: How well stamina is used.
    *NEW* **Mana**: For magical exertion, special moves, sprinting.
    *NEW* **MaxMana**: Maximum mana.
    *NEW* **ManaRegenRate**: How quickly mana recovers.
    *NEW* **ManaEfficiency**: How well mana is used.
    *NEW* **Experience**: Character experience. (Create experience curve)
    *NEW* **Level**: Character level.


---

**II. Combat Attributes**
*(How a character performs and endures in combat situations)*

    *   **A. Offensive Capabilities:**
        *   `MeleeAccuracy`: Base chance to hit in melee.
        *   `MeleeDamage_Base`: Base damage output before weapon/skill mods.
        *   `MeleeAttackSpeed`: How quickly melee attacks can be performed.
        *   `MeleeCritChance`: Chance for a melee attack to be a critical hit.
        *   `MeleeCritMultiplier`: Damage multiplier for melee critical hits.
        *   `MeleePenetration_Blunt`: Ability of blunt melee attacks to bypass armor.
        *   `MeleePenetration_Sharp`: Ability of sharp melee attacks to bypass armor.
        *   `RangedAccuracy`: Base chance to hit with ranged weapons.
        *   `RangedDamage_Base`: Base damage output before weapon/skill mods.
        *   `RangedAimTime`: Time taken to aim for optimal accuracy.
        *   `RangedAttackSpeed`: Firing rate with ranged weapons (affected by weapon type too).
        *   `RangedCritChance`: Chance for a ranged attack to be a critical hit.
        *   `RangedCritMultiplier`: Damage multiplier for ranged critical hits.
        *   `RangedEffectiveRange`: Multiplier for weapon's effective range.
        *   `UnarmedDamage`: Damage dealt with fists/kicks.


    *   **B. Defensive Capabilities:**
        *   `Evasion_Melee`: Chance to dodge incoming melee attacks.
        *   `Evasion_Ranged`: Chance to dodge incoming ranged projectiles (harder).
        *   `ParryChance`: Chance to block a melee attack (often requires a melee weapon).
    *New*   `BlockChance`: Chance to block a ranged attack (often requires a ranged weapon).
        *   `BlockEffectiveness`: Percentage of damage reduced when successfully blocking.
        *   `Armor_Blunt_Natural`: Innate resistance to blunt trauma.
        *   `Armor_Sharp_Natural`: Innate resistance to sharp trauma.
        *   `Armor_Heat_Natural`: Innate resistance to heat/burns.
        *   `Armor_Cold_Natural`: Innate resistance to cold/frostbite.
        *   `PainResistance`: (0-1) How much incoming pain is reduced. (e.g., 0.2 means 20% less pain taken).
        *   `StunResistance`: (0-1) Resistance to being stunned or dazed.
        *   `KnockbackResistance`: Resistance to being pushed or knocked down.

    *   **C. Utility & Tactical:**

        *   `ThreatLevel`: How likely enemies are to target this character.
        *   `StealthEffectiveness`: Ability to remain undetected.
        
---

**III. Resistances & Vulnerabilities**
*(How a character withstands various external threats, often percentage-based 0.0 to 1.0)*

    *   `FireResistance`
    *   `ColdResistance`
    *   `PoisonResistance`
    *   `ToxicResistance` (industrial chemicals, fallout)
    *   `RadiationResistance`
    *   `ElectricalResistance`
    *   `AcidResistance`
*NEW*   `PhysicalResistance`
    *   `DiseaseResistance_General`: General resistance to common illnesses.


---

**IV. Needs & Bodily Functions**
*(The biological and psychological imperatives, typically 0-100 where 100 is full)*

    *   `Hunger`
    *   `Thirst`
    *   `Rest` (Energy/Sleepiness)
    *   `RecreationNeed` (Joy, Fun)
    *   `ComfortNeed` (Physical comfort from furniture, temperature)
    *   `BeautyNeed` (Aesthetic appeal of surroundings)
    *   `SpaceNeed` (Dislike of crowding)
    *   `HygieneNeed`: Need to be clean. Low hygiene affects mood, social, disease risk.
    *   `OxygenNeed`: (If environment can lack oxygen).
    
---

**V. Mental & Psychological State**
*(Mood, stress, sanity, and facets of personality)*

    *   `Mood`: (e.g., -100 to +100 or 0-100) Overall emotional state.
    *   `StressLevel`: (0-100) Accumulated stress. High stress leads to breakdowns.
    *   `MentalBreakThreshold`: Point at which stress causes a mental break.
    *   `Sanity`: (0-100, if you have Lovecraftian elements) Resistance to maddening influences.
    *   `CreativityUrge`: (0-100) Need to express creativity, if unfulfilled can affect mood.

---

**VI. Social Attributes & Relationships**
*(How characters interact, perceive each other, and function in groups)*

    *   `SocialNeed`: Need for interaction with others.
    *   `MaxSocialNeed`.
    *   `Sociability`: (0-100) General pleasantness and skill in social interactions.
    *   `Charisma`: (0-100) Ability to charm, persuade, or lead.
    *   `DeceptionSkill`: (0-100) Ability to lie or mislead effectively.
    *   `IntimidationFactor`: (0-100) Ability to inspire fear or compliance through presence.
    *   `NegotiationSkill`: (0-100) Affects trading prices, diplomatic outcomes.
    *   `Aggressiveness`: (0-100) Likelihood to initiate conflict or respond forcefully.
    *   `Loyalty`: (0-100) Allegiance to their faction/group/loved ones.
    *   `FactionStanding_[FactionID]`: Current diplomatic status with a faction. (Could be a separate system but listed for completeness). **We need to discuss how to create a faction system. NPCs should change their aggression, loyalty, and diplomacy based on their faction standing.**
    *   `OpinionOf_[CharacterID]`: Current opinion of a specific character. (Also likely a separate system).

---

**VII. Skills & Aptitudes**
*(Proficiencies in various tasks, typically 0-100 or 0-20 like Rimworld, with passion levels as separate modifiers)*

    *   **A. Physical & Manual Labor:**
        *   `ConstructionSkill`
        *   `MiningSkill`
        *   `PlantingSkill` (Farming/Agriculture)
        *   `HarvestingSkill` (Gathering plants/resources)
        *   `WoodcuttingSkill`
        *   `AnimalTamingSkill`
        *   `AnimalTrainingSkill`
        *   `ButcheringSkill` (Processing animal carcasses)
        *   `CleaningSkill`
        *   `HaulingEfficiency` (How much they can carry / speed penalty reduction)

    *   **B. Crafting & Technical:**
        *   `Crafting_GeneralSkill` (For simple items)
        *   `Crafting_WeaponsmithingSkill`
        *   `Crafting_ArmorsmithingSkill`
        *   `Crafting_TailoringSkill` (Clothing)
        *   `Crafting_MachiningSkill` (Complex machinery, components)
        *   `Crafting_ElectronicsSkill`
        *   `Crafting_AlchemySkill` (Potions, drugs)
        *   `TinkeringSkill` (Repairing, jury-rigging)
        *   `CookingSkill`
        *   `BrewingSkill` (Alcohol, advanced beverages)
        *   `StonecuttingSkill` (Making blocks from stone chunks)
        *   `SmeltingSkill`

    *   **C. Intellectual & Cognitive:**
        *   `ResearchSkill` (Scientific discovery)
        *   `MedicalSkill_Tending` (First aid, treating wounds/diseases)
        *   `MedicalSkill_Surgery` (Performing operations)
        *   `DrugSynthesisSkill` (Creating medicine/combat drugs)

    *   **D. Creative & Artistic:**
        *   `ArtisticSkill_Sculpting`
        *   `ArtisticSkill_Painting`
        *   `ArtisticSkill_Music` (Composition or Performance)


    *   **E. Social & Interpersonal (Skill aspect):**
        *   `SocialInfluenceSkill` (Active use of persuasion, charm - distinct from passive Charisma)
        *   `TeachingSkill` (Ability to impart knowledge to others)
        *   `RecruitmentSkill` (Convincing prisoners or outsiders to join)
        *   `TradeNegotiationSkill` (Getting better prices)

    *   **F. Combat Skills (Underlying proficiency that feeds into Combat Attributes):**
        *   `MeleeWeaponSkill_[WeaponType]` (e.g., `MeleeWeaponSkill_Swords`, `MeleeWeaponSkill_Blunt`)
        *   `RangedWeaponSkill_[WeaponType]` (e.g., `RangedWeaponSkill_Pistols`, `RangedWeaponSkill_Rifles`)
        *   `MartialArtsSkill` (Unarmed combat specialization)
        *   `ShieldUseSkill`
        *   `TacticsSkill` (Improves squad command, flanking, etc.)**Only Players have this skill.**

---

**VIII. Movement & Physical Capabilities (Non-Combat)**
*(How a character moves and manipulates the world outside direct combat)*

    *   `BaseMoveSpeed`
    *   `CarryingCapacity`:**Hauling efficiency??**
    *   `ManipulationEfficiency`: (0-100%) General effectiveness of hands/limbs for work tasks (affected by injury, consciousness, pain).
    *   `WorkSpeedMultiplier`: General multiplier affecting the speed of all work tasks.
    *   `EatingSpeed`
    *   `SleepingEfficiency`: How quickly `Rest` is regained while sleeping.
    *   `Dexterity`: Affects manipulation, some crafting speeds, lockpicking, disarming traps.
    *   `Strength`: Affects carrying capacity, melee damage, some physical labor speeds.
    *   `Agility`: Affects move speed, evasion, climbing/traversal speed.
    *   `Endurance`: Affects stamina, recovery from injuries, and overall physical resilience.
    *   `Intelligence`: Affects problem-solving, decision-making, and overall cognitive function.
    *   `Charisma`: Affects social interactions, persuasion, and overall social influence.


---

**IX. Environmental Interaction & Perception**
*(Attributes related to how a character is affected by or perceives their surroundings)*

    *   `TemperatureComfort_Min`: Minimum comfortable ambient temperature.
    *   `TemperatureComfort_Max`: Maximum comfortable ambient temperature.
    *   `LightSensitivity`: How much vision is impaired in bright/dark conditions.
    *   `HearingAcuity`: How well the character can hear subtle sounds or noises from afar *Iteraction with sense components?*. 
    *   `SmellAcuity`: (If relevant for tracking, finding food, detecting dangers)
    *   `TerrainAdaptability`: How well the character moves across different difficult terrains (swamp, snow, rubble).

---

**X. Hidden/Developmental/Meta Attributes**
*(Less directly visible, driving underlying systems or character growth)*

    *   `LearningRate_Global`: Multiplier for all skill XP gain.
    *   `LearningRate_[SkillCategory]`: (e.g., `LearningRate_Combat`, `LearningRate_Crafting`)
    *   `SkillDecayRate`: How quickly unused skills degrade (if applicable).
    *   `AttributeGainModifier_[Attribute]`: If some attributes can be trained/improved.
    *   `Luck_General`: A subtle global luck factor.
    *   `Potential_[Skill]`: A hidden cap or aptitude indicator for how high a skill can naturally develop.
    *   `AddictionProgress_[Substance]`: (0-100) Tracks progression towards addiction.
    *   `ToleranceLevel_[Substance]`: How much of a substance is needed for desired effect / to avoid withdrawal.
    *   `InspirationCharge`: (0-100) Builds up, when full can trigger an inspiration event.
    *   `MagicPowerLevel`: (If psionics exist) Max psionic energy or strength.
    
---

**XI. Quirks/Traits (Conceptual - could be attributes or tags/GEs)**
*(These often manifest as modifiers to other attributes or trigger specific GEs/behaviors)*

    *   `WorkEthic`: (Multiplier) Affects work speed, likelihood to idle.
    *   `GreedFactor`: Influences desire for wealth, reaction to item distribution.
    *   `PyromaniaUrge`: Desire to start fires.
    *   `CleanlinessObsession`: Strong desire for clean surroundings, high penalty for dirt.
    *   `GourmandDesire`: Strong preference for fine meals, higher mood penalty for bad food.
    *   `AsceticDesire`: Preference for simplicity, gets mood buff from austere conditions.
    *   `NightOwlFactor`: More effective at night, less during the day.
    *   `DayPersonFactor`: Opposite of Night Owl.










--------------------------------------------------------------------------








Okay, this is perfect! Seeing your physics material list helps solidify how granular your limb damage system can be. We will definitely integrate these into the Combat Attributes section by showing how the health of these specific limbs (which would be actual `FGameplayAttributeData` in your `UAS_Limbs` or similar AttributeSet from Category I) influences combat performance.

You're absolutely right about the organization. It's crucial to distinguish between attributes that *live on the character's ASC* and stats that *belong to weapons/equipment* but are used in calculations involving the character.

Here's the revised and complete version of **Category II: Combat Attributes**, incorporating your limb list, feedback, and the requested organizational structure.

---

**Category II: Combat Attributes - Final Refined Version**

**Preamble on Organization:**

This section is divided into two main parts:
1.  **Character Combat Attributes:** These are `FGameplayAttributeData` that reside on the character's Ability System Component (ASC) within one or more AttributeSets. They represent the character's inherent or trained combat capabilities.
2.  **Conceptual Weapon/Equipment Attributes:** These are stats intrinsic to weapons or other equipment. They are **not** attributes on the character's ASC. Instead, their values are read from the equipped item's data (e.g., Data Table) and used in Gameplay Effect Execution Calculations, often in conjunction with the Character Combat Attributes.

**Limb Health Integration:**
The following attributes from **Category I: Core Physical & Vitality** (specifically your limb list) will significantly influence many Character Combat Attributes. Damage to these limbs will typically trigger Gameplay Effects that apply temporary penalties (Modifiers) to the relevant combat attributes:
*   `LimbHealth_Pelvis`
*   `LimbHealth_Torso`
*   `LimbHealth_Head`
*   `LimbHealth_UpperArm_L`, `LimbHealth_UpperArm_R`
*   `LimbHealth_Shoulder_L`, `LimbHealth_Shoulder_R`
*   `LimbHealth_LowerArm_L`, `LimbHealth_LowerArm_R`
*   `LimbHealth_Hand_L`, `LimbHealth_Hand_R`
*   `LimbHealth_UpperLeg_L`, `LimbHealth_UpperLeg_R` (assuming L/R based on arm pattern for your `UpperLeg_MAT`)
*   `LimbHealth_LowerLeg_L`, `LimbHealth_LowerLeg_R` (assuming L/R for your `LowerLeg_MAT`)
*   `LimbHealth_Foot_L`, `LimbHealth_Foot_R` (assuming L/R for your `Feet_MAT`)
*   `LimbHealth_Eyes`

---

**1. Character Combat Attributes (On ASC)**

**A. Offensive Capabilities:**

1.  **`Accuracy`**
    *   **Purpose:** Character's base proficiency in aiming and landing attacks successfully.
    *   **Unit:** Percentage (e.g., 0-100%) or a base value for formulas.
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Dexterity` (VIII), `Consciousness` (I), `Luck_General` (X), relevant `WeaponSkill` (VII.F), buffs/debuffs.
        *   **Limb Impact:**
            *   Reduced `LimbHealth_UpperArm_L/R`, `LimbHealth_LowerArm_L/R`, `LimbHealth_Hand_L/R`, `LimbHealth_Shoulder_L/R` applies penalties (e.g., GE with `Accuracy Additive -X`).
            *   Reduced `LimbHealth_Eyes` applies significant penalties, especially for ranged.
            *   Reduced `LimbHealth_Head` (affecting `Consciousness`) indirectly penalizes.
        *   **For Melee:** `FinalMeleeHitChance = (Attacker.Accuracy + Attacker.MeleeWeaponSkill_BonusToAccuracy + Weapon.BonusAccuracy + LimbPenalties) - Target.Evasion_Melee`
        *   **For Ranged:** `FinalRangedHitChance = (Attacker.Accuracy + Attacker.RangedWeaponSkill_BonusToAccuracy + Weapon.BonusAccuracy + AimBonus + LimbPenalties) - RangePenalty - Target.Evasion_Ranged - Target.CoverBonus`

2.  **`MeleeDamage_Bonus`**
    *   **Purpose:** Character's innate bonus damage added to melee attacks, derived from physical prowess.
    *   **Unit:** Flat damage value.
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Strength` (VIII), `MeleeWeaponSkill_[WeaponType]` (VII.F), buffs/debuffs.
        *   **Limb Impact:** Severe damage to `LimbHealth_UpperArm_L/R`, `LimbHealth_Shoulder_L/R` could apply a GE reducing effective `Strength` contribution or directly penalizing this bonus.
        *   `FinalMeleeDamageContribution = Attacker.MeleeDamage_Bonus + Attacker.MeleeWeaponSkill_BonusToDamage + LimbPenalties`
        *   *(Note: Total damage is `(Weapon.BaseDamage + FinalMeleeDamageContribution) * CritFactor ...etc.)*

3.  **`AttackSpeed_Multiplier`**
    *   **Purpose:** Character's modifier to the base attack speed of equipped weapons or unarmed attacks.
    *   **Unit:** Multiplier (e.g., 1.0 = normal, 1.2 = 20% faster).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Dexterity` (VIII), `Agility` (VIII), relevant `WeaponSkill` (VII.F), `Stamina` (I), buffs/debuffs.
        *   **Limb Impact:** Reduced `LimbHealth_UpperArm_L/R`, `LimbHealth_LowerArm_L/R`, `LimbHealth_Hand_L/R`, `LimbHealth_Shoulder_L/R` applies penalties.
        *   `FinalAPS = Weapon.BaseAPS * Attacker.AttackSpeed_Multiplier * Skill_SpeedFactor * LimbPenalties * Stamina_Modifier`

4.  **`CritChance`**
    *   **Purpose:** Character's base probability of landing a critical hit, dealing bonus effects/damage.
    *   **Unit:** Percentage (e.g., 0-100%).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Dexterity` (VIII) or a dedicated `Perception` stat, `Luck_General` (X), relevant `WeaponSkill` (VII.F), `Consciousness` (I), buffs/debuffs.
        *   **Limb Impact:** While less direct, severe penalties to `Accuracy` from injured `LimbHealth_Eyes` or `LimbHealth_Hands` could make aimed criticals (if they exist) harder. `Consciousness` drop from `LimbHealth_Head` damage also impacts.
        *   `FinalCritChance = Attacker.CritChance + Weapon.BonusCritChance + Skill_BonusToCritChance + (Luck_General * LuckToCritFactor) + LimbPenalties`

5.  **`CritMultiplier`**
    *   **Purpose:** Character's base damage multiplier for critical hits.
    *   **Unit:** Multiplier (e.g., 1.5 for +50% damage).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Relevant `WeaponSkill` (VII.F), specific perks/abilities.
        *   `FinalCritDamageFactor = Attacker.CritMultiplier + Weapon.BonusCritMultiplier + Skill_BonusToCritMultiplier`

6.  **`RangedAimTime_Multiplier`** (Specific to Ranged)
    *   **Purpose:** Character's modifier to how quickly they can achieve maximum potential accuracy for a ranged shot. Lower is better.
    *   **Unit:** Multiplier (e.g., 0.8 means 20% faster aim).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `RangedWeaponSkill_[WeaponType]` (VII.F), `Dexterity` (VIII), `Consciousness` (I).
        *   **Limb Impact:** Reduced `LimbHealth_UpperArm_L/R`, `LimbHealth_LowerArm_L/R`, `LimbHealth_Hand_L/R`, `LimbHealth_Shoulder_L/R`, `LimbHealth_Eyes` applies penalties.
        *   `TimeToFullAim = Weapon.BaseAimTime * Attacker.RangedAimTime_Multiplier * Skill_AimSpeedFactor * LimbPenalties`

7.  **`UnarmedDamage_Base`**
    *   **Purpose:** The fundamental damage the character deals with no weapons equipped.
    *   **Unit:** Flat damage value.
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Strength` (VIII), `MartialArtsSkill` (VII.F).
        *   **Limb Impact:** Reduced `LimbHealth_Hand_L/R`, `LimbHealth_Foot_L/R` (for kicks), `LimbHealth_LowerArm_L/R`, `LimbHealth_UpperArm_L/R` applies penalties.
        *   `FinalUnarmedDamage = (Attacker.UnarmedDamage_Base + Strength_Bonus + MartialArtsSkill_Bonus + LimbPenalties) * CritFactor`

**B. Defensive Capabilities:**

1.  **`Evasion_Melee`**
    *   **Purpose:** Character's chance to completely avoid an incoming melee attack through agility and movement.
    *   **Unit:** Percentage (e.g., 0-100%).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Agility` (VIII), `MeleeWeaponSkill_[WeaponType]` (if it includes defensive footwork), `Consciousness` (I), encumbrance.
        *   **Limb Impact:** Reduced `LimbHealth_UpperLeg_L/R`, `LimbHealth_LowerLeg_L/R`, `LimbHealth_Foot_L/R` applies penalties. Damage to `LimbHealth_Pelvis` or `LimbHealth_Torso` affecting core mobility also penalizes.

2.  **`Evasion_Ranged`**
    *   **Purpose:** Character's chance to completely avoid an incoming ranged projectile (typically lower than melee).
    *   **Unit:** Percentage.
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Agility` (VIII), `Consciousness` (I), movement speed, cover.
        *   **Limb Impact:** Same as `Evasion_Melee`.

3.  **`ParryChance`**
    *   **Purpose:** Character's chance to use a melee weapon or shield to deflect/block an incoming melee attack.
    *   **Unit:** Percentage.
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `MeleeWeaponSkill_[WeaponType]` (VII.F), `ShieldUseSkill` (VII.F), `Dexterity` (VIII), `Consciousness` (I).
        *   **Limb Impact:** Reduced `LimbHealth_UpperArm_L/R`, `LimbHealth_LowerArm_L/R`, `LimbHealth_Hand_L/R` (of the arm used for parrying) applies penalties.

4.  **`BlockChance`**
    *   **Purpose:** Character's chance to use a shield (or sturdy item) to intercept an incoming ranged projectile or melee attack.
    *   **Unit:** Percentage.
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `ShieldUseSkill` (VII.F), `Dexterity` (VIII), `Consciousness` (I).
        *   **Limb Impact:** Same as `ParryChance`.

5.  **`BlockEffectiveness`**
    *   **Purpose:** The percentage of damage reduced when a `Parry` or `Block` is successful.
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Strength` (VIII) (for bracing), `ShieldUseSkill` (VII.F) or relevant `MeleeWeaponSkill` (VII.F), quality of blocking item.
        *   **Limb Impact:** Severe damage to blocking `LimbHealth_Shoulder_L/R` or `LimbHealth_UpperArm_L/R` could reduce bracing ability, penalizing effectiveness.

6.  **`Armor_[DamageType]_Natural`** (e.g., `Armor_Blunt_Natural`, `Armor_Sharp_Natural`, etc.)
    *   **Purpose:** Character's innate, biological resistance to specific damage types (skin toughness, bone density).
    *   **Unit:** Flat damage reduction points or percentage.
    *   **Interactions & Pseudo-Calculation:** Generally static but could be affected by long-term mutations or racial traits. Not directly impacted by acute limb damage, but overall `MaxHealth` of a limb is its own armor in a sense.

7.  **`PainResistance`**
    *   **Purpose:** Reduces the amount of `PainLevel` (I) gained from damage or other painful stimuli.
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:** Influenced by `Endurance` (VIII), traits.

8.  **`StunResistance`**
    *   **Purpose:** Reduces the chance of being stunned or the duration of stuns.
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:** Influenced by `Endurance` (VIII), `Consciousness` (I). `LimbHealth_Head` damage drastically increases stun susceptibility if it results in a concussion effect.

9.  **`KnockbackResistance`**
    *   **Purpose:** Reduces the chance of being knocked back or the distance of knockback.
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:** Influenced by `Strength` (VIII), `Endurance` (VIII), character mass. Intact `LimbHealth_Legs` and `LimbHealth_Feet` contribute to stability.

**C. Utility & Tactical:**

1.  **`ThreatLevel`**
    *   **Purpose:** A value indicating how likely AI enemies are to prioritize this character as a target.
    *   **Unit:** Arbitrary points or a multiplier.
    *   **Interactions & Pseudo-Calculation:** Influenced by actions (damage dealt, healing), abilities (taunts), gear. Not directly by limb health.

2.  **`StealthEffectiveness`**
    *   **Purpose:** Determines how difficult it is for enemies to detect this character.
    *   **Unit:** Percentage (0-100%) or modifier.
    *   **Interactions & Pseudo-Calculation:** Influenced by `Agility` (VIII), `StealthSkill` (if any), environment.
        *   **Limb Impact:** Reduced `LimbHealth_LowerLeg_L/R` or `LimbHealth_Foot_L/R` can cause limping/noise, applying a penalty.

---

**2. Conceptual Weapon/Equipment Attributes (Stats on Items, NOT on ASC)**

These attributes are defined on the weapon or equipment data itself. Execution Calculations will query these from the equipped item.

*   **`Weapon_BaseDamage`**: The fundamental damage value of the weapon per hit/shot.
*   **`Weapon_DamageType`**: (e.g., Blunt, Sharp, Fire, etc.) Determines which armor/resistance values are checked.
*   **`Weapon_BonusAccuracy`**: An inherent accuracy modifier provided by the weapon.
*   **`Weapon_BaseAPS` (Attack Per Second)**: The unmodified attack speed of the weapon.
*   **`Weapon_BonusCritChance`**: Critical chance added by the weapon itself.
*   **`Weapon_BonusCritMultiplier`**: Additional critical damage multiplier from the weapon.
*   **`Weapon_Penetration_[DamageType]`**: (e.g., `Weapon_Penetration_Sharp`) The amount or percentage of armor of a specific type the weapon ignores.
*   **`Weapon_BaseAimTime`**: (For Ranged) The unmodified time it takes to fully aim the weapon.
*   **`Weapon_BaseEffectiveRange`**: (For Ranged) The optimal engagement distance for the weapon.
*   **`Weapon_ParryModifier` / `Weapon_BlockModifier`**: How effective the weapon is at parrying or blocking.
*   **`Armor_Value_[DamageType]`**: (For Armor pieces) The amount of damage reduction provided against a specific type.
*   **`Equipment_Weight` / `Equipment_Encumbrance`**: Affects movement, stamina, evasion.

**Interaction Example (Melee Hit):**
1.  Ability `GA_MeleeAttack` is activated.
2.  Its `ExecutionCalculation` (`UGEExec_MeleeDamage`) captures:
    *   From Attacker (Character Attributes): `Accuracy`, `MeleeDamage_Bonus`, `MeleeWeaponSkill`, `Strength`, `CritChance`, `CritMultiplier`, `AttackSpeed_Multiplier`, relevant `LimbHealth` values.
    *   From Attacker's Equipped Weapon (Weapon Attributes): `Weapon_BaseDamage`, `Weapon_DamageType`, `Weapon_BonusAccuracy`, `Weapon_Penetration_Sharp`.
    *   From Target (Character Attributes): `Evasion_Melee`, `Armor_Sharp_Natural`, `ParryChance`, relevant `LimbHealth`.
    *   From Target's Equipped Armor (Equipment Attributes): `Armor_Value_Sharp`.
3.  The Execution then calculates:
    *   Hit chance based on Attacker `Accuracy` + `Weapon_BonusAccuracy` + Skill bonuses + Limb penalties vs. Target `Evasion_Melee`.
    *   If hit, potential for Parry/Block.
    *   If still a hit, damage: `(Weapon_BaseDamage + Attacker.MeleeDamage_Bonus + Skill_DamageBonus)`
    *   Damage reduction: `FinalDamage = CalculatedDamage - (Target.TotalArmor_Sharp - Weapon_Penetration_Sharp)` (where `TotalArmor_Sharp` includes natural + equipment).
    *   Apply crit if rolled.
    *   The resulting damage is then applied to the target's `Health` and potentially a specific `LimbHealth_X` based on your physics material hit.







----------------------------------------------------------------------













**Category III: Resistances & Vulnerabilities - Final Refined Version**

*(These attributes determine how well a character withstands various types of damage, environmental effects, or debilitating conditions. They are FGameplayAttributeData on the character's Ability System Component (ASC) and typically represent a percentage reduction (0.0 to 1.0, where 1.0 is full immunity). They apply to specific damage types or adverse effects, often after initial mitigation like armor.)*

1.  **`FireResistance`**
    *   **Purpose:** Reduces damage taken from fire sources and may reduce the chance/severity of being set "On Fire."
    *   **Unit:** Percentage (0.0 to 1.0, representing 0-100% resistance).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Traits (e.g., "Pyrophobic" [-], "Fireblood" [+]), equipment (fire-resistant gear), temporary GEs (e.g., "Doused" status).
        *   `DamageTaken_FromFireSource = IncomingFireDamage * (1 - Character.FireResistance)`
        *   `ChanceToIgnite_FromSource = BaseIgniteChance * (1 - Character.FireResistance)`
        *   May also reduce the duration or tick damage of "Burning" status effects.

2.  **`ColdResistance`**
    *   **Purpose:** Reduces damage/effects from cold sources (e.g., ice attacks, freezing environments) and may reduce the chance/severity of "Frozen" or "Hypothermia" status effects.
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Traits, equipment (insulated gear), GEs (e.g., "Warmth" buff).
        *   `DamageTaken_FromColdSource = IncomingColdDamage * (1 - Character.ColdResistance)`
        *   `RateOfHypothermiaGain = BaseHypothermiaRate_FromEnvironment * (1 - Character.ColdResistance)`
        *   Interacts with `TemperatureComfort_Min` (IX) to determine susceptibility to environmental cold.

3.  **`PoisonResistance`**
    *   **Purpose:** Reduces the effectiveness of poisons, such as damage over time, attribute drains, or other debilitating effects.
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Endurance` (VIII), traits (e.g., "Mithridatism"), specific anti-poison GEs.
        *   `PoisonDamagePerTick_Applied = BasePoisonDamagePerTick * (1 - Character.PoisonResistance)`
        *   `PoisonDebuffMagnitude_Applied = BasePoisonDebuffMagnitude * (1 - Character.PoisonResistance)`
        *   May increase chance to "save" against a poison effect if a threshold check is involved.

4.  **`ToxicResistance`**
    *   **Purpose:** Reduces the build-up of `Toxicity` (I) from environmental sources (e.g., pollutants, chemical spills) or specific attacks, and mitigates the negative effects of high `Toxicity`.
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Traits, specialized gear (e.g., gas masks, hazmat suits), `OrganEfficiency_Kidneys`/`_Liver` (I).
        *   `ToxicityBuildupRate_FromSource = BaseToxicityRateFromSource * (1 - Character.ToxicResistance)`
        *   `NegativeEffectSeverity_FromToxicity = BaseNegativeEffectSeverity * (1 - Character.ToxicResistance)`

5.  **`RadiationResistance`**
    *   **Purpose:** Reduces the absorption of radiation and mitigates its negative effects (e.g., radiation sickness, detrimental mutations).
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Traits, specialized gear (lead-lined clothing), anti-radiation drugs/GEs.
        *   `RadiationAbsorbedPerTick_FromSource = BaseRadiationInArea * (1 - Character.RadiationResistance)`
        *   `SeverityOfRadiationSickness_Gained = RadiationEffectMagnitude * (1 - Character.RadiationResistance)`

6.  **`ElectricalResistance`**
    *   **Purpose:** Reduces damage from electrical attacks or hazards and may reduce the chance/duration of being stunned or having systems disrupted.
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Traits, gear (rubber-soled boots, insulated armor).
        *   `DamageTaken_FromElectricSource = IncomingElectricDamage * (1 - Character.ElectricalResistance)`
        *   `StunChanceOrDuration_FromShock = BaseStunEffect * (1 - Character.ElectricalResistance)`

7.  **`AcidResistance`**
    *   **Purpose:** Reduces damage from corrosive acids and may slow down the degradation of armor/equipment if acid also affects gear.
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Traits, specialized gear (acid-resistant plating).
        *   `DamageTaken_FromAcidSource = IncomingAcidDamage * (1 - Character.AcidResistance)`
        *   `(If applicable) ArmorDegradationRate_FromAcid = BaseDegradationRate * (1 - Character.AcidResistance)`

8.  **`PhysicalResistance`** (Option A: Post-Armor Damage Reduction)
    *   **Purpose:** Provides a general percentage-based reduction to all physical damage (blunt, sharp, piercing, etc.) *after* armor and penetration have been calculated. Represents overall character toughness or resilience to physical trauma.
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Endurance` (VIII), traits (e.g., "Thick Skinned," "Indomitable"), potentially very high `Health` (I) levels granting a minor innate resistance.
        *   `FinalPhysicalDamageTaken = DamageAfterArmorAndPenetration * (1 - Character.PhysicalResistance)`
    *   *Note: This acts as a final mitigation layer for physical damage specifically.*

9.  **`DiseaseResistance_General`**
    *   **Purpose:** Reduces the chance of contracting common diseases from various vectors (environmental, contact, contaminated food/water).
    *   **Unit:** Percentage (0.0 to 1.0).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `ImmunityStrength` (I - derived), `HygieneNeed` (IV - low hygiene can be a temporary debuff to this), traits (e.g., "Sickly" [-] vs. "Robust Constitution" [+]), current `Health` (I).
        *   `ChanceToContractDisease_FromExposure = BaseInfectionChanceFromSource * (1 - Character.DiseaseResistance_General)`
        *   May also influence the initial severity or progression speed of a contracted disease.






-------------------------------------------------------------------






**Category IV: Needs & Bodily Functions - Final Refined Version**

*(These attributes represent the fundamental biological and psychological requirements of a character. They typically operate on a scale of 0-100, where 100 represents full satisfaction and 0 represents critical deprivation. Needs usually decay over time and are replenished by specific actions or environmental conditions. Failure to meet needs often results in negative Gameplay Effects, mood debuffs, health decline, or even death. All needs are assumed to have a fixed Max of 100 unless specified otherwise.)*

1.  **`Hunger`**
    *   **Purpose:** Represents the character's need for food and nourishment.
    *   **Unit:** Scale, 0 to 100 (100 = Full, 0 = Starving).
    *   **Interactions & Pseudo-Calculation:**
        *   **Decay:** `Hunger -= BaseHungerDecayRate * Character.MetabolismRate_Multiplier * DeltaTime`.
            *   `MetabolismRate_Multiplier` (from Category I or hidden) influenced by traits.
        *   **Replenishment:** Increased by consuming food items (Gameplay Ability). Amount depends on food's nutritional value.
        *   **Low Thresholds & Effects (via GEs):**
            *   **Peckish (<60):** Minor mood debuff.
            *   **Hungry (<40):** Moderate mood debuff, slight penalties to `WorkSpeedMultiplier` (VIII), `StaminaRegenRate` (I). Tag: `Status.Need.Hunger.Moderate`.
            *   **Ravenous (<20):** Significant mood debuff, penalties to `WorkSpeedMultiplier`, `StaminaRegenRate`, `Consciousness` (I), `LearningRate_Global` (X). Tag: `Status.Need.Hunger.Severe`.
            *   **Starving (<=0):** Extreme mood debuff, severe penalties, periodic `Health` (I) damage. Tag: `Status.Need.Hunger.Critical`.

2.  **`Thirst`**
    *   **Purpose:** Represents the character's need for water/fluids.
    *   **Unit:** Scale, 0 to 100 (100 = Quenched, 0 = Dehydrated).
    *   **Interactions & Pseudo-Calculation:**
        *   **Decay:** `Thirst -= BaseThirstDecayRate * Temperature_Modifier * Character.HydrationEfficiency_Multiplier * DeltaTime`.
            *   `Temperature_Modifier`: Higher in hot environments.
            *   `HydrationEfficiency_Multiplier`: Influenced by traits.
        *   **Replenishment:** Increased by consuming water/hydrating beverages.
        *   **Low Thresholds & Effects (via GEs):** Similar progression to Hunger, impacting `Stamina`, `Consciousness`, and eventually `Health`.

3.  **`Rest`** (Energy/Sleepiness)
    *   **Purpose:** Represents the character's need for sleep and physical/mental recuperation.
    *   **Unit:** Scale, 0 to 100 (100 = Fully Rested, 0 = Exhausted).
    *   **Interactions & Pseudo-Calculation:**
        *   **Decay:** `Rest -= CurrentRestDecayRate * DeltaTime`.
            *   `CurrentRestDecayRate` = `BaseRestDecayRate`. Modified upwards by GEs from:
                *   Strenuous tasks (temporary GE with tag `Character.IsWorkingHard`).
                *   High mental load (temporary GE with tag `Character.IsMentallyTaxed`).
                *   High `PainLevel` (I).
        *   **Replenishment:** Increased by sleeping. Quality of bed affects `SleepingEfficiency` (VIII).
        *   **Low Thresholds & Effects (via GEs):**
            *   **Tired (<50):** Minor mood debuff, slight `WorkSpeedMultiplier` & `LearningRate_Global` penalty.
            *   **Very Tired (<30):** Moderate mood debuff, penalties to most actions, `Accuracy` (II.A) penalty. Tag: `Status.Need.Rest.Severe`.
            *   **Exhausted (<10):** Significant mood debuff, severe penalties, reduced `MoveSpeed` (VIII), `Consciousness` drop. Tag: `Status.Need.Rest.Critical`.
            *   **Collapse (<=0):** Character may pass out.

4.  **`RecreationNeed`** (Joy/Fun/Entertainment)
    *   **Purpose:** Represents the character's need for leisure, entertainment, and stress relief.
    *   **Unit:** Scale, 0 to 100 (100 = Fully Entertained, 0 = Desperate for Fun).
    *   **Interactions & Pseudo-Calculation:**
        *   **Decay:** `RecreationNeed -= BaseRecDecayRate * Stress_Modifier * DeltaTime`.
            *   `Stress_Modifier`: Higher `StressLevel` (V) might increase decay or make satisfaction harder.
        *   **Replenishment:**
            *   Slow passive gain during "Idle" time (AI state).
            *   Faster gain from interacting with specific recreational objects/activities (later implementation).
        *   **Low Thresholds & Effects (via GEs):**
            *   **Dull (<60):** Minor `Mood` (V) debuff.
            *   **Bored (<40):** Moderate `Mood` debuff. Tag: `Status.Need.Recreation.Moderate`.
            *   **Stifled (<20):** Significant `Mood` debuff, increased `StressLevel` gain. Tag: `Status.Need.Recreation.Severe`.
            *   **Recreation Starved (<=0):** Extreme `Mood` debuff, high risk of `MentalBreakThreshold` (V) being crossed. Tag: `Status.Need.Recreation.Critical`.

5.  **`EnvironmentSatisfaction`** (Consolidated from Comfort, Beauty, Space/CabinFever)
    *   **Purpose:** Represents the character's overall contentment with their immediate physical surroundings, encompassing aspects of physical comfort, aesthetic appeal, and sense of appropriate space.
    *   **Unit:** Scale, 0 to 100 (100 = Very Satisfied, 0 = Very Dissatisfied).
    *   **Interactions & Pseudo-Calculation:**
        *   **Character Attribute (Hidden/Derived):** `EnvironmentExpectation` (0-100). This is the baseline satisfaction level the character expects. Influenced by traits, character level/progression/wealth.
        *   **Room/Area Property:** `CurrentEnvironmentQuality` (0-100+). This is a calculated value for the character's current location (e.g., room they are in).
            *   `CurrentEnvironmentQuality` is derived from:
                *   **Comfort Factors:** Quality of furniture used (bed, chair), ambient temperature vs. character's preferred range (`TemperatureComfort_Min/Max` from IX).
                *   **Beauty Factors:** Presence of art, nice flooring/walls, cleanliness, view. Negative factors like dirt, gore, ugly structures.
                *   **Space Factors:** Room size, feeling of openness vs. being cramped (relevant if character is indoors for long periods or in tight spaces). This could be a multiplier or additive component to the room quality.
        *   **Need Dynamics:**
            *   If `CurrentEnvironmentQuality >= Character.EnvironmentExpectation`:
                `EnvironmentSatisfaction += GainRate * (CurrentEnvironmentQuality_Normalized - Character.EnvironmentExpectation_Normalized) * DeltaTime`. (Gain is better if quality far exceeds expectation). Capped at 100.
            *   If `CurrentEnvironmentQuality < Character.EnvironmentExpectation`:
                `EnvironmentSatisfaction -= LossRate * (Character.EnvironmentExpectation_Normalized - CurrentEnvironmentQuality_Normalized) * DeltaTime`. (Loss is faster if quality is far below expectation). Floored at 0.
            *   A base slow decay might also apply if `CurrentEnvironmentQuality` is neutral or the character is in an undefined/outdoor "average" space, always nudging it towards a baseline.
        *   **Effects (via GEs):**
            *   **High Satisfaction (>70):** `Mood` (V) buff, may improve `Rest` gain efficiency, slightly increase `StressLevel` (V) recovery.
            *   **Neutral Satisfaction (30-70):** No significant effect.
            *   **Low Satisfaction (<30):** `Mood` debuff, slight `StressLevel` gain.
            *   **Very Low Satisfaction (<10):** Significant `Mood` debuff, noticeable `StressLevel` gain, may negatively impact `Rest` quality or other cognitive functions.

6.  **`HygieneNeed`**
    *   **Purpose:** Represents the character's need to be clean.
    *   **Unit:** Scale, 0 to 100 (100 = Perfectly Clean, 0 = Filthy).
    *   **Interactions & Pseudo-Calculation:**
        *   **Decay/Getting Dirty:** Decreases from dirty work, being in dirty environments. `HygieneNeed -= DirtAccumulationRate * DeltaTime`.
        *   **Replenishment:** Increased by using cleaning facilities (showers, etc.).
        *   **Low Thresholds & Effects (via GEs):**
            *   **Slightly Grimy (<70):** Minor `Mood` debuff for "Neat" traits.
            *   **Dirty (<40):** General `Mood` debuff, negative impact on `Sociability` (VI), slight penalty to `DiseaseResistance_General` (III). Tag: `Status.Need.Hygiene.Moderate`.
            *   **Filthy (<10):** Significant `Mood` debuff, major social penalty, noticeable disease susceptibility. Tag: `Status.Need.Hygiene.Severe`.

7.  **`OxygenNeed`**
    *   **Purpose:** Represents the need for breathable air. (Primarily relevant in specific zones/conditions).
    *   **Unit:** Scale, 0 to 100 (100 = Full Lungs, 0 = Suffocating).
    *   **Interactions & Pseudo-Calculation:**
        *   **Decay:** Decreases rapidly in oxygen-deprived environments. `OxygenNeed -= BaseOxygenConsumptionRate * DeltaTime`.
        *   **Replenishment:** Maintained at max if in a breathable atmosphere or using an oxygen supply.
        *   **Low Thresholds & Effects (via GEs):**
            *   **Low Oxygen (<50):** `Consciousness` (I) penalty, `StaminaRegenRate` (I) penalty, `MoveSpeed` (VIII) penalty.
            *   **Very Low Oxygen (<20):** Severe `Consciousness` drop, rapid `Health` (I) loss. Tag: `Status.Need.Oxygen.Critical`.
            *   **Suffocating (<=0):** Rapid `Health` loss leading to death.




-------------------------------------------------------------------------











**Category V: Mental & Psychological State - Final Refined Version**

*(These attributes reflect the character's emotional well-being, stress levels, and overall psychological stability. They are heavily influenced by the satisfaction of needs (Category IV), social interactions (Category VI), traumatic events, achievements, and character traits (Category XI).)*

1.  **`Mood`**
    *   **Purpose:** Represents the character's current overall emotional state or happiness level. This is a cornerstone attribute that many other systems feed into and that dictates many behavioral responses.
    *   **Unit:** Scale, 0 to 100 (0 = Despondent, 50 = Neutral/Baseline, 100 = Ecstatic).
    *   **Interactions & Pseudo-Calculation:**
        *   `Mood` is the aggregate result of multiple active "Thought" Gameplay Effects (GEs), each applying a positive or negative modifier.
        *   **Calculation:** `CurrentMood = BaseMoodValue (e.g., 50) + SUM(ModifierValueOfAllActiveMoodGEs)`.
            *   Each Mood GE (e.g., `GE_Thought_AteFineMeal`, `GE_Thought_SleptOnFloor`, `GE_Thought_FriendDied`) has:
                *   A specific `Mood` modifier (e.g., +10, -5, -20).
                *   A specific duration.
                *   Stacking rules (e.g., "Ate Fine Meal" refreshes, "Minor Pain" can stack up to 3 times).
                *   A descriptive name/icon for UI display.
        *   **Influenced by (via application of specific Mood GEs):**
            *   **Needs (IV):** Satisfaction/deprivation of `Hunger`, `Thirst`, `Rest`, `RecreationNeed`, `EnvironmentSatisfaction`, `HygieneNeed`.
            *   **PainLevel (I):** High `PainLevel`.
            *   **Social (VI):** Quality of social interactions, unmet `SocialNeed`.
            *   **Events & Achievements:** Combat outcomes, project completions, deaths, discoveries.
            *   **Traits (XI):** Can apply permanent Mood GEs or modify the magnitude/duration of situational Mood GEs.
        *   **Effects of Mood Level (via GEs or direct checks):**
            *   **Very High Mood (>85):** Increased `LearningRate_Global` (X), `WorkSpeedMultiplier` (VIII), `InspirationCharge` (X) gain rate. Tag: `Status.Mood.VeryHigh`.
            *   **High Mood (70-85):** Minor positive buffs. Tag: `Status.Mood.High`.
            *   **Content (30-70):** Normal functioning.
            *   **Low Mood (15-30):** Minor penalties, increased `StressLevel` gain. Tag: `Status.Mood.Low`.
            *   **Very Low Mood (<15):** Significant penalties, higher `StressLevel` gain, risk of `MentalBreakThreshold` being reached via `StressLevel`. Tag: `Status.Mood.VeryLow`.

2.  **`StressLevel`**
    *   **Purpose:** Represents accumulated psychological pressure from sustained negative experiences, unmet needs, and demanding situations. Acts as a slower-moving indicator of a character's resilience wearing down.
    *   **Unit:** Scale, 0 to 100 (0 = Calm, 100 = Overwhelmed).
    *   **Interactions & Pseudo-Calculation:**
        *   **Gain:** `StressLevel += CurrentStressGainRate * DeltaTime`.
            *   `CurrentStressGainRate` increases due to (often via temporary GEs that boost this rate):
                *   Sustained Low `Mood`.
                *   Critically unmet Needs (IV).
                *   High `PainLevel` (I).
                *   Witnessing disturbing events (death, gore – severity modified by traits).
                *   Ongoing combat or dangerous situations.
                *   Difficult/undesirable work assignments.
                *   Environmental stressors (e.g., persistent loud noise, extreme temperature).
                *   Traits (e.g., "Nervous" increases base gain, "Volatile" increases gain from negative stimuli).
        *   **Decay (Recovery):** `StressLevel -= CurrentStressDecayRate * DeltaTime`.
            *   `CurrentStressDecayRate` increases when:
                *   `Mood` is consistently high.
                *   Needs are well-met.
                *   Engaged in satisfying `RecreationNeed` activities.
                *   Sleeping well (high `Rest` gain).
                *   In a high `EnvironmentSatisfaction` area.
                *   Traits (e.g., "Composed," "Iron Will" increase base decay rate).
        *   **Effects of StressLevel:**
            *   **Moderate Stress (>40):** Minor negative impact on `Mood`, reduced focus/concentration (e.g., slight penalty to `LearningRate_Global`, `WorkSpeedMultiplier` for complex tasks).
            *   **High Stress (>70):** More significant `Mood` debuff, impaired decision-making (for AI), higher chance of negative social outbursts. Tag: `Status.Stress.High`.
            *   **Extreme Stress (>90):** Severe `Mood` debuff. Very high likelihood of `StressLevel` reaching `MentalBreakThreshold`. Tag: `Status.Stress.Extreme`.

3.  **`MentalBreakThreshold`**
    *   **Purpose:** The point on the `StressLevel` scale at which a character is at high risk of suffering a mental break (e.g., tantrum, daze, hiding, violent outburst).
    *   **Unit:** Value on the `StressLevel` scale (e.g., typical default of 80-90).
    *   **Interactions & Pseudo-Calculation:**
        *   Primarily a static value per character, defining their general mental resilience.
        *   **Influenced by:**
            *   **Traits (XI):** "Iron Will" = higher threshold (e.g., 95), "Volatile" = lower threshold (e.g., 65).
            *   **Long-term Conditions (rare):** Profound, sustained trauma or recovery might very slowly adjust this over long periods, but generally, it's trait-defined.
        *   **Triggering a Break:**
            *   When `Character.StressLevel >= Character.MentalBreakThreshold`:
                *   A periodic check (e.g., every few game minutes) or event-driven check (on significant `StressLevel` gain) occurs.
                *   `If RandomRoll(0,100) < ChanceToBreak_AtOrAboveThreshold` then trigger a mental break Gameplay Ability.
                *   The *type* of mental break chosen can be influenced by traits, current unmet needs (e.g., starving character might binge), or recent events.
            *   A mental break ability typically applies a strong, temporary GE that dictates behavior, and might partially reset `StressLevel` (representing catharsis or shutdown).

4.  **`CreativityUrge`**
    *   **Purpose:** Represents an intrinsic drive in some characters to engage in creative expression. A high urge makes a character "primed" for an inspiration event.
    *   **Unit:** Scale, 0 to 100 (0 = No Urge/Recently Satisfied, 100 = Strong Urge/Primed). (Builds up over time).
    *   **Interactions & Pseudo-Calculation:**
        *   **Gain (Build-up):** `CreativityUrge += CurrentUrgeBuildupRate * DeltaTime`.
            *   `CurrentUrgeBuildupRate` is non-zero primarily for characters with "Creative" or "Artistic" traits (XI).
            *   Rate might increase slightly when `Mood` is high and other needs are met (mind is free).
            *   Witnessing exceptional art/craftsmanship or solving complex non-creative problems might provide a small boost.
        *   **Reduction (Satisfaction/Reset):**
            *   Performing creative tasks (using skills from Category VII.D) significantly reduces `CreativityUrge`. The amount of reduction can depend on the scope/success of the creative work.
            *   Triggering an "Inspiration" event will typically reset or greatly reduce `CreativityUrge`.
        *   **Effects of High Urge:**
            *   **Primed for Inspiration (>80-90):** Character gains a tag like `Status.PrimedForInspiration`. Does not directly cause mood debuffs by itself but indicates readiness.
        *   **Triggering Inspiration (Interaction with `InspirationCharge` from Category X):**
            *   When a character has `Status.PrimedForInspiration` tag:
                *   Specific game events (e.g., "Beautiful Sunrise," "Major Faction Achievement," "Quiet Moment of Reflection" (random tick for primed characters), "Witnessing Masterpiece Art") can trigger an "Inspiration Check."
                *   If check succeeds, an `Inspiration_TypeX` GE is applied (e.g., `GE_Inspiration_Crafting`, `GE_Inspiration_Research`).
                *   This GE grants temporary powerful bonuses to specific skills or work types and consumes the `Status.PrimedForInspiration` (by reducing `CreativityUrge`).
                *   The `InspirationCharge` attribute (X) could be a prerequisite *in addition* to `CreativityUrge` for some very powerful inspirations, or `CreativityUrge` could directly contribute to filling `InspirationCharge`. For simplicity, let's assume `CreativityUrge` is the primary gate for now.









------------------------------------------------------------------------





1.  **Gameplay Tags:** When Pawn A attacks Pawn X, Pawn X (and its nearby allies) might temporarily gain a tag like `HostileTo.PawnA_UniqueID`. Pawn A might gain `AggressedAgainst.FactionOfPawnX`.
2.  **Perception System:** The AI Perception system (or a custom "crime" system) detects the hostile act.
3.  **Behavior Trees/State Machines:** The AI witnessing or being victim to the attack transitions to a "Combat" or "Retaliate" state specifically against the aggressor.
4.  **Temporary Relationship Modifiers:** While the global `FactionStanding` might not change instantly, a temporary, localized "effective standing" or "aggro level" can be applied between the involved parties.



---

**Category VI: Social Attributes & Relationships - Final Refined Version**

*(These attributes govern how characters perceive each other, interact socially, and form relationships. Many of these attributes influence the success chance and outcome of social abilities, and contribute to a character's `Mood` (V) and `SocialNeed` (IV). Faction and interpersonal opinions are complex and often managed by separate systems/data structures that these attributes feed into.)*

1.  **`SocialNeed`**
    *   **Purpose:** Represents the character's fundamental need for positive social interaction and belonging. (Satisfied via interactions within this category).
    *   **Unit:** Scale, 0 to 100 (100 = Socially Fulfilled, 0 = Isolated/Lonely).
    *   **Interactions & Pseudo-Calculation:**
        *   **Decay:** `SocialNeed -= BaseSocialDecayRate * PersonalityModifier_Trait * DeltaTime`.
        *   **Replenishment:** Increased by successful and positive social interactions. Amount depends on interaction quality and `OpinionOf` the other character.
        *   **Low Thresholds & Effects (via GEs):** Leads to `Mood` debuffs and potentially increased `StressLevel` when unmet. (Detailed in Category IV).

2.  **`Charisma`** (Consolidated from former `Sociability` and `Charisma`)
    *   **Purpose:** Represents a character's overall social effectiveness, encompassing their general likeability, ability to make good impressions, persuade, lead, and inspire.
    *   **Unit:** Scale, 0 to 100 (0 = Socially Inept/Unconvincing, 100 = Highly Likeable/Persuasive/Magnetic).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Traits (e.g., "Kind," "Abrasive," "NaturalLeader," "InspiringRhetoric"), current `Mood` (V) (low mood can reduce effective charisma), `HygieneNeed` (IV) (being filthy is uncharismatic), `SocialInfluenceSkill` (VII.E) (skill can improve it or be amplified by it).
        *   Impacts success chance and outcome of:
            *   **General Socializing:** "Friendly Chat," "Offer Compliment" (higher charisma leads to better `OpinionOf` gain and `SocialNeed` fulfillment for both).
            *   **Active Influence:** "Persuade," "Negotiate," "Give Inspiring Speech," "Recruit Prisoner."
        *   `SocialSuccessChance_Base = (Actor.Charisma_Effective + Target.Receptiveness_Modifier) / 2 + SkillModifiers`
        *   `PersuasionCheckSuccess = (Actor.Charisma_Effective * Actor.SocialInfluenceSkill_Factor) > (Target.Resolve_Or_CounterAttribute + SituationDifficulty)`
        *   `Charisma_Effective` can be base `Charisma` modified by temporary states (mood, hygiene).
    *   **Example:** High `Charisma` improves trade prices (via `NegotiationSkill`), makes recruitment easier, and leads to more positive daily social outcomes.

3.  **`NegotiationSkill`** (Primarily a Skill from VII.E, but with strong social impact here)
    *   **Purpose:** Proficiency in bartering, trading, and reaching diplomatic agreements.
    *   **Unit:** Scale, 0 to 100 (as per other skills).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Charisma` (this category), `Intelligence` (VIII), the `TradeNegotiationSkill` level itself (VII.E).
        *   Directly affects trade price modifiers.
        *   Impacts outcomes of diplomatic abilities.
    *   **Example:** A character with high `NegotiationSkill` (boosted by `Charisma`) gets significantly better deals.

4.  **`Aggressiveness`**
    *   **Purpose:** A character's predisposition to initiate conflict, respond forcefully, or escalate tense situations.
    *   **Unit:** Scale, 0 to 100 (0 = Pacifist/Meek, 100 = Extremely Combative/Short-Tempered).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Traits ("Hot-Headed," "Calm"), current `StressLevel` (V), low `Mood` (V).
        *   Higher `Aggressiveness` increases likelihood of:
            *   Starting social fights (non-lethal usually) over insults or low `OpinionOf`.
            *   Choosing violent solutions in events.
            *   AI: More likely to engage enemies in combat.
        *   Lower `Aggressiveness` leads to de-escalation and conflict avoidance.

5.  **`Loyalty`**
    *   **Purpose:** Represents a character's allegiance and commitment to their current faction/group and its members/ideals.
    *   **Unit:** Scale, 0 to 100 (0 = Disloyal/Treacherous, 100 = Unwaveringly Loyal).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `FactionStanding` (if applicable), `OpinionOf` key faction members, overall treatment by faction (needs met, safety, fairness), traits ("Loyal," "Selfish"), time in faction, shared positive/negative experiences.
        *   **High Loyalty:** Reduces chance of desertion/betrayal, resists enemy conversion, may grant morale benefits when fighting for faction.
        *   **Low Loyalty:** Increases risk of desertion, susceptibility to enemy recruitment, may disobey orders or trigger internal strife.

---
**External Data Structures Critical for Social Dynamics (Not direct GAS Attributes on ASC but heavily interacted with):**

6.  **`FactionStanding_[FactionID]`** (Managed by a global Faction System/Manager)
    *   **Purpose:** The controlling faction's (e.g., Player's Faction) current diplomatic relationship level with other defined Factions in the game.
    *   **Unit:** Scale, e.g., -100 (Hostile/War) to +100 (Ally). 0 = Neutral.
    *   **Interactions:**
        *   Determines default AI behavior between members of different factions (e.g., attack on sight, ignore, allow trade).
        *   Influences availability of faction-specific quests, traders, recruitment pools.
        *   Modified by global actions (e.g., declaring war, forming alliances, completing major quests for/against a faction) and can be influenced by cumulative individual actions.
        *   Individual characters know their `OwnFactionTag`. AI uses this tag to query the Faction System: `FactionManager.GetEffectiveStanding(MyFactionTag, TargetFactionTag)`.
        *   **Localized Aggression:** Individual hostile acts (e.g., Player attacking a neutral faction member) will trigger immediate localized hostility from the victim and their nearby allies towards the aggressor, even if overall `FactionStanding` is neutral or friendly. This is handled by combat state, perception events, and temporary aggro mechanics, not by instantly changing global `FactionStanding`.

7.  **`OpinionOf_[TargetCharacterID]`** (Stored per character, e.g., in a `TMap<FGuid, float>` on a character component or the ASC itself if preferred, though often better in a dedicated component if opinions are numerous).
    *   **Purpose:** How one character privately feels about another specific character. The bedrock of interpersonal relationships.
    *   **Unit:** Scale, e.g., -100 (Despises) to +100 (Loves/Idolizes). 0 = Neutral.
    *   **Interactions:**
        *   Modified by:
            *   Outcome of social interactions (driven by `Charisma`, skills, traits).
            *   Shared experiences (combat, rescue, collaboration).
            *   Observed actions (kindness, cruelty, theft).
            *   Trait compatibility/conflicts (e.g., "Neat" dislikes "Slob" over time).
            *   Romantic interactions.
        *   **High `OpinionOf`:** Leads to more positive autonomous interactions, easier formation of friendships/romances, `Mood` buffs when near the liked character, `Mood` debuffs if they are harmed.
        *   **Low `OpinionOf`:** Leads to negative autonomous interactions (insults, arguments), social fights (usually non-lethal), `Mood` debuffs when near the disliked character.
        *   Does NOT typically cause lethal "attack on sight" between members of the same non-hostile faction unless `OpinionOf` is catastrophically low AND combined with extreme traits (e.g., "Psychopath") or a mental break.











----------------------------------------------------------------------------





**Category VII: Skills & Aptitudes - Final Refined Version**

*(These attributes represent a character's proficiency and learned expertise in various tasks and fields. They typically range from 0 to a cap (e.g., 0-20 or 0-100). Skills are improved through practice (gaining XP) and influenced by `LearningRate` attributes (X), "passion" modifiers (tags/multipliers affecting XP gain), and relevant base attributes like `Intelligence` (VIII) or `Dexterity` (VIII). Higher skill levels generally lead to faster task completion, higher quality results, and unlocking more complex abilities or recipes.)*

**A. Physical & Manual Labor:**

1.  **`ConstructionSkill`**
    *   **Purpose:** Proficiency in building, deconstructing, and repairing structures, furniture, and defenses.
    *   **Interactions:** Affects speed, structural integrity/quality of built items, quality/beauty of furniture. May unlock complex blueprints.
    *   **XP Gain:** Building, deconstructing, repairing.

2.  **`MiningSkill`**
    *   **Purpose:** Proficiency in extracting resources from terrain.
    *   **Interactions:** Affects speed, resource yield, chance of bonus yield/discoveries.
    *   **XP Gain:** Mining ore, stone, etc.

3.  **`PlantingSkill`** (Farming/Agriculture)
    *   **Purpose:** Proficiency in sowing seeds and tending crops.
    *   **Interactions:** Affects planting speed, success chance, crop growth speed bonus, potentially yield. May unlock difficult plant types.
    *   **XP Gain:** Sowing seeds, tending plants.

4.  **`HarvestingSkill`** (Gathering plants/resources)
    *   **Purpose:** Proficiency in gathering mature crops and wild resources.
    *   **Interactions:** Affects speed, resource yield, quality of harvested goods.
    *   **XP Gain:** Harvesting crops, foraging.

5.  **`WoodcuttingSkill`**
    *   **Purpose:** Proficiency in felling trees.
    *   **Interactions:** Affects felling speed, wood yield.
    *   **XP Gain:** Chopping trees.

6.  **`AnimalTamingSkill`**
    *   **Purpose:** Proficiency in domesticating wild animals.
    *   **Interactions:** Affects taming success chance (vs. animal wildness). Higher skill for more difficult animals.
    *   **XP Gain:** Attempting to tame animals.

7.  **`AnimalTrainingSkill`**
    *   **Purpose:** Proficiency in teaching tamed animals commands/roles.
    *   **Interactions:** Affects training speed/success. Unlocks advanced training.
    *   **XP Gain:** Actively training animals.

8.  **`ButcheringSkill`**
    *   **Purpose:** Proficiency in processing animal carcasses.
    *   **Interactions:** Affects speed, meat/leather yield, potential for resource quality.
    *   **XP Gain:** Butchering carcasses.

9.  **`CleaningSkill`**
    *   **Purpose:** Proficiency in cleaning messes.
    *   **Interactions:** Affects cleaning speed. Contributes to `EnvironmentSatisfaction` (IV).
    *   **XP Gain:** Cleaning tiles.

10. **`HaulingEfficiency`** (Considered a derived capability, not a trained skill)
    *   **Purpose:** How effectively a character moves items.
    *   **Interactions:** Derived from `CarryingCapacity` (VIII), `BaseMoveSpeed` (VIII), and potentially encumbrance effects. Not directly trained via XP.

**B. Crafting & Technical:**

11. **`Crafting_GeneralSkill`**
    *   **Purpose:** Proficiency in basic crafting tasks and simple items.
    *   **Interactions:** Affects speed and quality for simple recipes. May be prerequisite for specialized crafting.
    *   **XP Gain:** Crafting at a general crafting spot.

12. **`Crafting_WeaponsmithingSkill`**
    *   **Purpose:** Proficiency in creating weaponry.
    *   **Interactions:** Affects speed and quality (damage, accuracy, durability) of weapons. Unlocks advanced recipes.
    *   **XP Gain:** Crafting weapons.

13. **`Crafting_ArmorsmithingSkill`**
    *   **Purpose:** Proficiency in creating armor.
    *   **Interactions:** Affects speed and quality (protection, durability) of armor. Unlocks advanced recipes.
    *   **XP Gain:** Crafting armor.

14. **`Crafting_TailoringSkill`** (Clothing)
    *   **Purpose:** Proficiency in creating clothing and textiles.
    *   **Interactions:** Affects speed and quality (insulation, beauty, durability) of clothing. Unlocks advanced recipes.
    *   **XP Gain:** Crafting clothing.

15. **`Crafting_MachiningSkill`** (Complex machinery, components)
    *   **Purpose:** Proficiency in assembling complex machinery and advanced components.
    *   **Interactions:** Affects speed and quality/reliability of machines. Unlocks high-tier recipes.
    *   **XP Gain:** Crafting at a machining table.

16. **`Crafting_ElectronicsSkill`**
    *   **Purpose:** Proficiency in creating/repairing electronic devices and circuits.
    *   **Interactions:** Affects speed and quality/reliability of electronics. Unlocks advanced electronics. Used for repairing complex devices.
    *   **XP Gain:** Crafting/repairing electronics.

17. **`Crafting_AlchemySkill`** (Potions, drugs, explosives)
    *   **Purpose:** Proficiency in chemical concoctions (potions, drugs, explosives, medicines).
    *   **Interactions:** Affects speed, potency/purity/stability of created substances. Unlocks advanced recipes.
    *   **XP Gain:** Crafting at an alchemy lab/drug station.

18. **`CookingSkill`** (Includes Brewing)
    *   **Purpose:** Proficiency in preparing meals and brewing beverages.
    *   **Interactions:** Affects speed, nutritional value/tastiness of meals, quality/potency of brews, chance of food poisoning. Unlocks complex recipes.
    *   **XP Gain:** Cooking meals, brewing beverages.

19. **`StonecuttingSkill`**
    *   **Purpose:** Proficiency in shaping stone chunks into usable blocks.
    *   **Interactions:** Affects speed, block yield per chunk.
    *   **XP Gain:** Cutting stone blocks.

20. **`SmeltingSkill`**
    *   **Purpose:** Proficiency in extracting metals from ores or recycling items.
    *   **Interactions:** Affects speed, metal yield.
    *   **XP Gain:** Smelting ores/metal items.

**C. Intellectual & Cognitive:**

21. **`ResearchSkill`**
    *   **Purpose:** Proficiency in conducting research to unlock technologies.
    *   **Interactions:** Affects research point generation speed. Higher skill for advanced projects.
    *   **XP Gain:** Performing research.

22. **`MedicalSkill`** (Includes Tending and Surgery)
    *   **Purpose:** Proficiency in all medical care: diagnosing, tending wounds/diseases, administering medicine, and performing surgery.
    *   **Interactions:** Affects tending/surgery speed, quality of care (healing speed, infection chance, disease treatment success, surgery success chance). Higher skill for more complex procedures.
    *   **XP Gain:** Tending patients, performing surgeries.

**D. Creative & Artistic:**

23. **`ArtisticSkill`** (Consolidated: Sculpting, Painting, Music)
    *   **Purpose:** Proficiency in creating all forms of art.
    *   **Interactions:** Affects creation speed and artistic quality/beauty of art objects (influencing `EnvironmentSatisfaction` and item value). Affects quality of musical performance/composition (for `RecreationNeed`/`Mood`).
    *   **XP Gain:** Creating art, performing/composing music.

**E. Social & Interpersonal:**

24. **`TeachingSkill`**
    *   **Purpose:** Proficiency in effectively teaching others skills.
    *   **Interactions:** Affects student's XP gain rate when taught by this character.
    *   **XP Gain:** Actively teaching.

25. **`RecruitmentSkill`**
    *   **Purpose:** Specialized skill in persuading individuals to join the faction.
    *   **Interactions:** Increases success chance of "Recruit" attempts (vs. target's resolve/loyalty). Influenced by `Charisma` (VI).
    *   **XP Gain:** Attempting to recruit.

26. **`NegotiationSkill`**
    *   **Purpose:** Learned proficiency in bartering, trading, and reaching favorable agreements. This is the trainable skill level.
    *   **Interactions:** Directly modifies trade price calculations (buy/sell prices). Effectiveness is influenced by `Charisma` (VI).
    *   **XP Gain:** Successfully completing trades.

**F. Combat Skills:**

27. **`MeleeWeaponSkill_[WeaponType]`** (e.g., `MeleeWeaponSkill_Swords`, `MeleeWeaponSkill_Blunt`)
    *   **Purpose:** Specific proficiency with a class of melee weapons.
    *   **Interactions:** Provides bonuses to relevant combat attributes (II) when using that weapon type (e.g., Accuracy, Damage, Attack Speed). Unlocks special abilities.
    *   **XP Gain:** Using that weapon type in combat.

28. **`RangedWeaponSkill_[WeaponType]`** (e.g., `RangedWeaponSkill_Pistols`, `RangedWeaponSkill_Rifles`)
    *   **Purpose:** Specific proficiency with a class of ranged weapons.
    *   **Interactions:** Provides bonuses to relevant combat attributes (II) when using that weapon type. Unlocks special abilities.
    *   **XP Gain:** Using that weapon type in combat.

29. **`MartialArtsSkill`** (Unarmed combat)
    *   **Purpose:** Proficiency in unarmed combat.
    *   **Interactions:** Enhances unarmed combat attributes (II.A `UnarmedDamage_Base`, accuracy, crit). May grant special unarmed moves.
    *   **XP Gain:** Using unarmed attacks in combat.

30. **`ShieldUseSkill`**
    *   **Purpose:** Proficiency in using shields.
    *   **Interactions:** Increases `BlockChance` and `BlockEffectiveness` (II.B). May unlock shield abilities.
    *   **XP Gain:** Successfully blocking with a shield.

31. **`TacticsSkill`** (Player Character primarily)
    *   **Purpose:** Understanding of battlefield command and coordination.
    *   **Interactions:** May provide passive buffs to nearby allies (if leader) or unlock/enhance player-activated tactical squad abilities.
    *   **XP Gain:** Successful combat encounters using tactical abilities or achieving tactical advantages.

---


















--------------------------------------------------------------------







**Category VIII: Movement & Physical Capabilities / Core Physical & Mental Aptitudes - Final Refined Version**

*(These attributes represent a character's fundamental physical and mental prowess. They are often more inherent or change slowly, though some could be trained or affected by long-term conditions, bionics, or aging. They serve as foundational values that influence many other derived attributes, skill effectiveness, and action success chances across various categories.)*

**A. Derived Physical Capabilities:**

1.  **`BaseMoveSpeed`**
    *   **Purpose:** The character's unmodified walking/running speed on flat, clear terrain.
    *   **Unit:** Meters per second (or Unreal Units per second).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Species/race (provides base value), `Dexterity` (this category – specifically its agility aspects), `LimbHealth_Legs`/`_Feet` (I) (injuries reduce it), `Health` (I) (very low health might slow), and encumbrance (see `CarryingCapacity`).
        *   `ActualMoveSpeed = Character.BaseMoveSpeed_SpeciesBase * Dexterity_MoveSpeedModifier * LimbHealth_Modifier * Encumbrance_PenaltyFactor * Terrain_Modifier`
        *   Directly affects how quickly characters traverse the map.

2.  **`CarryingCapacity`**
    *   **Purpose:** The maximum weight or bulk of items a character can carry before suffering significant movement penalties or being unable to carry more. This attribute solely defines the character's carrying limit.
    *   **Unit:** Kilograms or arbitrary bulk units.
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Strength` (this category), species/race, specific gear (e.g., backpack).
        *   **Encumbrance Effects (Derived, not a separate skill/attribute):**
            *   If `CurrentLoad <= Character.CarryingCapacity * ComfortableCarryFactor (e.g., 0.75)`: Minimal or no penalty.
            *   If `CurrentLoad > Character.CarryingCapacity * ComfortableCarryFactor` but `< Character.CarryingCapacity`: Minor `BaseMoveSpeed` reduction, slightly increased `Stamina` (I) cost for movement.
            *   If `CurrentLoad > Character.CarryingCapacity`: Significant `BaseMoveSpeed` reduction (scaling with % over capacity), high `Stamina` cost, potential inability to perform certain actions (e.g., sprint, dodge).
            *   If `CurrentLoad >> Character.CarryingCapacity`: Character may be unable to move.
        *   These effects are applied via Gameplay Effects based on the ratio of `CurrentLoad` to `CarryingCapacity`.

3.  **`ManipulationEfficiency`**
    *   **Purpose:** General effectiveness and dexterity of a character's hands/limbs for performing work tasks.
    *   **Unit:** Percentage (0% to 100%+; 100% = normal human efficiency).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Dexterity` (this category – specifically its fine motor skill aspects), `Consciousness` (I), `PainLevel` (I), `LimbHealth_Arms`/`_Hands`/`_Shoulders` (I), species/race.
        *   Directly multiplies the speed of many work tasks: `EffectiveWorkSpeed_Task = BaseWorkSpeed_FromSkill * Character.ManipulationEfficiency`.
        *   Affects crafting, construction, medical tending, planting speeds, etc.

4.  **`WorkSpeedMultiplier`**
    *   **Purpose:** A global multiplier for non-combat work tasks, representing focus, diligence, or general "get-it-done-ness," applied after skill and manipulation.
    *   **Unit:** Multiplier (e.g., 1.0 = normal).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: `Mood` (V), `Rest` (IV), traits ("Industrious," "Lazy"), `WorkEthic` (XI - trait modifier), drug effects.
        *   `FinalTaskSpeed = BaseSpeedFromSkill * ManipulationEfficiency * Character.WorkSpeedMultiplier`.

5.  **`EatingSpeed`**
    *   **Purpose:** How quickly a character consumes food.
    *   **Unit:** Multiplier on base meal consumption time.
    *   **Interactions & Pseudo-Calculation:** Influenced by species/race, `LimbHealth_Head` (jaw/mouth injuries), `Consciousness` (I).

6.  **`SleepingEfficiency`**
    *   **Purpose:** How effectively a character regains `Rest` (IV) while sleeping.
    *   **Unit:** Multiplier (e.g., 1.0 = normal Rest gain rate).
    *   **Interactions & Pseudo-Calculation:** Influenced by bed quality (`EnvironmentSatisfaction` factor IV), `PainLevel` (I), traits.

---
**B. Core Physical & Mental Aptitudes (S.P.E.C.I.A.L.-like):**
*(Innate or slowly changing stats defining raw potential. Set at creation, modified by bionics, permanent injuries, or powerful long-term GEs.)*

7.  **`Strength` (STR)**
    *   **Purpose:** Raw physical power, brawn, musculature.
    *   **Unit:** Points (e.g., 1-10 or 1-20).
    *   **Influences:** `CarryingCapacity` (this category), `MeleeDamage_Bonus` (II.A), `UnarmedDamage_Base` (II.A) contribution, speed of some heavy labor skills (e.g., Mining), `KnockbackResistance` (II.B), damage with STR-based weapons.

8.  **`Dexterity` (DEX)** (Combined Dexterity & Agility)
    *   **Purpose:** Physical adroitness, coordination, reflexes, fine motor skills, and nimbleness.
    *   **Unit:** Points.
    *   **Influences:**
        *   `Accuracy` (II.A), `AttackSpeed_Multiplier` (II.A), `ManipulationEfficiency` (this category), speed/quality of delicate crafting skills, `MedicalSkill` precision, `ParryChance` (II.B) (fine motor/reflex aspects).
        *   `BaseMoveSpeed` (this category), `Evasion_Melee`/`_Ranged` (II.B), `StealthEffectiveness` (II.C) (body control/nimbleness aspects).

9.  **`Endurance` (END)** (or Constitution)
    *   **Purpose:** Healthiness, resilience to harm, stamina, resistance to disease/poison/fatigue.
    *   **Unit:** Points.
    *   **Influences:** `MaxHealth` (I), `HealingRate` (I), `MaxStamina` (I), `StaminaRegenRate` (I), `PainResistance` (II.B), `DiseaseResistance_General` (III), `PoisonResistance` (III), `PhysicalResistance` (III), `StunResistance` (II.B), `KnockbackResistance` (II.B), fatigue rate from exertion.

10. **`Intelligence` (INT)**
    *   **Purpose:** Learning aptitude, reasoning, memory, problem-solving ability, cognitive function.
    *   **Unit:** Points.
    *   **Influences:** `LearningRate_Global` (X), effectiveness/speed of intellectual skills (`ResearchSkill`, `MedicalSkill` diagnosis, complex crafting), potential for advanced dialogue/event solutions, `MaxMana`/`ManaRegenRate` (I) if mana is mental.

11. **`Charisma` (CHA)** (Defined in VI, but listed here as a core aptitude)
    *   **Purpose:** Social grace, persuasiveness, leadership ability.
    *   **Unit:** Points.
    *   **Influences:** All social interactions and effectiveness of social skills (`NegotiationSkill` VII.E, `RecruitmentSkill` VII.E, `TeachingSkill` VII.E) as detailed in Category VI.













--------------------------------------------------------------------------





**Category IX: Environmental Interaction & Perception - Final Refined Version**

*(These attributes govern how a character senses their surroundings and how they are affected by or adapt to various environmental conditions. They often interact with Needs (IV) and can influence awareness or performance in specific situations.)*

1.  **`TemperatureComfort_Min`**
    *   **Purpose:** The minimum ambient temperature a character can comfortably tolerate without suffering negative effects or needing protective gear.
    *   **Unit:** Temperature (e.g., degrees Celsius/Fahrenheit, consistent with game units).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Species/race, traits (e.g., "Cold Acclimated," "Heat Sensitive").
        *   **Gear Interaction:** Clothing/armor items will possess `Insulation_Cold` properties. The character's total `Insulation_Cold` from gear effectively lowers the *perceived ambient temperature* or expands their comfort range.
            *   `EffectiveTemperature = AmbientTemperature - TotalGearInsulation_Cold` (simplified model).
        *   If `EffectiveTemperature < Character.TemperatureComfort_Min`:
            *   Triggers negative "Cold" GEs affecting `EnvironmentSatisfaction` (IV) and `Mood` (V).
            *   `ColdResistance` (III) mitigates severity/onset of further negative effects.
            *   Prolonged severe exposure leads to "Hypothermia" (GE causing debuffs to `Consciousness` (I), `ManipulationEfficiency` (VIII), `MoveSpeed` (VIII), and eventually `Health` (I) damage).

2.  **`TemperatureComfort_Max`**
    *   **Purpose:** The maximum ambient temperature a character can comfortably tolerate.
    *   **Unit:** Temperature.
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Species/race, traits (e.g., "Heat Acclimated," "Cold Sensitive").
        *   **Gear Interaction:** Clothing/armor items will possess `Insulation_Heat` properties (less common than cold, might be "heat dissipation" or "cooling"). This effectively raises the *perceived ambient temperature* less quickly or expands the comfort range.
            *   `EffectiveTemperature = AmbientTemperature + TotalGearInsulation_Heat` (if heat-trapping gear) or modified by cooling gear.
        *   If `EffectiveTemperature > Character.TemperatureComfort_Max`:
            *   Triggers negative "Hot" GEs affecting `EnvironmentSatisfaction` (IV) and `Mood` (V).
            *   `FireResistance` (III) (as general heat resistance) might mitigate.
            *   Prolonged severe exposure leads to "Heatstroke" (GE causing debuffs to `Consciousness`, increased `Thirst` (IV) decay, reduced `Stamina` (I), and `Health` damage).

3.  **`LightSensitivity`**
    *   **Purpose:** Defines how a character's vision, `Accuracy`, and potentially `Mood` are affected by different ambient light levels.
    *   **Unit:** Could be a scale (e.g., -1.0 for strong dark preference to +1.0 for strong light preference, 0.0 neutral) or specific tags/enum defining preference.
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Species/race (nocturnal, diurnal), traits ("NightOwl," "Photosensitive"), bionics/mutations.
        *   This attribute modifies a pawn's "EffectiveVision" or directly applies GEs with `Accuracy` (II.A) modifiers based on current ambient light level vs. their preference.
            *   **Example (NightOwl - prefers dark):**
                *   In Bright Light: Applies `GE_Debuff_BrightLight_AccuracyPenalty`, possible minor `GE_Thought_TooBright_MoodPenalty`.
                *   In Darkness: Applies `GE_Buff_DarkVision_AccuracyBonus` (or no penalty), possible minor `GE_Thought_ComfortingDarkness_MoodBuff`.
        *   Impacts effectiveness of sight-based tasks and AI's visual perception range.

4.  **`HearingAcuity`**
    *   **Purpose:** The character's ability to detect sounds, determine their direction, and discern subtle noises.
    *   **Unit:** Multiplier (e.g., 1.0 = normal human hearing; >1.0 is better, <1.0 is worse).
    *   **Interactions & Pseudo-Calculation:**
        *   Influenced by: Species/race, `LimbHealth_Head` (ear damage), `Consciousness` (I), traits ("KeenEared," "HardOfHearing"), gear (sound-dampening helmets [-], hearing aids [+]).
        *   `EffectiveHearingRange_ForAI = BaseHearingRange_FromAISenseConfig * Character.HearingAcuity`.
        *   Affects:
            *   AI's ability to detect sound events via `UAISense_Hearing`.
            *   Chance to be woken by noises during `Rest` (IV).
            *   Susceptibility/reaction to loud noises or sonic effects (high acuity might be more sensitive to overload).



-------------------------------------------------------------------------









1.  **`LearningRate_[SkillCategory]` Removal:**
    *   **Your Note:** *"It does. With LearningRate_Global and passion modifiers, we can " specialize" pawns already."*
    *   **My Opinion & Recommendation:** Agreed. `LearningRate_Global` plus a passion system (which we'll assume is implemented via tags or a simple data structure on the character that modifies XP gain for specific skills) provides sufficient control for skill specialization without needing another layer of category-specific learning rates.
    *   **Decision:** Remove `LearningRate_[SkillCategory]`.

2.  **`SkillDecayRate` - Default to 0:**
    *   **Your Note:** *"Agreed. It is meant to be present only in a Hardcore mode or something similar. Default value is 0.0 until told otherwise."*
    *   **My Opinion & Recommendation:** Perfect. This makes it an opt-in feature for difficulty, which is a good approach for potentially frustrating mechanics.
    *   **Decision:** Keep `SkillDecayRate`, note its default is 0.

3.  **`Potential_[SkillName]` - Default to Max, Niche Use:**
    *   **Your Note:** *"This will also be available through specific modes or configurations. For now the default limit for all skills is 100. No need for aptitude curves or anything like that. Passions take care of that."*
    *   **My Opinion & Recommendation:** Understood. So, this isn't about variable soft caps per character by default, but rather a hard cap (e.g., 100 or 20) for all skills, which this "Potential" could theoretically lower for specific characters/modes if ever needed. Effectively, it might not be an active attribute for most characters.
    *   **Decision:** Keep the concept of `Potential_[SkillName]` but note that its default value for any skill would be the game's maximum skill level (e.g., 100 or 20), and only specific traits or game modes might lower it for an individual.

4.  **`InspirationCharge` vs. `CreativityUrge`:**
    *   **Your Note:** *"Does this replace Urge or are they separate? Because I think our change to Urge invalidates this system."*
    *   **My Opinion & Recommendation:** You are absolutely right to question this. With our previous refinement of `CreativityUrge` (from Category V) to be the "readiness/primed for inspiration" state, having a separate `InspirationCharge` becomes redundant. The `CreativityUrge` *is* the charge.
        *   **Revised Flow:**
            1.  `CreativityUrge` (Cat V) builds up for creative characters.
            2.  When `CreativityUrge` reaches a high threshold (e.g., >80-90), the character gains the `Status.PrimedForInspiration` tag.
            3.  Specific game events can then check for this tag and trigger an "Inspiration" GE.
            4.  The "Inspiration" GE applies its buff and then reduces/resets `CreativityUrge` (consuming the "charge").
    *   **Decision:** Remove `InspirationCharge`. `CreativityUrge` (from Category V) will serve this "charge-up" function.

5.  **`MagicPowerLevel` - Modifier for Talented Characters:**
    *   **Your Note:** *"I would like to use this as a modifier for specially magically talented characters."*
    *   **My Opinion & Recommendation:** This is a good use case. It acts as an innate talent score that can multiply the effectiveness of spells, determine access to higher-tier magic, or influence `MaxMana`/`ManaRegenRate`.
    *   **Decision:** Keep `MagicPowerLevel` with this focus.

6.  **Addiction & Tolerance - Placeholder/Generic Diminishing Returns:**
    *   **Your Notes:** *"Haven't really worked out the addiction system yet. Let's keep it as a placeholder for now."* and *"Maybe a generic diminishing returns system?"*
    *   **My Opinion & Recommendation:**
        *   **Addiction:** Keeping `AddictionProgress_[SubstanceName]` as a placeholder is fine. It's a common mechanic.
        *   **Tolerance:** A generic "DiminishingReturnsFactor" could work if you want to simplify. However, `ToleranceLevel_[SubstanceName]` allows different substances to have different tolerance build-up rates and effects, which is more granular.
            *   If you want very simple diminishing returns for *all* buffs, you could have a global character attribute like `BuffEffectivenessMultiplier` that slowly decreases with many active buffs, but this is different from substance tolerance.
            *   **Recommendation:** Keep `ToleranceLevel_[SubstanceName]` for now. It allows specific balancing per substance. If you find it too complex later, you can simplify. A "generic diminishing returns" on all GEs is a much broader and potentially more disruptive mechanic.
    *   **Decision:** Keep both `AddictionProgress_` and `ToleranceLevel_` as placeholders for now, acknowledging they need future design.

7.  **Meta Attributes & Replication/Hacking:**
    *   **Your Note:** *"Not replicating these couldn't open a path to dynamic memory hacking? Can you expand on how unreal handles this?"*
    *   **My Opinion & Explanation:**
        *   **Server Authority:** In a networked Unreal Engine game using GAS, the server is authoritative over all Gameplay Attribute changes and Gameplay Effect applications.
        *   **Execution Calculations (`UGameplayEffectExecutionCalculation`):** These C++ classes run *on the server* when a Gameplay Effect that uses an execution is applied.
            *   The "Meta Attributes" (like `IncomingDamage`) are defined within an AttributeSet.
            *   Inside the `Execute_Implementation` function of an ExecutionCalculation:
                1.  You "capture" attributes from the Source and Target (e.g., Target's `Health`, Source's `AttackPower`).
                2.  You perform your calculations.
                3.  You then "output" a calculated value to a Meta Attribute using `OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageAttribute, EGameplayModOp::Additive, CalculatedDamageValue));`.
                4.  Crucially, *immediately after this*, the Gameplay Effect system itself will take the value from that Meta Attribute (e.g., `CalculatedDamageValue` now "in" `DamageAttribute`) and apply it to a *real* attribute (e.g., `Health Additive -CalculatedDamageValue`).
        *   **Transient Nature:** The value in the Meta Attribute (`DamageAttribute`) only exists for the brief moment of this execution. It's not stored long-term on the ASC, and it's not intended to be replicated. The *effect* of its value (the change to `Health`) is what gets replicated if `Health` itself is a replicated attribute.
        *   **Client-Side Prediction:** Clients can predict the outcome of these executions for responsiveness, but the server has the final say. If a client tries to hack their `IncomingDamage` meta attribute locally, it won't affect the server's calculation. The server will calculate the correct damage and replicate the true `Health` change, overwriting any incorrect client prediction.
        *   **Why Not Replicate Meta Attributes?**
            *   **Network Traffic:** They change constantly during calculations; replicating them would be excessive.
            *   **State:** They don't represent a persistent state of the character. `Health` is a state. `IncomingDamage` for a single attack is a momentary calculation step.
        *   **Memory Hacking (Single Player):** In a purely single-player game without server authority, yes, a sufficiently skilled person could use tools like Cheat Engine to find the memory addresses of *any* variable, including temporary ones used in calculations, and try to modify them mid-execution. This is a general vulnerability of client-side applications. Not replicating meta attributes doesn't inherently make this easier or harder than hacking any other local variable. The protection in multiplayer comes from server authority. In single-player, you accept this risk or implement more complex anti-cheat measures if it's a major concern (though for a Rimworld-style game, it's usually less of a focus than for competitive multiplayer).
    *   **Conclusion:** The non-replication of meta attributes is standard GAS practice and is safe in a server-authoritative multiplayer context. For single-player, it doesn't fundamentally change the memory hacking landscape.

-


---------------------------------------------------------------------



Absolutely! That's a very wise question. Having a grand design is one thing; implementing it systematically without getting overwhelmed is another. Here's a logical implementation order, broken into phases, keeping in mind your current progress and the goal of getting playable loops as early as possible.

**Phase 0: Foundation (You are mostly here)**

1.  **Core Character Controller & GAS Integration (DONE/IN PROGRESS):**
    *   Locomotion, Camera, Pawn Selection.
    *   Base C++ classes for Character, PlayerState, ASC, AttributeSet.
    *   ASC set up on PlayerState (for player) and Pawn (for AI, or PlayerState if AI needs persistence).
    *   **Reasoning:** This is the absolute bedrock. Nothing else works without it.

**Phase 1: Core Vitals & Basic Interaction Loop**

1.  **AttributeSet: Core Vitality (Subset):**
    *   Implement `Health`, `MaxHealth`.
    *   **Reasoning:** The most fundamental state. Needed for any kind of damage or healing.
2.  **Basic UI: Health Display:**
    *   A simple health bar or text display for the selected character.
    *   **Reasoning:** Visual feedback is crucial for testing and understanding.
3.  **Initialization GE: Core Vitals:**
    *   Create a `GE_InitializeCoreVitals` to set initial `Health` and `MaxHealth`.
    *   **Reasoning:** Characters need starting stats.
4.  **Meta AttributeSet & Basic Damage Execution:**
    *   Implement `UAS_MetaCombat` with `IncomingDamage`.
    *   Create a simple `UGEExec_ApplyDamage` that takes `IncomingDamage` and subtracts it from `Health`.
    *   **Reasoning:** The simplest form of attribute modification.
5.  **Gameplay Ability: Basic "Debug Damage" / "Debug Heal":**
    *   A simple ability (can be triggered by a key press initially) that applies a GE with a fixed `IncomingDamage` (positive or negative) to the target.
    *   **Reasoning:** Allows you to test health changes and the damage execution.
6.  **AttributeSet: Basic Need (e.g., Hunger):**
    *   Implement `Hunger`, `MaxHunger`. Initialize it.
    *   **Reasoning:** Introduces the concept of decaying needs and replenishment.
7.  **Gameplay Effect: Passive Hunger Drain:**
    *   A periodic GE that slowly reduces `Hunger`.
    *   **Reasoning:** First example of a passive, time-based attribute change.
8.  **Gameplay Ability & Effect: "Eat Debug Food":**
    *   Ability that applies a GE to restore `Hunger`.
    *   **Reasoning:** Completes the basic need loop (decay and replenishment).
9.  **Basic UI: Hunger Display:**
    *   Simple hunger bar/text.
    *   **Reasoning:** Visual feedback for the need system.
10. **Animation: Basic Idle, Walk/Run (Placeholder is fine):**
    *   If not already done, ensure basic movement animations are playing.
    *   **Reasoning:** Makes the character feel alive during testing.
11. **Sound: Placeholder damage sound, eating sound.**
    *   **Reasoning:** Audio feedback enhances testing.

    *   **Goal of Phase 1:** Character can exist, take damage, heal (via debug), get hungry, and eat. Very basic "aliveness."

**Phase 2: Basic Combat Loop**

1.  **AttributeSet: Core Combat Stats (Subset):**
    *   `Accuracy`, `MeleeDamage_Bonus` (or just weapon damage initially).
    *   **Reasoning:** Foundation for combat calculations.
2.  **Weapon System (Basic):**
    *   Define a simple weapon data structure (e.g., struct or Data Table row) with `BaseDamage`.
    *   Ability to equip/represent a basic weapon on the character (even if just data).
    *   **Reasoning:** Combat usually involves weapons.
3.  **Gameplay Ability: Basic Melee Attack:**
    *   Costs (e.g., `Stamina` - implement `Stamina`/`MaxStamina` now if not done).
    *   Targets another character.
    *   Applies a GE that uses an `ExecutionCalculation`.
4.  **Execution Calculation: Melee Damage:**
    *   Reads weapon damage, attacker's `MeleeDamage_Bonus` (if any), calculates damage, applies to target `Health`.
    *   Later: Add accuracy checks using `Accuracy` vs. target's `Evasion_Melee` (implement `Evasion_Melee`).
    *   **Reasoning:** Core of combat logic.
5.  **AI (Rudimentary): "Hostile Dummy":**
    *   A simple AI character that can be targeted and take damage. Doesn't need to fight back yet.
    *   Use existing Character class, give it an ASC and core attributes.
    *   **Reasoning:** Need something to hit.
6.  **Animation: Basic Attack Animation (Melee):**
    *   A single, simple attack animation.
    *   Triggered by the attack ability.
    *   **Reasoning:** Visual feedback for combat actions.
7.  **VFX/Sound: Hit impact VFX, weapon swing sound.**
    *   **Reasoning:** Enhances combat feel.
8.  **UI: Target Frame (Basic):**
    *   Show selected target's name and health.
    *   **Reasoning:** Essential for understanding combat state.

    *   **Goal of Phase 2:** Player can attack and damage a target. Basic combat is possible.

**Phase 3: Expanding Needs & Basic AI Behavior**

1.  **More Needs Attributes:**
    *   `Rest`, `RecreationNeed`, `EnvironmentSatisfaction`.
    *   Implement their passive decay GEs.
    *   **Reasoning:** Flesh out the character's internal state.
2.  **Basic Objects for Need Fulfillment:**
    *   "Bed" object (placeholder model).
    *   "Fun Object" (placeholder model).
    *   Define basic "Comfort" values for areas/objects.
    *   **Reasoning:** Characters need ways to fulfill new needs.
3.  **Gameplay Abilities: Sleep, Use Fun Object:**
    *   Abilities that interact with these objects to restore `Rest` and `RecreationNeed`.
    *   Ability to interact with environment to update `EnvironmentSatisfaction`.
    *   **Reasoning:** Makes needs actionable.
4.  **AI Behavior (Basic Needs):**
    *   Simple Behavior Tree/State Machine logic:
        *   If `Hunger` is low, find "Food Source" (can be a designated spot for now) and play "Eat" ability.
        *   If `Rest` is low, find "Bed" and play "Sleep" ability.
    *   **Reasoning:** First steps towards autonomous AI based on attributes.
5.  **UI: More Need Displays:**
    *   Bars/icons for new needs.
    *   **Reasoning:** Player needs to manage these.
6.  **World Building Assets (Basic):**
    *   Simple placeable walls, floors, doors, beds, food replicator.
    *   **Reasoning:** To create a basic test environment for needs and AI.

    *   **Goal of Phase 3:** Characters have a wider range of needs, and AI can autonomously attempt to satisfy some of them. The world starts to feel more like a simulation.

**Phase 4: Skills & Crafting/Work Loop**

1.  **AttributeSet: Core Skills (Subset):**
    *   `CookingSkill`, `ConstructionSkill`.
    *   `LearningRate_Global`.
    *   **Reasoning:** Introduce skill progression and its impact on tasks.
2.  **XP & Leveling System (Basic):**
    *   Implement `Experience` and `Level` attributes.
    *   Basic XP gain from performing skilled actions.
    *   A simple level-up effect (e.g., +1 to a chosen core stat or skill point).
    *   **Reasoning:** Core progression loop.
3.  **Workstation Actors (Basic):**
    *   "Stove" actor, "Construction Bench" actor.
    *   **Reasoning:** Locations for performing skilled tasks.
4.  **Gameplay Abilities: Cook Meal, Build Wall:**
    *   These abilities now check skill level.
    *   Higher skill = faster action, better result (e.g., "Nutritious Meal" vs. "Mush," "Sturdy Wall" vs. "Shaky Wall").
    *   Grant skill XP on completion.
    *   **Reasoning:** Connects skills to tangible outcomes.
5.  **Inventory System (Basic):**
    *   Ability for characters to hold items (raw food, wood, crafted meals, tools).
    *   Represented by data, UI display.
    *   **Reasoning:** Crafting requires resources.
6.  **Resource Spawners (Basic):**
    *   Placeable actors that generate "Raw Food" or "Wood" items.
    *   **Reasoning:** Source of materials for crafting.
7.  **AI Behavior (Basic Work):**
    *   If food stores are low and AI has `CookingSkill`, go to Stove and "Cook Meal."
    *   If player designates a "Wall Blueprint," AI with `ConstructionSkill` goes and "Builds Wall."
    *   **Reasoning:** AI contributing to the colony/base.
8.  **UI: Inventory, Skill List/Levels, Crafting Queue:**
    *   **Reasoning:** Player needs to manage resources, see character progression, and assign tasks.

    *   **Goal of Phase 4:** Characters can learn skills, craft basic items, and build. The game gets a core production/management loop.

**Phase 5: Expanding Combat & Social**

1.  **More Combat Attributes & Resistances:**
    *   Armor, Evasion, Crit Chance, Resistances (Fire, Cold).
    *   **Reasoning:** Add depth to combat.
2.  **Equipment System (Armor & More Weapons):**
    *   Data for different armor types (with armor/resistance values) and more weapon types.
    *   Visual representation of equipped gear.
    *   **Reasoning:** Player choice and progression in combat.
3.  **Ranged Combat:**
    *   Ranged weapon abilities, ammo (optional), aiming mechanics.
    *   Animations for ranged attacks.
    *   **Reasoning:** Significant expansion of combat options.
4.  **AI (Combat):**
    *   Basic enemy AI that can pathfind, attack the player/allies, use cover (rudimentary).
    *   Friendly AI that can defend.
    *   Faction system (basic: Hostile, Neutral, Friendly) to determine targets.
    *   **Reasoning:** Make combat challenging and interactive.
5.  **Social Attributes (Subset):**
    *   `Mood`, `Charisma`, `OpinionOf`.
    *   **Reasoning:** Foundation for social interaction.
6.  **Gameplay Abilities: Basic Social Interactions:**
    *   "Friendly Chat" ability. Modifies `OpinionOf` and `Mood` based on `Charisma`.
    *   **Reasoning:** First step in characters forming relationships.
7.  **UI: Mood/Thoughts Display, Relationship Panel (Basic):**
    *   **Reasoning:** Player needs to understand character psychology and social dynamics.

    *   **Goal of Phase 5:** More engaging combat, characters start to have personalities and relationships.

**Subsequent Phases: Iteration and Expansion**

*   **Advanced AI:** More complex behaviors, job priorities, schedules, recreation choices.
*   **Story/Event System:** Random events, quests, narrative elements.
*   **More Content:** More skills, items, buildings, enemies, biomes, traits.
*   **Advanced UI/UX:** Polish, tutorials, detailed management screens.
*   **Art & Audio Polish:** Replace placeholders with final assets, add more sound variety, music.
*   **Optimization & Bug Fixing:** Continuous throughout.
*   **Specific Systems:** Magic, advanced medical, research tree, detailed environment interaction, etc., based on your game's unique features.

**General Principles for Prioritization:**

*   **Playable Loops First:** Get a core gameplay loop (e.g., survive > satisfy need > improve) working ASAP, even with placeholder assets.
*   **Vertical Slices:** Sometimes it's better to implement one feature deeply (e.g., combat with several mechanics) rather than many features shallowly. But early on, breadth for core loops is good.
*   **Dependencies:** Implement systems that other systems depend on first (e.g., attributes before abilities that use them).
*   **Feedback:** The sooner you can play and test, the sooner you can get feedback and adjust course.
*   **Placeholder Everything:** Don't get bogged down in perfect art or sound early. Use simple shapes, basic sounds, and debug text.

This is a high-level roadmap. You'll inevitably adjust it as you go, but it should provide a logical sequence to tackle the mountain of work ahead!





------------------------------------------------------------------------










**Project Root (`Content/`)**

```
Content/
│
├── Blueprints/  (Or your game's main BP folder, e.g., `MyGame/Blueprints/`)
│   │
│   ├── Abilities/  (Gameplay Abilities - GA)
│   │   ├── GA_Combat/
│   │   │   ├── GA_Combat_Melee/
│   │   │   │   ├── GA_Melee_SwordAttack.uasset
│   │   │   │   └── GA_Melee_UnarmedStrike.uasset
│   │   │   ├── GA_Combat_Ranged/
│   │   │   │   └── GA_Ranged_PistolShot.uasset
│   │   │   └── GA_Combat_Shared/
│   │   │       └── GA_Combat_Dodge.uasset
│   │   │
│   │   ├── GA_Needs/
│   │   │   ├── GA_Need_EatFood.uasset
│   │   │   ├── GA_Need_DrinkWater.uasset
│   │   │   └── GA_Need_Sleep.uasset
│   │   │
│   │   ├── GA_Work/
│   │   │   ├── GA_Work_BuildStructure.uasset
│   │   │   ├── GA_Work_CookMeal.uasset
│   │   │   └── GA_Work_MineResource.uasset
│   │   │
│   │   ├── GA_Social/
│   │   │   ├── GA_Social_FriendlyChat.uasset
│   │   │   └── GA_Social_Recruit.uasset
│   │   │
│   │   ├── GA_Magic/ (If applicable)
│   │   │   └── GA_Magic_Fireball.uasset
│   │   │
│   │   └── GA_Utility/ (Passives, toggles, non-categorized)
│   │       └── GA_Passive_FastLearner.uasset
│   │
│   ├── Effects/ (Gameplay Effects - GE)
│   │   ├── GE_CoreAttributes/
│   │   │   ├── GE_Init_BaseStats.uasset
│   │   │   ├── GE_Damage_Physical.uasset (Could be an Instant GE using an Exec)
│   │   │   └── GE_Heal_Base.uasset
│   │   │
│   │   ├── GE_Combat/
│   │   │   ├── GE_Combat_Buffs/
│   │   │   │   └── GE_Buff_AttackPowerUp.uasset
│   │   │   ├── GE_Combat_Debuffs/
│   │   │   │   └── GE_Debuff_AccuracyDown.uasset
│   │   │   ├── GE_Combat_DamageTypes/ (If using specific GEs for damage types)
│   │   │   │   ├── GE_Damage_FireDOT.uasset
│   │   │   │   └── GE_Damage_PoisonDOT.uasset
│   │   │   └── GE_Combat_Costs/
│   │   │       └── GE_Cost_Stamina_MeleeAttack.uasset
│   │   │
│   │   ├── GE_Needs/
│   │   │   ├── GE_Need_HungerDrain_Passive.uasset
│   │   │   ├── GE_Need_HungerRestore_BasicMeal.uasset
│   │   │   ├── GE_Need_ThirstDrain_Passive.uasset
│   │   │   └── GE_Need_SleepDeprivation_Debuff.uasset
│   │   │
│   │   ├── GE_Mood/ (For "Thoughts" / Moodlets)
│   │   │   ├── GE_Thought_AteFineMeal_Buff.uasset
│   │   │   ├── GE_Thought_SleptOnFloor_Debuff.uasset
│   │   │   ├── GE_Thought_FriendDied_Debuff_Major.uasset
│   │   │   └── GE_Thought_ComfortableEnvironment_Buff.uasset
│   │   │
│   │   ├── GE_Stress/
│   │   │   ├── GE_Stress_Gain_LowMood.uasset
│   │   │   └── GE_Stress_Relief_Recreation.uasset
│   │   │
│   │   ├── GE_Skills/
│   │   │   ├── GE_Skill_XP_Gain_Construction.uasset (Could be generic with params)
│   │   │   └── GE_Skill_Inspired_Crafting.uasset
│   │   │
│   │   ├── GE_Environmental/
│   │   │   ├── GE_Env_Hypothermia_Debuff.uasset
│   │   │   └── GE_Env_Heatstroke_Debuff.uasset
│   │   │
│   │   └── GE_Utility/ (Passives, initialization not covered elsewhere)
│   │       └── GE_Passive_Trait_Optimist.uasset
│   │
│   ├── Cues/ (Gameplay Cues - GC - for non-cosmetic effects that need replication, or cosmetic if preferred)
│   │   ├── GC_Combat/
│   │   │   ├── GC_Combat_HitImpact_Flesh.uasset
│   │   │   └── GC_Combat_HealPulse.uasset
│   │   └── GC_Needs/
│   │       └── GC_Needs_AteFood_Sound.uasset
│   │
│   ├── Characters/
│   │   ├── BP_Character_Base.uasset
│   │   ├── BP_PlayerCharacter.uasset
│   │   └── BP_AICharacter_Colonist.uasset
│   │
│   ├── Player/
│   │   ├── BP_PlayerState_Base.uasset
│   │   ├── BP_PlayerController_Base.uasset
│   │   └── BP_HUD_Main.uasset
│   │
│   ├── AI/
│   │   ├── BP_AIController_Base.uasset
│   │   ├── BT_ (Behavior Trees)
│   │   │   ├── BT_Colonist_Idle.uasset
│   │   │   └── BT_Colonist_Work.uasset
│   │   └── Blackboard/
│   │       └── BB_Colonist.uasset
│   │
│   ├── UI/
│   │   ├── Widgets/
│   │   │   ├── WBP_HealthBar.uasset
│   │   │   ├── WBP_NeedsPanel.uasset
│   │   │   └── WBP_CharacterSheet.uasset
│   │   └── Themes/ (Fonts, styles)
│   │
│   ├── Items/
│   │   ├── BP_Item_Base.uasset
│   │   ├── Weapons/
│   │   │   └── BP_Weapon_Sword.uasset
│   │   ├── Armor/
│   │   │   └── BP_Armor_Chestplate.uasset
│   │   └── Consumables/
│   │       └── BP_Consumable_FoodRation.uasset
│   │
│   └── World/
│       ├── BP_Workstation_Stove.uasset
│       └── BP_ResourceNode_IronOre.uasset
│
├── Cplusplus/ (Or your game's main C++ folder, e.g., `MyGame/Source/MyGame/`)
│   │
│   ├── Public/ (Header files .h)
│   │   ├── AbilitySystem/
│   │   │   ├── Attributes/
│   │   │   │   ├── MyAttributeSet_Base.h
│   │   │   │   ├── MyAttributeSet_CoreVitality.h (Health, Stamina, Mana, etc.)
│   │   │   │   ├── MyAttributeSet_CombatStats.h (Accuracy, DamageBonus, etc.)
│   │   │   │   ├── MyAttributeSet_Needs.h (Hunger, Thirst, Rest, etc.)
│   │   │   │   ├── MyAttributeSet_MentalState.h (Mood, Stress, etc.)
│   │   │   │   ├── MyAttributeSet_Skills.h (All Category VII skills)
│   │   │   │   ├── MyAttributeSet_MetaCalculations.h (IncomingDamage, etc.)
│   │   │   │   └── ... (Other Attribute Sets as needed)
│   │   │   │
│   │   │   ├── Abilities/
│   │   │   │   ├── MyGameplayAbility_Base.h
│   │   │   │   └── ... (Base C++ classes for specific ability types if needed)
│   │   │   │
│   │   │   ├── Effects/
│   │   │   │   ├── MyGameplayEffectExecutionCalculation_Base.h
│   │   │   │   ├── MyExecCalc_Damage.h
│   │   │   │   └── ... (Other custom executions)
│   │   │   │
│   │   │   ├── MyAbilitySystemComponent.h
│   │   │   └── MyGameplayCueManager.h (If you customize cue manager)
│   │   │
│   │   ├── Characters/
│   │   │   ├── MyCharacter_Base.h
│   │   │   └── ...
│   │   │
│   │   ├── Player/
│   │   │   ├── MyPlayerState.h
│   │   │   └── MyPlayerController.h
│   │   │
│   │   ├── Items/
│   │   │   └── ... (Base C++ classes for items if complex logic)
│   │   │
│   │   └── Systems/
│   │       ├── FactionManager.h
│   │       └── OpinionManagerComponent.h (Could be a component on PlayerState/Character)
│   │
│   └── Private/ (Source files .cpp)
│       │   (Mirrors Public folder structure for .cpp files)
│       ├── AbilitySystem/
│       │   ├── Attributes/
│       │   ├── Abilities/
│       │   └── Effects/
│       │
│       └── ... (Rest of the structure)
│
├── Data/
│   ├── DT_CharacterBaseStats.uasset (Data Table for initializing attributes)
│   ├── DT_WeaponStats.uasset
│   ├── DT_ArmorStats.uasset
│   ├── DT_FoodStats.uasset
│   ├── DT_XPCurves.uasset
│   └── Curve_Float_MoodToStressFactor.uasset (Example Curve Asset)
│
├── Art/ (Or `Meshes/`, `Textures/`, `Materials/` separately or combined)
│   ├── Characters/
│   │   ├── Player/ (Meshes, Textures, Materials)
│   │   └── AI/
│   │       └── Colonist/
│   ├── Weapons/
│   ├── Environment/
│   │   ├── Props/
│   │   └── Structures/
│   └── UI/ (Icons, backgrounds)
│
├── Animations/
│   ├── Player/
│   │   ├── AnimBP_Player.uasset
│   │   ├── Locomotion/
│   │   └── Combat/
│   └── AI/
│       └── Colonist/
│           ├── AnimBP_Colonist.uasset
│           └── ...
│
├── Audio/
│   ├── SFX/
│   │   ├── Combat/
│   │   ├── UI/
│   │   └── Environment/
│   └── Music/
│
└── Maps/
    ├── MAP_MainMenu.umap
    └── MAP_MainGameplayLevel.umap
```

**Key Principles of this Structure:**

1.  **Modularity by Feature/System:** Top-level folders like `Abilities`, `Effects`, `Characters`, `Items` group related assets.
2.  **Type-Based Subfolders:** Within feature folders, further subdivide by asset type (e.g., `GA_Combat` for combat abilities, `GE_Needs` for need-related effects).
3.  **Specificity:** The deeper you go, the more specific the category. `GA_Combat_Melee` is more specific than `GA_Combat`.
4.  **Consistency:** Use consistent naming conventions (e.g., prefixing with `GA_`, `GE_`, `BP_`, `WBP_`, `DT_`, `SK_`, `SM_`, `T_`, `M_`).
5.  **C++ Parity:** Your C++ folder structure should ideally mirror the conceptual organization of your Blueprints where logical, especially for GAS components.
6.  **Data Centralization:** Keep Data Tables and Curve Assets in a `Data` folder for easy access and modification.
7.  **Art/Audio Separation:** Standard practice to keep raw art and audio assets separate from Blueprint/C++ logic.
8.  **"Shared" or "Utility" Folders:** For assets that are used across multiple sub-categories or don't fit neatly (e.g., a generic "Apply Knockback" GA, a passive "Trait" GE).


**Important Considerations:**

*   **Source Control:** This structure works well with source control (Git, Perforce). Clear organization helps avoid conflicts and makes it easier to see who is working on what.
*   **Iteration:** This is a starting point. As your project grows, you might find reasons to add more subfolders or reorganize slightly. Don't be afraid to refactor your folder structure if it starts to feel unwieldy, but do it carefully.
*   **Team Size:** For solo or small teams, this level of detail is good. For very large teams, even more granular breakdowns might be needed within these.
*   **Engine Conventions:** Try to align with common Unreal Engine naming and folder conventions where it makes sense, as it helps new team members or if you look at example projects.
