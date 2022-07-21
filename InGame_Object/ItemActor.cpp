// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"

// Sets default values
AItemActor::AItemActor() :itemID(-1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();

	//ID√ ±‚»≠ æ»µ 
	check(itemID != -1);

}

void AItemActor::DestroyItemActor()
{
	Destroy();
}
