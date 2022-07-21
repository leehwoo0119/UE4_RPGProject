// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapActor_3Mesh.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LHW/MainChar.h"
#include "LHW/CharStatusComponent.h"

ATrapActor_3Mesh::ATrapActor_3Mesh()
{
    firstMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("firstMesh"));

    secondMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("secondMesh"));
    secondMesh->SetupAttachment(firstMesh);

    thirdMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("thirdMesh"));
    thirdMesh->SetupAttachment(secondMesh);

    damageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("damageCollision"));
    damageCollision->SetupAttachment(thirdMesh);
}

void ATrapActor_3Mesh::BeginPlay()
{
    Super::BeginPlay();

    AddTrapArray(this);

    damageCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor_3Mesh::TrapCollisionOnOverlapBegin);
}

void ATrapActor_3Mesh::TrapCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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