// Fill out your copyright notice in the Description page of Project Settings.


#include "CharBowComponent.h"
#include "ECharState.h"
#include "MainChar.h"
#include "LSH/MonsterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "MainCharWeaponSlot.h"
#include "MainCharBow.h"
#include "MainCharArrow.h"
#include "ArrowCheckWidget.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CharWidgetComponent.h"
#include "CharStatusComponent.h"

UCharBowComponent::UCharBowComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bowChargingCameraCurveLength = 0.8f;
	bowMaxDis = 5000.0f;

}

// Called when the game starts
void UCharBowComponent::BeginPlay()
{
	Super::BeginPlay();

	mainChar = Cast<AMainChar>(GetOwner());
	check(mainChar != nullptr);

	camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	check(camManager != nullptr);

	check(bowChargingCameraCurveFloat != nullptr);
	FOnTimelineFloat bowChargingCameraCallback;
	bowChargingCameraCallback.BindUFunction(this, FName("BowChargingCameraZoomStart"));
	bowChargingCameraCurveFTimeline.AddInterpFloat(bowChargingCameraCurveFloat, bowChargingCameraCallback);
	bowChargingCameraCurveFTimeline.SetTimelineLength(bowChargingCameraCurveLength);

	charSprintArmSocketVec = mainChar->GetCharSpringArm()->SocketOffset;
	charSpringArmLength = mainChar->GetCharSpringArm()->TargetArmLength;
}

void UCharBowComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bowChargingCameraCurveFTimeline.TickTimeline(DeltaTime);

	if (mainChar)
	{
		if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow &&
			mainChar->GetCharWidgetComponent()->GetArrowCheckWidget())
		{
			SetArrowShootDir();
		}
	}
}

void UCharBowComponent::StartBowChargingCameraCurveFTimeline()
{
	mainChar->GetCharWidgetComponent()->CheckShootArrowTarget(true);
	bowChargingCameraCurveFTimeline.PlayFromStart();
}

void UCharBowComponent::ReverseBowChargingCameraCurveFTimeline()
{
	mainChar->GetCharWidgetComponent()->CheckShootArrowTarget(false);
	bowChargingCameraCurveFTimeline.Reverse();
}

void UCharBowComponent::SetArrowShootDir(bool _bShooting)
{
	FHitResult outHit;

	FTransform arrowSpawnTransform;
	FVector start = camManager->GetCameraLocation();
	FVector end = start + (camManager->GetActorForwardVector() * bowMaxDis);
	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(mainChar);
	GetWorld()->LineTraceSingleByChannel(
		outHit,
		start,
		end,
		ECC_Visibility,
		collisionParams);

	if (_bShooting)
	{
		if (outHit.bBlockingHit)
		{
			arrowSpawnTransform =
				FTransform(
					UKismetMathLibrary::FindLookAtRotation(
						mainChar->GetMainCharWeaponSlot()->GetMainCharBow()->GetActorLocation(),
						outHit.ImpactPoint),
					mainChar->GetMainCharWeaponSlot()->GetMainCharBow()->GetActorLocation(),
					FVector(1, 1, 1)
				);
		}
		else
		{
			arrowSpawnTransform =
				FTransform(
					UKismetMathLibrary::FindLookAtRotation(
						mainChar->GetMainCharWeaponSlot()->GetMainCharBow()->GetActorLocation(),
						end),
					mainChar->GetMainCharWeaponSlot()->GetMainCharBow()->GetActorLocation(),
					FVector(1, 1, 1)
				);
		}
		mainChar->GetMainCharWeaponSlot()->SpawnArrow(arrowSpawnTransform);
		mainChar->GetMainCharWeaponSlot()->GetMainCharArrow()->Fire();
	}
	else
	{
		mainChar->GetCharWidgetComponent()->GetArrowCheckWidget()->SetCheckTarget(false);
		if (outHit.bBlockingHit)
		{
			AMonsterBase* enemy = Cast<AMonsterBase>(outHit.Actor);

			if (enemy)
				mainChar->GetCharWidgetComponent()->GetArrowCheckWidget()->SetCheckTarget(true);

			else
				mainChar->GetCharWidgetComponent()->GetArrowCheckWidget()->SetCheckTarget(false);
		}
	}
}

void UCharBowComponent::BowChargingCameraZoomStart(float _val)
{
	mainChar->GetCharSpringArm()->TargetArmLength = charSpringArmLength - _val;
	mainChar->GetCharSpringArm()->SocketOffset = 
		FVector(charSprintArmSocketVec.X,
				charSprintArmSocketVec.Y + (_val / 4),
				charSprintArmSocketVec.Z + (_val / 15));
}