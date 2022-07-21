// Fill out your copyright notice in the Description page of Project Settings.


#include "CharStealthState.h"
#include "MainChar.h"
#include "MainCharAnimInstance.h"
#include "LSH/MonsterBase_Normal.h"
#include "LSH/MonsterCombatComponent.h"
#include "CharStatusComponent.h"
#include "CharCombatComponent.h"
#include "StealthCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

UCharStealthState::UCharStealthState():stealthCameraZoomCurveLength(0.05f)
{
	PrimaryComponentTick.bCanEverTick = true;

	minStealth_1_CameraSpringOffset = 0.0;
	maxStealth_1_CameraSpringOffset = 80.f;

	minStealth_2_CameraSpringLength = 150.f;
	maxStealth_2_CameraSpringLength = 350.0f;
}

void UCharStealthState::BeginPlay()
{
	Super::BeginPlay();

	FOnTimelineFloat Stealth_1_CameraZoomCallback;

	Stealth_1_CameraZoomCallback.BindUFunction(this, FName("Stealth_1_cameraZoomStart"));

	stealth_1_CameraZoomCurveFTimeline.AddInterpFloat(mainChar->GetCharCombatComponent()->GetExecutionCameraZoomCurveFloat(), Stealth_1_CameraZoomCallback);
	stealth_1_CameraZoomCurveFTimeline.SetTimelineLength(stealthCameraZoomCurveLength);

	FOnTimelineFloat Stealth_2_CameraZoomCallback;

	Stealth_2_CameraZoomCallback.BindUFunction(this, FName("Stealth_2_cameraZoomStart"));

	stealth_2_CameraZoomCurveFTimeline.AddInterpFloat(mainChar->GetCharCombatComponent()->GetExecutionCameraZoomCurveFloat(), Stealth_2_CameraZoomCallback);
	stealth_2_CameraZoomCurveFTimeline.SetTimelineLength(stealthCameraZoomCurveLength);

	FOnTimelineFloat Stealth_3_CameraZoomCallback;

	Stealth_3_CameraZoomCallback.BindUFunction(this, FName("Stealth_3_cameraZoomStart"));

	stealth_3_CameraZoomCurveFTimeline.AddInterpFloat(mainChar->GetCharCombatComponent()->GetExecutionCameraZoomCurveFloat(), Stealth_3_CameraZoomCallback);
	stealth_3_CameraZoomCurveFTimeline.SetTimelineLength(stealthCameraZoomCurveLength);
}

void UCharStealthState::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	stealth_1_CameraZoomCurveFTimeline.TickTimeline(DeltaTime);
	stealth_2_CameraZoomCurveFTimeline.TickTimeline(DeltaTime);
	stealth_3_CameraZoomCurveFTimeline.TickTimeline(DeltaTime);
}


void UCharStealthState::Enter()
{
	Super::Enter();	

	targetMonster = mainChar->GetCharStatusComponent()->GetStealthTarget();
	check(targetMonster != nullptr);

	mainChar->GetCharStatusComponent()->SetKatanaState(EKatanaState_Type::Stealth);
	mainChar->GetCharStatusComponent()->SetStaminaPointRecovery(false);

	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(
		mainChar->GetCharCombatComponent()->GetCharStealthCamera()->GetStealthCamera()->GetOwner(), 0.3f);
}

void UCharStealthState::Execute()
{
	Super::Execute();
	uint8 stealthType = FMath::RandRange(0, 2);

	EStealthState_Type castingStealthType = static_cast<EStealthState_Type>(stealthType);
	mainChar->GetCharStatusComponent()->SetStealthState(castingStealthType);

	mainChar->GetCharStatusComponent()->GetStealthTarget()->MON_COMBATCOMP->ExecuteStealth(castingStealthType);
	SetCharPos();
	mainChar->GetCharStatusComponent()->GetStealthTarget()->MON_COMBATCOMP->Die();

	switch (mainChar->GetCharStatusComponent()->GetStealthState())
	{
	case EStealthState_Type::Stealth_1:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->stealthMontage, 0.8f, FName("Stealth_1"));
		break;
	case EStealthState_Type::Stealth_2:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->stealthMontage, 0.8f, FName("Stealth_2"));
		break;
	case EStealthState_Type::Stealth_3:
		mainChar->GetAnimInstance()->AnimMontagePlay(mainChar->GetAnimInstance()->stealthMontage, 0.8f, FName("Stealth_3"));
		break;
	case EStealthState_Type::Default:
		break;
	}
}

void UCharStealthState::Exit()
{
	Super::Exit();

	targetMonster = nullptr;
	mainChar->GetCharStatusComponent()->SetStealthTarget(nullptr);

	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	check(camManager != nullptr);
	FRotator resetCameraRot = FRotator(
		camManager->GetCameraRotation().Pitch,
		mainChar->GetActorRotation().Yaw,
		camManager->GetCameraRotation().Roll);
	UGameplayStatics::GetPlayerController(this, 0)->SetControlRotation(resetCameraRot);

	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(
		mainChar->GetCharCamera()->GetOwner(), 0.3f);

	mainChar->GetCharStatusComponent()->SetStealthState(EStealthState_Type::Default);
}

void UCharStealthState::SetCharPos()
{
	FTransform targetTransform = mainChar->GetCharStatusComponent()->GetStealthTarget()->GetActorTransform();

	FVector moveCharLocation = targetTransform.GetLocation() + (mainChar->GetCharStatusComponent()->GetStealthTarget()->GetActorForwardVector() * -70.0f);
	FRotator moveCharRotation = mainChar->GetCharStatusComponent()->GetStealthTarget()->GetActorRotation();
	FVector charScale = mainChar->GetActorScale();

	FTransform moveChatTransform;
	moveChatTransform.SetLocation(moveCharLocation);
	moveChatTransform.SetRotation(FQuat(moveCharRotation));
	moveChatTransform.SetScale3D(charScale);

	mainChar->SetActorTransform(moveChatTransform);
}

void UCharStealthState::StartStealth_1_cameraZoomCurveFTimeline()
{
	stealth_1_CameraZoomCurveFTimeline.PlayFromStart();
}

void UCharStealthState::ReverseStealth_1_cameraZoomCurveFTimeline()
{
	stealth_1_CameraZoomCurveFTimeline.Reverse();
}

void UCharStealthState::StartStealth_2_cameraZoomCurveFTimeline()
{
	stealth_2_CameraZoomCurveFTimeline.PlayFromStart();
}

void UCharStealthState::ReverseStealth_2_cameraZoomCurveFTimeline()
{
	stealth_2_CameraZoomCurveFTimeline.Reverse();
}

void UCharStealthState::StartStealth_3_cameraZoomCurveFTimeline()
{
	stealth_3_CameraZoomCurveFTimeline.PlayFromStart();
}

void UCharStealthState::ReverseStealth_3_cameraZoomCurveFTimeline()
{
	stealth_3_CameraZoomCurveFTimeline.Reverse();
}

void UCharStealthState::Stealth_1_cameraZoomStart(float _val)
{
	float lerpSpringOffset = FMath::Lerp(minStealth_1_CameraSpringOffset, maxStealth_1_CameraSpringOffset, _val);

	mainChar->GetCharCombatComponent()->GetCharStealthCamera()->GetSpringArm()->SocketOffset = FVector(0.0f, -lerpSpringOffset, -lerpSpringOffset / 3);
	//mainChar->GetCharCombatComponent()->GetCharStealthCamera()->GetSpringArm()->TargetArmLength = lerpSpringLength;
}

void UCharStealthState::Stealth_2_cameraZoomStart(float _val)
{
	float lerpSpringLength = FMath::Lerp(maxStealth_2_CameraSpringLength, minStealth_2_CameraSpringLength, _val);
	mainChar->GetCharCombatComponent()->GetCharStealthCamera()->GetSpringArm()->TargetArmLength = lerpSpringLength;
}

void UCharStealthState::Stealth_3_cameraZoomStart(float _val)
{
	float lerpSpringOffset = FMath::Lerp(minStealth_1_CameraSpringOffset, maxStealth_1_CameraSpringOffset, _val);

	mainChar->GetCharCombatComponent()->GetCharStealthCamera()->GetSpringArm()->SocketOffset = FVector(0.0f, -lerpSpringOffset, 0.0f);
}
