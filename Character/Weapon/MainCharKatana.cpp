// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharKatana.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "LSH/SecondBossPatternWeapon.h"
#include "LSH/CanTargetingActor.h"
#include "LSH/MonsterBase.h"
#include "LSH/MonsterWeapon.h"
#include "LSH/MonsterStateMachine.h"
#include "LSH/MonsterCombatComponent.h"
#include "MainChar.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "CharStatusComponent.h"

// Sets default values
AMainCharKatana::AMainCharKatana()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	katana = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("katana"));
	katana->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	katana->SetWorldRotation(FRotator(-90.0f, 90.0f, 0.0f));
	check(katana != nullptr);

	katanaAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("katanaAttackCollision"));
	katanaAttackCollision->SetupAttachment(katana);
}

// Called when the game starts or when spawned
void AMainCharKatana::BeginPlay()
{
	Super::BeginPlay();
		
	katanaAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AMainCharKatana::KatanaDamageCollisionOnOverlapBegin);

	mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);

	check(katanaAttackCollision != nullptr);
	katanaAttackCollision->AttachToComponent(katana, FAttachmentTransformRules::KeepWorldTransform);
	katanaAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	katanaAttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	katanaAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	katanaAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap); 
	katanaAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
}


void AMainCharKatana::KatanaDamageCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ACanTargetingActor* target = Cast<ACanTargetingActor>(OtherActor);
		
		if (!target)return;
		if (!target->GetAttackOnce())
		{	
			target->SetAttackOnce(true);
			mainChar->GetCharStatusComponent()->AddAttackedMonsterArr(target);

			mainChar->GetAnimInstance()->Montage_Pause(mainChar->GetAnimInstance()->attackMontage);

			FTimerHandle attackDelay;
			GetWorld()->GetTimerManager().SetTimer(attackDelay, FTimerDelegate::CreateLambda([&]()
			{
				mainChar->GetAnimInstance()->Montage_Resume(mainChar->GetAnimInstance()->attackMontage);

			}), 0.13f, false);			

			AMonsterBase* monsterBase = Cast<AMonsterBase>(OtherActor);
			ASecondBossPatternWeapon* patternWeapon = Cast<ASecondBossPatternWeapon>(OtherActor);

			if (monsterBase)
			{
				if (monsterBase->MON_COMBATSTATE != EAI_Combat_State::SpecialSkill)
				{
					mainChar->GetCharStatusComponent()->PlusOrSetCurCharRecoveryPoint(10.0f);
				}		
				UGameplayStatics::ApplyDamage(monsterBase, 10.0f, mainChar->GetController(), mainChar, NULL);

				if (monsterBase->MON_COMBATCOMP->GetAIMonsterType() == EAI_MonsterType::Boss ||
					(monsterBase->MON_COMBATSTATE == EAI_Combat_State::Hit || monsterBase->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero))
				{
					PlayCharAttackSound();
				}
			}
			if(patternWeapon)
			{
				PlayCharAttackMetalSound();
				patternWeapon->CheckDestroy();
			}
		}				
	}
}

void AMainCharKatana::SetAttackCollisionActivate()
{
	//UE_LOG(LogTemp, Log, TEXT("Activate"));
	for (auto monster : mainChar->GetCharStatusComponent()->GetAttackedMonsterArr())
	{
		monster->SetAttackOnce(false);
	}
	mainChar->GetCharStatusComponent()->DeleteAttackedMonsterArr();
	katanaAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMainCharKatana::SetAttackCollisionDeactivate()
{
	katanaAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
}

void AMainCharKatana::KatanaParryingOrDefenseParticles(char _value)
{
	float randValue = FMath::RandRange(-10.0f, 10.0f);
	FVector particleLoc = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 20.0f + randValue);

	switch (_value)
	{
	case 'D':
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), defenseParticle, particleLoc);
		break;
	case 'P':
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), parryParticle, particleLoc);
		break;
	}
}

void AMainCharKatana::WeaponRagdol()
{
	katana->SetCollisionProfileName(FName("ragdoll"));
	katana->SetSimulatePhysics(true);
	katana->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	katana->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}
