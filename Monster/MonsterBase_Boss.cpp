// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase_Boss.h"
#include "MonsterController_Boss.h"
#include "MonsterBaseAnimInstance.h"
#include "MonsterStateMachine.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterWeaponSlot.h"
#include "MonsterWeapon.h"
#include "Components/BoxComponent.h"
#include "Monster_Boss_WidgetComponent.h"
#include "MonsterBossStatus.h"

AMonsterBase_Boss::AMonsterBase_Boss()
{
	monsterStateMachine = CreateDefaultSubobject<UMonsterStateMachine>(TEXT("monsterStateMachine"));
	check(monsterStateMachine != nullptr);

	monsterStatus = CreateDefaultSubobject<UMonsterBossStatus>(TEXT("monsterStatus"));
	check(monsterStatus != nullptr);

	stealthArea = CreateDefaultSubobject<UBoxComponent>(TEXT("stealthArea"));
	stealthArea->SetupAttachment(GetRootComponent());
	stealthArea->SetBoxExtent(FVector(60.0f, 70.0f, 60.0f));
	stealthArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	stealthArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AMonsterBase_Boss::BeginPlay()
{
	Super::BeginPlay();

	GetMonsterWeaponSlot()->GetMonsterFirstWeapon()->SetMonsterBase(this);
	if (GetMonsterWeaponSlot()->GetMonsterSecondWeapon())
		GetMonsterWeaponSlot()->GetMonsterSecondWeapon()->SetMonsterBase(this);

}

void AMonsterBase_Boss::SetBossWidgetComponent(UMonster_Boss_WidgetComponent* _comp)
{
	monsterWidgetComponent = _comp;
	check(monsterWidgetComponent != nullptr);
}


