// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterNormalCombatComponent.h"
#include "MonsterNormal_SF_CombatComponent.generated.h"

class AMonsterElementalProjectile;

UCLASS()
class LSH_API UMonsterNormal_SF_CombatComponent : public UMonsterNormalCombatComponent
{
	GENERATED_BODY()
	
public:
	UMonsterNormal_SF_CombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterElementalProjectile> elementalProjectile_Flame;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterElementalProjectile> elementalProjectile_Poison;

public:
	void TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)override;

	void Attack(EAI_MeleeAttack_State _state)override;

	void MeleeAttackGroup_1()override;

	void LookAround(bool _stop = false)override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(FVector _fireLocation, FRotator _fireRotator);
};
