// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainCharController.generated.h"

class AMainChar;

UCLASS()
class LHW_API AMainCharController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainCharController();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	// Called to bind functionality to input
	void SetupInputComponent() override;

private:
	AMainChar* mainChar;

	float turnRate;

	bool bMoveForward;

	bool bMoveRight;

	float charWalkSpeed;

	float charRunSpeed;

	float charSprintSpeed;

	float charNormalMaxAcceleration;

	float charHitMaxAcceleration;

	float maxMinAimRotate;

private:
	UFUNCTION()
	void MoveForward(float _input);

	UFUNCTION()
	void MoveRight(float _input);

	UFUNCTION()
	void LookUpRate(float _input);

	UFUNCTION()
	void LookUpRightRate(float _input);

	UFUNCTION()
	void Jump();

	UFUNCTION()
	void DashStart();

	UFUNCTION()
	void SilentWalkStart();

	UFUNCTION()
	void SilentWalkEnd();

	UFUNCTION()
	void SprintStart();

	UFUNCTION()
	void SprintEnd();

	UFUNCTION()
	void Crouch();

	UFUNCTION()
	void MeleeAttackOrCharging();

	UFUNCTION()
	void Shooting();

	UFUNCTION()
	void DefenseOrChargeStart();

	UFUNCTION()
	void DefenseOrChargeEnd();

	UFUNCTION()
	void UltimateAttack();

	UFUNCTION()
	void TargetLockOn();

	UFUNCTION()
	void TargetLockOnLeft();

	UFUNCTION()
	void TargetLockOnRight();

	UFUNCTION()
	void WeaponSwap();

	UFUNCTION()
	void Recovery();

	UFUNCTION()
	void AttackAvoid();

	UFUNCTION()
	void Menu();

	UFUNCTION()
	void Cheat();

public:
	FORCEINLINE bool GetMoveForward()const { return bMoveForward; }
	FORCEINLINE bool GetMoveRight()const { return bMoveRight; }
	FORCEINLINE float GetCharWalkSpeed()const { return charWalkSpeed; }
	FORCEINLINE float GetCharRunSpeed()const { return charRunSpeed; }
	FORCEINLINE float GetCharSprintSpeed()const { return charSprintSpeed; }
	FORCEINLINE float GetCharNormalMaxAcceleration()const { return charNormalMaxAcceleration; }
	FORCEINLINE float GetCharHitMaxAcceleration()const { return charHitMaxAcceleration; }
};
