// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharState.h"
#include "Components/TimeLineComponent.h"
#include "CharExecutionState.generated.h"

class AMainChar;
class AMonsterBase;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LHW_API UCharExecutionState : public UCharState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharExecutionState();

protected:
	// Called when the game starts
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float distance;
	uint8 executionType;
	bool bBossPhaseChangeOrGroggyKill;

	AMonsterBase* targetMonster;

	FTimeline executionCameraZoomCurveFTimeline;
	float executionCameraZoomCurveLength;

	float minExecutionCameraSpringLength;
	float maxExecutionCameraSpringLength;

protected:
	void Enter()override;

	void Execute()override;

	void Exit()override;

public:
	void ExecutionSetting();

	UFUNCTION(BlueprintCallable)
		void MonsterNormalExecution();

	UFUNCTION(BlueprintCallable)
		void MonsterBossExecution(bool _bBossPhaseChange);

	void SetCharPos(bool _bBossPhaseChange = false);

	void StartExecutionCamera();
	void ResetCameraRot();

	UFUNCTION(BlueprintImplementableEvent)
		void MoveToLocaition(FVector _targetLoc, AMainChar* _mainChar, bool _bBoss, bool _bBossPhaseChange, FRotator _rot);

	UFUNCTION(BlueprintCallable)
		void MonsterMove();

	UFUNCTION(BlueprintCallable)
		void StartExecutioncameraZoomCurveFTimeline();

	UFUNCTION(BlueprintCallable)
		void ReverseExecutioncameraZoomCurveFTimeline();

	UFUNCTION()
		void ExecutioncameraZoomStart(float _val);

	UFUNCTION(BlueprintCallable)
		FORCEINLINE AMonsterBase* GetTargetMonster()const { return targetMonster; }
};