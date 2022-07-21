// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterBaseController.generated.h"

class AMonsterBase;
class UAISenseConfig_Sight;

UCLASS()
class LSH_API AMonsterBaseController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterBaseController();

protected:
	virtual void BeginPlay() override;

protected:
	AMonsterBase* monsterBase;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	UAISenseConfig_Sight* sightConfig;
	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	float aiSightRadius;
	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	float aiLoseSightRadius;
	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	float aiFieldOfView;
	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	float aiViewBackwardOffset;
	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	float aiNearClippingRadius;

	

public:
	void LogicStop();

	virtual void PlayerAttackDetected() {}
	virtual bool SpecialSkill() { return false; }
};