// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "UpperBladeAttackWeapon.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AMonsterBase;
class AUpperBladeAttackPattern;

UCLASS()
class LHW_API AUpperBladeAttackWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUpperBladeAttackWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "dummyPos", meta = (AllowPrivateAccess = "true"))
	USceneComponent* dummyPos;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Area", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* damageArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* bottomMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* damageWeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "monster", meta = (AllowPrivateAccess = "true"))
	AMonsterBase* monster;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "monster", meta = (AllowPrivateAccess = "true"))
	AUpperBladeAttackPattern* patternParent;

	bool bDamageOnce;

public:
	UFUNCTION()
	virtual void DamageCollisionOnOverlapBegin(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void WeaponOpacityChange(float _val);

	void SetDamageOnce();
};
