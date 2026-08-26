// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Components/SphereComponent.h"

// Sets default values
AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the mesh component and attach to the root.
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	RootComponent = ItemMesh;
	
	// Attach a sphere to the root.
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	// Attach the sphere overlap function and bind it to the delegate.
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	
	// Bind the end overlap as a delegate to the sphere component
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin() const
{
	return Amplitude * FMath::Sin(RunningTime*TimeConstant);
}

float AItem::TransformedCos() const
{
	return Amplitude * FMath::Cos(RunningTime*TimeConstant);
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1,5.f, FColor::Red, "Overlapped: " + OtherActor->GetName());
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1,5.f, FColor::Green, "EndOverlap: " + OtherActor->GetName());
	}
}


// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime+=DeltaTime;

	float DeltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant); // Calculate difference in Z axis. Period = 2*pi/K
	AddActorWorldOffset(FVector(0.f,0.f,DeltaZ));
}

