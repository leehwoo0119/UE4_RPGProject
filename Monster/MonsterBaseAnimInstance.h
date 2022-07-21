// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EAIState.h"
#include "MonsterBaseAnimInstance.generated.h"

class AMonsterBase;
class UMonsterStateMachine;

UCLASS()
class LSH_API UMonsterBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	void NativeInitializeAnimation() override;

	void NativeBeginPlay() override;

private:
	UAnimInstance* animInstance;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	APawn* pawn;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	AMonsterBase* aiChar;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float moveSpeed;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float direction;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EAI_Behavior_State state;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	EAI_BossWeaponType_State weaponTypeState;

	UMonsterStateMachine* monsterStateMachine;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimMontage")
	UAnimMontage* attackMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimMontage")
	UAnimMontage* stealthedMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimMontage")
	UAnimMontage* specialSkillMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimMontage")
	UAnimMontage* hitMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimMontage")
	UAnimMontage* executedMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimMontage")
	UAnimMontage* lookAroundMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimMontage")
	UAnimMontage* weaponSwapMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimMontage")
	UAnimMontage* offenserseRecorveryMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimMontage")
	UAnimMontage* specialAttackMontage;

public:
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void UpdateAnimationProperties();

	UFUNCTION(exec)
	void AnimMontagePlay(UAnimMontage* _animMontage, float _animSpeed, FName _montageSectionName);
};
