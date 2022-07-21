// Fill out your copyright notice in the Description page of Project Settings.


#include "CharUltimateState.h"
#include "MainChar.h"
#include "Components/CapsuleComponent.h"
#include "MainCharAnimInstance.h"
#include "CharCombatComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharController.h"
#include "LSH/MonsterBase.h"
#include "LSH/MonsterBase_Boss.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "CharCombatComponent.h"
#include "CharStatusComponent.h"
#include "SecondBossAreaPattern.h"
#include "SaveCamp.h"
#include "InstantDeathCamera.h"
#include "MainCharWeaponSlot.h"
#include "MainCharKatana.h"
#include "LSH/MonsterWeapon.h"
#include "LSH/MonsterBaseStatus.h"

UCharUltimateState::UCharUltimateState()
{
    PrimaryComponentTick.bCanEverTick = false;

}

void UCharUltimateState::BeginPlay()
{
    Super::BeginPlay();

}


void UCharUltimateState::Enter()
{
    Super::Enter();
    mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Skill);
    mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);
    mainChar->GetCharStatusComponent()->SetMoveStop(true);
    mainChar->GetCharStatusComponent()->PlusOrSetCurCharUltimatePoint(0.0f, true);
}

void UCharUltimateState::Execute()
{
    Super::Execute();

    startLoc = mainChar->GetActorLocation();
    mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->skillMontage, 1.0f, FName("UltimateAttack"));
    mainChar->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void UCharUltimateState::Exit()
{
    Super::Exit();
    mainChar->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
    mainChar->GetCharStatusComponent()->SetMoveStop(false);
}

void UCharUltimateState::UltimateMoveCompTo(float _value)
{
    UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(
        mainChar->GetCharCombatComponent()->GetCharInstantDeathCamera()->GetUltimateAttackCamera()->GetOwner(), 0.3f);

    FVector targetLoc;
    float moveSpeed = 0.2f;

    targetLoc = mainChar->GetActorLocation() + mainChar->GetActorForwardVector() * _value;
    FRotator targetRot = mainChar->GetActorRotation();

    float collisionDis = 0.0f;

    FVector startVec = FVector(mainChar->GetActorLocation().X, mainChar->GetActorLocation().Y, mainChar->GetActorLocation().Z + 50.0f);
    FVector endVec = FVector(targetLoc.X, targetLoc.Y, targetLoc.Z + 50.0f);

    collisionDis = CheckMoveCompToCollision(startVec, endVec);
    float plusMinus = (_value > 0.0f) ? 1.0f : -1.0f;
    //반환 값이 0이 아닐시 이동경로에 충돌이 있음
    if (collisionDis != 0.0f)
    {
        //UE_LOG(LogTemp, Log, TEXT("%f"), plusMinus * (collisionDis - (collisionDis * 0.2f)));
        //0.2f는 안전거리
        targetLoc = mainChar->
            GetActorLocation() + (mainChar->GetActorForwardVector() * (plusMinus * (collisionDis - (collisionDis * 0.1f))));
    }
    //처음부터 충돌했을경우
    else if (collisionDis == -1.0f)
    {
        return;
    }
    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = GetOwner();

    endLoc = targetLoc;
    UKismetSystemLibrary::MoveComponentTo(mainChar->GetRootComponent(), targetLoc, targetRot, false, false, moveSpeed, false, EMoveComponentAction::Type::Move, LatentInfo);
}

float UCharUltimateState::CheckMoveCompToCollision(FVector _startVec, FVector _endVec)
{
    TArray<FHitResult> outHitArr;

    TArray<AActor*> actorsToIgnore;
    actorsToIgnore.Add(mainChar);
    actorsToIgnore.Add(mainChar->GetMainCharWeaponSlot()->GetMainCharKatana());

    TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;

    objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
    objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

    UKismetSystemLibrary::CapsuleTraceMultiForObjects(
        GetWorld(),
        _startVec,
        _endVec,
        mainChar->GetCapsuleComponent()->GetScaledCapsuleRadius(),
        mainChar->GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
        objectTypesArray,
        false,
        actorsToIgnore,
        EDrawDebugTrace::ForDuration,
        outHitArr,
        true);

    float collisionDis = 10000000.0f;

    for (auto hitResult : outHitArr)
    {
        ASecondBossAreaPattern* bossAreaPattern = Cast<ASecondBossAreaPattern>(hitResult.Actor);
        ASaveCamp* saveCamp = Cast<ASaveCamp>(hitResult.Actor);
        AMonsterWeapon* monsterWeapon = Cast<AMonsterWeapon>(hitResult.Actor);

        if (bossAreaPattern)continue;
        if (saveCamp)continue;
        if (monsterWeapon)continue;

        if (hitResult.bBlockingHit)
        {
            if (hitResult.Distance < collisionDis)
            {
                collisionDis = hitResult.Distance;
            }
        }
    }
    //충돌 한 상태에서 실행시
    if (collisionDis == 0.0f)
    {
        collisionDis = -1.0f;
    }
    //충돌이 없을 시
    else if (collisionDis == 10000000.0f)
    {
        collisionDis = 0.0f;
    }
    return collisionDis;
}

TArray<AMonsterBase*> UCharUltimateState::CheckMonster()
{
    TArray<FHitResult> outHitArr;
    TArray<AActor*> actorsToIgnore;
    TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;

    objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

    UKismetSystemLibrary::BoxTraceMultiForObjects(
        GetWorld(),
        startLoc,
        endLoc,
        FVector(150.0f, 150.0f, 100.0f),
        FRotator(0.0f, 0.0f, 0.0f),
        objectTypesArray,
        false,
        actorsToIgnore,
        EDrawDebugTrace::None,
        outHitArr,
        true);

    TArray<AMonsterBase*> monsterArr;
    damage = 200.0f;

    for (auto hitActor : outHitArr)
    {
        auto enemy = Cast<AMonsterBase>(hitActor.Actor);
        auto boss = Cast<AMonsterBase_Boss>(hitActor.Actor);


        if (boss)damage = boss->GetMonsterStatus()->GetMaxMonsterHealthPoint() * 0.1f;
        if (enemy)
        {
            monsterArr.AddUnique(enemy);
            enemy->CustomTimeDilation = 0.1f;
        }
    }

    return monsterArr;
}

void UCharUltimateState::UltimateEnd()
{
    UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(
        mainChar->GetCharCamera()->GetOwner(), 0.3f);
}
