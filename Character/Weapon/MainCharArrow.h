// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainCharArrow.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class USphereComponent;
class USceneComponent;
class AMainChar;
class UParticleSystemComponent;
class USoundBase;

UCLASS()
class LHW_API AMainCharArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainCharArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	bool bSoundOnce;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* arrowMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* projectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	USphereComponent* arrowDamageCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* arrowTrailParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* arrowHitSound;

	AMainChar* mainChar;

	FVector spawnLocation;

public:
	FORCEINLINE UStaticMeshComponent* GetArrowMesh()const { return arrowMesh; }

	void Fire();

	UFUNCTION()
	virtual void ArrowDamageCollisionOnOverlapBegin(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
};
