// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "CharTargetLockOnComponent.h"
#include "CharStateMachine.h"
#include "CharStateActor.h"
#include "CharDashState.h"
#include "CharNormalState.h"
#include "CharCrouchState.h"
#include "CharMeleeAttackState.h"
#include "CharMeleeAttackStartState.h"
#include "CharDefenseState.h"
#include "CharParryingState.h"
#include "CharStealthState.h"
#include "CharUltimateState.h"
#include "CharWeaponSwapState.h"
#include "CharBowChargingState.h"
#include "CharFullChargingState.h"
#include "CharFullChargingFailState.h"
#include "CharExecutionState.h"
#include "CharRecoveryState.h"
#include "LSH/MonsterBase.h"
#include "LSH/MonsterCombatComponent.h"
#include "CharStatusComponent.h"
#include "CharCheckAttackAvoidState.h"
#include "ProjectGameInstance.h"


AMainCharController::AMainCharController() :bMoveForward(false), bMoveRight(false)
{
	turnRate = 65.0f;
	charWalkSpeed = 200.0f;
	charRunSpeed = 500.0f;
	charSprintSpeed = 1000.0f;
	charHitMaxAcceleration = 10000.0f;
	charNormalMaxAcceleration = 2048.0f;
	maxMinAimRotate = 40.0f;
}

void AMainCharController::BeginPlay()
{
	Super::BeginPlay();

	mainChar = Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);

	mainChar->GetCharacterMovement()->MaxWalkSpeed = charRunSpeed;

}

void AMainCharController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharController::Jump);
	InputComponent->BindAction("Dash", IE_Pressed, this, &AMainCharController::DashStart);
	InputComponent->BindAction("SilentWalk", IE_Pressed, this, &AMainCharController::SilentWalkStart);
	InputComponent->BindAction("SilentWalk", IE_Released, this, &AMainCharController::SilentWalkEnd);
	InputComponent->BindAction("Sprint", IE_DoubleClick, this, &AMainCharController::SprintStart);
	InputComponent->BindAction("Sprint", IE_Released, this, &AMainCharController::SprintEnd);
	InputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharController::Crouch);
	InputComponent->BindAction("MeleeAttack", IE_Pressed, this, &AMainCharController::MeleeAttackOrCharging);
	InputComponent->BindAction("MeleeAttack", IE_Released, this, &AMainCharController::Shooting);
	InputComponent->BindAction("Defense", IE_Pressed, this, &AMainCharController::DefenseOrChargeStart);
	InputComponent->BindAction("Defense", IE_Released, this, &AMainCharController::DefenseOrChargeEnd);
	InputComponent->BindAction("Ultimate", IE_Pressed, this, &AMainCharController::UltimateAttack);
	InputComponent->BindAction("TargetLockOn", IE_Pressed, this, &AMainCharController::TargetLockOn);
	InputComponent->BindAction("TatgetLockOnLeft", IE_Pressed, this, &AMainCharController::TargetLockOnLeft);
	InputComponent->BindAction("TatgetLockOnRight", IE_Pressed, this, &AMainCharController::TargetLockOnRight);
	InputComponent->BindAction("Swap", IE_Pressed, this, &AMainCharController::WeaponSwap);
	InputComponent->BindAction("Recovery", IE_Pressed, this, &AMainCharController::Recovery);
	InputComponent->BindAction("AttackAvoid", IE_Pressed, this, &AMainCharController::AttackAvoid);
	InputComponent->BindAction("Menu", IE_Pressed, this, &AMainCharController::Menu);
	InputComponent->BindAction("Cheat", IE_Pressed, this, &AMainCharController::Cheat);

	InputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharController::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharController::MoveRight);
	InputComponent->BindAxis(TEXT("LookUp"), this, &AMainCharController::LookUpRate);
	InputComponent->BindAxis(TEXT("LookUpRight"), this, &AMainCharController::LookUpRightRate);
}

void AMainCharController::MoveForward(float _input)
{
	if (_input != 0.0f)
	{
		if (_input == 1.0f)mainChar->GetCharStatusComponent()->SetDashState(EDashState_Type::FrontDash);
		else mainChar->GetCharStatusComponent()->SetDashState(EDashState_Type::BackDash);
		bMoveForward = true;

		if (mainChar->GetCharStatusComponent()->GetMoveStop())return;
		
		const FRotator rot = GetControlRotation();
		const FRotator yawrot(0.f, rot.Yaw, 0.f);
		const FVector direction = FRotationMatrix(yawrot).GetUnitAxis(EAxis::X);
		mainChar->AddMovementInput(direction, _input);
	}
	else
	{
		bMoveForward = false;
	}
}

void AMainCharController::MoveRight(float _input)
{
	if (_input != 0.0f)
	{
		if (_input == 1.0f)mainChar->GetCharStatusComponent()->SetDashState(EDashState_Type::RightDash);
		else mainChar->GetCharStatusComponent()->SetDashState(EDashState_Type::LeftDash);
		bMoveRight = true;

		if (mainChar->GetCharStatusComponent()->GetMoveStop())return;
		
		const FRotator rot = GetControlRotation();
		const FRotator yawrot(0.f, rot.Yaw, 0.f);
		const FVector direction = FRotationMatrix(yawrot).GetUnitAxis(EAxis::Y);
		mainChar->AddMovementInput(direction, _input);
	}
	else
	{
		bMoveRight = false;
	}
}

void AMainCharController::LookUpRate(float _input)
{
	if (!mainChar->GetCharStatusComponent()->GetTargetLockOn())
	{
		if (mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::FullCharging || 
			mainChar->GetCharStatusComponent()->GetBowState() == EBowState_Type::Aiming)
		{
			float axisPitch = UKismetMathLibrary::ClampAxis
			(UKismetMathLibrary::ClampAngle
			(mainChar->GetController()->GetControlRotation().Pitch + -_input,
				-maxMinAimRotate,
				maxMinAimRotate));

			FRotator clampRot = FRotator(axisPitch, mainChar->GetController()->GetControlRotation().Yaw, mainChar->GetController()->GetControlRotation().Roll);
			mainChar->GetController()->SetControlRotation(clampRot);
		}
		else mainChar->AddControllerPitchInput(_input * turnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMainCharController::LookUpRightRate(float _input)
{
	if (!mainChar->GetCharStatusComponent()->GetTargetLockOn())
		mainChar->AddControllerYawInput(_input * turnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharController::Jump()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Jump);
			mainChar->Jump();
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::Normal:
			mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::Jump);
			mainChar->Jump();
			break;
		}
	}
}

void AMainCharController::DashStart()
{
	if (!mainChar->GetCharStatusComponent()->CheckCanUseStaminaBehavior())return;

	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charDashState);
			break;
		case EKatanaState_Type::MeleeAttackStart:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charDashState);
			break;
		case EKatanaState_Type::CanNextMeleeAttack:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charDashState);
			break;
		case EKatanaState_Type::MeleeAttack:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charDashState);
			break;
		case EKatanaState_Type::Defense:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charDashState);
			break;
		case EKatanaState_Type::Parrying:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charDashState);
			break;
		case EKatanaState_Type::CanDashRetry:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charDashState);
			break;
		case EKatanaState_Type::TakeDownCanDash:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charDashState);
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::TakeDownCanDash:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charDashState);
			break;
		}
	}
}

void AMainCharController::SilentWalkStart()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::SilentWalk);
			mainChar->GetCharacterMovement()->MaxWalkSpeed = charWalkSpeed;
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::Normal:
			mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::SilentWalk);
			mainChar->GetCharacterMovement()->MaxWalkSpeed = charWalkSpeed;
			break;
		case EBowState_Type::Default:
			break;
		}
	}
}

void AMainCharController::SilentWalkEnd()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::SilentWalk:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charNormalState);
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::SilentWalk:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charNormalState);
			break;
		case EBowState_Type::Default:
			break;
		}
	}
}

void AMainCharController::SprintStart()
{
	if (mainChar->GetCharStatusComponent()->GetTargetLockOn())return;

	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Sprint);
			mainChar->GetCharacterMovement()->MaxWalkSpeed = charSprintSpeed;
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::Normal:
			mainChar->GetCharStatusComponent()->SetBowState(EBowState_Type::Sprint);
			mainChar->GetCharacterMovement()->MaxWalkSpeed = charSprintSpeed;
			break;
		case EBowState_Type::Default:
			break;
		}
	}
}

void AMainCharController::SprintEnd()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Sprint:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charNormalState);
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::Sprint:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charNormalState);
			break;
		case EBowState_Type::Default:
			break;
		}
	}
}

void AMainCharController::Crouch()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charCrouchState);
			break;
		case EKatanaState_Type::Crouch:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charNormalState);
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::Normal:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charCrouchState);
			break;
		case EBowState_Type::Crouch:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charNormalState);
			break;
		case EBowState_Type::Default:
			break;
		}
	}
}

void AMainCharController::MeleeAttackOrCharging()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			if (mainChar->GetCharStatusComponent()->GetStealthTarget())
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charStealthState);
			}		
			else if (mainChar->GetCharStatusComponent()->GetExecutionTarget())
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charExecutionState);
			}			
			else
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charMeleeAttackStartState);
			}			
			break;
		case EKatanaState_Type::CanNextMeleeAttack:
			if (mainChar->GetCharStatusComponent()->GetExecutionTarget())
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charExecutionState);
			}	
			else
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charMeleeAttackState);
			}			
			break;
		case EKatanaState_Type::Parrying:
			if (mainChar->GetCharStatusComponent()->GetExecutionTarget())
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charExecutionState);
			}	
			else
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charMeleeAttackStartState);
			}		
			break;
		case EKatanaState_Type::Defense:
			if (mainChar->GetCharStatusComponent()->GetExecutionTarget())
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charExecutionState);
			}	
			else
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charMeleeAttackStartState);
			}
			break;
		case EKatanaState_Type::SilentWalk:
			if (mainChar->GetCharStatusComponent()->GetStealthTarget())
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charStealthState);
			}
			else if (mainChar->GetCharStatusComponent()->GetExecutionTarget())
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charExecutionState);
			}
			else
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charMeleeAttackStartState);
			}
			break;
		case EKatanaState_Type::Crouch:
			if (mainChar->GetCharStatusComponent()->GetStealthTarget())
			{
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charStealthState);
			}
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
}

void AMainCharController::Shooting()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::FullCharging:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charFullChargingState);
			break;
		case EBowState_Type::Aiming:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charFullChargingFailState);
			break;
		}
	}
}

void AMainCharController::DefenseOrChargeStart()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		mainChar->GetCharStatusComponent()->SetDefense(true);
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charParryingState);
			break;
		case EKatanaState_Type::MeleeAttackStart:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charParryingState);
			break;
		case EKatanaState_Type::CanNextMeleeAttack:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charParryingState);
			break;
		case EKatanaState_Type::MeleeAttack:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charParryingState);
			break;
		case EKatanaState_Type::ParryingSuccess:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charParryingState);
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::Normal:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charBowChargingState);
			break;
		case EBowState_Type::Default:
			break;
		}
	}
}

void AMainCharController::DefenseOrChargeEnd()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		mainChar->GetCharStatusComponent()->SetDefense(false);
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Defense:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charNormalState);
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::FullCharging:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charFullChargingFailState);
			break;
		case EBowState_Type::Aiming:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charFullChargingFailState);
			break;
		}
	}
}

void AMainCharController::UltimateAttack()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			//if(mainChar->GetCharStatusComponent()->GetCurCharUltimatePoint() == 
			//	mainChar->GetCharStatusComponent()->GetMaxCharUltimatePoint())
				mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charUltimateState);
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
}

void AMainCharController::TargetLockOn()
{
	if (mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Execution ||
		mainChar->GetCharStatusComponent()->GetKatanaState() == EKatanaState_Type::Stealth) return;

	if (!mainChar->GetCharStatusComponent()->GetTargetLockOn())
	{
		if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
		{
			SprintEnd();
			mainChar->GetCharTargetLockOnComponent()->TargetLockOn();
		}
	}
	else
	{
		mainChar->GetCharTargetLockOnComponent()->ResetTarget();
	}
}

void AMainCharController::TargetLockOnLeft()
{
	if (mainChar->GetCharStatusComponent()->GetTargetLockOn())
	{
		mainChar->GetCharTargetLockOnComponent()->SwitchTargetLockOn(ETargetSwitchState_Type::TargetSwitchLeft);
	}
}

void AMainCharController::TargetLockOnRight()
{
	if (mainChar->GetCharStatusComponent()->GetTargetLockOn())
	{
		mainChar->GetCharTargetLockOnComponent()->SwitchTargetLockOn(ETargetSwitchState_Type::TargetSwitchRight);
	}
}

void AMainCharController::WeaponSwap()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charWeaponSwapState);
			break;
		case EKatanaState_Type::Crouch:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charWeaponSwapState);
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::Normal:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charWeaponSwapState);
			break;
		case EBowState_Type::Crouch:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charWeaponSwapState);
			break;
		case EBowState_Type::Default:
			break;
		}
	}
}

void AMainCharController::Recovery()
{
	//if (!mainChar->GetCharStatusComponent()->CheckCanUseRecoveryBehavior())return;

	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charRecoveryState);
			break;
		case EKatanaState_Type::Crouch:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charRecoveryState);
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
	else if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Bow)
	{
		switch (mainChar->GetCharStatusComponent()->GetBowState())
		{
		case EBowState_Type::Normal:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charRecoveryState);
			break;
		case EBowState_Type::Crouch:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charRecoveryState);
			break;
		case EBowState_Type::Default:
			break;
		}
	}
}

void AMainCharController::AttackAvoid()
{
	if (mainChar->GetCharStatusComponent()->GetWeaponState() == EWeaponState_Type::Katana)
	{
		switch (mainChar->GetCharStatusComponent()->GetKatanaState())
		{
		case EKatanaState_Type::Normal:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charCheckAttackAvoidState);
			break;
		case EKatanaState_Type::CanNextMeleeAttack:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charCheckAttackAvoidState);
			break;
		case EKatanaState_Type::Parrying:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charCheckAttackAvoidState);
			break;
		case EKatanaState_Type::Defense:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charCheckAttackAvoidState);
			break;
		case EKatanaState_Type::SilentWalk:
			mainChar->GetCharStateMachine()->SetCharState(mainChar->GetCharStateMachine()->charStateActor->charCheckAttackAvoidState);
			break;
		case EKatanaState_Type::Default:
			break;
		}
	}
}

void AMainCharController::Menu()
{
	UProjectGameInstance* gameIS = Cast<UProjectGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	check(gameIS != nullptr);

	gameIS->DrawMenu();
}

void AMainCharController::Cheat()
{
	mainChar->GetCharStatusComponent()->SetInvincibility(!mainChar->GetCharStatusComponent()->GetInvincibility());
}


