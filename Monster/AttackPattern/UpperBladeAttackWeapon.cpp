// Fill out your copyright notice in the Description page of Project Settings.


#include "UpperBladeAttackWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainChar.h"
#include "LSH/MonsterBase.h"
#include "UpperBladeAttackPattern.h"

// Sets default values
AUpperBladeAttackWeapon::AUpperBladeAttackWeapon() :bDamageOnce(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	dummyPos = CreateDefaultSubobject<USceneComponent>(TEXT("dummyPos"));
	dummyPos->SetupAttachment(GetRootComponent());
	check(dummyPos != nullptr);

	damageWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("damageWeaponMesh"));
	check(damageWeaponMesh != nullptr);
	damageWeaponMesh->SetupAttachment(dummyPos);

	damageArea = CreateDefaultSubobject<UBoxComponent>(TEXT("damageArea"));
	check(damageArea != nullptr);
	damageArea->SetupAttachment(damageWeaponMesh);

	bottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bottomMesh"));
	check(bottomMesh != nullptr);
	bottomMesh->SetupAttachment(dummyPos);

}

// Called when the game starts or when spawned
void AUpperBladeAttackWeapon::BeginPlay()
{
	Super::BeginPlay();

	damageArea->OnComponentBeginOverlap.AddDynamic(this, &AUpperBladeAttackWeapon::DamageCollisionOnOverlapBegin);

}

void AUpperBladeAttackWeapon::DamageCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainChar* mainChar = Cast<AMainChar>(OtherActor);

		if (mainChar)
		{
			if (!bDamageOnce)
			{
				check(monster != nullptr);
				if (patternParent)
				{
					patternParent->SetAttackOnce();
				}
				else
				{
					bDamageOnce = true;
				}

;				UGameplayStatics::ApplyDamage(mainChar, 45.0f, monster->GetController(), monster, NULL);
			}			
		}
	}
}

void AUpperBladeAttackWeapon::WeaponOpacityChange(float _val)
{
	for (int num = 0; num < damageWeaponMesh->GetNumMaterials(); num++)
	{
		damageWeaponMesh->CreateDynamicMaterialInstance(
			num,
			damageWeaponMesh->GetMaterial(num))
			->SetScalarParameterValue(
				FName("Appearance"),
				_val);
	}
}

void AUpperBladeAttackWeapon::SetDamageOnce()
{
	bDamageOnce = true;
}
