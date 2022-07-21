// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveCamp.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ProjectGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASaveCamp::ASaveCamp() :bSearch(false), itemID(-1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	saveCampMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("elevatorPeace_4"));
	saveCampMesh->SetupAttachment(GetRootComponent());
	check(saveCampMesh != nullptr);

	saveCampCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("saveCampCollision"));
	saveCampCollision->SetupAttachment(saveCampMesh);
	saveCampCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	saveCampCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	check(saveCampCollision != nullptr);
}

// Called when the game starts or when spawned
void ASaveCamp::BeginPlay()
{
	Super::BeginPlay();

	gameIS = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(gameIS != nullptr);
	
	//IDÃÊ±âÈ­ ¾ÈµÊ
	check(itemID != -1);
}

void ASaveCamp::SetSaveCampCollision(bool _bCollision)
{
	if (_bCollision)
	{
		saveCampCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		saveCampCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}


