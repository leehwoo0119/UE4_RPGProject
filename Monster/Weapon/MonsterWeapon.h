// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterWeapon.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class AMonsterBase;
class UProjectGameInstance;
class AMainChar;
class UNiagaraSystem;

UCLASS()
class LSH_API AMonsterWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	AMonsterBase* monsterBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* enemyWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* enemyWeaponAttackCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* defenseParticle;

public:
	FORCEINLINE UStaticMeshComponent* GetMonsterWeaponMesh()const { return enemyWeapon; }

	UFUNCTION()
	virtual void MonsterWeaponDamageCollisionOnOverlapBegin(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SetDamageCollisionActivate();

	void SetCollisionActivate();

	UFUNCTION(BlueprintCallable)
	void SetDamageCollisionDeactivate();

	void SetMonsterBase(AMonsterBase* _monsterBase);

	void ApplyDamageByType(AMainChar* _mainChar);

	UFUNCTION(BlueprintCallable)
	void RagdolWeapon();

	void MonsterWeaponDefenseParticles();

	UFUNCTION(BlueprintImplementableEvent)
	void MonsterDefenseSound();
};
