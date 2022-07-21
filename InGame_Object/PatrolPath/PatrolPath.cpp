// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"
#include "Components/SplineComponent.h"

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	patrolPath = CreateDefaultSubobject<USplineComponent>("patrolPath");
}

// Called when the game starts or when spawned
void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

void APatrolPath::IncreasePath()
{
	check(patrolPath != nullptr);
	patrolPathNum += 1;
	patrolPathNum %= patrolPath->GetNumberOfSplinePoints();
}

FVector APatrolPath::GetNextPointWorldPos() const
{
	check(patrolPath != nullptr);
	return patrolPath->GetLocationAtSplinePoint(patrolPathNum, ESplineCoordinateSpace::World);
}



