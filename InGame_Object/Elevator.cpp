// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"
#include "MainChar.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AElevator::AElevator() :
	height(0.0f),
	elevatorCurveLength(10.0f),
	pushButtonDelayTime(2.0f),
	buttonPushHeight(17.0f),
	buttonPushCurveLength(0.5f),
	bElevatorMove(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	dummyPos = CreateDefaultSubobject<USceneComponent>(TEXT("dummyPos"));
	dummyPos->SetupAttachment(GetRootComponent());
	check(dummyPos != nullptr);

	elevatorPeace_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("elevatorPeace_1"));
	elevatorPeace_1->SetupAttachment(dummyPos);
	check(elevatorPeace_1 != nullptr);

	elevatorPushButton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("elevatorButton"));
	elevatorPushButton->SetupAttachment(dummyPos);
	check(elevatorPushButton != nullptr);

	elevatorPeace_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("elevatorPeace_2"));
	elevatorPeace_2->SetupAttachment(dummyPos);
	check(elevatorPeace_2 != nullptr);

	elevatorPeace_3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("elevatorPeace_3"));
	elevatorPeace_3->SetupAttachment(dummyPos);
	check(elevatorPeace_3 != nullptr);

	elevatorPeace_4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("elevatorPeace_4"));
	elevatorPeace_4->SetupAttachment(dummyPos);
	check(elevatorPeace_4 != nullptr);	

	buttonPushCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ButtonCollision"));
	buttonPushCollision->SetupAttachment(elevatorPushButton);
	buttonPushCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	buttonPushCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	check(buttonPushCollision != nullptr);
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();
	
	check(elevatorCurveFloat != nullptr);
	FOnTimelineFloat ElevatorCallback;
	FOnTimelineEventStatic ElevatorFinishedCallback;

	ElevatorCallback.BindUFunction(this, FName("ElevatorStart"));
	ElevatorFinishedCallback.BindUFunction(this, FName("ElevatorEnd"));

	elevatorCurveFTimeline.AddInterpFloat(elevatorCurveFloat, ElevatorCallback);
	elevatorCurveFTimeline.SetTimelineFinishedFunc(ElevatorFinishedCallback);
	elevatorCurveFTimeline.SetTimelineLength(elevatorCurveLength);

	FOnTimelineFloat ElevatorButtonCallback;

	ElevatorButtonCallback.BindUFunction(this, FName("ElevatorButtonPushStart"));

	elevatorButtonCurveFTimeline.AddInterpFloat(elevatorButtonCurveFloat, ElevatorButtonCallback);
	elevatorButtonCurveFTimeline.SetTimelineLength(buttonPushCurveLength);

	buttonPushCollision->OnComponentBeginOverlap.AddDynamic(this, &AElevator::ElevatorButtonOnOverlapBegin);
	buttonPushCollision->OnComponentEndOverlap.AddDynamic(this, &AElevator::ElevatorButtonOnOverlapEnd);

	startVec = GetActorLocation();
	endVec = GetActorLocation() + (GetActorUpVector() * height);

}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	elevatorCurveFTimeline.TickTimeline(DeltaTime);
	elevatorButtonCurveFTimeline.TickTimeline(DeltaTime);
}

void AElevator::StartElevatorCurveFTimeline()
{
	if (bElevatorMove)return;
	bElevatorMove = true;
	elevatorCurveFTimeline.PlayFromStart();
}

void AElevator::ElevatorStart(float _val)
{
	FVector lerpVec = FMath::Lerp(startVec, endVec, _val);
	SetActorLocation(lerpVec);
}

void AElevator::ElevatorEnd()
{
	FVector tempVec = startVec;
	startVec = endVec;
	endVec = tempVec;
	bElevatorMove = false;
	SetButtonVector();
	ReverseElevatorButtonCurveFTimeline();
}

void AElevator::StartElevatorButtonCurveFTimeline()
{
	if (bElevatorMove)return;
	SetButtonVector();
	elevatorButtonCurveFTimeline.PlayFromStart();
}

void AElevator::ReverseElevatorButtonCurveFTimeline()
{
	if (bElevatorMove)return;
	elevatorButtonCurveFTimeline.Reverse();
}

void AElevator::ElevatorButtonPushStart(float _val)
{
	FVector lerpVec = FMath::Lerp(buttonStartVec, buttonEndVec, _val);
	elevatorPushButton->
		SetWorldLocation(lerpVec);
}

void AElevator::SetButtonVector()
{
	buttonStartVec = GetActorLocation() - (GetActorUpVector() * buttonPushHeight);
	buttonEndVec = GetActorLocation() - (GetActorUpVector() * buttonPushHeight * 2);
}

void AElevator::ElevatorButtonOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainChar* castMainChar = Cast<AMainChar>(OtherActor);

		if (castMainChar)
		{
			StartElevatorButtonCurveFTimeline();
			GetWorld()->GetTimerManager().SetTimer(pushButtonHandle, FTimerDelegate::CreateLambda([&]()
			{
					StartElevatorCurveFTimeline();

			}), pushButtonDelayTime, false);
		}
	}
}

void AElevator::ElevatorButtonOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainChar* castMainChar = Cast<AMainChar>(OtherActor);
		
		if (castMainChar)
		{
			ReverseElevatorButtonCurveFTimeline();
			if(!bElevatorMove)
				GetWorldTimerManager().ClearTimer(pushButtonHandle);
		}
	}
}

