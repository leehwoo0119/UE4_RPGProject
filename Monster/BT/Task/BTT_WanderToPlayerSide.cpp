// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_WanderToPlayerSide.h"
#include "MonsterController_Normal.h"
#include "MonsterBase_Normal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BlackBoard_Monster_Key.h"
#include "Math/UnrealMathUtility.h"


UBTT_WanderToPlayerSide::UBTT_WanderToPlayerSide()
{
	NodeName = TEXT("WanderToPlayerSide");
}

EBTNodeResult::Type UBTT_WanderToPlayerSide::ExecuteTask(UBehaviorTreeComponent& _ownerComp, uint8* _nodeMemory)
{
	UBlackboardComponent* myBlackboard = _ownerComp.GetBlackboardComponent();
	if (!myBlackboard)
	{
		UE_LOG(LogTemp, Warning, TEXT("WanderToPlayerSide myBlackboard null Error!"));
		return EBTNodeResult::Failed;
	}
	auto playerChar = myBlackboard->GetValueAsObject("focusActorKey");
	if (!playerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("WanderToPlayerSide playerChar null Error!"));
		return EBTNodeResult::Failed;
	}
	
	APawn* castPlayerChar = Cast<APawn>(playerChar);
	if (!castPlayerChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("WanderToPlayerSide castPlayerChar null Error!"));
		return EBTNodeResult::Failed;
	}

	AAIController* aiCharController = _ownerComp.GetAIOwner();
	if (!aiCharController)
	{
		UE_LOG(LogTemp, Warning, TEXT("WanderToPlayerSide aiCharController null Error!"));
		return EBTNodeResult::Failed;
	}
	aiCharController->SetFocus(castPlayerChar);

	AMonsterBase* aIMonster = Cast<AMonsterBase>(_ownerComp.GetAIOwner()->GetPawn());
	if (!aIMonster)
	{
		UE_LOG(LogTemp, Warning, TEXT("WanderToPlayerSide aIMonster null Error!"));
		return EBTNodeResult::Failed;
	}
	FVector aiCharLocation = aIMonster->GetActorLocation();
	FVector aiCharForwardVec = aIMonster->GetActorForwardVector();
	FVector aiCharRightVec = aIMonster->GetActorRightVector();
	FVector sumVec;

	int behaviorNum = FMath::RandRange(0, 1);

	if (aIMonster->GetWanderMoveCancel())
	{
		myBlackboard->SetValueAsVector("targetLocationKey", castPlayerChar->GetActorLocation());
	}
	else
	{
		switch (behaviorNum)
		{
		case 0:
			// Left Wander
			sumVec = (aiCharLocation + (aiCharForwardVec * 200.0f) + (aiCharRightVec * (-300.0f)));
			//UE_LOG(LogTemp, Log, TEXT("Left Wander"));
			break;
		case 1:
			// Right Wander
			sumVec = (aiCharLocation + (aiCharForwardVec * 200.0f) + (aiCharRightVec * 300.0f));
			//UE_LOG(LogTemp, Log, TEXT("Right Wander"));
			break;
		default:
			break;
		}

		myBlackboard->SetValueAsVector("targetLocationKey", sumVec);
	}
	
	return EBTNodeResult::Succeeded;
}