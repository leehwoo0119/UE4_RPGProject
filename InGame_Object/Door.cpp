// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

// Sets default values
ADoor::ADoor() :itemID(-1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ADoor::BeginPlay()
{
	Super::BeginPlay();

	//ID√ ±‚»≠ æ»µ 
	check(itemID != -1);

}