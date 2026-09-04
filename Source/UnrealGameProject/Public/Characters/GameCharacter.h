// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Characters/CharacterTypes.h"
#include "GameCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;
class AWeapon;
class UAnimMontage;

UCLASS()
class UNREALGAMEPROJECT_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGameCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* PlayerInputContext;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MovementInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookInputAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* EquipInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AttackInputAction;
	
	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Input")
	UInputAction* SecondaryAttackInputAction;
	
protected:
	virtual void BeginPlay() override;
	/**
	 * Handle Input Functions 
	 */
	void Move(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void InteractKeyPressed(const FInputActionValue& Value);
	void DisarmKeyPressed(const FInputActionValue& Value);
	void StartAttack(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void StartStrongAttack(const FInputActionValue& Value);
	void StrongAttack(const FInputActionValue& Value);

	/**
	 * Handle Attack Montages: 
	 */
	void HandleAttackMontage(EAttackType AttackType, bool bIsAutoAttack);
	void HandleEquipMontage(FName SectionName);

	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void AttackEndBlockAutoAttack();
	bool CannotAutoAttack() const;
	bool CanDisarm() const;
	bool CanArm() const;
	bool CannotAttack() const;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "State")
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	EActionState ActionState = EActionState::EAS_Unoccupied;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	
	UPROPERTY(VisibleAnywhere)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	AWeapon* EquippedWeapon;
	
	/*
	 * Animation montages:
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* EquipMontage;
	
	UPROPERTY()
	uint8 AttackComboSection = 0;
	
public:
	void SetOverlappingItem(AItem* Item);
	ECharacterState GetCharacterState() const;
	UFUNCTION(BlueprintCallable, Category = "State")
	EActionState GetActionState() const;
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetActionState(EActionState NewActionState);
};
