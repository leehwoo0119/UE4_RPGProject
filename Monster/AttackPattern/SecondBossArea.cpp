// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondBossArea.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainChar.h"

// Sets default values
ASecondBossArea::ASecondBossArea() :bCharInArea(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	area = CreateDefaultSubobject<UBoxComponent>(TEXT("area"));
	check(area != nullptr);
	bottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bottomMesh"));
	check(bottomMesh != nullptr);
	bottomMesh->SetupAttachment(area);
}

// Called when the game starts or when spawned
void ASecondBossArea::BeginPlay()
{
	Super::BeginPlay();
	
	area->OnComponentBeginOverlap.AddDynamic(this, &ASecondBossArea::DamageCollisionOnOverlapBegin);
	area->OnComponentEndOverlap.AddDynamic(this, &ASecondBossArea::DamageCollisionOnOverlapEnd);

	SetAreaMaterialOpacity(true);
}

void ASecondBossArea::SetSafeAreaMaterial(bool _reset)
{
	if (_reset)
	{
		bottomMesh->SetScalarParameterValueOnMaterials(TEXT("ColorType"), 0.0f);
	}
	else
	{
		bottomMesh->SetScalarParameterValueOnMaterials(TEXT("ColorType"), 1.0f);
	}
}

void ASecondBossArea::SetAreaMaterialOpacity(bool _reset)
{
	if (_reset)
	{
		bottomMesh->SetScalarParameterValueOnMaterials(TEXT("Opacity"), 0.0f);
	}
	else
	{
		bottomMesh->SetScalarParameterValueOnMaterials(TEXT("Opacity"), 0.7f);
	}
}

bool ASecondBossArea::CheckDamage()
{
	return bCharInArea;
}

void ASecondBossArea::DamageCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainChar* mainChar = Cast<AMainChar>(OtherActor);

		if (mainChar)
		{
			bCharInArea = true;
		}
	}
}

void ASecondBossArea::DamageCollisionOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainChar* mainChar = Cast<AMainChar>(OtherActor);

		if (mainChar)
		{
			bCharInArea = false;
		}
	}
}



