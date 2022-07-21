// Fill out your copyright notice in the Description page of Project Settings.


#include "DotDamageActor.h"
#include "LHW/MainChar.h"
#include "MonsterBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "LHW/CharStatusComponent.h"
#include "Kismet/GameplayStatics.h"

ADotDamageActor::ADotDamageActor()
{
	PrimaryActorTick.bCanEverTick = false;

	particleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("particleSystemComp"));
	SetRootComponent(particleSystemComp);

	Tags.Add("DotDamage");

	flameDamageNum = 0;
	poisonDamageNum = 0;
	poisonStack = 0;
	frozenStack = 0;

	maxFlameDamageNum = 4;
	maxPoisonDamageNum = 6;
	maxPoisonStack = 3;
	maxFrozenStack = 3;

	poisonDotTimer = 7;
	FrozenCCTimer = 7;
}

void ADotDamageActor::BeginPlay()
{
	Super::BeginPlay();

	mainChar = Cast<AMainChar>(GetOwner());
	monsterBase = Cast<AMonsterBase>(GetOwner());
}

void ADotDamageActor::DotDamageActorDestroy()
{
	GetWorldTimerManager().ClearTimer(flameDotDamageHandle);
	GetWorldTimerManager().ClearTimer(poisonDotDamageHandle);
	GetWorldTimerManager().ClearTimer(poisonStackHandle);
	GetWorldTimerManager().ClearTimer(frozenCrowdControlHandle);
	GetWorldTimerManager().ClearTimer(frozenStackHandle);
	flameDamageNum = 0;
	poisonDamageNum = 0;
	poisonStack = 0;
	frozenStack = 0;
	poisonDotTimer = 7;
	FrozenCCTimer = 7;
	Destroy();
}

void ADotDamageActor::SetFlameDamageNum(int _value)
{
	flameDamageNum = _value;
}

void ADotDamageActor::PlusPoisonStack()
{
	poisonStack++;
}

void ADotDamageActor::PlusFrozenStack()
{
	frozenStack++;
}

void ADotDamageActor::SetPoisonDotTimerNum(int _value)
{
	poisonDotTimer = _value;
}

void ADotDamageActor::SetFrozenCCTimerNum(int _value)
{
	FrozenCCTimer = _value;
}

