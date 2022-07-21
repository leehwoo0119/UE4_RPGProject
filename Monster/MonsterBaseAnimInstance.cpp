// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBaseAnimInstance.h"
#include "MonsterBase.h"
#include "MonsterStateMachine.h"
#include "Kismet/GameplayStatics.h"
#include "LHW/CharStatusComponent.h"

void UMonsterBaseAnimInstance::NativeInitializeAnimation()
{
	pawn = TryGetPawnOwner();

	if (pawn)
		aiChar = Cast<AMonsterBase>(pawn);
	if (aiChar)
		animInstance = Cast<UAnimInstance>(aiChar->GetMesh()->GetAnimInstance());

	state = EAI_Behavior_State::Idle;
	weaponTypeState = EAI_BossWeaponType_State::Katana;
}

void UMonsterBaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	check(pawn != nullptr);
	check(aiChar != nullptr);
	check(animInstance != nullptr);

	check(attackMontage != nullptr);
	check(stealthedMontage != nullptr);
	//check(defenseMontage != nullptr);
	check(hitMontage != nullptr);
	check(executedMontage != nullptr);
	//check(lookAroundMontage != nullptr);
	check(weaponSwapMontage != nullptr);

	monsterStateMachine = aiChar->MON_STATEMACHINE;
	check(monsterStateMachine != nullptr);
}

void UMonsterBaseAnimInstance::UpdateAnimationProperties()
{
	if (pawn) {
		FVector speed = pawn->GetVelocity();
		moveSpeed = speed.Size();
		FRotator rot = pawn->GetActorRotation();
		direction = animInstance->CalculateDirection(speed, rot);
	}
	if (monsterStateMachine)
	{
		state = monsterStateMachine->GetBehaviorState();
		weaponTypeState = monsterStateMachine->GetBossWeaponTypeState();
	}
}

void UMonsterBaseAnimInstance::AnimMontagePlay(UAnimMontage* _animMontage, float _animSpeed, FName _montageSectionName)
{
	aiChar->GetMesh()->GetAnimInstance()->Montage_Play(_animMontage, _animSpeed);
	aiChar->GetMesh()->GetAnimInstance()->Montage_JumpToSection(_montageSectionName, _animMontage);
}