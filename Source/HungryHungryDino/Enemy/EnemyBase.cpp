// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Hide skeletal mesh
	GetMesh()->SetVisibility(false);
	GetMesh()->SetHiddenInGame(true);

	// Disable collision
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Optional: detach it
	GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

	// Create static mesh
	StaticCharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	// Attach to capsule (root)
	StaticCharacterMesh->SetupAttachment(GetRootComponent());

	// Adjust transform if needed
	StaticCharacterMesh->SetRelativeLocation(FVector(0.f, 0.f, -70.f));
	StaticCharacterMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

