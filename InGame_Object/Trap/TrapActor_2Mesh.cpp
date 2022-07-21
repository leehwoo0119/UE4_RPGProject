// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapActor_2Mesh.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LHW/MainChar.h"
#include "LHW/CharStatusComponent.h"

ATrapActor_2Mesh::ATrapActor_2Mesh()
{
    firstMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("firstMesh"));

    secondMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("secondMesh"));
    secondMesh->SetupAttachment(firstMesh);

    damageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("skipesCollision"));
    damageCollision->SetupAttachment(secondMesh);
}

void ATrapActor_2Mesh::BeginPlay()
{
    Super::BeginPlay();

    AddTrapArray(this);

    damageCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor_2Mesh::TrapCollisionOnOverlapBegin);
}

void ATrapActor_2Mesh::TrapCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        AMainChar* mainChar = Cast<AMainChar>(OtherActor);

        if (mainChar)
        {
            UGameplayStatics::ApplyDamage(mainChar, mainChar->GetCharStatusComponent()->GetMaxCharHealthPoint() * 0.4f, nullptr, this, nullptr);
        }
    }
}