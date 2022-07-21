// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBaseStatus.h"
#include "MonsterBossStatus.generated.h"


UCLASS()
class LSH_API UMonsterBossStatus : public UMonsterBaseStatus
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UMonsterBossStatus();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int currentGroggyCount;
	int maxGroggyCount;

	int maxAttackCount;
	int currentProjectileCount;
	int maxProjectileCount;
	int mageAttackCnt;
	int maxMageAttackCnt;

	bool bCheckOffenserseRecorvery;	
	bool bIsEnableHealthDrain;
	bool bCanAvoidAttack;
	TArray<bool> bSpecialAttackArr;

	int whirlwindCnt;
	int maxWhirlwindCnt;

	int maxOffenserseRecoveryAttackedCnt;
	int offenserseRecoveryAttackedCnt;

public:
	FORCEINLINE int GetCurrentGroggyCount()const override{ return currentGroggyCount; }
	FORCEINLINE int GetMaxGroggyCount()const override { return maxGroggyCount; }

	FORCEINLINE int GetMaxAttackCount()const override { return maxAttackCount; }
	FORCEINLINE int GetCurrentProjectileCount()const override { return currentProjectileCount; }
	FORCEINLINE int GetMaxProjectileCount()const override { return maxProjectileCount; }
	FORCEINLINE bool GetSpecialAttack(int _index)const override { return bSpecialAttackArr[_index]; }
	FORCEINLINE int GetMageAttackCnt()const override { return mageAttackCnt; }
	FORCEINLINE int GetMaxMageAttackCnt()const override { return maxMageAttackCnt; }

	FORCEINLINE bool GetCheckOffenserseRecorvery()const override { return bCheckOffenserseRecorvery; }
	FORCEINLINE bool GetEnableHealthDrain()const override { return bIsEnableHealthDrain; }
	FORCEINLINE bool GetCanAvoidAttack()const override { return bCanAvoidAttack; }

	FORCEINLINE int GetWhirlwindCnt()const override { return whirlwindCnt; }
	FORCEINLINE int GetMaxWhirlwindCnt()const override { return maxWhirlwindCnt; }

	FORCEINLINE int GetOffenserseRecoveryAttackedCnt()const { return offenserseRecoveryAttackedCnt; }

	void SetCurrentGroggyCount(int _value)override;
	void SetMaxGroggyCount(int _value)override;
	void PlusCurrentGroggyCount(int _value)override;
	void PlusMageAttackCnt(bool _reset = false)override;

	void SetMaxAttackCount(int _value)override;
	void SetCurrentProjectileCount(int _value)override;
	void SetMaxProjectileCount(int _value)override;
	void PlusCurrentProjectileCount()override;

	void SetCheckOffenserseRecorvery(bool _value)override;
	void SetTrueSpecialAttack(int _index)override;
	void SetEnableHealthDrain(bool _value)override;
	void SetCanAvoidAttack(bool _value)override;

	void SetMaxWhirlwindCnt(int _value)override;
	void PlusOrResetWhirlwindCnt(bool _reset = false)override;
	bool CheckOffenserseRecovery()override;
};
