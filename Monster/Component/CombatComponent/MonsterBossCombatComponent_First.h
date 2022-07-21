// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBossCombatComponent.h"
#include "MonsterBossCombatComponent_First.generated.h"

class AMonsterProjectile;
class AMainChar;

UCLASS()
class LSH_API UMonsterBossCombatComponent_First : public UMonsterBossCombatComponent
{
	GENERATED_BODY()
	
public:
	UMonsterBossCombatComponent_First();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterProjectile> elementalProjectile;

public:
	void TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)override;

	void ExecuteTakeDamageHealth(float _damage)override;

	void Attack(EAI_MeleeAttack_State _state)override;

	void AttackBlock()override;

	void SpecialSkill()override;

	void ExecuteExecuted(EExecutionState_Type _state)override;

	void Die()override;

	void BossPhaseChange(bool _bHealthZero)override;

	void GroggyExecuted()override;

	void OffenserseRecorvery();

	UFUNCTION(BlueprintCallable)
	void OffenserseRecoverySuccess();

	void Groggy();

	UFUNCTION(BlueprintCallable)
	void GroggyEnd();					//≈∏¿Ã∏”	

	void SpecialAttack()override;

	void MeleeAttack();

	UFUNCTION(BlueprintCallable)
	void WhirlwindCnt(AMainChar* _target = nullptr);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnChair();

};
