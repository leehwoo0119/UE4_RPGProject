// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CanTargetingActor.h"
#include "SecondBossPatternWeapon.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UWidgetComponent;
class ASecondBossAreaPattern;

UCLASS()
class LSH_API ASecondBossPatternWeapon : public ACanTargetingActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASecondBossPatternWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "dummyPos", meta = (AllowPrivateAccess = "true"))
	USceneComponent* dummyPos;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Area", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* damageArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* weaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	ASecondBossAreaPattern* bossPattern;

	int destroyCnt;
	int maxDestroyCnt;

public:
	void CheckDestroy();
};
