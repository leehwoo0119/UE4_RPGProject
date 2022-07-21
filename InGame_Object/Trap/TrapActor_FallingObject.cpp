// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapActor_FallingObject.h"
#include "DestructibleComponent.h"
#include "Components/BoxComponent.h"
#include "LHW/MainCharArrow.h"
#include "LHW/MainChar.h"
#include "MonsterBase.h"
#include "Kismet/GameplayStatics.h"
#include "LHW/CharStatusComponent.h"
#include "MonsterStateMachine.h"
#include "MonsterCombatComponent.h"

ATrapActor_FallingObject::ATrapActor_FallingObject()
{
	destructibleMesh = CreateDefaultSubobject<UDestructibleComponent>(TEXT("destructibleMesh"));
	destructibleMesh->SetupAttachment(RootComponent);

	trapCollision->SetSimulatePhysics(true);
	destructibleMesh->SetSimulatePhysics(true);
	trapCollision->SetEnableGravity(false);
	destructibleMesh->SetEnableGravity(false);

	destroyTimer = 5;
}

void ATrapActor_FallingObject::BeginPlay()
{
	Super::BeginPlay();

	trapCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapActor_FallingObject::TrapCollisionOnOverlapBegin);
}

void ATrapActor_FallingObject::TrapCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharArrow* mainCharArrow = Cast<AMainCharArrow>(OtherActor);
		AMainChar* mainChar = Cast<AMainChar>(OtherActor);
		AMonsterBase* monsterBase = Cast<AMonsterBase>(OtherActor);

		if (mainCharArrow)
		{
			trapCollision->SetEnableGravity(true);
			destructibleMesh->SetEnableGravity(true);
		}

		if (mainChar)
		{
			UGameplayStatics::ApplyDamage(mainChar, mainChar->GetCharStatusComponent()->GetMaxCharHealthPoint() * 0.1f, nullptr, this, nullptr);
		}

		if (monsterBase)
		{
			monsterBase->MON_COMBATCOMP->MonsterDeathProcessing();
			monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::Die);
		}
	}

	if (SweepResult.GetComponent())
	{
		GetWorld()->GetTimerManager().SetTimer(destroyHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (destroyTimer <= 0)
			{
				GetWorldTimerManager().ClearTimer(destroyHandle);
				Destroy();
			}
			destroyTimer--;
		}), 1.0f, true);
	}
}