// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharState.h"
#include "CharUltimateState.generated.h"

class AMonsterBase;

UCLASS()
class LHW_API UCharUltimateState : public UCharState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharUltimateState();

public:
	FVector startLoc;
	FVector endLoc;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float damage;

protected:
	// Called when the game starts
	void BeginPlay() override;

protected:
	void Enter()override;

	void Execute()override;

	void Exit()override;

public:
	UFUNCTION(BlueprintCallable)
		void UltimateMoveCompTo(float _value);

	float CheckMoveCompToCollision(FVector _startVec, FVector _endVec);

	UFUNCTION(BlueprintCallable)
		TArray<AMonsterBase*> CheckMonster();

	UFUNCTION(BlueprintCallable)
		void UltimateEnd();

};