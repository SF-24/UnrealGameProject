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
	
protected:
	virtual void BeginPlay() override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void InteractKeyPressed(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true), Category = "State")
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	
	UPROPERTY(VisibleAnywhere)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere)
	AItem* EquippedItem;
	
	/*
	 * Animation montages:
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Montages")
	UAnimMontage* AttackMontage;
	
	UPROPERTY()
	uint8 AttackComboSection = 0;
	
public:
	void SetOverlappingItem(AItem* Item);
	ECharacterState GetCharacterState() const;
};
