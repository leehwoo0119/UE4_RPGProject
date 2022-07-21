// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapActor_Floor.h"
#include "TrapActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "LHW/ProjectGameInstance.h"
#include "Kismet/GameplayStatics.h"

ATrapActor_Floor::ATrapActor_Floor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	floorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("floorMesh"));
	floorMesh->SetupAttachment(RootComponent);

	shootPos = CreateDefaultSubobject<USceneComponent>(TEXT("shootPos"));
	shootPos->SetupAttachment(RootComponent);

	floorCurveLength = 0.3f;
	makeProjectileNum = 3;
	projectileNum = 0;
}

void ATrapActor_Floor::BeginPlay()
{
	Super::BeginPlay();

	check(arrowHitSound != nullptr);
	
	gameIS = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(gameIS != nullptr);

	FOnTimelineFloat floorCurveCallback;
	FOnTimelineEventStatic floorFinishedCallback;

	floorCurveCallback.BindUFunction(this, FName("PressDrawing"));
	floorFinishedCallback.BindUFunction(this, FName("PressOver"));

	floorCurveFTimeline.AddInterpFloat(floorDrawCurveFloat, floorCurveCallback);
	floorCurveFTimeline.SetTimelineFinishedFunc(floorFinishedCallback);

	floorCurveFTimeline.SetTimelineLength(floorCurveLength);

	trapCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor_Floor::TrapCollisionOnOverlapBegin);
}

void ATrapActor_Floor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	floorCurveFTimeline.TickTimeline(DeltaTime);
}

void ATrapActor_Floor::TrapCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//gameIS->PlaySoundManager(arrowHitSound, GetActorLocation(), 1.0f, 1.0f, 0.0f);
	startVec = GetActorLocation();
	floorCurveFTimeline.PlayFromStart();
}

void ATrapActor_Floor::PressDrawing(float _value)
{
	float zPos = FMath::Lerp(startVec.Z, startVec.Z - 40.0f, _value);
	FVector pressLoc = FVector(GetActorLocation().X, GetActorLocation().Y, zPos);
	SetActorRelativeLocation(pressLoc);
}

void ATrapActor_Floor::PressOver()
{
	fireLoc = shootPos->GetComponentLocation();
	fireRot = GetActorRotation();
	
	GetWorld()->GetTimerManager().SetTimer(
		fireHandle, this, &ATrapActor_Floor::Fire, 0.5f, true);
}

void ATrapActor_Floor::Fire()
{
	if (projectileNum == makeProjectileNum)
	{
		GetWorldTimerManager().ClearTimer(fireHandle);
		Destroy();
	}
	else
	{		
		FActorSpawnParameters spawnParams;
		GetWorld()->SpawnActor<ATrapActor>(trapProjectile, fireLoc, fireRot, spawnParams);
	}
	projectileNum++;
}
