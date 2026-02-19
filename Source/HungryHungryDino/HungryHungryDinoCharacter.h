// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HungryHungryDinoCharacter.generated.h"

class UCurveFloat;
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class AHungryHungryDinoCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	UCameraComponent* FollowCamera;

protected:

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	// scaling baselines — captured at BeginPlay, used by SetScale()
	UPROPERTY(BlueprintReadOnly, Category="Scaling", meta=(AllowPrivateAccess="true"))
	float BaseArmLength = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Scaling", meta=(AllowPrivateAccess="true"))
	float BaseStepHeight = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Scaling", meta=(AllowPrivateAccess="true"))
	float BaseHalfHeight = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Scaling", meta=(AllowPrivateAccess="true"))
	float BaseRadius = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category="Scaling", meta=(AllowPrivateAccess="true"))
	float BaseWalkSpeed = 0.0f;

	// optional curve: X = scale, Y = speed multiplier (if unset, speed scales linearly)
	UPROPERTY(EditAnywhere, Category="Scaling")
	UCurveFloat* SpeedScaleCurve = nullptr;

	// current scale factor, updated only by SetScale()
	UPROPERTY(BlueprintReadOnly, Category="Scaling", meta=(AllowPrivateAccess="true"))
	float CurrentScale = 1.0f;

public:

	AHungryHungryDinoCharacter();

protected:

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:

	// sole entry point for changing scale — updates actor, capsule, camera, speed
	UFUNCTION(BlueprintCallable, Category="Scaling")
	virtual void SetScale(float NewScale);

	// debug: press G to grow (remove after testing)
	void DebugGrow();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
