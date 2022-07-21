// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterNormalCombatComponent.h"
#include "Monster_Tutorial_CombatComponent.generated.h"


UCLASS()
class LSH_API UMonster_Tutorial_CombatComponent : public UMonsterNormalCombatComponent
{
	GENERATED_BODY()
	
public:
	UMonster_Tutorial_CombatComponent();

private:
	bool bIgnoreDamage;

protected:
	virtual void BeginPlay() override;

public:
	FTimerHandle recoveryHandle;

public:
	void TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)override;

	void Attack(EAI_MeleeAttack_State _state)override;

	void AttackBlock()override;

	void SetIgnoreDamage(bool _value);

	void RecoveryAll();

	FORCEINLINE bool GetIgnoreDamage() { return bIgnoreDamage; }
};
