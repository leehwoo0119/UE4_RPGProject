// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondBossPatternWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "LHW/SecondBossAreaPattern.h"
#include "LHW/MainChar.h"
#include "LHW/CharTargetLockOnComponent.h"
#include "LHW/CharStatusComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASecondBossPatternWeapon::ASecondBossPatternWeapon() :destroyCnt(0), maxDestroyCnt(5)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	dummyPos = CreateDefaultSubobject<USceneComponent>(TEXT("dummyPos"));
	dummyPos->SetupAttachment(GetRootComponent());
	check(dummyPos != nullptr);

	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("damageWeaponMesh"));
	check(weaponMesh != nullptr);
	weaponMesh->SetupAttachment(dummyPos);

	damageArea = CreateDefaultSubobject<UBoxComponent>(TEXT("damageArea"));
	check(damageArea != nullptr);
	damageArea->SetupAttachment(weaponMesh);

	targetLockOnWidget = CreateDefaultSubobject<UWidgetComponent>("targetLockOnWidget");
	targetLockOnWidget->AttachToComponent(dummyPos, FAttachmentTransformRules::KeepRelativeTransform);
	check(targetLockOnWidget != nullptr);
}


// Called when the game starts or when spawned
void ASecondBossPatternWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASecondBossPatternWeapon::CheckDestroy()
{
	check(bossPattern != nullptr);
	destroyCnt++;

	if (destroyCnt >= maxDestroyCnt)
	{
		AMainChar* mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		check(mainChar != nullptr);

		bossPattern->CheckPatternWeaponCnt(-1);
		mainChar->GetCharTargetLockOnComponent()->ResetTarget();
		mainChar->GetCharStatusComponent()->DeleteAttackedMonsterArr();
		Destroy();
	}
}
