// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UpperBladeAttackPattern.generated.h"

class AMonsterBase;
class AUpperBladeAttackWeapon;

UCLASS()
class LHW_API AUpperBladeAttackPattern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUpperBladeAttackPattern();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "monster", meta = (AllowPrivateAccess = "true"))
	AMonsterBase* monster;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "monster", meta = (AllowPrivateAccess = "true"))
	TArray<AUpperBladeAttackWeapon*> weaponArr;

public:
	void SetAttackOnce();
};
