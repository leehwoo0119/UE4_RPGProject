// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharState.h"
#include "CharFullChargingFailState.generated.h"


UCLASS()
class LHW_API UCharFullChargingFailState : public UCharState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharFullChargingFailState();

protected:
	// Called when the game starts
	void BeginPlay() override;

protected:
	void Enter()override;

	void Execute()override;

	void Exit()override;
};
