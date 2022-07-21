// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrapActor.h"
#include "TrapActor_Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class LSH_API ATrapActor_Projectile : public ATrapActor
{
    GENERATED_BODY()

public:
    ATrapActor_Projectile();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* projectileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trap", meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* projectileMovement;

    FVector spawnLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
    UParticleSystemComponent* arrowTrailParticle;

public:
    UFUNCTION()
    virtual void TrapCollisionOnOverlapBegin(
            UPrimitiveComponent* OverlappedComponent,
            AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
            bool bFromSweep, const FHitResult& SweepResult);
};