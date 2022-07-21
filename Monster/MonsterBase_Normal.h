// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "LHW/PatrolPathInterface.h"
#include "MonsterBase_Normal.generated.h"

class EEAIState;
class UMonsterStateMachine;
class APatrolPath;
class UProjectGameInstance;

UCLASS()
class LSH_API AMonsterBase_Normal : public AMonsterBase, public IPatrolPathInterface
{
	GENERATED_BODY()
	
public:
	AMonsterBase_Normal();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "PatrolPath", meta = (AllowPrivateAccess = "true"))
	APatrolPath* patrolPath;

	UProjectGameInstance* gameIS;

public:
	APatrolPath* GetPatrolPath() override;

};
