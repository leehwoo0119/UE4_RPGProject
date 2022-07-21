// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController_Boss.h"
#include "MonsterBase.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"
#include "EAIState.h"
#include "MonsterStateMachine.h"
#include "MonsterWidgetComponent.h"
#include "MonsterBaseStatus.h"
#include "MonsterBaseAnimInstance.h"
#include "LHW/MainChar.h"
#include "MonsterCombatComponent.h"

AMonsterController_Boss::AMonsterController_Boss()
{
	bAttachToPawn = true;

	aiSightRadius = 5000.0f;
	aiLoseSightRadius = 5000.0f;
	aiFieldOfView = 180.0f;
	aiViewBackwardOffset = 100.0f;
	aiNearClippingRadius = 100.0f;

	PerceptionSetting();
}

void AMonsterController_Boss::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterController_Boss::PerceptionSetting()
{
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));

	sightConfig->SightRadius = aiSightRadius;
	sightConfig->LoseSightRadius = aiLoseSightRadius;
	sightConfig->PeripheralVisionAngleDegrees = aiFieldOfView;
	sightConfig->PointOfViewBackwardOffset = aiViewBackwardOffset;
	sightConfig->NearClippingRadius = aiNearClippingRadius;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterController_Boss::OnTargetDetected);

	GetPerceptionComponent()->SetDominantSense(*sightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*sightConfig);
}

void AMonsterController_Boss::OnTargetDetected(AActor* _actor, FAIStimulus const _stimulus)
{
	if (monsterBase->MON_BEHAVIORSTATE != EAI_Behavior_State::Die)
	{
		if (auto mainChar = Cast<AMainChar>(_actor))
		{
			GetBlackboardComponent()->SetValueAsObject("focusActorKey", mainChar);
			GetBlackboardComponent()->SetValueAsVector("targetLocationKey", mainChar->GetActorLocation());
			SetFocus(mainChar);

			//monsterBase->MON_COMBATCOMP->WeaponEquip();
		}
	}
}

