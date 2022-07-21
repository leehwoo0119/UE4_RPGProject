// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCombatComponent.h"
#include "MonsterBase.h"
#include "MonsterBaseAnimInstance.h"
#include "MonsterBaseStatus.h"
#include "Kismet/GameplayStatics.h"
#include "LHW/MainChar.h"
#include "LHW/CharStatusComponent.h"
#include "LHW/CharTargetLockOnComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterBaseController.h"
#include "MonsterWeaponSlot.h"
#include "MonsterWeapon.h"
#include "TargetLockOnWidget.h"
#include "MonsterStateMachine.h"
#include "MonsterWidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "MonsterTimerComponent.h"
#include "LHW/ProjectGameInstance.h"
#include "LHW/SecondBossAreaPattern.h"
#include "LHW/SecondBossArea.h"
#include "DotDamageManager.h"

// Sets default values for this component's properties
UMonsterCombatComponent::UMonsterCombatComponent(): deathEffectTime(2.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bloodForwardVector = 200.0f;
	bloodRightVector = 0.0f;
	bloodPitchRotator = 0.0f;
	bloodYawRotator = 0.0f;

}

// Called when the game starts
void UMonsterCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	check(bloodParticle != nullptr);
	
	monsterBase = Cast<AMonsterBase>(GetOwner());
	check(monsterBase != nullptr);

	mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);

	gameIS = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(gameIS != nullptr);

	check(deathEffectCurveFloat != nullptr);
	FOnTimelineFloat DrawDeathCallback;
	FOnTimelineEventStatic DrawDeathFinishedCallback;

	DrawDeathCallback.BindUFunction(this, FName("DrawDeathEffect"));
	DrawDeathFinishedCallback.BindUFunction(this, FName("MonsterDeathDestroy"));

	deathEffectCurveFTimeline.AddInterpFloat(deathEffectCurveFloat, DrawDeathCallback);
	deathEffectCurveFTimeline.SetTimelineFinishedFunc(DrawDeathFinishedCallback);
	deathEffectCurveFTimeline.SetTimelineLength(deathEffectTime);

	monsterBase->OnTakeAnyDamage.AddDynamic(this, &UMonsterCombatComponent::TakeDamage);
}

void UMonsterCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	deathEffectCurveFTimeline.TickTimeline(DeltaTime);
}

void UMonsterCombatComponent::TakeDamage(AActor* _damagedActor, float _damage, const UDamageType* _damageType, AController* _instigatedBy, AActor* _damageCauser)
{
	if (monsterBase->MON_BEHAVIORSTATE == EAI_Behavior_State::Die ||
		monsterBase->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero)return;

}

void UMonsterCombatComponent::TakeDamageCheckDir(float _hitDirection)
{
	monsterBase->MON_STATUS->SetDefenseCnt(0);

	monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::Hit);

	if (_hitDirection >= -135.0f && _hitDirection < -45.0f)
	{
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->hitMontage, 1.0f, FName("Hit_R"));
	}
	else if (_hitDirection >= -45.0f && _hitDirection < 45.0f)
	{
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->hitMontage, 1.0f, FName("Hit_F"));
	}
	else if (_hitDirection >= 45.0f && _hitDirection < 135.0f)
	{
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->hitMontage, 1.0f, FName("Hit_L"));
	}
	else
	{
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->hitMontage, 1.0f, FName("Hit_B"));
	}	
}

void UMonsterCombatComponent::Die()
{
	monsterBase->MON_TIMERCOMP->AllMonsterTimerClear();

	monsterBase->CustomTimeDilation = 1.0f;
	if (bloodEffect)bloodEffect->CustomTimeDilation = 1.0f;

	gameIS->AddDestroyedMonsterArr(monsterBase);

	monsterBase->MON_ANIM->Montage_Stop(1.0f, monsterBase->MON_ANIM->hitMontage);

	monsterBase->GetStealthArea()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	mainChar->GetCharStatusComponent()->DeleteAttackedMonsterArr(monsterBase);

	mainChar->GetCharTargetLockOnComponent()->ResetTarget();
	
	monsterBase->MON_STATEMACHINE->SetBehaviorState(EAI_Behavior_State::Die);
	monsterBase->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	monsterBase->MON_CTRL->LogicStop();
	monsterBase->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mainChar->GetDotDamageManager()->PersonalActorDestroy(monsterBase);

	GetWorld()->GetTimerManager().SetTimer(
		monsterBase->MON_TIMERCOMP->deathDestroyTimer, this, &UMonsterCombatComponent::DrawDeathEffectStart, 6.f, false);
}

void UMonsterCombatComponent::Attack(EAI_MeleeAttack_State _state)
{
	// 플레이어가 사망하면 더 이상 공격을 하지 않음
	if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Die ||
		mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::Die)
		return;
}

void UMonsterCombatComponent::RangeAttack(float _forwardVal, float _size, FVector _location)
{
	FVector startVec;
	FVector endVec;
	if (_location != FVector(0.0f, 0.0f, 0.0f))
	{
		startVec = _location;
		endVec = startVec;
	}
	else
	{
		startVec = monsterBase->GetActorLocation() + (monsterBase->GetActorForwardVector() * _forwardVal);
		endVec = startVec;
	}
	TArray<FHitResult> hitResultArr;	

	TArray<AActor*> actorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		startVec,
		endVec,
		_size,
		objectTypesArray,
		false,
		actorsToIgnore,
		EDrawDebugTrace::None,
		hitResultArr,
		true);

	for (auto hitActor : hitResultArr)
	{
		AMainChar* castMainChar = Cast<AMainChar>(hitActor.Actor);
		
		if (castMainChar)
		{
			UGameplayStatics::ApplyDamage(castMainChar, 20.0f,
				monsterBase->GetController(), monsterBase, NULL);
		}
	}
}

void UMonsterCombatComponent::ExecuteExecuted(EExecutionState_Type _state)
{
	monsterBase->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	monsterBase->GetCharacterMovement()->DisableMovement();
	monsterBase->CustomTimeDilation = 1.0f;
	if (bloodEffect)bloodEffect->CustomTimeDilation = 1.0f;
	monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::Executed, true);

	switch (_state)
	{
		//일단뺌
	case EExecutionState_Type::Execution_1:
		//monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->executedMontage, 0.8f, FName("Executed_1"));
		break;
	case EExecutionState_Type::Execution_2:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->executedMontage, 0.7f, FName("Executed_2"));
		break;
	case EExecutionState_Type::Execution_3:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->executedMontage, 0.8f, FName("Executed_3"));
		break;
	case EExecutionState_Type::Execution_4:
		monsterBase->MON_ANIM->AnimMontagePlay(monsterBase->MON_ANIM->executedMontage, 0.7f, FName("Executed_4"));
		break;
	case EExecutionState_Type::Default:
		break;
	}
}

void UMonsterCombatComponent::RagdolMonster()
{
	monsterBase->GetMesh()->SetCollisionProfileName(FName("ragdoll"));
	monsterBase->GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("pelvis"), true);
	monsterBase->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void UMonsterCombatComponent::OffenserseZero()
{
	if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Skill || 
		monsterBase->MON_STATEMACHINE->GetBehaviorState() == EAI_Behavior_State::Die)return;

	mainChar->GetCharTargetLockOnComponent()->CheckTargetLockOn(monsterBase);
	
	mainChar->GetCharStatusComponent()->SetExecutionTarget(monsterBase);
	
	monsterBase->CustomTimeDilation = 0.1f;

	monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::OffenserseZero);
	monsterBase->MON_WIDGETCOMP->GetTargetLockOnHUD()->SetOffenserseZero(true);

	GetWorld()->GetTimerManager().SetTimer(
		monsterBase->MON_TIMERCOMP->offenserseZeroTimer, this, &UMonsterCombatComponent::OffenserseZeroEnd, 1.8f, false);
}

void UMonsterCombatComponent::OffenserseZeroEnd()
{
	if (mainChar->GetCharStatusComponent()->GetExecutionTarget() == monsterBase)
		mainChar->GetCharStatusComponent()->SetExecutionTarget(nullptr);

	if (mainChar->GetCharStatusComponent()->GetKatanaState() != EKatanaState_Type::Execution)
	{
		monsterBase->CustomTimeDilation = 1.0f;		
	}
	if (bloodEffect)bloodEffect->CustomTimeDilation = 1.0f;

	if (monsterBase->MON_COMBATSTATE != EAI_Combat_State::Executed)
		monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::Wait, true);
	monsterBase->MON_WIDGETCOMP->GetTargetLockOnHUD()->SetOffenserseZero(false);
}

void UMonsterCombatComponent::PrintAttackParticle()
{
	TArray<FHitResult> hitResultArr;
	FVector startVec = mainChar->GetActorLocation();
	FVector endVec = monsterBase->GetActorLocation();

	TArray<AActor*> actorsToIgnore;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		startVec,
		endVec,
		1.0f,
		objectTypesArray,
		false,
		actorsToIgnore,
		EDrawDebugTrace::None,
		hitResultArr,
		true);

	for (auto hitActor : hitResultArr)
	{
		AMonsterBase* monster = Cast<AMonsterBase>(hitActor.Actor);
		if (monster)
		{
			SetAttackBloodDirection();
			
			FVector bloodEffectLoc =
				monster->GetMesh()->GetBoneLocation(
					monster->GetMesh()->FindClosestBone(hitActor.ImpactPoint)) +
				monster->GetActorRightVector() * bloodRightVector +
				mainChar->GetActorForwardVector() * bloodForwardVector;

			FRotator bloodEffectRot = FRotator(
				mainChar->GetActorRotation().Pitch + bloodPitchRotator,
				mainChar->GetActorRotation().Yaw + bloodYawRotator,
				mainChar->GetActorRotation().Roll);

			bloodEffect = UGameplayStatics::SpawnEmitterAttached(
				bloodParticle,
				monster->GetRootComponent(),
				FName("root"));

			bloodEffect->SetWorldLocation(bloodEffectLoc);
			bloodEffect->SetWorldRotation(bloodEffectRot);

			if (monsterBase->MON_COMBATSTATE == EAI_Combat_State::OffenserseZero)
				bloodEffect->CustomTimeDilation = 0.1f;
		}
	}
}

void UMonsterCombatComponent::SetAttackBloodDirection()
{
	switch (mainChar->GetCharStatusComponent()->GetBloodDirectionState())
	{
	case EBloodDirectionState_Type::LeftDown:
		bloodPitchRotator = -60.0f;
		bloodYawRotator = -90.0f;
		bloodRightVector = 20.0f;
		break;
	case EBloodDirectionState_Type::LeftUp:
		bloodPitchRotator = 0.0f;
		bloodYawRotator = -140.0f;
		bloodRightVector = 20.0f;
		break;
	case EBloodDirectionState_Type::Right:
		bloodPitchRotator = -30.0f;
		bloodYawRotator = 90.0f;
		bloodRightVector = -20.0f;
		break;
	case EBloodDirectionState_Type::RightUp:
		bloodPitchRotator = 0.0f;
		bloodYawRotator = 140.0f;
		bloodRightVector = -20.0f;
		break;
	case EBloodDirectionState_Type::Default:
		bloodPitchRotator = 0.0f;
		bloodYawRotator = 0.0f;
		bloodRightVector = 0.0f;
		break;
	}
}

void UMonsterCombatComponent::MonsterDeathProcessing()
{
	Die();
	RagdolMonster();
	monsterBase->GetMonsterWeaponSlot()->GetMonsterFirstWeapon()->RagdolWeapon();
	if (monsterBase->GetMonsterWeaponSlot()->GetMonsterSecondWeapon())
		monsterBase->GetMonsterWeaponSlot()->GetMonsterSecondWeapon()->RagdolWeapon();
}

void UMonsterCombatComponent::WeaponEquip()
{
	if (monsterBase->MON_STATEMACHINE->GetBehaviorState() == EAI_Behavior_State::WeaponSwap)return;

	monsterBase->MON_ANIM->AnimMontagePlay(
		monsterBase->MON_ANIM->weaponSwapMontage,
		1.2f,
		FName("Equip"));
}

void UMonsterCombatComponent::SetAttackEnableTrue(float _minDelay, float _maxDelay)
{
	float randomDelay = FMath::RandRange(_minDelay, _maxDelay);
	GetWorld()->GetTimerManager().SetTimer(monsterBase->MON_TIMERCOMP->setAttackEnableTimer, FTimerDelegate::CreateLambda([&]()
		{
			monsterBase->MON_STATUS->SetAttackEnable(true);
		}), randomDelay, false);
}

void UMonsterCombatComponent::DrawDeathEffectStart()
{
	deathEffectCurveFTimeline.PlayFromStart();
}

void UMonsterCombatComponent::DrawDeathEffect(float _val)
{
	for (int num = 0; num < monsterBase->GetMesh()->GetNumMaterials(); num++)
	{	
		monsterBase->GetMesh()->CreateDynamicMaterialInstance(
			num,
			monsterBase->GetMesh()->GetMaterial(num))
		->SetScalarParameterValue(
			FName("Appearance"),
			_val);
	}

	if (monsterBase->GetHeadComponent())
	{
		for (int num = 0; num < monsterBase->GetHeadComponent()->GetNumMaterials(); num++)
		{
			monsterBase->GetHeadComponent()->CreateDynamicMaterialInstance(
				num,
				monsterBase->GetHeadComponent()->GetMaterial(num))
				->SetScalarParameterValue(
					FName("Appearance"),
					_val);
		}
	}
	
}

void UMonsterCombatComponent::MonsterDeathDestroy()
{
	monsterBase->MON_TIMERCOMP->AllMonsterTimerClear();
	monsterBase->Destroy();
	monsterBase->GetMonsterWeaponSlot()->GetMonsterFirstWeapon()->Destroy();
	if (monsterBase->GetMonsterWeaponSlot()->GetMonsterSecondWeapon())
	{
		monsterBase->GetMonsterWeaponSlot()->GetMonsterSecondWeapon()->Destroy();
	}
}
void UMonsterCombatComponent::MonsterMoveCompTo(float _value, bool _bIgnoreCollision, FVector _target)
{
	FVector targetLoc;
	float moveSpeed = 0.4f;

	if (_target != FVector(0.0f, 0.0f, 0.0f))
	{
		moveSpeed = _value;
		targetLoc = FVector(
			_target.X,
			_target.Y,
			_target.Z + 50.0f);
	}
	else
	{
		targetLoc = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * _value;
	}
	FRotator targetRot = GetOwner()->GetActorRotation();

	float collisionDis = 0.0f;

	if (!_bIgnoreCollision)
	{
		collisionDis = CheckMoveCompToCollision(monsterBase->GetActorLocation(), targetLoc);
	}
	float plusMinus = (_value > 0.0f) ? 1.0f : -1.0f;
	//반환 값이 0이 아닐시 이동경로에 충돌이 있음
	if (collisionDis != 0.0f)
	{
		//UE_LOG(LogTemp, Log, TEXT("%f"), plusMinus * (collisionDis - (collisionDis * 0.2f)));
		//0.2f는 안전거리
		targetLoc = GetOwner()->
			GetActorLocation() + (GetOwner()->GetActorForwardVector() * (plusMinus * (collisionDis - (collisionDis * 0.1f))));
		if (_target != FVector(0.0f, 0.0f, 0.0f))targetLoc.Z = _target.Z + 50.0f;
	}
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = GetOwner();

	targetLoc += GetOwner()->GetActorForwardVector() * -80.0f;

	UKismetSystemLibrary::MoveComponentTo(GetOwner()->GetRootComponent(), targetLoc, targetRot, false, false, moveSpeed, false, EMoveComponentAction::Type::Move, LatentInfo);
}

float UMonsterCombatComponent::CheckMoveCompToCollision(FVector _startVec, FVector _endVec)
{
	TArray<FHitResult> hitResultArr;

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(monsterBase);
	actorsToIgnore.Add(mainChar);
	actorsToIgnore.Add(monsterBase->MON_WEAPONSLOT->GetMonsterFirstWeapon());
	// 두번째 무기가 있는 경우 추가
	if (monsterBase->MON_WEAPONSLOT->GetMonsterSecondWeapon())
	{
		actorsToIgnore.Add(monsterBase->MON_WEAPONSLOT->GetMonsterSecondWeapon());
	}

	if (bossAreaPattern)
	{
		for (auto x : bossAreaPattern->GetPatternArea())
		{
			actorsToIgnore.Add(x);
		}	
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;

	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

	UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		GetWorld(),
		_startVec,
		_endVec,
		monsterBase->GetCapsuleComponent()->GetScaledCapsuleRadius(),
		monsterBase->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.7f,
		objectTypesArray,
		false,
		actorsToIgnore,
		EDrawDebugTrace::None,
		hitResultArr,
		true);

	float collisionDis = 10000000.0f;

	for (auto hitResult : hitResultArr)
	{
		if (hitResult.bBlockingHit)
		{
			if (hitResult.Distance < collisionDis)
			{
				collisionDis = hitResult.Distance;
			}
		}
	}
	//충돌이 없을 시
	if (collisionDis == 10000000.0f)
	{
		collisionDis = 0.0f;
	}
	return collisionDis;
}