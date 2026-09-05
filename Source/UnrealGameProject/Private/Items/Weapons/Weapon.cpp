// Fill out your copyright notice in the Description page of Project Settings.
 
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

AWeapon::AWeapon()
{
	WeaponHitBox = CreateDefaultSubobject<UBoxComponent>(FName("WeaponHitBox"));
	WeaponHitBox->SetupAttachment(GetRootComponent());
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::AttackMeshToSocket(USceneComponent* InParent, FName InSocketName)
{
	if (InParent) {
		ItemMesh->AttachToComponent(InParent,FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), InSocketName);		
	}
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	AttackMeshToSocket(InParent, InSocketName);
	ItemState=EItemState::EIS_Equipped;
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),EquipSound,GetActorLocation());
	}
	if (SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::Unequip()
{
	ItemMesh->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule(EAttachLocation::KeepWorldPosition),false));
	ItemState=EItemState::EIS_Hovering;
}
