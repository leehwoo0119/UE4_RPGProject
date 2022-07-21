// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterCombatComponent.h"
#include "MonsterBossCombatComponent.generated.h"

class AMainChar;
class AMonsterProjectile;

UCLASS()
class LSH_API UMonsterBossCombatComponent : public UMonsterCombatComponent
{
	GENERATED_BODY()

public:
	UMonsterBossCombatComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMonsterProjectile> slashProjectile;

public:
	void ExecuteTakeDamageHealth(float _damage)override;

	void ExecuteTakeDamageOffenserse(float _damage)override;

	void Die()override;

	void TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)override;

	void HealthDrain(float _value);

	void ChangeHealthState();

	bool CheckOffensersePointDecrease(float _damage);

	virtual void SpecialAttack() {}
	
	UFUNCTION(BlueprintImplementableEvent)
	void DrawClearUI();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SpawnElementalProjectile();

	UFUNCTION(BlueprintImplementableEvent)
	void DestroyAllElementalProjectile();

	void CheckSpawnElementalProjectile();

	UFUNCTION(BlueprintCallable)
	void SpawnSlashProjectile();
};
