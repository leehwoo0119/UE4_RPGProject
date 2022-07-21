// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

TArray<ATrapActor*> ATrapActor::trapArray;

ATrapActor::ATrapActor()
{
	PrimaryActorTick.bCanEverTick = false;

	trapCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("trapCollision"));
	trapCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = trapCollision;

	trapActive = true;
}

void ATrapActor::BeginPlay()
{
	Super::BeginPlay();
}

void ATrapActor::AddTrapArray(ATrapActor* _trapActor)
{
	//UE_LOG(LogTemp, Log, TEXT("trapActor Name :: %s"), *_trapActor->GetName());
	trapArray.AddUnique(_trapActor);
	//UE_LOG(LogTemp, Warning, TEXT("trapArray Num: %d"), trapArray.Num());
}

void ATrapActor::RemoveTrapArray(ATrapActor* _trapActor)
{
	if (trapArray.Find(_trapActor))
		trapArray.Remove(_trapActor);
	//UE_LOG(LogTemp, Warning, TEXT("R trapArray Num: %d"), trapArray.Num());
}

void ATrapActor::NotifyTrapDeactive(ETrapSection _trapSection)
{
	TArray<ATrapActor*> allTrapArray = trapArray;
	for (auto* actor : allTrapArray)
	{
		if (actor->GetTrapSection() == _trapSection)
		{
			actor->trapActive = false;
			RemoveTrapArray(actor);
		}
	}
}