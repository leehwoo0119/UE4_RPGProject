// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterNormalCombatComponent.h"
#include "MonsterNormal_KN_CombatComponent.generated.h"


UCLASS()
class LSH_API UMonsterNormal_KN_CombatComponent : public UMonsterNormalCombatComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UMonsterNormal_KN_CombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)override;

	void Attack(EAI_MeleeAttack_State _state)override;

	void MeleeAttackGroup_1()override;

	void MeleeAttackGroup_2()override;

	void AttackBlock()override;

	void SpecialSkill()override;

	void LookAround(bool _stop = false)override;
};
