// Fill out your copyright notice in the Description page of Project Settings.


#include "CanTargetingActor.h"

// Sets default values
ACanTargetingActor::ACanTargetingActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACanTargetingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACanTargetingActor::SetAttackOnce(bool _val)
{
	bAttackOnce = _val;
}


