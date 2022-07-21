// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EAIState.h"
#include "LHW/ECharState.h"
#include "Components/TimeLineComponent.h"
#include "MonsterCombatComponent.generated.h"

class AMonsterBase;
class AMainChar;
class UParticleSystem;
class UProjectGameInstance;
class ASecondBossAreaPattern;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LSH_API UMonsterCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMonsterCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float bloodRightVector;
	float bloodForwardVector;
	float bloodPitchRotator;
	float bloodYawRotator;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* bloodParticle;

	FTimeline deathEffectCurveFTimeline;
	float deathEffectTime;

	UProjectGameInstance* gameIS;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* deathEffectCurveFloat;

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	AMonsterBase* monsterBase;
	AMainChar* mainChar;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "MonsterType", meta = (AllowPrivateAccess = "true"))
	EAI_MonsterType monsterType;

public:
	UParticleSystemComponent* bloodEffect;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ASecondBossAreaPattern* bossAreaPattern;

protected:
	UFUNCTION()
	virtual void TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser);

public:
	void TakeDamageCheckDir(float _hitDirection);

	UFUNCTION(BlueprintCallable)
	void RagdolMonster();

	void OffenserseZero();

	void OffenserseZeroEnd();			//≈∏¿Ã∏”

	void PrintAttackParticle();

	void SetAttackBloodDirection();

	void MonsterDeathProcessing();

	UFUNCTION(BlueprintCallable)
	void WeaponEquip();

	UFUNCTION(BlueprintCallable)
	void SetAttackEnableTrue(float _minDelay, float _maxDelay);

	void DrawDeathEffectStart();

	UFUNCTION()
	void DrawDeathEffect(float _val);

	UFUNCTION(BlueprintCallable)
	void MonsterDeathDestroy();

	UFUNCTION(BlueprintCallable)
	void MonsterMoveCompTo(float _value, bool _bIgnoreCollision, FVector _target = FVector(0.0f, 0.0f, 0.0f));

	float CheckMoveCompToCollision(FVector _startVec, FVector _endVec);

public:
	virtual void ExecuteTakeDamageHealth(float _damage) {}

	virtual void ExecuteTakeDamageOffenserse(float _damage) {}

	virtual void Die();

	virtual void Attack(EAI_MeleeAttack_State _state);
	 
	virtual void MeleeAttackGroup_1() {}
	
	virtual void MeleeAttackGroup_2() {}

	UFUNCTION(BlueprintCallable)
	void RangeAttack(float _forwardVal, float _size, FVector _location = FVector(0.0f, 0.0f, 0.0f));
	 
	virtual void AttackBlock() {}
	
	virtual void SpecialSkill() {}
	
	virtual void DefenseEnd() {}
	
	virtual void TimerDefenseEnd() {}
	 
	virtual void DefenseSuccess() {}

	virtual void ExecuteExecuted(EExecutionState_Type _state);

	virtual void LookAround(bool _stop = false) {}

	virtual void ExecuteStealth(EStealthState_Type _state){}

	virtual void BossPhaseChange(bool _healthZero) {}

	virtual void GroggyExecuted(){}

	virtual void ExecuteAttackAvoid() {}

	UFUNCTION(BlueprintCallable)
	virtual void TeleportEffectOpacityStart() {}

	UFUNCTION(BlueprintCallable)
	virtual void TeleportEffectOpacityReverse() {}

	UFUNCTION()
	virtual	void TeleportEffectOpacity(float _val) {}

	virtual void SetWeaponOpacity(float _val) {}

public:
	FORCEINLINE EAI_MonsterType GetAIMonsterType()const { return monsterType; }

};
