// Fill out your copyright notice in the Description page of Project Settings.


#include "DotDamageActor_Poison.h"
#include "LHW/MainChar.h"
#include "MonsterBase.h"
#include "MonsterBaseController.h"
#include "MonsterCombatComponent.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseStatus.h"
#include "LHW/CharStatusComponent.h"
#include "Kismet/GameplayStatics.h"

ADotDamageActor_Poison::ADotDamageActor_Poison()
{
    Tags.Add("DotDamage_Poison");
}

void ADotDamageActor_Poison::BeginPlay()
{
    Super::BeginPlay();

    PoisonDotDamage();
}

void ADotDamageActor_Poison::PoisonDotDamage()
{
    if (mainChar)
    {
        AttachToComponent(mainChar->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("dotDamageSocket"));
    }
    if (monsterBase)
    {
        AttachToComponent(monsterBase->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("dotDamageSocket"));
    }

    poisonDotTimer = 7;
    poisonStack++;

    GetWorld()->GetTimerManager().SetTimer(poisonStackHandle, FTimerDelegate::CreateLambda([&]()
        {
            if (maxPoisonStack <= poisonStack)
            {
                PoisonDamageStart();
                GetWorldTimerManager().ClearTimer(poisonStackHandle);
                poisonStack = 0;
            }
            if (poisonDotTimer <= 0)
            {
                DotDamageActorDestroy();
            }
            poisonDotTimer--;
        }), 1.0f, true);

}

void ADotDamageActor_Poison::PoisonDamageStart()
{
    if (mainChar)
    {
        GetWorld()->GetTimerManager().SetTimer(poisonDotDamageHandle, FTimerDelegate::CreateLambda([&]()
            {
                if (mainChar == nullptr)
                {
                    DotDamageActorDestroy();
                    return;
                }

                poisonDamageNum++;
                float playerDotDamage = mainChar->GetCharStatusComponent()->GetMaxCharHealthPoint() * 0.05f;
                UGameplayStatics::ApplyDamage(mainChar, playerDotDamage, mainChar->GetController(), this, nullptr);

                if (maxPoisonDamageNum <= poisonDamageNum)
                {
                    DotDamageActorDestroy();
                }

            }), 0.5f, true);
    }

    if (monsterBase)
    {
        GetWorld()->GetTimerManager().SetTimer(poisonDotDamageHandle, FTimerDelegate::CreateLambda([&]()
            {
                if (monsterBase == nullptr)
                {
                    DotDamageActorDestroy();
                    return;
                }

                poisonDamageNum++;
                float monDotDamage = monsterBase->MON_STATUS->GetMaxMonsterHealthPoint() * 0.1f;
                UGameplayStatics::ApplyDamage(monsterBase, monDotDamage, monsterBase->MON_CTRL, this, nullptr);

                if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Die || maxPoisonDamageNum <= poisonDamageNum)
                {
                    DotDamageActorDestroy();
                }

            }), 0.5f, true);
    }
}