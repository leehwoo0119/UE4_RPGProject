// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondBossAreaPattern.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SecondBossArea.h"
#include "Kismet/GameplayStatics.h"
#include "MainChar.h"
#include "LSH/MonsterBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
//#include "LSH/SecondBossPatternWeapon.h"
#include "LSH/MonsterBossCombatComponent_Sec.h"
#include "CharStateMachine.h"
#include "CharStateActor.h"
#include "CharInstantDeathState.h"
#include "CharStatusComponent.h"
#include "LSH/MonsterStateMachine.h"

// Sets default values
ASecondBossAreaPattern::ASecondBossAreaPattern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	areaSize = 1200.0f;
	maxPatternWeaponCnt = 5;
	maxPatternCnt = 3;
}

// Called when the game starts or when spawned
void ASecondBossAreaPattern::BeginPlay()
{
	Super::BeginPlay();
	
	check(secondBossAreaBP != nullptr);
	check(damageParticle != nullptr);

	FVector actorLoc = GetActorLocation();
	
	areaPosArr.AddUnique(FVector(-areaSize / 3 * 2, -areaSize / 3 * 2, 0.0));
	areaPosArr.AddUnique(FVector(0.0f, -areaSize / 3 * 2, 0.0f));
	areaPosArr.AddUnique(FVector(areaSize / 3 * 2, -areaSize / 3 * 2, 0.0f));
	areaPosArr.AddUnique(FVector(-areaSize / 3 * 2, 0.0f, 0.0f));
	areaPosArr.AddUnique(FVector(0.0f, 0.0f, 0.0f));
	areaPosArr.AddUnique(FVector(areaSize / 3 * 2, 0.0f, 0.0f));
	areaPosArr.AddUnique(FVector(-areaSize / 3 * 2, areaSize / 3 * 2, 0.0f));
	areaPosArr.AddUnique(FVector(0.0f, areaSize / 3 * 2, 0.0f));
	areaPosArr.AddUnique(FVector(areaSize / 3 * 2, areaSize / 3 * 2, 0.0f));

	for (int i = 0; i < 9; i++)
	{
		ASecondBossArea* area = GetWorld()->SpawnActor<ASecondBossArea>(secondBossAreaBP);
		area->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		area->GetArea()->SetRelativeLocation(areaPosArr[i]);
		area->GetArea()->SetBoxExtent(FVector(areaSize / 3, areaSize / 3, areaSize / 3));
		areaArr.AddUnique(area);
	}
	
	//CheckSafeArea();
}

void ASecondBossAreaPattern::CheckSafeArea()
{
	patternCnt++;

	safeArea = nullptr;
	dangerousAreaMap.Empty();

	for (auto area : areaArr)
	{
		float checkDis = area->GetDistanceTo(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		dangerousAreaMap.Add(checkDis, area);
		area->SetAreaMaterialOpacity(false);
		area->SetSafeAreaMaterial(true);
	}

	dangerousAreaMap.KeySort([](int32 A, int32 B) {
		return A > B; // sort keys in reverse
		});

	int randVal = FMath::RandRange(0, 2);

	auto it = dangerousAreaMap.CreateConstIterator();
	
	for (int i = 0; i < randVal; i++)
	{
		++it;
	}
	safeArea = it.Value();
	
	safeArea->SetSafeAreaMaterial(false);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), 
		damageParticle,
		areaArr[4]->GetActorLocation(),
		areaArr[4]->GetActorRotation(),
		FVector(2.3f, 2.3f, 2.3f));

	GetWorld()->GetTimerManager().SetTimer(damageTimer, FTimerDelegate::CreateLambda([&]()
		{
			CheckDangerousArea();

		}), 4.5f, false);	
}

void ASecondBossAreaPattern::CheckDangerousArea()
{
	bool bDamage = false;
	for (auto area : dangerousAreaMap)
	{			
		if (area.Value == safeArea)continue;
		bDamage = area.Value->CheckDamage();
		
		if (bDamage)break;
	}
	
	if (bDamage)
	{
		GetWorldTimerManager().ClearTimer(executeDamageTimer);

		GetWorldTimerManager().SetTimer(
			executeDamageTimer, 
			this,
			&ASecondBossAreaPattern::ExecuteDamage,
			0.2f, 
			true);
		
	}

	for (auto area : areaArr)
	{
		area->SetAreaMaterialOpacity(true);
	}
}

void ASecondBossAreaPattern::CheckPatternWeapon()
{	
	bool bCanSpawn = false;
	FVector spawnPos;

	while (!bCanSpawn)
	{
		spawnPos = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), FVector(areaSize / 3 * 2, areaSize / 3 * 2, 10.0f));

		for (auto pos : patternWeaponAreaArr)
		{
			float dis = (spawnPos - pos).Size();

			if (dis > 300.0f)
			{
				break;
			}
		}
		bCanSpawn = true;
	}
	patternWeaponAreaArr.AddUnique(spawnPos);
	spawnPos = FVector(spawnPos.X, spawnPos.Y, spawnPos.Z + 1000.0f);

	SpawnPatternWeapon(spawnPos);
}

void ASecondBossAreaPattern::CheckPatternWeaponCnt(int _val)
{
	patternWeaponCnt += _val;

	if (patternWeaponCnt == maxPatternWeaponCnt)
	{
		AMainChar* mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		check(mainChar != nullptr);
		if (mainChar->GetCharStatusComponent()->GetKatanaState() != EKatanaState_Type::Die ||
			mainChar->GetCharStatusComponent()->GetBowState() != EBowState_Type::Die)
		{
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charInstantDeathState);

			check(monster != nullptr);
			UMonsterBossCombatComponent_Sec* combatComp = Cast<UMonsterBossCombatComponent_Sec>(monster->MON_COMBATCOMP);
			check(combatComp != nullptr);
			combatComp->ExecuteInstantDeath();
		}		
		DestroyPatternArea();
	}
}

void ASecondBossAreaPattern::DestroyPatternArea()
{
	bLoop = false;
	//GetWorldTimerManager().ClearTimer(spawnTimer);
	GetWorldTimerManager().ClearTimer(damageTimer);
	GetWorldTimerManager().ClearTimer(executeDamageTimer);
	
	Destroy();
}

void ASecondBossAreaPattern::ResetPatternCnt()
{
	patternCnt = 0;
}

void ASecondBossAreaPattern::ExecuteDamage()
{
	check(monster != nullptr);

	if (damageCnt == 3)
	{
		damageCnt = 0;
		GetWorldTimerManager().ClearTimer(executeDamageTimer);
	}
	else if (damageCnt == 2)
	{
		monster->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::TakeDownTypeAttack);
	}
	else
	{
		monster->MON_STATEMACHINE->SetAttackTypeState(EAI_AttackType_State::NormalTypeAttack);
	}
	AMainChar* mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);

	damageCnt++;

	UGameplayStatics::ApplyDamage(mainChar, 15.0f, monster->GetController(), monster, NULL);

}

