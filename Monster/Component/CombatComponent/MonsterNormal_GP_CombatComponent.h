// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterNormalCombatComponent.h"
#include "MonsterNormal_GP_CombatComponent.generated.h"

class AMonsterElementalProjectile;
class AMonsterElementalOrb;
class AMonsterElementalPillar;

UCLASS()
class LSH_API UMonsterNormal_GP_CombatComponent : public UMonsterNormalCombatComponent
{
	GENERATED_BODY()

public:
	UMonsterNormal_GP_CombatComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterElementalProjectile> elementalProjectile_Poison;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterElementalProjectile> elementalProjectile_Frozen;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterElementalOrb> elementalOrb_Frozen;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterElementalPillar> elementalPillar_Poison;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterElementalPillar> elementalPillar_Frozen;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, meta = (AllowPrivateAccess = "true"))
	bool bPatternUseAble;

public:
	void TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)override;

	void Attack(EAI_MeleeAttack_State _state)override;

	void MeleeAttackGroup_1()override;

	void SpecialSkill()override;

	void LookAround(bool _stop = false)override;

	void SpawnElemental(FVector _fireLocation, FRotator _fireRotator, float _scale) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SettingDelayProjectile();
};
