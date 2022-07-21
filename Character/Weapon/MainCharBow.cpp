// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharBow.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharBowAnimInstance.h"
#include "CharStatusComponent.h"
#include "MainChar.h"

// Sets default values
AMainCharBow::AMainCharBow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bow = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("bow"));
	bow->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	chargingCurveLength = 1.0f;
}

// Called when the game starts or when spawned
void AMainCharBow::BeginPlay()
{
	Super::BeginPlay();

	check(bow != nullptr);
	
	animInstance = Cast<UMainCharBowAnimInstance>(bow->GetAnimInstance());
	check(animInstance != nullptr);

	mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);

	check(chargingDrawCurveFloat != nullptr);
	FOnTimelineFloat ChargingCurveCallback;
	FOnTimelineEventStatic ChargingFinishedCallback;

	ChargingCurveCallback.BindUFunction(this, FName("ChargingDrawing"));
	ChargingFinishedCallback.BindUFunction(this, FName("FullCharging"));

	chargingCurveFTimeline.AddInterpFloat(chargingDrawCurveFloat, ChargingCurveCallback);
	chargingCurveFTimeline.SetTimelineFinishedFunc(ChargingFinishedCallback);

	chargingCurveFTimeline.SetTimelineLength(chargingCurveLength);
}

void AMainCharBow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	chargingCurveFTimeline.TickTimeline(DeltaTime);
}

void AMainCharBow::StartCharge()
{
	chargingCurveFTimeline.PlayFromStart();
}

void AMainCharBow::EndCharge()
{
	chargingCurveFTimeline.Reverse();
}

void AMainCharBow::ChargingDrawing(float _val)
{
	animInstance->SetChargeGauge(_val * 100);
}

void AMainCharBow::FullCharging()
{	
	if (chargingCurveFTimeline.GetPlaybackPosition() == chargingCurveFTimeline.GetTimelineLength())
	{
		mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::FullCharging);
	}
}

