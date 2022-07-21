// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapActor_DeactivateSwitch.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "LHW/MainCharArrow.h"

ATrapActor_DeactivateSwitch::ATrapActor_DeactivateSwitch()
{
	switchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("switchMesh"));
	switchMesh->SetupAttachment(RootComponent);
}

void ATrapActor_DeactivateSwitch::BeginPlay()
{
	Super::BeginPlay();

	trapCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor_DeactivateSwitch::TrapCollisionOnOverlapBegin);
}

void ATrapActor_DeactivateSwitch::TrapCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharArrow* mainCharArrow = Cast<AMainCharArrow>(OtherActor);

		if (mainCharArrow)
		{
			NotifyTrapDeactive(trapSection);
		}
	}
}
