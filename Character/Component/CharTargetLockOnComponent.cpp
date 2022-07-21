// Fill out your copyright notice in the Description page of Project Settings.


#include "CharTargetLockOnComponent.h"
#include "MainChar.h"
#include "LSH/CanTargetingActor.h"
#include "LSH/MonsterBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "CharStatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "LSH/MonsterStateMachine.h"

UCharTargetLockOnComponent::UCharTargetLockOnComponent() : maxCanTargetLockOnDis(2000.0f), switchCombatModeCameraCurveLength(0.2f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	interpSpeed = 20.0f;
	cameraPitch = 90.0f;
	objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
}

// Called when the game starts
void UCharTargetLockOnComponent::BeginPlay()
{
	Super::BeginPlay();

	mainChar = Cast<AMainChar>(GetOwner());
	check(mainChar != nullptr);

	camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	check(camManager != nullptr);

	check(switchCombatModeCameraCurveFloat != nullptr);
	FOnTimelineFloat switchCombatModeCameraCallback;
	switchCombatModeCameraCallback.BindUFunction(this, FName("SwitchCameraCombatModeStart"));
	switchCombatModeCameraCurveFTimeline.AddInterpFloat(switchCombatModeCameraCurveFloat, switchCombatModeCameraCallback);
	switchCombatModeCameraCurveFTimeline.SetTimelineLength(switchCombatModeCameraCurveLength);

	defaultCameraSocketOffset = mainChar->GetCharSpringArm()->SocketOffset;
}

void UCharTargetLockOnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switchCombatModeCameraCurveFTimeline.TickTimeline(DeltaTime);

	if (targetMonsterBase && mainChar)
	{
		if (mainChar->GetCharStatusComponent()->GetTargetLockOn())
		{
			if (GetOwner()->GetDistanceTo(targetMonsterBase) > maxCanTargetLockOnDis)
				ResetTarget();
			else
			{
				SetCameraRotationToTarget();
				SetCharRotationToTarget();
			}
		}
	}
}

void UCharTargetLockOnComponent::TargetLockOn(ACanTargetingActor* _target)
{
	mainChar->GetCharStatusComponent()->SetTargetSwitchState(ETargetSwitchState_Type::NotSwitch);
	if (!_target)
	{
		if (SetTargetMonster(GetMonsterAtDot(CheckTargetLockOn())))
		{
			SwitchCameraCombatMode(true);
		}
	}
	else
	{
		SetTargetMonster(_target);
	}
}

void UCharTargetLockOnComponent::SwitchTargetLockOn(ETargetSwitchState_Type _switchType)
{
	mainChar->GetCharStatusComponent()->SetTargetSwitchState(_switchType);
	SetTargetMonster(GetMonsterAtDot(TargetSwitch(_switchType)));
}

TArray<ACanTargetingActor*> UCharTargetLockOnComponent::CheckTargetLockOn(ACanTargetingActor* _monster)
{
	TArray<ACanTargetingActor*> targetMonsterBaseArr;
	if (_monster)
	{
		SetTargetMonster(_monster);
		return targetMonsterBaseArr;
	}

	FVector start = mainChar->GetActorLocation();
	FVector end = mainChar->GetActorLocation();
	TArray<AActor*> actorsToIgnore;
	TArray<FHitResult> outHitArr;

	UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		start,
		end,
		maxCanTargetLockOnDis,
		objectTypesArray,
		false,
		actorsToIgnore,
		EDrawDebugTrace::None,
		outHitArr,
		true);

	for (auto hitActor : outHitArr)
	{
		ACanTargetingActor* targetMonster = Cast<ACanTargetingActor>(hitActor.Actor);
		if (targetMonster)
		{
			//타겟과 캐릭터 사이에 벽 등이 있는지 체크
			if (CheckCharCanSeeTarget(targetMonster))
			{
				targetMonsterBaseArr.AddUnique(targetMonster);
			}		
		}
	}
	return targetMonsterBaseArr;
}

bool UCharTargetLockOnComponent::CheckCharCanSeeTarget(ACanTargetingActor* _checkTarget)
{
	FHitResult outHit;

	FVector start = camManager->GetCameraLocation();
	FVector end = _checkTarget->GetActorLocation();
	FCollisionQueryParams collisionParams;

	collisionParams.AddIgnoredActor(mainChar);
	collisionParams.AddIgnoredActor(_checkTarget);

	GetWorld()->LineTraceSingleByChannel(
		outHit,
		start,
		end,
		ECC_Visibility,
		collisionParams);

	return !outHit.bBlockingHit;
}

ACanTargetingActor* UCharTargetLockOnComponent::GetMonsterAtDot(TArray<ACanTargetingActor*> _monsterArr)
{
	ACanTargetingActor* returnMonsterBase = nullptr;
	int index = 0;
	float minDisDot = 0.0f;

	for (auto monster : _monsterArr)
	{
		if (index == 0)
		{
			minDisDot = FVector::DotProduct(camManager->GetActorForwardVector(), (monster->GetActorLocation() - camManager->GetCameraLocation()).GetSafeNormal());
			returnMonsterBase = monster;
		}
		else
		{
			float checkDisDot = FVector::DotProduct(camManager->GetActorForwardVector(), (monster->GetActorLocation() - camManager->GetCameraLocation()).GetSafeNormal());
			if (checkDisDot > minDisDot)
			{
				minDisDot = checkDisDot;
				returnMonsterBase = monster;
			}
		}
		//UE_LOG(LogTemp, Log, TEXT("%f"), minDisDot);
		index++;
	}
	return returnMonsterBase;
}

bool UCharTargetLockOnComponent::CheckTargetFront(FVector _vecA, FVector _vecB, FVector _forwardVec)
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

void UCharTargetLockOnComponent::ResetTarget(ACanTargetingActor* _target)
{
	if (_target && targetMonsterBase)
	{
		if (_target != targetMonsterBase)return;
	}
	if (targetMonsterBase)
	{
		targetMonsterBase->GetTargetLockOnWidget()->SetVisibility(false);
	}
		
	mainChar->GetCharStatusComponent()->SetTargetSwitchState(ETargetSwitchState_Type::NotSwitch);
	SwitchCameraCombatMode(false);
	mainChar->GetCharStatusComponent()->SetTargetLockOn(false);
	mainChar->GetCharacterMovement()->bOrientRotationToMovement = true;
	targetMonsterBase = nullptr;
}

TArray<ACanTargetingActor*> UCharTargetLockOnComponent::TargetSwitch(ETargetSwitchState_Type _switchType)
{
	TArray<ACanTargetingActor*> switchMonsterBaseArr;
	TArray<ACanTargetingActor*> leftTargetMonsterBaseArr;
	TArray<ACanTargetingActor*> rightTargetMonsterBaseArr;
	TArray<ACanTargetingActor*> returnSwitchMonsterBaseArr;
	if (mainChar->GetCharStatusComponent()->GetTargetLockOn())
	{
		switchMonsterBaseArr = CheckTargetLockOn();
		if (switchMonsterBaseArr.Find(targetMonsterBase) != -1)
		{
			switchMonsterBaseArr.Remove(targetMonsterBase);
		}

		for (auto switchMonster : switchMonsterBaseArr)
		{
			FVector switchMonsterLoc = switchMonster->GetActorLocation() - camManager->GetCameraLocation();
			FVector targetMonsterLoc = targetMonsterBase->GetActorLocation() - camManager->GetCameraLocation();
			float dot = FVector::CrossProduct(targetMonsterLoc, switchMonsterLoc).GetSafeNormal().Z;

			if (dot < 0)
				leftTargetMonsterBaseArr.AddUnique(switchMonster);
			else
				rightTargetMonsterBaseArr.AddUnique(switchMonster);
		}

		switch (_switchType)
		{
		case ETargetSwitchState_Type::TargetSwitchLeft:
			if (leftTargetMonsterBaseArr.Num() != 0)
			{
				returnSwitchMonsterBaseArr = leftTargetMonsterBaseArr;
			}
			break;
		case ETargetSwitchState_Type::TargetSwitchRight:
			if (rightTargetMonsterBaseArr.Num() != 0)
			{
				returnSwitchMonsterBaseArr = rightTargetMonsterBaseArr;
			}
			break;
		}
	}
	return returnSwitchMonsterBaseArr;
}

bool UCharTargetLockOnComponent::SetTargetMonster(ACanTargetingActor* _tagetMonster)
{
	if (_tagetMonster)
	{
		AMonsterBase* monsterBase = Cast<AMonsterBase>(_tagetMonster);
		if (monsterBase)
		{
			if (monsterBase->MON_COMBATSTATE == EAI_Combat_State::Teleport)
			{
				return false;
			}
		}	
		if (targetMonsterBase)
			targetMonsterBase->GetTargetLockOnWidget()->SetVisibility(false);
		targetMonsterBase = _tagetMonster;
		mainChar->GetCharStatusComponent()->SetTargetLockOn(true);
		targetMonsterBase->GetTargetLockOnWidget()->SetVisibility(true);
		return true;
	}
	return false;
}

void UCharTargetLockOnComponent::SetCameraRotationToTarget()
{
	AMonsterBase* monsterBase = Cast<AMonsterBase>(targetMonsterBase);

	float zPos = 0.0f;
	if (monsterBase)zPos = targetMonsterBase->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.7;
	else zPos = 300.0f;

	FVector enemyVec = FVector(
		targetMonsterBase->GetActorLocation().X,
		targetMonsterBase->GetActorLocation().Y, 
		targetMonsterBase->GetActorLocation().Z - zPos);

	FVector camVec = FVector(
		mainChar->GetActorLocation().X, 
		mainChar->GetActorLocation().Y, 
		camManager->GetCameraLocation().Z);

	FRotator cameraLookAtEnemyRot =
		UKismetMathLibrary::FindLookAtRotation(
			camVec,
			enemyVec);

	FRotator CameraInterpRot = FMath::RInterpTo(
		camManager->GetCameraRotation(),
		cameraLookAtEnemyRot,
		GetWorld()->DeltaTimeSeconds,
		interpSpeed);

	FRotator finalCameraRot = FRotator(
		CameraInterpRot.Pitch,
		CameraInterpRot.Yaw,
		camManager->GetCameraRotation().Roll);
	if (finalCameraRot.Pitch <= -20.0f)
	{
		finalCameraRot.Pitch = -20.0f;
	}
	else if(finalCameraRot.Pitch >= 15.0f)
	{
		finalCameraRot.Pitch = 15.0f;
	}
	//UE_LOG(LogTemp, Log, TEXT("%f %f %f"), finalCameraRot.Pitch, finalCameraRot.Yaw, finalCameraRot.Roll);
	mainChar->GetController()->SetControlRotation(finalCameraRot);
}

void UCharTargetLockOnComponent::SetCharRotationToTarget()
{
	float moveSpeed = FVector(mainChar->GetVelocity().X, mainChar->GetVelocity().Y, 0.0f).Size();
	if (moveSpeed == 0.0f || mainChar->GetCharStatusComponent()->GetMoveStop())return;

	FVector enemyVec = targetMonsterBase->GetActorLocation();

	FRotator charLookAtEnemyRot =
		UKismetMathLibrary::FindLookAtRotation(
			mainChar->GetActorLocation(),
			enemyVec);

	FRotator CharLookAtLocationYaw = FRotator(0.0f, charLookAtEnemyRot.Yaw, 0.f);

	//finalCharRot
	FRotator CharInterpRotations = FMath::RInterpTo(
		mainChar->GetActorRotation(),
		CharLookAtLocationYaw,
		GetWorld()->DeltaTimeSeconds,
		interpSpeed);

	mainChar->SetActorRotation(CharInterpRotations);
}

void UCharTargetLockOnComponent::SwitchCameraCombatMode(bool _bCombatMode)
{
	if (_bCombatMode)
	{
		switchCombatModeCameraCurveFTimeline.PlayFromStart();
	}
	else
	{
		switchCombatModeCameraCurveFTimeline.Reverse();
	}
}

void UCharTargetLockOnComponent::SwitchCameraCombatModeStart(float _val)
{
	float moveVal = FMath::Lerp<float, float>(0, 100.0f, _val);
	
	mainChar->GetCharSpringArm()->SocketOffset =
		FVector(defaultCameraSocketOffset.X,
				defaultCameraSocketOffset.Y + moveVal,
				defaultCameraSocketOffset.Z);
}
