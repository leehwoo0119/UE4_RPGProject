// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterCombatComponent.h"
#include "MonsterNormalCombatComponent.generated.h"

class AMonsterBase_Normal;

UCLASS()
class LSH_API UMonsterNormalCombatComponent : public UMonsterCombatComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMonsterNormalCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	FVector investigatePosition;

protected:	
	int maxMeleeAttackValue;

	int curElementalOrbNum;
	int maxElementalOrbNum;

public:
	void ExecuteTakeDamageHealth(float _damage)override;

	void ExecuteTakeDamageOffenserse(float _damage)override;

	void Die()override;

	void ExecuteStealth(EStealthState_Type _state)override;

	void TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)override;

	void SetInvestigatePosition(FVector _value);

	FORCEINLINE FVector GetInvestigatePosition() { return investigatePosition; }

	UFUNCTION()
	virtual void StealthAreaOnOverlapBegin(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void StealthAreaOnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	virtual void SpawnElemental(FVector _fireLocation, FRotator _fireRotator, float _scale) {}

	UFUNCTION(BlueprintCallable)
	virtual void SpawnSlashProjectile() {}

	void PlusCurElementalOrbNum(int _value);

	FORCEINLINE int GetCurElementalOrbNum() { return curElementalOrbNum; }
	FORCEINLINE int GetMaxElementalOrbNum() { return maxElementalOrbNum; }
};
