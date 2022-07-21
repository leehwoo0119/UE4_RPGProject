// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBossCombatComponent.h"
#include "MonsterBossCombatComponent_Sec.generated.h"


UCLASS()
class LSH_API UMonsterBossCombatComponent_Sec : public UMonsterBossCombatComponent
{
	GENERATED_BODY()

public:
	UMonsterBossCombatComponent_Sec();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FTimeline teleportEffectCurveFTimeline;
	float teleportEffectTime;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* teleportEffectCurveFloat;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector BossPatternSpawnPos;

public:
	void TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)override;

	void ExecuteTakeDamageHealth(float _damage)override;

	void Die()override;

	void Attack(EAI_MeleeAttack_State _state)override;

	void AttackBlock()override;

	void KatanaAttackBlock();

	void MageAttackBlock();

	void SpecialSkill()override;

	void SpecialAttack()override;

	void ExecuteAttackAvoid()override;

	void BossPhaseChange(bool _bHealthZero)override;

	void KatanaAttack();

	void MageAttack();

	void CheckChangeMageState();

	void ChangeMageState();

	void ChangeKatanaState();

	void ExecuteInstantDeath();

	void TeleportEffectOpacityStart() override;

	void TeleportEffectOpacityReverse() override;

	void TeleportEffectOpacity(float _val) override;

	void SetWeaponOpacity(float _val) override;

	void ExecuteHealthDrain();
};
