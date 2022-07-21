// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "LHW/MainChar.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterBase.h"
#include "MonsterBase_Normal.h"
#include "LHW/MainCharKatana.h"
#include "MonsterBaseController.h"
#include "MonsterController_Normal.h"
#include "MonsterStateMachine.h"
#include "LHW/CharStatusComponent.h"
#include "LHW/ECharState.h"
#include "MonsterBossCombatComponent.h"
#include "MonsterBaseStatus.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AMonsterWeapon::AMonsterWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	enemyWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("enemyWeapon"));
	enemyWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	enemyWeaponAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("enemyWeaponAttackCollision"));
	enemyWeaponAttackCollision->SetupAttachment(enemyWeapon);
}

// Called when the game starts or when spawned
void AMonsterWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	check(enemyWeapon != nullptr);
	enemyWeaponAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AMonsterWeapon::MonsterWeaponDamageCollisionOnOverlapBegin);
	
	check(enemyWeaponAttackCollision != nullptr);
	enemyWeaponAttackCollision->AttachToComponent(enemyWeapon, FAttachmentTransformRules::KeepWorldTransform);
	enemyWeaponAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	enemyWeaponAttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	enemyWeaponAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	enemyWeaponAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	enemyWeaponAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
}

void AMonsterWeapon::MonsterWeaponDamageCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	if (OtherActor)
	{
		AMainChar* mainChar = Cast<AMainChar>(OtherActor);

		if (mainChar)
		{
			SetDamageCollisionDeactivate();
			ApplyDamageByType(mainChar);
		}		
	}
}

void AMonsterWeapon::SetDamageCollisionActivate()
{
	enemyWeaponAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//UE_LOG(LogTemp, Log, TEXT("Activate"));
}

void AMonsterWeapon::SetCollisionActivate()
{
	enemyWeaponAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	enemyWeaponAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	enemyWeaponAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	enemyWeaponAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AMonsterWeapon::SetDamageCollisionDeactivate()
{
	enemyWeaponAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//UE_LOG(LogTemp, Log, TEXT("Deactivate"));
}

void AMonsterWeapon::SetMonsterBase(AMonsterBase* _monsterBase)
{
	monsterBase = _monsterBase;
	check(monsterBase != nullptr);
}

void AMonsterWeapon::ApplyDamageByType(AMainChar* _mainChar)
{
	switch (monsterBase->MON_STATEMACHINE->GetAttackTypeState())
	{
		case EAI_AttackType_State::NormalTypeAttack:
			UGameplayStatics::ApplyDamage(_mainChar, monsterBase->GetMonsterStatus()->GetMonsterDamage(),
				monsterBase->GetController(), monsterBase, NULL);
			break;
		case EAI_AttackType_State::TakeDownTypeAttack:
			UGameplayStatics::ApplyDamage(_mainChar, monsterBase->GetMonsterStatus()->GetMonsterDamage() * 2,
				monsterBase->GetController(), monsterBase, NULL);
			break;
		case EAI_AttackType_State::SpecialTypeAttack:
			UGameplayStatics::ApplyDamage(_mainChar, monsterBase->GetMonsterStatus()->GetMonsterDamage(),
				monsterBase->GetController(), monsterBase, NULL);
			break;
	}
}

void AMonsterWeapon::RagdolWeapon()
{
	enemyWeapon->SetCollisionProfileName(FName("ragdoll"));
	enemyWeapon->SetSimulatePhysics(true);
	enemyWeapon->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	enemyWeapon->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AMonsterWeapon::MonsterWeaponDefenseParticles()
{
	FVector particleLoc = FVector(monsterBase->GetActorLocation() + (monsterBase->GetActorForwardVector() * 40.0f));
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), defenseParticle, particleLoc);
	MonsterDefenseSound();
}
