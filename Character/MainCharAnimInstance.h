// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ECharState.h"
#include "MainCharAnimInstance.generated.h"

class AMainChar;
class AMainCharController;
class UAnimMontage;
class UAnimInstance;
class UCharStatusComponent;
class UProjectGameInstance;

UCLASS()
class LHW_API UMainCharAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	void NativeInitializeAnimation()override;

	void NativeBeginPlay()override;

private:
	UAnimInstance* aniInstance;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	AMainCharController* mainCharController;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	APawn* pawn;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	AMainChar* mainChar;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float moveSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float dir;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float aimRotate;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float maxWalkSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float maxRunSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsinAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bMoveForward;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bMoveRight;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	UCharStatusComponent* charStatus;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EKatanaState_Type katanaState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EBowState_Type bowState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EWeaponState_Type weaponState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "GameIS", meta = (AllowPrivateAccess = "true"))
	UProjectGameInstance* gameIS;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* dashMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* attackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* defenseRootMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* defenseInplaceMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* stealthMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* skillMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* weaponSwapMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* bowAttackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* takeDamageMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* takeDownDamageFrontMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* takeDownDamageBackMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* defenseSuccessMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* parryingSuccessMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* executionMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* attackAvoidMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* dieMontage;

public:
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateAnimationProperties();

	UFUNCTION()
	void AnimMontagePlay(UAnimMontage* _animMontage, float _animSpeed, FName montageSectionName);

	FORCEINLINE float GetAimRotate()const { return aimRotate; }

	void SetAimRotate(float _val);
};
