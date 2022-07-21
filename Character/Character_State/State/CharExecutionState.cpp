// Fill out your copyright notice in the Description page of Project Settings.


#include "CharExecutionState.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "LSH/MonsterBase.h"
#include "MainChar.h"
#include "CharTargetLockOnComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "LSH/MonsterCombatComponent.h"
#include "LSH/MonsterStateMachine.h"
#include "LSH/MonsterBaseStatus.h"
#include "Kismet/GameplayStatics.h"
#include "ExecutionCamera.h"
#include "Camera/CameraComponent.h"
#include "CharCombatComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CharStatusComponent.h"

UCharExecutionState::UCharExecutionState() :
    distance(0.0f),
    bBossPhaseChangeOrGroggyKill(false),
    targetMonster(nullptr),
    executionCameraZoomCurveLength(0.05f)
{
    PrimaryComponentTick.bCanEverTick = true;

    minExecutionCameraSpringLength = 120.0;
    maxExecutionCameraSpringLength = 230.f;
}

void UCharExecutionState::BeginPlay()
{
    Super::BeginPlay();

    FOnTimelineFloat ExecutionCameraZoomCallback;

    ExecutionCameraZoomCallback.BindUFunction(this, FName("ExecutioncameraZoomStart"));

    executionCameraZoomCurveFTimeline.AddInterpFloat(mainChar->GetCharCombatComponent()->GetExecutionCameraZoomCurveFloat(), ExecutionCameraZoomCallback);
    executionCameraZoomCurveFTimeline.SetTimelineLength(executionCameraZoomCurveLength);
}

void UCharExecutionState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    executionCameraZoomCurveFTimeline.TickTimeline(DeltaTime);
}

void UCharExecutionState::Enter()
{
    Super::Enter();

    targetMonster = mainChar->GetCharStatusComponent()->GetExecutionTarget();
    check(targetMonster != nullptr);

    executionType = 0;
    mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Execution);
    mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
    //mainChar->GetCharStatusComponent()->SetMoveStop(true);
    bBossPhaseChangeOrGroggyKill = false;

}

void UCharExecutionState::Execute()
{
    Super::Execute();

    //보스
    if (targetMonster->MON_COMBATCOMP->GetAIMonsterType() == EAI_MonsterType::Boss)
    {
        bBossPhaseChangeOrGroggyKill = true;
        distance = 300.0f;

        if (targetMonster->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero)
        {
            if (targetMonster->MON_BOSSPHASESTATE == EAI_BossPhase_State::FirstPhase)
            {
                SetCharPos(true);
                return;
            }
            else if (targetMonster->MON_BOSSPHASESTATE == EAI_BossPhase_State::SecondPhase)
            {
                //죽을땐 일반 몹과 같다.
                bBossPhaseChangeOrGroggyKill = false;
                ExecutionSetting();
            }
        }
        else if (targetMonster->MON_COMBATSTATE == EAI_Combat_State::Groggy)
        {
            SetCharPos(false);
            return;
        }
    }
    //잡몹
    else
    {
        ExecutionSetting();
    }
}

void UCharExecutionState::Exit()
{
    Super::Exit();

    ResetCameraRot();

    mainChar->GetCharStatusComponent()->SetMoveStop(false);
    mainChar->GetCharStatusComponent()->SetExecutionTarget(nullptr);
    UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(
        mainChar->GetCharCamera()->GetOwner(), 0.3f);
    mainChar->GetCharStatusComponent()->SetExecutionState(EExecutionState_Type::Default);
}

void UCharExecutionState::ExecutionSetting()
{
    executionType = FMath::RandRange(1, 3);

    distance = 220.0f;

    if (executionType == 3)
    {
        distance = 270.0f;
    }

    SetCharPos();
}

void UCharExecutionState::MonsterNormalExecution()
{
    StartExecutionCamera();
    mainChar->bUseControllerRotationYaw = false;
    mainChar->GetCharTargetLockOnComponent()->ResetTarget();

    EExecutionState_Type castingExecutionType = static_cast<EExecutionState_Type>(executionType);
    mainChar->GetCharStatusComponent()->SetExecutionState(castingExecutionType);

    targetMonster->CustomTimeDilation = 1.0f;
    targetMonster->MON_COMBATCOMP->ExecuteExecuted(castingExecutionType);
    targetMonster->MON_COMBATCOMP->Die();

    mainChar->GetAnimInstance()->StopAllMontages(0.0f);
    switch (mainChar->GetCharStatusComponent()->GetExecutionState())
    {
        //일단뺌
    case EExecutionState_Type::Execution_1:
        //mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->executionMontage, 1.0f, FName("Execution_1"));
        break;
    case EExecutionState_Type::Execution_2:
        mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->executionMontage, 0.7f, FName("Execution_2"));
        break;
    case EExecutionState_Type::Execution_3:
        mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->executionMontage, 0.8f, FName("Execution_3"));
        break;
    case EExecutionState_Type::Execution_4:
        mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->executionMontage, 0.7f, FName("Execution_4"));
        break;
    case EExecutionState_Type::Default:
        break;
    }
}

void UCharExecutionState::MonsterBossExecution(bool _bBossPhaseChange)
{
    StartExecutionCamera();
    mainChar->bUseControllerRotationYaw = false;
    targetMonster->CustomTimeDilation = 1.0f;

    mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->executionMontage, 1.0f, FName("BossPhaseChangeExecution"));

    //페이즈 변환 애니
    if (_bBossPhaseChange)
    {
        targetMonster->MON_COMBATCOMP->BossPhaseChange(false);
    }
    //그로기 애니
    else
    {
        targetMonster->MON_COMBATCOMP->GroggyExecuted();
    }
}

void UCharExecutionState::SetCharPos(bool _bBossPhaseChange)
{
    FRotator enemyLookAtCharRot =
        UKismetMathLibrary::FindLookAtRotation(
            FVector(mainChar->GetCharStatusComponent()->GetExecutionTarget()->GetActorLocation().X,
                mainChar->GetCharStatusComponent()->GetExecutionTarget()->GetActorLocation().Y,
                0.0f),
            FVector(mainChar->GetActorLocation().X,
                mainChar->GetActorLocation().Y,
                0.0f));

    targetMonster->bUseControllerRotationYaw = false;
    targetMonster->SetActorRotation(enemyLookAtCharRot);

    FVector moveCharLocation =
        targetMonster->GetActorLocation() + (targetMonster->GetActorForwardVector() * distance);

    FRotator charLookAtEnemyRot =
        UKismetMathLibrary::FindLookAtRotation(
            FVector(mainChar->GetActorLocation().X,
                mainChar->GetActorLocation().Y,
                0.0f),
            FVector(targetMonster->GetActorLocation().X,
                targetMonster->GetActorLocation().Y,
                0.0f));

    mainChar->SetActorRotation(charLookAtEnemyRot);

    MoveToLocaition(moveCharLocation, mainChar, bBossPhaseChangeOrGroggyKill, _bBossPhaseChange, charLookAtEnemyRot);

}

void UCharExecutionState::StartExecutionCamera()
{
    UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(
        mainChar->GetCharCombatComponent()->GetCharExecutionCamera()->GetExecutionCamera()->GetOwner(), 0.3f);
}

void UCharExecutionState::ResetCameraRot()
{
    APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
    check(camManager != nullptr);
    FRotator resetCameraRot = FRotator(
        camManager->GetCameraRotation().Pitch,
        mainChar->GetActorRotation().Yaw,
        camManager->GetCameraRotation().Roll);
    UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(resetCameraRot);
}

void UCharExecutionState::MonsterMove()
{
    FVector monVec = mainChar->GetActorLocation() + (mainChar->GetActorForwardVector() * distance);
    monVec.Z = targetMonster->GetActorLocation().Z;

    targetMonster->SetActorLocation(monVec);
}

void UCharExecutionState::StartExecutioncameraZoomCurveFTimeline()
{
    executionCameraZoomCurveFTimeline.PlayFromStart();
}

void UCharExecutionState::ReverseExecutioncameraZoomCurveFTimeline()
{
    executionCameraZoomCurveFTimeline.Reverse();
}

void UCharExecutionState::ExecutioncameraZoomStart(float _val)
{
    float lerpSpringLength = FMath::Lerp(maxExecutionCameraSpringLength, minExecutionCameraSpringLength, _val);

    mainChar->GetCharCombatComponent()->GetCharExecutionCamera()->GetSpringArm()->TargetArmLength = lerpSpringLength;
}