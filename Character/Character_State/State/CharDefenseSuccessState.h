// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharState.h"
#include "Components/TimeLineComponent.h"
#include "CharDefenseSuccessState.generated.h"


UCLASS()
class LHW_API UCharDefenseSuccessState : public UCharState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharDefenseSuccessState();

private:
	FTimeline defenseSuccessCurveFTimeline;
	float defenseSuccessCurveLength;

protected:
	// Called when the game starts
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	void Enter()override;

	void Execute()override;

	void Exit()override;

public:
	void StartDefenseSuccessCurveFTimeline();

	UFUNCTION()
	void DefenseSuccessStart();

	UFUNCTION()
	void DefenseSuccessEnd();
};
