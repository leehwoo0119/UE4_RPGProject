// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialManager.h"
#include "LHW/MainChar.h"
#include "LHW/CharStatusComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterBase.h"
#include "MonsterBaseStatus.h"
#include "Monster_Tutorial_CombatComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "TutorialText.h"
#include "TutorialMissionList.h"
#include "Components/Image.h"
#include "LHW/Door.h"

ATutorialManager::ATutorialManager()
{
	PrimaryActorTick.bCanEverTick = false;

	sceneRootComp = CreateDefaultSubobject<USceneComponent>(TEXT("sceneRootComp"));
	sceneRootComp->SetupAttachment(RootComponent);

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("staticMesh"));
	staticMesh->SetupAttachment(sceneRootComp);

	boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("boxCollision"));
	boxCollision->SetupAttachment(staticMesh);

	section2_boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("section2_boxCollision"));
	section2_boxCollision->SetupAttachment(sceneRootComp);

	section3_boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("section3_boxCollision"));
	section3_boxCollision->SetupAttachment(sceneRootComp);

	section4_boxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("section4_boxCollision"));
	section4_boxCollision->SetupAttachment(sceneRootComp);
		
	currentTextIndex = 0;
	currentMissionIndex = 0;

	doOnce = true;
	sprintFirstClear = false;

	tutorialSection = ETutorialSection::Default;
	tutorialMissionCheck = ETutorialMissionCheck::MissionCheck1;
}

void ATutorialManager::BeginPlay()
{
	Super::BeginPlay();

	mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);

	tutorialMissionList->SetTutorialManager(this);

	PrintText();

	FTimerHandle tutorialHandle;
	GetWorld()->GetTimerManager().SetTimer(tutorialHandle, FTimerDelegate::CreateLambda([&]()
	{
		tutorialSection = ETutorialSection::Section1;
		PrintText();
		PrintMissionList();
		GetWorldTimerManager().ClearTimer(tutorialHandle);

		tutorialMonsterComp = Cast<UMonster_Tutorial_CombatComponent>(monsterBase->MON_COMBATCOMP);
		check(tutorialMonsterComp != nullptr);
	}), 3.0f, false);

	GetWorldTimerManager().SetTimer(missionTimerHandle, this, &ATutorialManager::TutorialCheckTimer, 0.1f, true);

	boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATutorialManager::BoxCollisionOnOverlapBegin);
	section2_boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATutorialManager::Section2_BoxCollisionOnOverlapBegin);
	section3_boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATutorialManager::Section3_BoxCollisionOnOverlapBegin);
	section4_boxCollision->OnComponentBeginOverlap.AddDynamic(this, &ATutorialManager::Section4_BoxCollisionOnOverlapBegin);
}

void ATutorialManager::PrintText()
{
	if (tutorialText->IsInViewport())
	{
		tutorialText->RemoveFromParent();
	}

	tutorialText->AddToViewport();
	tutorialText->textUIImage->SetBrushFromTexture(tutorialTextTexture[currentTextIndex], true);
	currentTextIndex++;
}

void ATutorialManager::PrintMissionList()
{
	if (tutorialMissionList->IsInViewport())
	{
		tutorialMissionList->RemoveFromParent();
	}

	tutorialMissionList->AddToViewport();
	tutorialMissionList->missionUIImage->SetBrushFromTexture(tutorialMissionTexture[currentMissionIndex], true);
	currentMissionIndex++;
}

void ATutorialManager::NextMissionCheck()
{
	if (tutorialMissionList)
	{
		uint8 tempInt = (uint8)tutorialMissionCheck;
		tempInt++;
		tutorialMissionCheck = (ETutorialMissionCheck)tempInt;
	}
}

void ATutorialManager::InitializationMissionList()
{
	switch (tutorialSection)
	{
	case ETutorialSection::Section1:
	{
		tutorialMissionCheck = ETutorialMissionCheck::MissionCheck1;
		PrintMissionList();
		tutorialSection = ETutorialSection::Section2;
		break;
	}
	case ETutorialSection::Section2:
	{
		tutorialMissionCheck = ETutorialMissionCheck::MissionCheck1;
		PrintMissionList();
		tutorialSection = ETutorialSection::Section3;
		break;
	}
	case ETutorialSection::Section3:
	{
		tutorialMissionCheck = ETutorialMissionCheck::MissionCheck1;
		PrintMissionList();
		tutorialSection = ETutorialSection::Section4;
		break;
	}
	case ETutorialSection::Section4:
	{
		tutorialMissionCheck = ETutorialMissionCheck::Default;
		PrintMissionList();
		tutorialSection = ETutorialSection::ClearAllSection;
		break;
	}
	default:
		break;
	}
}

void ATutorialManager::MissionSuccess(float _fTimerValue)
{
	if (doOnce)
	{
		doOnce = false;

		FTimerHandle initializationHandle;
		GetWorld()->GetTimerManager().SetTimer(initializationHandle, FTimerDelegate::CreateLambda([&]()
		{
			NextMissionCheck();
			PrintText();
			if (tutorialMissionCheck == ETutorialMissionCheck::Default)
			{
				InitializationMissionList();
			}
			doOnce = true;
		}), _fTimerValue, false);
	}
}

void ATutorialManager::TutorialCheckTimer()
{
	switch (tutorialSection)
	{
	case ETutorialSection::Section1:
	{
		Section1_Mission();
		break;
	}
	case ETutorialSection::Section2:
	{
		Section2_Mission();
		break;
	}
	case ETutorialSection::Section3:
	{
		Section3_Mission();
		break;
	}
	case ETutorialSection::Section4:
	{
		Section4_Mission();
		break;
	}
	default:
		break;
	}
}

void ATutorialManager::Section1_Mission()
{
	switch (tutorialMissionCheck)
	{
	case ETutorialMissionCheck::MissionCheck2:
	{
		if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::SilentWalk)
		{
			MissionSuccess(0.5f);
		}
		break;
	}
	case ETutorialMissionCheck::MissionCheck3:
	{
		if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Crouch)
		{
			MissionSuccess(0.5f);
		}
		break;
	}
	case ETutorialMissionCheck::MissionCheck4:
	{
		if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Jump)
		{
			MissionSuccess(0.5f);
			doorArray[0]->OpenDoor();
		}
		break;
	}
	default:
		break;
	}
}

void ATutorialManager::Section2_Mission()
{
	switch (tutorialMissionCheck)
	{
	case ETutorialMissionCheck::MissionCheck1:
	{
		if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Sprint)
		{
			sprintFirstClear = true;
		}
		break;
	}
	case ETutorialMissionCheck::MissionCheck2:
	{
		sprintFirstClear = false;

		if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Dash)
		{
			MissionSuccess(0.5f);
		}
		break;
	}
	case ETutorialMissionCheck::MissionCheck3:
	{
		if (mainChar->GetCharStatusComponent()->GetTargetLockOn())
		{
			MissionSuccess(0.5f);
		}
		break;
	}
	case ETutorialMissionCheck::MissionCheck4:
	{
		if (mainChar->GetCharStatusComponent()->GetTargetSwitchState() != ETargetSwitchState_Type::NotSwitch)
		{
			doorArray[1]->OpenDoor();
		}
		break;
	}
	default:
		break;
	}
}

void ATutorialManager::Section3_Mission()
{
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharHealthPoint(100.0f);

	switch (tutorialMissionCheck)
	{
	case ETutorialMissionCheck::MissionCheck1:
	{
		if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::MeleeAttack)
		{
			MissionSuccess(0.5f);
		}
		break;
	}
	case ETutorialMissionCheck::MissionCheck2:
	{
		if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::ParryingSuccess)
		{
			MissionSuccess(0.5f);
			tutorialMonsterComp->SetIgnoreDamage(false);
		}
		break;
	}
	case ETutorialMissionCheck::MissionCheck3:
	{
		if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Execution)
		{
			MissionSuccess(4.0f);
			GetWorldTimerManager().ClearTimer(tutorialMonsterComp->recoveryHandle);
			doorArray[4]->OpenDoor();
		}
		break;
	}
	case ETutorialMissionCheck::MissionCheck4:
	{
		if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Stealth)
		{
			doorArray[2]->OpenDoor();
		}
		break;
	}
	default:
		break;
	}
}

void ATutorialManager::Section4_Mission()
{
	switch (tutorialMissionCheck)
	{
	case ETutorialMissionCheck::MissionCheck1:
	{
		if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::WeaponSwap ||
			mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::CrouchWeaponSwap)
		{
			MissionSuccess(0.5f);
		}
		break;
	}
	case ETutorialMissionCheck::MissionCheck2:
	{
		if (mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::FullCharging)
		{
			MissionSuccess(0.5f);
		}
		break;
	}
	case ETutorialMissionCheck::MissionCheck3:
	{
		if (mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::Shooting)
		{
			MissionSuccess(0.5f);
		}
		break;
	}
	case ETutorialMissionCheck::MissionCheck4:
	{
		if (mainChar->GetCharStatusComponent()->GetCurCharHealthPoint() > 30.0f)
		{
			mainChar->GetCharStatusComponent()->PlusOrSetCurCharHealthPoint(-10.0f);
			mainChar->GetCharStatusComponent()->PlusOrSetCurCharRecoveryPoint(100.0f);
		}

		if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::HealthRecovery ||
			mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::HealthRecovery)
		{
			MissionSuccess(0.5f);
			doorArray[3]->OpenDoor();
			GetWorldTimerManager().ClearTimer(missionTimerHandle);
		}
		break;
	}
	default:
		break;
	}
}

void ATutorialManager::SetMonsterBase(AMonsterBase* _monsterBase)
{
	monsterBase = _monsterBase;
}

void ATutorialManager::SetTutorialText(UTutorialText* _tutorialText)
{
	tutorialText = _tutorialText;
}

void ATutorialManager::SetTutorialMissionList(UTutorialMissionList* _tutorialMissionList)
{
	tutorialMissionList = _tutorialMissionList;
}

void ATutorialManager::BoxCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (tutorialSection == ETutorialSection::Section1 && tutorialMissionCheck == ETutorialMissionCheck::MissionCheck1)
	{
		MissionSuccess(0.5f);
		staticMesh->DestroyComponent();
	}
}

void ATutorialManager::Section2_BoxCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainChar* castMainChar = Cast<AMainChar>(OtherActor);

	if (castMainChar)
	{
		if (tutorialSection == ETutorialSection::Section2 && sprintFirstClear)
		{
			MissionSuccess(0.5f);
		}
	}
}

void ATutorialManager::Section3_BoxCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainChar* castMainChar = Cast<AMainChar>(OtherActor);

	if (castMainChar)
	{
		if (tutorialSection == ETutorialSection::Section2)
		{
			MissionSuccess(0.5f);
		}
	}
}

void ATutorialManager::Section4_BoxCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainChar* castMainChar = Cast<AMainChar>(OtherActor);

	if (castMainChar)
	{
		if (tutorialSection == ETutorialSection::Section3)
		{
			MissionSuccess(0.5f);
		}
	}
}
