// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBaseController.h"
#include "Perception/AIPerceptionTypes.h"
#include "MonsterController_Boss.generated.h"

class UAISenseConfig_Sight;

UCLASS()
class LSH_API AMonsterController_Boss : public AMonsterBaseController
{
	GENERATED_BODY()

public:
	AMonsterController_Boss();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void PerceptionSetting();
	UFUNCTION()
	virtual void OnTargetDetected(AActor* _actor, FAIStimulus const _stimulus);
};
