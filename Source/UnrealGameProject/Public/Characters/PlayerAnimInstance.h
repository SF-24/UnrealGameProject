// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CharacterTypes.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALGAMEPROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	class AGameCharacter* PlayerCharacter;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* CharacterMovementComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeedSquared;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool HasVerticalVelocity;
};
