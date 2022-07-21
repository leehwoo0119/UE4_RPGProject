// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharArrow.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "LSH/MonsterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MainChar.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "LSH/TrapActor_FallingObject.h"
#include "Sound/SoundBase.h"
#include "Perception/AISense_Hearing.h"
#include "LSH/MonsterNormalCombatComponent.h"

// Sets default values
AMainCharArrow::AMainCharArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	arrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("arrowMesh"));
	arrowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectileMovement"));
	projectileMovement->bRotationFollowsVelocity = true;
	projectileMovement->bSimulationEnabled = false;
	projectileMovement->ProjectileGravityScale = 0.0f;
	projectileMovement->InitialSpeed = 2000.0f;
	projectileMovement->MaxSpeed = 2000.0f;

	arrowDamageCollision = CreateDefaultSubobject<USphereComponent>(TEXT("arrowDamageCollision"));
	arrowDamageCollision->SetGenerateOverlapEvents(false);
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	arrowMesh->SetupAttachment(RootComponent);

	arrowTrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("arrowTrailParticle"));
	arrowTrailParticle->SetupAttachment(arrowMesh);
	arrowTrailParticle->Deactivate();
}

// Called when the game starts or when spawned
void AMainCharArrow::BeginPlay()
{
	Super::BeginPlay();

	check(arrowMesh != nullptr);
	check(projectileMovement != nullptr);

	arrowDamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AMainCharArrow::ArrowDamageCollisionOnOverlapBegin);

	mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);

	spawnLocation = GetActorLocation();

	check(arrowDamageCollision != nullptr);
	arrowDamageCollision->AttachToComponent(arrowMesh, FAttachmentTransformRules::KeepRelativeTransform, FName("damageSocket"));
	arrowDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	arrowDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	arrowDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	arrowDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	arrowDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
}

void AMainCharArrow::Fire()
{
	projectileMovement->bSimulationEnabled = true;
	arrowDamageCollision->SetGenerateOverlapEvents(true);
	arrowDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	arrowTrailParticle->Activate();
	arrowTrailParticle->BeginTrails(FName("StartSocket"), FName("EndSocket"), ETrailWidthMode::ETrailWidthMode_FromFirst, 1.0f);

	FTimerHandle destroyTimer;
	GetWorld()->GetTimerManager().SetTimer(destroyTimer, FTimerDelegate::CreateLambda([&]()
		{
			Destroy();

		}), 10.0f, false);
}

void AMainCharArrow::ArrowDamageCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		UE_LOG(LogTemp, Log, TEXT("ArrowDamageCollisionOnOverlapBegin"));
		AMonsterBase* monster = Cast<AMonsterBase>(OtherActor);
		AMainCharArrow* arrow = Cast<AMainCharArrow>(OtherActor);
		ATrapActor_FallingObject* trapFallingObject = Cast<ATrapActor_FallingObject>(OtherActor);

		if (monster)
		{
			FHitResult hitResult;
			FVector startEnd = arrowMesh->GetSocketLocation(FName("damageSocket"));
			TArray<AActor*> actorsToIgnore;
			TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;
			objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

			UKismetSystemLibrary::SphereTraceSingleForObjects(
				GetWorld(),
				startEnd,
				startEnd,
				30.0f,
				objectTypesArray,
				false,
				actorsToIgnore,
				EDrawDebugTrace::None,
				hitResult,
				true);

			if (hitResult.bBlockingHit)
			{
				arrowDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				projectileMovement->bSimulationEnabled = false;
				FName hitBone = monster->GetMesh()->USkinnedMeshComponent::FindClosestBone(hitResult.Location);
				if (hitBone == FName("ik_hand_gun"))hitBone = FName("spine_03");
				FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
				AttachToComponent(monster->GetMesh(),
					rules,
					hitBone);
				UMonsterNormalCombatComponent* normalComp = Cast<UMonsterNormalCombatComponent>(monster->MON_COMBATCOMP);
				if (normalComp)
				{
					normalComp->SetInvestigatePosition(spawnLocation);
					UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation() + (GetActorForwardVector() * -100.0f), 1.0f, mainChar, 0.0f, FName("PlayerNoise"));
				}
				
				UGameplayStatics::ApplyDamage(monster, 1.0f, mainChar->GetController(), this, NULL);
			
			}
			
		}
		else if (trapFallingObject)
		{
			Destroy();
		}
		else if(!arrow)
		{
			float arrowDepth = FMath::RandRange(20.0f, 30.0f);
			projectileMovement->StopMovementImmediately();
			SetActorRelativeLocation(GetActorLocation() + (GetActorForwardVector() * arrowDepth));
			UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation() + (GetActorForwardVector() * -200.0f), 1.0f, mainChar, 0.0f, FName("PlayerNoise"));
		}

		if (!bSoundOnce)
		{
			bSoundOnce = true;
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), arrowHitSound, GetActorLocation(), 0.2f);
		}
		
		arrowTrailParticle->EndTrails();
	}
}


