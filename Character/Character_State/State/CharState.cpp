// Fill out your copyright notice in the Description page of Project Settings.


#include "CharState.h"
#include "Kismet/GameplayStatics.h"
#include "MainChar.h"
#include "MainCharController.h"
#include "CharStateMachine.h"
#include "MainCharAnimInstance.h"

// Sets default values for this component's properties
UCharState::UCharState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UCharState::BeginPlay()
{
	Super::BeginPlay();

	mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);

	charController = Cast<AMainCharController>(mainChar->GetController());
	check(charController != nullptr);

	charStateMachine = Cast<UCharStateMachine>(mainChar->GetCharStateMachine());
	check(charStateMachine != nullptr);
}
