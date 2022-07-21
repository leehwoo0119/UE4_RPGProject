// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase_Normal.h"
#include "MonsterController_Normal.h"
#include "MonsterBaseAnimInstance.h"
#include "MonsterStateMachine.h"
#include "LHW/PatrolPath.h"
#include "LHW/ProjectGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterWeaponSlot.h"
#include "MonsterWeapon.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Monster_Normal_WidgetComponent.h"
#include "MonsterNormalStatus.h"

AMonsterBase_Normal::AMonsterBase_Normal()
{
	PrimaryActorTick.bCanEverTick = false;

	monsterStatus = CreateDefaultSubobject<UMonsterNormalStatus>(TEXT("monsterStatus"));
	check(monsterStatus != nullptr);

	monsterWidgetComponent = CreateDefaultSubobject<UMonster_Normal_WidgetComponent>("monsterWidgetComponent");
	check(monsterWidgetComponent != nullptr);

	monsterStateMachine = CreateDefaultSubobject<UMonsterStateMachine>(TEXT("monsterStateMachine"));
	check(monsterStateMachine != nullptr);
	
	monsterPointBarWidget = CreateDefaultSubobject<UWidgetComponent>("monsterPointBarWidget");
	monsterPointBarWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	check(monsterPointBarWidget != nullptr);

	stealthArea = CreateDefaultSubobject<UBoxComponent>(TEXT("stealthArea"));
	stealthArea->SetupAttachment(GetRootComponent());
	stealthArea->SetBoxExtent(FVector(60.0f, 70.0f, 60.0f));
	stealthArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	stealthArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AMonsterBase_Normal::BeginPlay()
{
	Super::BeginPlay();

	gameIS = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(gameIS != nullptr);

	GetMonsterWeaponSlot()->GetMonsterFirstWeapon()->SetMonsterBase(this);
	if (GetMonsterWeaponSlot()->GetMonsterSecondWeapon())
		GetMonsterWeaponSlot()->GetMonsterSecondWeapon()->SetMonsterBase(this);
}

APatrolPath* AMonsterBase_Normal::GetPatrolPath()
{
	return patrolPath;
}