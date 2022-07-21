// Fill out your copyright notice in the Description page of Project Settings.


#include "DotDamageActor_Flame.h"
#include "LHW/MainChar.h"
#include "MonsterBase.h"
#include "MonsterBaseController.h"
#include "MonsterCombatComponent.h"
#include "MonsterStateMachine.h"
#include "MonsterBaseStatus.h"
#include "LHW/CharStatusComponent.h"
#include "Kismet/GameplayStatics.h"

ADotDamageActor_Flame::ADotDamageActor_Flame()
{
    Tags.Add("DotDamage_Flame");
}

void ADotDamageActor_Flame::BeginPlay()
{
    Super::BeginPlay();

    FlameDotDamage();
}

void ADotDamageActor_Flame::FlameDotDamage()
{
    if (mainChar)
    {
        AttachToComponent(mainChar->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("dotDamageSocket"));

        GetWorld()->GetTimerManager().SetTimer(flameDotDamageHandle, FTimerDelegate::CreateLambda([&]()
            {
                if (mainChar == nullptr)
                {
                    DotDamageActorDestroy();
                    return;
                }

                flameDamageNum++;
                float playerDotDamage = mainChar->GetCharStatusComponent()->GetMaxCharHealthPoint() * 0.05f;
                UGameplayStatics::ApplyDamage(mainChar, playerDotDamage, mainChar->GetController(), this, nullptr);

                if (maxFlameDamageNum <= flameDamageNum)
                {
                    DotDamageActorDestroy();
                }

            }), 2.0f, true);
    }

    if (monsterBase)
    {
        AttachToComponent(monsterBase->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("dotDamageSocket"));

        GetWorld()->GetTimerManager().SetTimer(flameDotDamageHandle, FTimerDelegate::CreateLambda([&]()
            {
                if (monsterBase == nullptr)
                {
                    DotDamageActorDestroy();
                    return;
                }

                flameDamageNum++;
                float monDotDamage = monsterBase->MON_STATUS->GetMaxMonsterHealthPoint() * 0.05f;
                UGameplayStatics::ApplyDamage(monsterBase, monDotDamage, monsterBase->MON_CTRL, this, nullptr);

                if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Die || maxFlameDamageNum <= flameDamageNum)
                {
                    DotDamageActorDestroy();
                }

            }), 2.0f, true);
    }
}