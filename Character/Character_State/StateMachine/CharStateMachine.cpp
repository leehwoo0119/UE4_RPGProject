// Fill out your copyright notice in the Description page of Project Settings.


#include "CharStateMachine.h"
#include "Kismet/GameplayStatics.h"
#include "CharState.h"

// Sets default values for this component's properties
UCharStateMachine::UCharStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UCharStateMachine::BeginPlay()
{
	Super::BeginPlay();

}

void UCharStateMachine::ChangeState(UCharState* _state)
{
	curCharState->Exit();
	curCharState = _state;
	curCharState->Enter();
	StateUpdate();
}

void UCharStateMachine::SetCurState(UCharState* _state)
{
	curCharState = _state;
}

void UCharStateMachine::StateUpdate()
{
	curCharState->Execute();
}

void UCharStateMachine::SetCharState(UCharState* _state)
{
	if (!_state)return;
	ChangeState(_state);
}
