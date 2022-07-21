// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharAnimInstance.h"
#include "MainChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharStatusComponent.h"
#include "ProjectGameInstance.h"

void UMainCharAnimInstance::NativeInitializeAnimation()
{
	pawn = TryGetPawnOwner();

	if (pawn)
		mainChar = Cast<AMainChar>(pawn);

	if (mainChar)
		aniInstance = Cast<UAnimInstance>(mainChar->GetMesh()->GetAnimInstance());

	katanaState = EKatanaState_Type::Normal;
	bowState = EBowState_Type::Normal;
	weaponState = EWeaponState_Type::Katana;
}

void UMainCharAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	gameIS= Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(gameIS != nullptr);

	check(pawn != nullptr);
	check(mainChar != nullptr);
	check(aniInstance != nullptr);
	charStatus = mainChar->GetCharStatusComponent();
	check(charStatus != nullptr);

	mainCharController = Cast<AMainCharController>(mainChar->GetController());
	check(mainCharController != nullptr);

	check(dashMontage != nullptr);
	check(attackMontage != nullptr);
	check(defenseRootMontage != nullptr);
	check(defenseInplaceMontage != nullptr);
	check(stealthMontage != nullptr);
	check(skillMontage != nullptr);
	check(weaponSwapMontage != nullptr);
	check(bowAttackMontage != nullptr);
	check(takeDamageMontage != nullptr);
	check(takeDownDamageFrontMontage != nullptr);	
	check(takeDownDamageBackMontage != nullptr);
	check(defenseSuccessMontage != nullptr);
	check(parryingSuccessMontage != nullptr); 
	check(executionMontage != nullptr); 
	check(attackAvoidMontage != nullptr);
	check(dieMontage != nullptr);
	
	maxWalkSpeed = mainCharController->GetCharWalkSpeed();
	maxRunSpeed = mainCharController->GetCharRunSpeed();
}

void UMainCharAnimInstance::UpdateAnimationProperties()
{
	if (pawn)
	{
		FVector speed = pawn->GetVelocity();
		speed.Z = 0;
		moveSpeed = speed.Size();
		FRotator rot = pawn->GetActorRotation();

		dir = aniInstance->CalculateDirection(speed, rot);
	}

	if (mainChar && mainCharController)
	{
		bMoveForward = mainCharController->GetMoveForward();
		bMoveRight = mainCharController->GetMoveRight();
		bIsinAir = mainChar->GetCharacterMovement()->IsFalling();
		aimRotate = UKismetMathLibrary::NormalizedDeltaRotator(mainCharController->GetControlRotation(), mainChar->GetActorRotation()).Pitch;
	}
	if (charStatus)
	{
		katanaState = charStatus->GetKatanaState();
		bowState = charStatus->GetBowState();
		weaponState = charStatus->GetWeaponState();
	}
}

void UMainCharAnimInstance::AnimMontagePlay(UAnimMontage* _animMontage, float _animSpeed, FName montageSectionName)
{
	mainChar->GetMesh()->GetAnimInstance()->Montage_Play(
		_animMontage,
		_animSpeed
	);
	mainChar->GetMesh()->GetAnimInstance()->Montage_JumpToSection(
		montageSectionName,
		_animMontage
	);
}

void UMainCharAnimInstance::SetAimRotate(float _val)
{
	aimRotate = _val;
}
