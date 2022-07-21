// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterNormalCombatComponent.h"
#include "MonsterNormal_GS_CombatComponent.generated.h"


UCLASS()
class LSH_API UMonsterNormal_GS_CombatComponent : public UMonsterNormalCombatComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UMonsterNormal_GS_CombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)override;

	bool CheckTargetFront(FVector _vecA, FVector _vecB, FVector _forwardVec);

	void Attack(EAI_MeleeAttack_State _state)override;

	void MeleeAttackGroup_1()override;

	void MeleeAttackGroup_2()override;

	void AttackBlock()override;

	void SpecialSkill()override;

	void DefenseEnd();

	void TimerDefenseEnd();

	void DefenseSuccess();

	void LookAround(bool _stop = false)override;
};
