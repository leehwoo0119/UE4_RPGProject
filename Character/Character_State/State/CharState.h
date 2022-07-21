// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharState.generated.h"

class AMainChar;
class AMainCharController;
class UCharStateMachine;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LHW_API UCharState : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharState();

protected:
	// Called when the game starts
	void BeginPlay() override;
	
protected:
	AMainChar* mainChar;
	AMainCharController* charController;
	UCharStateMachine* charStateMachine;
	
public:
	virtual void Enter() {}

	virtual void Execute() {}

	virtual void Exit() {}
};
