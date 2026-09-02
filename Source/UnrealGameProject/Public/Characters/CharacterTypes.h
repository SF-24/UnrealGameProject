#pragma once


UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One-Handed Weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_UnoccupiedDisableAutoAttack UMETA(DisplayName = "Unoccupied (Cannot Auto-attack)"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	AT_MeleeNormal UMETA(DisplayName="Normal Melee"),
	AT_MeleeStrong UMETA(DisplayName="Strong Melee"),
};