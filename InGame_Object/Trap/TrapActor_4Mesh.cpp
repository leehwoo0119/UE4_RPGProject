// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapActor_4Mesh.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LHW/MainChar.h"
#include "LHW/CharStatusComponent.h"

ATrapActor_4Mesh::ATrapActor_4Mesh()
{
    firstMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("firstMesh"));

    pivotMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pivotMesh"));
    pivotMesh->SetupAttachment(firstMesh);

    secondMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("secondMesh"));
    secondMesh->SetupAttachment(pivotMesh);

    thirdMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("thirdMesh"));
    thirdMesh->SetupAttachment(secondMesh);

    damageCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("damageCollision"));
    damageCollision->SetupAttachment(thirdMesh);
}

void ATrapActor_4Mesh::BeginPlay()
{
    Super::BeginPlay();

    AddTrapArray(this);

    damageCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor_4Mesh::TrapCollisionOnOverlapBegin);
}

void ATrapActor_4Mesh::TrapCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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