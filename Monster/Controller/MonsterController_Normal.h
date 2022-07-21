// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBaseController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterController_Normal.generated.h"

class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UBlackboardData;
class UBehaviorTree;
class UAISenseConfig_Hearing;

UCLASS()
class LSH_API AMonsterController_Normal : public AMonsterBaseController
{
	GENERATED_BODY()
	
public:
	AMonsterController_Normal();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = AI, meta = (AllowPrivateAccess = "true"))
	float aiHearingRange;

protected:
	UPROPERTY()
	UBlackboardData* BBAsset;
	UPROPERTY()
	UBehaviorTree* BTAsset;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	UAISenseConfig_Hearing* hearingConfig;

	FAISenseID sightId;
	FAISenseID hearingId;

public:
	UFUNCTION()
	void PerceptionSetting();
	UFUNCTION()
	virtual void OnTargetDetected(AActor* _actor, FAIStimulus const _stimulus);

	void PlayerAttackDetected()override {}
	bool SpecialSkill()override { return false; }
	void HearingSensePause(bool _value);
};
