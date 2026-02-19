// Copyright Epic Games, Inc. All Rights Reserved.

#include "HungryHungryDinoCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "HungryHungryDino.h"
#include "GrowthComponent.h"
#include "EatingComponent.h"
#include "Curves/CurveFloat.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AHungryHungryDinoCharacter::AHungryHungryDinoCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AHungryHungryDinoCharacter::BeginPlay()
{
	Super::BeginPlay();

	// capture baseline values before any scaling happens
	if (CameraBoom)
	{
		BaseArmLength = CameraBoom->TargetArmLength;
	}

	if (GetCharacterMovement())
	{
		BaseStepHeight = GetCharacterMovement()->MaxStepHeight;
		BaseWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	}

	if (GetCapsuleComponent())
	{
		BaseHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
		BaseRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	}

	UE_LOG(LogTemplateCharacter, Log,
		TEXT("scaling baseline: arm=%.1f step=%.1f half=%.1f radius=%.1f"),
		BaseArmLength, BaseStepHeight, BaseHalfHeight, BaseRadius);
}

void AHungryHungryDinoCharacter::SetScale(float NewScale)
{
	// only place that calls SetActorScale3D — all growth goes through here
	SetActorScale3D(FVector(NewScale, NewScale, NewScale));

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxStepHeight = BaseStepHeight * NewScale;

		// use speed curve if set, otherwise linear
		const float SpeedMult = SpeedScaleCurve
			? SpeedScaleCurve->GetFloatValue(NewScale)
			: NewScale;
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed * SpeedMult;
	}

	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = BaseArmLength * NewScale;
	}

	CurrentScale = NewScale;
}

void AHungryHungryDinoCharacter::DebugGrow()
{
	if (UGrowthComponent* GC = FindComponentByClass<UGrowthComponent>())
	{
		GC->AddGrowth(1.0f);
	}
}

void AHungryHungryDinoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// debug: press G to grow (remove after testing)
	PlayerInputComponent->BindKey(EKeys::G, IE_Pressed, this, &AHungryHungryDinoCharacter::DebugGrow);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHungryHungryDinoCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AHungryHungryDinoCharacter::Look);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHungryHungryDinoCharacter::Look);
	}
	else
	{
		UE_LOG(LogHungryHungryDino, Error, TEXT("'%s' failed to find enhanced input component"), *GetNameSafe(this));
	}
}

void AHungryHungryDinoCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void AHungryHungryDinoCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AHungryHungryDinoCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AHungryHungryDinoCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController())
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AHungryHungryDinoCharacter::DoJumpStart()
{
	Jump();
}

void AHungryHungryDinoCharacter::DoJumpEnd()
{
	StopJumping();
}
