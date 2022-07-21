// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController_Normal.h"
#include "MonsterBase.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"
#include "GameFramework/Character.h"
#include "MonsterStateMachine.h"
#include "MonsterWidgetComponent.h"
#include "MonsterBaseStatus.h"
#include "LHW/MainChar.h"
#include "MonsterBaseAnimInstance.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterCombatComponent.h"
#include "LHW/MainCharArrow.h"
#include "DrawDebugHelpers.h"

AMonsterController_Normal::AMonsterController_Normal()
{
	bAttachToPawn = true;
}

void AMonsterController_Normal::BeginPlay()
{
	Super::BeginPlay();	

	sightId = sightConfig->GetSenseID();
	hearingId = hearingConfig->GetSenseID();

	if (UseBlackboard(BBAsset, Blackboard))
	{
		RunBehaviorTree(BTAsset);
	}

	GetBlackboardComponent()->SetValueAsEnum("enum_AIBehaviorState", (uint8)EAI_Behavior_State::Idle);
	GetBlackboardComponent()->SetValueAsEnum("enum_AICombatState", (uint8)EAI_Combat_State::Wait);
	GetBlackboardComponent()->SetValueAsEnum("enum_AIMeleeAttackState", (uint8)EAI_MeleeAttack_State::AttackGroup_1);
}

void AMonsterController_Normal::PerceptionSetting()
{
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	hearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));

	sightConfig->SightRadius = aiSightRadius;
	sightConfig->LoseSightRadius = aiLoseSightRadius;
	sightConfig->PeripheralVisionAngleDegrees = aiFieldOfView;
	sightConfig->PointOfViewBackwardOffset = aiViewBackwardOffset;
	sightConfig->NearClippingRadius = aiNearClippingRadius;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	hearingConfig->HearingRange = aiHearingRange;
	hearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	hearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	hearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterController_Normal::OnTargetDetected);

	GetPerceptionComponent()->SetDominantSense(*sightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*sightConfig);
	GetPerceptionComponent()->SetDominantSense(*hearingConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*hearingConfig);
}

void AMonsterController_Normal::OnTargetDetected(AActor* _actor, FAIStimulus const _stimulus)
{
	if (monsterBase->MON_BEHAVIORSTATE != EAI_Behavior_State::WeaponSwap &&
		monsterBase->MON_BEHAVIORSTATE != EAI_Behavior_State::Battle &&
		monsterBase->MON_BEHAVIORSTATE != EAI_Behavior_State::ReturnPatrol &&
		monsterBase->MON_BEHAVIORSTATE != EAI_Behavior_State::Die)
	{
		AMainChar* mainChar = Cast<AMainChar>(_actor);
		AMainCharArrow* mainCharArrow = Cast<AMainCharArrow>(_actor);

		if (mainChar)
		{
			if (_stimulus.Type == sightId)
			{
				monsterBase->MON_ANIM->Montage_Stop(1.0f, NULL);
				GetBlackboardComponent()->SetValueAsObject("focusActorKey", mainChar);
				GetBlackboardComponent()->SetValueAsVector("targetLocationKey", mainChar->GetActorLocation());

				monsterBase->MON_COMBATCOMP->WeaponEquip();
			}
		}

		if (_stimulus.Type == hearingId)
		{
			if (_stimulus.Tag == "PlayerNoise")
			{
				float maxBottomDis = 1000.0f;

				FHitResult hitResult;			
				FVector noiseBottomLoc = FVector(_stimulus.StimulusLocation.X, _stimulus.StimulusLocation.Y, _stimulus.StimulusLocation.Z - maxBottomDis);
				FCollisionObjectQueryParams collisionObjectParams;
				collisionObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
				collisionObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

				FCollisionQueryParams collisionParams;

				collisionParams.AddIgnoredActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

				GetWorld()->LineTraceSingleByObjectType(
					hitResult,
					_stimulus.StimulusLocation,
					noiseBottomLoc,
					collisionObjectParams,
					collisionParams);

				FHitResult wallHitResult;
				FCollisionObjectQueryParams collisionWallObjParams;
				collisionWallObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
				collisionWallObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
				FCollisionQueryParams collisionWallParams;
				collisionWallParams.AddIgnoredActor(monsterBase);
				collisionWallParams.AddIgnoredActor(mainChar);

				GetWorld()->LineTraceSingleByObjectType(
					wallHitResult,
					monsterBase->GetActorLocation(),
					_stimulus.StimulusLocation,
					collisionWallObjParams,
					collisionWallParams);
	
				if (hitResult.bBlockingHit && wallHitResult.Actor == mainCharArrow)
				{
					FVector noiseLoc = hitResult.Location;
					
					FVector randomNoiseVec = UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), noiseLoc, 200.0f);
					GetBlackboardComponent()->SetValueAsVector("targetLocationKey", randomNoiseVec);
					monsterBase->MON_STATUS->SetInvestigateMoveStart(false);
					monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::Investigate);
					monsterBase->MON_WIDGETCOMP->SetMonsterStateImageVisible();
				}
			}
		}
	}
	
	if (auto mainChar = Cast<AMainChar>(_actor) && _stimulus.Type == sightId) 
	{
		monsterBase->MON_STATUS->SetPlayerSee(_stimulus.WasSuccessfullySensed());
		GetBlackboardComponent()->SetValueAsBool("bIsCanSeePlayer", _stimulus.WasSuccessfullySensed());
	}
}

void AMonsterController_Normal::HearingSensePause(bool _value)
{
	GetPerceptionComponent()->SetSenseEnabled(UAISense_Hearing::StaticClass(), _value);

	// 1.5초 후 감지 복구
	FTimerHandle WaitHandle;
	GetWorld()->GetTimerManager().SetTimer(WaitHandle, FTimerDelegate::CreateLambda([&]()
	{
		GetPerceptionComponent()->SetSenseEnabled(UAISense_Hearing::StaticClass(), !_value);
	}), 1.5f, false);
}