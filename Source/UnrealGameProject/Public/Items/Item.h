// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGizmos/GizmoElementShared.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

// Uint8, so no negative values are used.
enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class UNREALGAMEPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintPure)	
	float TransformedSin() const;
	UFUNCTION(BlueprintPure)
	float TransformedCos() const;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Mesh")
	UStaticMeshComponent* ItemMesh;
	
	EItemState ItemState = EItemState::EIS_Hovering;
	
private:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"), Category="Mesh")
	float RunningTime;
	
	UPROPERTY(EditAnywhere,Category="Trig Parameters")
	float Amplitude = 0.5f; //0.25f;
	UPROPERTY(EditAnywhere,Category="Trig Parameters")
	float TimeConstant = 2.5f; // was 5.f
	
	UPROPERTY(VisibleAnywhere, Category="Mesh")
	USphereComponent* SphereComponent;

};
