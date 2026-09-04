// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/GameCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"

AGameCharacter::AGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// 420 = default, was 650, then 520
	GetCharacterMovement()->JumpZVelocity = 520.0f;
	// Allows moving the character while jumping. Default: 0.05, then 0.5
	GetCharacterMovement()->AirControl = 0.5f;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f); // Was initially 360 degrees
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.0f;
	
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(FName("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
}

void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerInputContext, 0);
		}
	}
	
}

void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGameCharacter::Move(const FInputActionValue& Value)
{
	if (ActionState==EActionState::EAS_Attacking || ActionState==EActionState::EAS_UnoccupiedDisableAutoAttack)
	{
		return;
	}

	const FVector2D MoveVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	// Forward vector the controller is pointing in
	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardVector, MoveVector.Y); // Forward, Y Axis
	
	// Strafing, by getting sidewards direction from the controller yaw rotation
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightVector, MoveVector.X);   // Right, X Axis
}

void AGameCharacter::Jump(const FInputActionValue& Value)
{
	if (ActionState==EActionState::EAS_Attacking || ActionState==EActionState::EAS_UnoccupiedDisableAutoAttack)
	{
		return;
	}
	
	ACharacter::Jump();
}

void AGameCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void AGameCharacter::InteractKeyPressed(const FInputActionValue& Value)
{
	// If an item is equipped, unequip it.
	if (EquippedWeapon)
	{
		EquippedWeapon->Unequip();
		EquippedWeapon=nullptr;
	}
	
	// Pickup the item pickup
	if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
	{
		EquippedWeapon = OverlappingWeapon;
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingWeapon = nullptr;
	}
}

void AGameCharacter::DisarmKeyPressed(const FInputActionValue& Value)
{
	if (CanDisarm())
	{
		HandleEquipMontage("Unequip");
		CharacterState=ECharacterState::ECS_Unequipped;
		OverlappingItem=nullptr;
	}
	else if (CanArm())
	{
		HandleEquipMontage("Equip");
		CharacterState=ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem=EquippedWeapon;
	}
}

void AGameCharacter::StartAttack(const FInputActionValue& Value)
{
	HandleAttackMontage(EAttackType::AT_MeleeNormal, false);
}

void AGameCharacter::Attack(const FInputActionValue& Value)
{
	HandleAttackMontage(EAttackType::AT_MeleeNormal, true);
}

void AGameCharacter::StartStrongAttack(const FInputActionValue& Value)
{
	HandleAttackMontage(EAttackType::AT_MeleeStrong, false);
}

void AGameCharacter::StrongAttack(const FInputActionValue& Value)
{
	HandleAttackMontage(EAttackType::AT_MeleeStrong, true);
}

void AGameCharacter::HandleAttackMontage(EAttackType AttackType, bool bIsAutoAttack)
{
	// If in an invalid state, cannot attack.
	if (CannotAttack() || (bIsAutoAttack&&CannotAutoAttack()))
	{
		return;
	}
	// Set attacking state
	ActionState = EActionState::EAS_Attacking;
	
	// Handle the animation, based on attack type.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	// Return if the null check fails.
	if (!AnimInstance || !AttackMontage)
	{
		return;
	}
	
	FName SectionName = FName();
	AnimInstance->Montage_Play(AttackMontage, 1.5f);
	// const uint8 AttackSelection = FMath::RandRange(0,1);
		
	switch (AttackType)
	{
	case EAttackType::AT_MeleeNormal:
		switch (FMath::RandRange(0,1))
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			return;
		}
		break;
	case EAttackType::AT_MeleeStrong:
		SectionName = FName("Attack3");
		break;
	default: 
		return;
	}
	AnimInstance->Montage_JumpToSection(SectionName);
}

void AGameCharacter::HandleEquipMontage(FName SectionName)
{
	if (EquipMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(EquipMontage, 1.0f);
			AnimInstance->Montage_JumpToSection(SectionName,EquipMontage);
		}
	}
}

// Make the function inline (compile with actual code at runtime)
FORCEINLINE void AGameCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

ECharacterState AGameCharacter::GetCharacterState() const
{
	return CharacterState;
}

EActionState AGameCharacter::GetActionState() const
{
	return ActionState;
}

void AGameCharacter::SetActionState(EActionState NewActionState)
{
	ActionState = NewActionState;
}

void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MovementInputAction, ETriggerEvent::Triggered, this, &AGameCharacter::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AGameCharacter::Look);
		
		// Trigger Jump when pressed
	    EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &AGameCharacter::Jump);
		
		// Trigger equip when pressed
		EnhancedInputComponent->BindAction(EquipInputAction, ETriggerEvent::Triggered, this, &AGameCharacter::InteractKeyPressed);
		
		// Trigger on attack
		EnhancedInputComponent->BindAction(AttackInputAction, ETriggerEvent::Started, this, &AGameCharacter::StartAttack);
		EnhancedInputComponent->BindAction(AttackInputAction, ETriggerEvent::Triggered, this, &AGameCharacter::Attack);
		EnhancedInputComponent->BindAction(SecondaryAttackInputAction, ETriggerEvent::Started, this, &AGameCharacter::StartStrongAttack);
		EnhancedInputComponent->BindAction(SecondaryAttackInputAction, ETriggerEvent::Triggered, this, &AGameCharacter::StrongAttack);
	}
}

void AGameCharacter::AttackEnd() {
	ActionState = EActionState::EAS_Unoccupied;
}

void AGameCharacter::AttackEndBlockAutoAttack() {
	if (ActionState == EActionState::EAS_Attacking)
	{
		ActionState = EActionState::EAS_UnoccupiedDisableAutoAttack;
	}
}

bool AGameCharacter::CannotAttack() const
{
	return (ActionState != EActionState::EAS_Unoccupied && ActionState != EActionState::EAS_UnoccupiedDisableAutoAttack) || GetCharacterState() == ECharacterState::ECS_Unequipped;
}

bool AGameCharacter::CannotAutoAttack() const
{
	return ActionState != EActionState::EAS_Unoccupied || GetCharacterState() == ECharacterState::ECS_Unequipped;
}

bool AGameCharacter::CanDisarm() const
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState!=ECharacterState::ECS_Unequipped;
}

bool AGameCharacter::CanArm() const
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState!=ECharacterState::ECS_EquippedOneHandedWeapon && EquippedWeapon;
}

