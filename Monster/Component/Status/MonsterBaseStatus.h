// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EAIState.h"
#include "Components/TimeLineComponent.h"
#include "MonsterBaseStatus.generated.h"

class AMonsterBase;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LSH_API UMonsterBaseStatus : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterBaseStatus();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AMonsterBase* monsterBase;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MonsterID", meta = (AllowPrivateAccess = "true"))
	int monsterID;

	EBloodDirectionState_Type bloodTypeDirectionState;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MonsterID", meta = (AllowPrivateAccess = "true"))
	float maxMonsterHealthPoint;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MonsterID", meta = (AllowPrivateAccess = "true"))
	float maxMonsterOffensersePoint;

	float calMonsterHealthPoint;
	float calMonsterOffensersePoint;
	float curMonsterHealthPoint;
	float curMonsterOffensersePoint;

	bool bCantParryingAttack;
	bool bGuardBreakAttack;
	bool bCantGuardAttack;
	bool bAttackEnable;
	bool bPlayerSee;
	bool bCantGuardProjectile;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* pointDecreaseCurveFloat;

	FTimeline healthDecreaseCurveFTimeline;
	FTimeline offenserseDecreaseCurveFTimeline;

	float pointDecreaseCurveLength;

	float chaseDis;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float monsterDamage;

public:
	//normal
	virtual FORCEINLINE int GetDefenseCnt() const { return 0; }
	virtual FORCEINLINE bool GetInvestigateMoveStart()const { return false; }

	UFUNCTION(BlueprintCallable)
	virtual	FORCEINLINE EAI_ElementalType_State GetElementalType() const { return EAI_ElementalType_State::Default; }

	virtual void SetDefenseCnt(int _cnt) {}
	virtual void SetInvestigateMoveStart(bool _val) {}

	UFUNCTION(BlueprintCallable)
	virtual void SetElementalType(EAI_ElementalType_State _elemental) {}

	//boss
	virtual FORCEINLINE int GetCurrentGroggyCount()const { return 0; }
	virtual FORCEINLINE int GetMaxGroggyCount()const { return 0; }

	virtual FORCEINLINE int GetMaxAttackCount()const { return 0; }
	virtual FORCEINLINE int GetCurrentProjectileCount()const { return 0; }
	virtual FORCEINLINE int GetMaxProjectileCount()const { return 0; }
	virtual FORCEINLINE bool GetSpecialAttack(int _index)const { return false; }
	virtual FORCEINLINE int GetMageAttackCnt()const { return 0; }
	virtual FORCEINLINE int GetMaxMageAttackCnt()const { return 0; }

	virtual FORCEINLINE bool GetCheckOffenserseRecorvery()const { return false; }	
	virtual FORCEINLINE bool GetEnableHealthDrain()const { return false; }
	virtual FORCEINLINE bool GetCanAvoidAttack()const { return false; }

	virtual FORCEINLINE int GetWhirlwindCnt()const { return 0; }
	virtual FORCEINLINE int GetMaxWhirlwindCnt()const { return 0; }

	virtual FORCEINLINE int GetOffenserseRecoveryAttackedCnt()const { return 0; }

	virtual void SetCurrentGroggyCount(int _value){}
	virtual void SetMaxGroggyCount(int _value) {}
	virtual void PlusCurrentGroggyCount(int _value) {}

	virtual void SetMaxAttackCount(int _value) {}
	virtual void SetCurrentProjectileCount(int _value) {}
	virtual void SetMaxProjectileCount(int _value) {}
	virtual void PlusCurrentProjectileCount() {}
	virtual void PlusMageAttackCnt(bool _reset = false) {}

	virtual void SetCheckOffenserseRecorvery(bool _value) {}
	virtual void SetTrueSpecialAttack(int _index) {}

	UFUNCTION(BlueprintCallable)
	virtual void SetEnableHealthDrain(bool _value) {}

	UFUNCTION(BlueprintCallable)
	virtual void SetCanAvoidAttack(bool _value) {}

	virtual void SetMaxWhirlwindCnt(int _value) {}

	virtual void PlusOrResetWhirlwindCnt(bool _reset = false) {}

	virtual bool CheckOffenserseRecovery() { return false; }

	//base
	FORCEINLINE int GetMonsterDamage()const { return monsterDamage; }

	FORCEINLINE int GetMonsterID()const { return monsterID; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxMonsterHealthPoint()const { return maxMonsterHealthPoint; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetMaxMonsterOffensersePoint()const { return maxMonsterOffensersePoint; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCalMonsterHealthPoint()const { return calMonsterHealthPoint; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCalMonsterOffensersePoint()const { return calMonsterOffensersePoint; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurMonsterHealthPoint()const { return curMonsterHealthPoint; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurMonsterOffensersePoint()const { return curMonsterOffensersePoint; }

	FORCEINLINE bool GetCantParryingAttack()const { return bCantParryingAttack; }
	FORCEINLINE bool GetGuardBreakAttack()const { return bGuardBreakAttack; }
	FORCEINLINE bool GetCantGuardAttack()const { return bCantGuardAttack; }
	FORCEINLINE bool GetAttackEnable()const { return bAttackEnable; }
	FORCEINLINE bool GetPlayerSee()const { return bPlayerSee; }
	FORCEINLINE bool GetCantGuardProjectile()const { return bCantGuardProjectile; }
	FORCEINLINE EBloodDirectionState_Type GetBloodTypeDirectionState()const { return bloodTypeDirectionState; }

	FORCEINLINE float GetChaseDis()const { return chaseDis; }

	void PlusCurMonsterHealthPoint(float _val, bool _bSet = false);
				
	void PlusCurMonsterOffensersePoint(float _val, float _setOffensersePoint = 0);

	UFUNCTION(BlueprintCallable)
	void SetCantParryingAttack(bool _value);

	UFUNCTION(BlueprintCallable)
	void SetGuardBreakAttack(bool _value);

	void SetCantGuardAttack(bool _value);

	void SetAttackEnable(bool _value);

	void SetPlayerSee(bool _value);

	UFUNCTION(BlueprintCallable)
	void SetCantGuardProjectile(bool _value);

	UFUNCTION(BlueprintCallable)
	void SetBloodTypeDirectionState(EBloodDirectionState_Type _state);

	void StartHealthDecreaseCurveFTimeline();

	void StartOffenserseDecreaseCurveFTimeline();

	void SetChaseDis(float _val);

	UFUNCTION()
	void HealthDecreaseStart(float _val);

	UFUNCTION()
	void OffenserseDecreaseStart(float _val);
};
