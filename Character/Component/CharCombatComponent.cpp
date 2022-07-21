// Fill out your copyright notice in the Description page of Project Settings.


#include "CharCombatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MainChar.h"
#include "LSH/MonsterBase.h"
#include "MainCharWeaponSlot.h"
#include "LSH/MonsterStateMachine.h"
#include "CharStateMachine.h"
#include "CharStateActor.h"
#include "CharTakeDamageState.h"
#include "CharTakeDownDamageState.h"
#include "CharDefenseSuccessState.h"
#include "CharParryingSuccessState.h"
#include "CharDefenseBrokenState.h"
#include "CharDieState.h"
#include "LSH/MonsterBaseController.h"
#include "LSH/MonsterBaseStatus.h"
#include "Kismet/GameplayStatics.h"
#include "InstantDeathCamera.h"
#include "ExecutionCamera.h"
#include "StealthCamera.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "MainCharKatana.h"
#include "CharStatusComponent.h"
#include "LSH/DotDamageActor.h"
#include "LSH/MonsterBossCombatComponent_Sec.h"
#include "CharAttackAvoidState.h"
#include "CharStunState.h"
#include "LSH/MonsterProjectile.h"
#include "LSH/MonsterElementalPillar.h"
#include "LSH/TrapActor.h"
#include "LSH/MonsterBase_Boss_Second.h"
#include "SecondBossAreaPattern.h"

// Sets default values for this component's properties
UCharCombatComponent::UCharCombatComponent() : targetMonsterBase(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bloodRightVector = 0.0f;
	bloodUpVector = 0.0f;
	bloodPitchRotator = 0.0f;
	bloodYawRotator = 0.0f;
}


// Called when the game starts
void UCharCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	check(recoveryParticle != nullptr);

	mainChar = Cast<AMainChar>(GetOwner());
	check(mainChar != nullptr);
	
	check(hitCurveFloat != nullptr);
	check(ultimateCurveFloat != nullptr);
	check(executionCameraZoomCurveFloat != nullptr);

	instantDeathCamera = GetWorld()->SpawnActor<AInstantDeathCamera>();
	check(instantDeathCamera != nullptr);
	instantDeathCamera->AttachToComponent(mainChar->instantDeathCameraPos, FAttachmentTransformRules::KeepRelativeTransform);

	executionCamera = GetWorld()->SpawnActor<AExecutionCamera>();
	check(executionCamera != nullptr);
	executionCamera->AttachToComponent(mainChar->executionCameraPos, FAttachmentTransformRules::KeepRelativeTransform);

	stealthCamera = GetWorld()->SpawnActor<AStealthCamera>();
	check(stealthCamera != nullptr);
	stealthCamera->AttachToComponent(mainChar->stealthCameraPos, FAttachmentTransformRules::KeepRelativeTransform);

	mainChar->OnTakeAnyDamage.AddDynamic(this, &UCharCombatComponent::TakeDamage);
}

bool UCharCombatComponent::CheckTargetFront(FVector _vecA, FVector _vecB, FVector _forwardVec)
{
	_vecA = FVector(_vecA.X, _vecA.Y, 0.0f);
	_vecB = FVector(_vecB.X, _vecB.Y, 0.0f);

	float dot = FVector::DotProduct((_vecA - _vecB).GetSafeNormal(), _forwardVec);
	//UE_LOG(LogTemp, Log, TEXT("%f"), dot);
	if (dot >= 0.0f)
		return true;
	else
		return false;
}

void UCharCombatComponent::StaminaRecoveryTimer()
{
	if (!mainChar->GetCharStatusComponent()->GetStaminaPointRecovery() ||
		mainChar->GetCharStatusComponent()->GetCurCharStaminaPoint() >= mainChar->GetCharStatusComponent()->GetMaxCharStaminaPoint())
	{
		GetWorld()->GetTimerManager().ClearTimer(staminaPointRecoveryTimer);
	}
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharStaminaPoint(mainChar->GetCharStatusComponent()->GetRecoveryStaminaValue());
}

void UCharCombatComponent::StaminaPointRecoveryStart(float _delayTime)
{
	GetWorld()->GetTimerManager().ClearTimer(staminaPointRecoveryTimer);

	GetWorld()->GetTimerManager().SetTimer(
		staminaPointRecoveryTimer,
		this,
		&UCharCombatComponent::StaminaRecoveryTimer,
		_delayTime,
		true);

	mainChar->GetCharStatusComponent()->StartStaminaDecreaseCurveFTimeline();
}

void UCharCombatComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Die ||
		mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::Die)
	{
		return;
	}
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Unarmed)
	{
		switch (mainChar->GetCharStatusComponent()->GetPrevWeaponState())
		{
		case EWeaponState_Type::Katana:
			mainChar->GetMainCharWeaponSlot()->SetKatanaSocket("KatanaArmedSocket");
			break;
		case EWeaponState_Type::Bow:
			mainChar->GetMainCharWeaponSlot()->SetBowSocket("BowArmedSocket");
			break;
		}
		mainChar->GetCharStatusComponent()->SetWeaponState(mainChar->GetCharStatusComponent()->GetPrevWeaponState());
	}
	FRotator findLookAtRot =
		UKismetMathLibrary::FindLookAtRotation(
			mainChar->GetActorLocation(),
			DamageCauser->GetActorLocation());

	float damageDirection =
		UKismetMathLibrary::NormalizedDeltaRotator(
			mainChar->GetActorRotation(),
			findLookAtRot).Yaw;

	mainChar->GetCharStatusComponent()->SetDamageDirection(damageDirection);
	mainChar->GetCharStatusComponent()->SetDamage(Damage);

	AMonsterBase* enemy = Cast<AMonsterBase>(DamageCauser);

	if (CheckAttackAvoid(DamageCauser))return;
	if (!CheckCharParryingOrDefense(DamageCauser))return;
	if (CheckInvincibility())return;

	PrintBloodPatricle(enemy);

	if (mainChar->GetCharStatusComponent()->PlusOrSetCurCharHealthPoint(-Damage))
	{
		AMonsterBase_Boss_Second* bossSecond = Cast<AMonsterBase_Boss_Second>(DamageCauser);
		if (bossSecond)
		{
			check(bossSecond->GetMonsterCombatComponent()->bossAreaPattern != nullptr);
			bossSecond->GetMonsterCombatComponent()->bossAreaPattern->DestroyPatternArea();
		}

		mainChar->GetCharStateMachine()->SetCharState(
			mainChar->GetCharStateMachine()->charStateActor->charDieState);
		return;
	}

	ATrapActor* trap = Cast<ATrapActor>(DamageCauser);
	if (trap)
	{
		SpawnBloodParticle(mainChar->GetActorLocation(), mainChar->GetActorRotation());
	}

	//도트 데미지는 애니메이션을 실행하지 않음
	ADotDamageActor* dotDamageActor = Cast<ADotDamageActor>(DamageCauser);
	if (dotDamageActor)
	{
		return;
	}

	if (enemy && enemy->MON_STATUS->GetEnableHealthDrain())
	{
		UMonsterBossCombatComponent_Sec* monComp = Cast<UMonsterBossCombatComponent_Sec>(enemy->MON_COMBATCOMP);

		if (monComp)
		{
			monComp->ExecuteHealthDrain();
		}
	}

	if (enemy && enemy->MON_STATEMACHINE->GetAttackTypeState() == EAI_AttackType_State::StunCheckTypeAttack)
	{
		mainChar->GetCharStatusComponent()->PlusStunCnt();
	}	
	if (enemy && enemy->MON_STATEMACHINE->GetAttackTypeState() == EAI_AttackType_State::TakeDownTypeAttack)
	{
		mainChar->GetCharStateMachine()->SetCharState(
			mainChar->GetCharStateMachine()->charStateActor->charTakeDownDamageState);//charTakeDownDamageState
	}	
	else if (enemy && enemy->MON_STATEMACHINE->GetAttackTypeState() == EAI_AttackType_State::StunTypeAttack)
	{
		mainChar->GetCharStateMachine()->SetCharState(
			mainChar->GetCharStateMachine()->charStateActor->charStunState);//charStunState
	}		
	else
	{
		mainChar->GetCharStateMachine()->SetCharState(
			mainChar->GetCharStateMachine()->charStateActor->charTakeDamageState);
	}		
}

bool UCharCombatComponent::CheckAttackAvoid(AActor* _damageCauser)
{
	AMonsterBase* monsterBase = Cast<AMonsterBase>(_damageCauser);

	if (!monsterBase)
	{
		return false;
	}

	if (monsterBase->GetMonsterStatus()->GetCanAvoidAttack())
	{
		if ((mainChar->GetCharStatusComponent()->GetAttackAvoid() == EAttackAvoidState_Type::AttackAvoidSuccess ||
			mainChar->GetCharStatusComponent()->GetAttackAvoid() == EAttackAvoidState_Type::AttackAvoidFail))
		{
			mainChar->GetCharStatusComponent()->SetAttackAvoidTarget(monsterBase);
			mainChar->GetCharStateMachine()->SetCharState(
				mainChar->GetCharStateMachine()->charStateActor->charAttackAvoidState);

			return true;
		}		
	}
	return false;
}

bool UCharCombatComponent::CheckCharParryingOrDefense(AActor* _damageCauser)
{
	if (!CheckPlayerDefensed())
	{
		return true;
	}
	if (!CheckTargetFront(_damageCauser->GetActorLocation(),
		mainChar->GetActorLocation(),
		mainChar->GetActorForwardVector()))
	{
		return true;
	}

	AMonsterBase* monsterBase = Cast<AMonsterBase>(_damageCauser);
	ADotDamageActor* dotDamageActor = Cast<ADotDamageActor>(_damageCauser);
	AMonsterProjectile* monsterProjectile = Cast<AMonsterProjectile>(_damageCauser);
	AMonsterElementalPillar* monsterPillar = Cast<AMonsterElementalPillar>(_damageCauser);

	//도트 데미지, MonsterElementalPillar는 방어 불가능
	if (dotDamageActor || monsterPillar)
	{
		return true;
	}

	//투사체 공격 방어
	if (monsterProjectile)
	{
		AMonsterBase* projectileMonsterBase = Cast<AMonsterBase>(monsterProjectile->GetOwner());

		// 가드 불가 투사체
		if (projectileMonsterBase->GetMonsterStatus()->GetCantGuardProjectile())
		{
			return true;
		}
		else
		{
			mainChar->GetCharStateMachine()->SetCharState(
				mainChar->GetCharStateMachine()->charStateActor->charDefenseSuccessState);
			mainChar->GetMainCharWeaponSlot()->GetMainCharKatana()->KatanaParryingOrDefenseParticles('D');
			return false;
		}
	}

	// 예외처리
	if (!monsterBase)
	{
		mainChar->GetCharStateMachine()->SetCharState(
			mainChar->GetCharStateMachine()->charStateActor->charDefenseSuccessState);
		mainChar->GetMainCharWeaponSlot()->GetMainCharKatana()->KatanaParryingOrDefenseParticles('D');
		return false;
	}

	//회피가능 공격은 방어할수 없다.
	if (monsterBase->GetMonsterStatus()->GetCanAvoidAttack())
	{
		return true;
	}

	if ((mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Defense ||
		mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Parrying) && 
		!monsterBase->MON_STATUS->GetCantGuardAttack())
	{		
		if (mainChar->GetCharStatusComponent()->GetCurCharStaminaPoint() - mainChar->GetCharStatusComponent()->GetDamage() <= 0)
		{
			mainChar->GetCharStateMachine()->SetCharState(
				mainChar->GetCharStateMachine()->charStateActor->charDefenseBrokenState);
		}
		//가드 파괴 공격
		else if (monsterBase->MON_STATUS->GetGuardBreakAttack())
		{
			mainChar->GetCharStateMachine()->SetCharState(
				mainChar->GetCharStateMachine()->charStateActor->charDefenseBrokenState);
		}
		else
		{
			mainChar->GetCharStateMachine()->SetCharState(
				mainChar->GetCharStateMachine()->charStateActor->charDefenseSuccessState);
		}
		mainChar->GetMainCharWeaponSlot()->GetMainCharKatana()->KatanaParryingOrDefenseParticles('D');
		return false;
	}
	else if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::CanParrying)
	{
		//가드 파괴 공격
		if (monsterBase->MON_STATUS->GetGuardBreakAttack())
		{
			mainChar->GetCharStateMachine()->SetCharState(
				mainChar->GetCharStateMachine()->charStateActor->charDefenseBrokenState);
			mainChar->GetMainCharWeaponSlot()->GetMainCharKatana()->KatanaParryingOrDefenseParticles('D');
		}
		else if (monsterBase->MON_STATEMACHINE->GetAttackTypeState() == EAI_AttackType_State::DotDamageTypeAttack ||
			monsterBase->MON_STATEMACHINE->GetAttackTypeState() == EAI_AttackType_State::SpecialTypeAttack ||
			monsterBase->MON_STATUS->GetCantParryingAttack())
		{
			mainChar->GetCharStateMachine()->SetCharState(
				mainChar->GetCharStateMachine()->charStateActor->charDefenseSuccessState);
			mainChar->GetMainCharWeaponSlot()->GetMainCharKatana()->KatanaParryingOrDefenseParticles('D');
		}
		else
		{
			mainChar->GetCharStateMachine()->SetCharState(
				mainChar->GetCharStateMachine()->charStateActor->charParryingSuccessState);
			mainChar->GetMainCharWeaponSlot()->GetMainCharKatana()->KatanaParryingOrDefenseParticles('P');
			monsterBase->MON_STATEMACHINE->SetCombatState(EAI_Combat_State::AttackBlock);
		}
		return false;
	}

	return true;
}

bool UCharCombatComponent::CheckInvincibility()
{
	if(mainChar->GetCharStatusComponent()->GetInvincibility())return true;

	if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Execution ||
		mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Stealth ||
		mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Skill ||
		mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::DashInvincibility)return true;
	return false;
}

bool UCharCombatComponent::CheckPlayerDefensed()
{
	if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Defense ||
		mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Parrying ||
		mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::CanParrying)
		return true;

	return false;
}

void UCharCombatComponent::AttackSweepTrace()
{
	TArray<FHitResult> hitResultArr;

	FVector traceSize = FVector(100.0f, 100.0f, 50.0f);

	TArray<AActor*> actorsToIgnore;

	FVector startVec = mainChar->GetActorLocation() + (mainChar->GetActorForwardVector() * 150.0f);
	FVector endVec = mainChar->GetActorLocation() + (mainChar->GetActorForwardVector() * 150.0f);

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetWorld(),
		startVec,
		endVec,
		traceSize,
		FRotator(0.0f, 0.0f, 0.0f),
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
			monster->MON_CTRL->PlayerAttackDetected();
		}
	}
}

void UCharCombatComponent::PrintBloodPatricle(AMonsterBase* _target)
{
	//투사체에대해선 생각봄 해봄
	if (_target)
	{
		TArray<FHitResult> hitResultArr;
		FVector startVec = mainChar->GetActorLocation();
		FVector endVec = _target->GetActorLocation();

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
			AMainChar* castMainChar = Cast<AMainChar>(hitActor.Actor);
			if (castMainChar)
			{
				SetAttackBloodDirection(_target);

				FVector bloodEffectLoc =
					castMainChar->GetMesh()->GetBoneLocation(
						castMainChar->GetMesh()->FindClosestBone(hitActor.ImpactPoint)) +
					castMainChar->GetActorRightVector() * bloodRightVector +
					_target->GetActorUpVector() * bloodUpVector;

				//적 입장에서 생각해야 앞뒤등 방향에따라 문제가없음
				FRotator bloodEffectRot = FRotator(
					_target->GetActorRotation().Pitch + bloodPitchRotator,
					_target->GetActorRotation().Yaw + bloodYawRotator,
					_target->GetActorRotation().Roll);

				SpawnBloodParticle(bloodEffectLoc, bloodEffectRot);
			}
		}
	}

}

void UCharCombatComponent::SpawnBloodParticle(FVector _vec, FRotator _rot)
{
	bloodEffect = UGameplayStatics::SpawnEmitterAttached(
		bloodParticle,
		mainChar->GetRootComponent(),
		FName("Root"));

	bloodEffect->SetWorldLocation(_vec);
	bloodEffect->SetWorldRotation(_rot);
}

void UCharCombatComponent::SetAttackBloodDirection(AMonsterBase* _target)
{
	bloodUpVector = 0.0f;
	switch (_target->MON_STATUS->GetBloodTypeDirectionState())
	{
	case EBloodDirectionState_Type::Up:
		bloodPitchRotator = 45.0f;
		bloodYawRotator = 0.0f;
		bloodRightVector = 0.0f;
		break;
	case EBloodDirectionState_Type::RightUp:
		bloodPitchRotator = 0.0f;
		bloodYawRotator = 140.0f;
		bloodRightVector = -20.0f;
		break;
	case EBloodDirectionState_Type::Right:
		bloodPitchRotator = -30.0f;
		bloodYawRotator = 90.0f;
		bloodRightVector = -20.0f;
		break;
	case EBloodDirectionState_Type::RightDown:
		bloodPitchRotator = -60.0f;
		bloodYawRotator = 90.0f;
		bloodRightVector = -20.0f;
		break;
	case EBloodDirectionState_Type::Down:
		bloodPitchRotator = -90.0f;
		bloodYawRotator = 0.0f;
		bloodRightVector = 0.0f;
		bloodUpVector = 100.0f;
		break;
	case EBloodDirectionState_Type::LeftDown:
		bloodPitchRotator = -60.0f;
		bloodYawRotator = -90.0f;
		bloodRightVector = 20.0f;
		break;
	case EBloodDirectionState_Type::Left:
		bloodPitchRotator = -60.0f;
		bloodYawRotator = -90.0f;
		bloodRightVector = 20.0f;
		break;
	case EBloodDirectionState_Type::LeftUp:
		bloodPitchRotator = 0.0f;
		bloodYawRotator = -140.0f;
		bloodRightVector = 20.0f;
		break;
	case EBloodDirectionState_Type::Back:
		bloodPitchRotator = -70.0f;
		bloodYawRotator = 0.0f;
		bloodRightVector = 0.0f;
		break;
	case EBloodDirectionState_Type::Default:
		bloodPitchRotator = 0.0f;
		bloodYawRotator = 0.0f;
		bloodRightVector = 0.0f;
		break;
	}
}


