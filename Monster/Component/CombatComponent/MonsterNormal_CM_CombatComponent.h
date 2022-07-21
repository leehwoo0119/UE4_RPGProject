// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterNormalCombatComponent.h"
#include "MonsterNormal_CM_CombatComponent.generated.h"

class AMonsterProjectile;

UCLASS()
class LSH_API UMonsterNormal_CM_CombatComponent : public UMonsterNormalCombatComponent
{
	GENERATED_BODY()

public:
	UMonsterNormal_CM_CombatComponent();

protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float _deltaTime, ELevelTick _tickType, FActorComponentTickFunction* _thisTickFunction) override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterProjectile> slashProjectile;

	FTimeline teleportEffectCurveFTimeline;
	float teleportEffectTime;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* teleportEffectCurveFloat;

public:
	void TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)override;

	void Attack(EAI_MeleeAttack_State _state)override;

	void MeleeAttackGroup_1()override;

	void MeleeAttackGroup_2()override;

	void AttackBlock()override;

	void SpecialSkill()override;

	void LookAround(bool _stop = false)override;

	void SpawnSlashProjectile() override;

	void TeleportEffectOpacityStart() override;

	void TeleportEffectOpacityReverse() override;

	void TeleportEffectOpacity(float _val) override;

	void SetWeaponOpacity(float _val) override;
};
