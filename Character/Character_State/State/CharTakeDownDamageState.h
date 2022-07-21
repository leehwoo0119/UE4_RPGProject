// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharState.h"
#include "Components/TimeLineComponent.h"
#include "CharTakeDownDamageState.generated.h"

UCLASS()
class LHW_API UCharTakeDownDamageState : public UCharState
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	UCharTakeDownDamageState();

protected:
	// Called when the game starts
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FTimeline takeDownDamageCurveFTimeline;
	float takeDownDamageCurveLength;

public:
	void Enter()override;

	void Execute()override;

	void Exit()override;

public:
	void StartTakeDownDamageCurveFTimeline();

	UFUNCTION()
	void TakeDownDamagStart();

	UFUNCTION(BlueprintCallable)
	void TakeDownDamagEnd();
};
