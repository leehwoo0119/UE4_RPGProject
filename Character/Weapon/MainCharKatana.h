// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainCharKatana.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class AMainChar;
class AMonsterBase;
class UNiagaraSystem;

UCLASS()
class LHW_API AMainCharKatana : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainCharKatana();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* katana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* katanaAttackCollision;

	AMainChar* mainChar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* defenseParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* parryParticle;

public:
	FORCEINLINE UStaticMeshComponent* GetKatanaMesh()const {return katana;}

	UFUNCTION()
	virtual void KatanaDamageCollisionOnOverlapBegin(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void SetAttackCollisionActivate();

	UFUNCTION(BlueprintCallable)
	void SetAttackCollisionDeactivate();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayCharAttackSound();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayCharAttackMetalSound();

	void KatanaParryingOrDefenseParticles(char _value);
	 
	UFUNCTION(BlueprintCallable)
	void WeaponRagdol();
};
