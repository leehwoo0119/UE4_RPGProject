// Fill out your copyright notice in the Description page of Project Settings.


#include "DotDamageActor_Frozen.h"
#include "LHW/MainChar.h"
#include "MonsterBase.h"
#include "MonsterBaseController.h"
#include "MonsterCombatComponent.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseStatus.h"
#include "LHW/CharStatusComponent.h"
#include "LHW/CharCombatComponent.h"
#include "Kismet/GameplayStatics.h"

ADotDamageActor_Frozen::ADotDamageActor_Frozen()
{
    Tags.Add("DotDamage_Frozen");
}

void ADotDamageActor_Frozen::BeginPlay()
{
    Super::BeginPlay();

    FrozenStack();
}

void ADotDamageActor_Frozen::FrozenStack()
{
    if (mainChar)
    {
        AttachToComponent(mainChar->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("dotDamageSocket"));
    }
    if (monsterBase)
    {
        AttachToComponent(monsterBase->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("dotDamageSocket"));
    }

    FrozenCCTimer = 7;
    frozenStack++;

    GetWorld()->GetTimerManager().SetTimer(frozenStackHandle, FTimerDelegate::CreateLambda([&]()
        {
            if (maxFrozenStack <= frozenStack)
            {
                FrozenCrowdControl();
                GetWorldTimerManager().ClearTimer(frozenStackHandle);
                frozenStack = 0;
            }
            if (FrozenCCTimer <= 0)
            {
                DotDamageActorDestroy();
            }
            FrozenCCTimer--;
        }), 1.0f, true);
}

void ADotDamageActor_Frozen::FrozenCrowdControl()
{
    if (mainChar)
    {
        if (mainChar->GetCharCombatComponent()->CheckInvincibility()) return;

        mainChar->CustomTimeDilation = 0.0f;

        GetWorld()->GetTimerManager().SetTimer(frozenCrowdControlHandle, FTimerDelegate::CreateLambda([&]()
            {
                mainChar->CustomTimeDilation = 1.0f;
                DotDamageActorDestroy();
            }), 3.0f, false);
    }

    if (monsterBase)
    {
        monsterBase->CustomTimeDilation = 0.0f;

        GetWorld()->GetTimerManager().SetTimer(frozenCrowdControlHandle, FTimerDelegate::CreateLambda([&]()
            {
                monsterBase->CustomTimeDilation = 1.0f;
                DotDamageActorDestroy();
            }), 3.0f, false);
    }
}
