// Fill out your copyright notice in the Description page of Project Settings.


#include "MainChar.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainCharAnimInstance.h"
#include "CharStateMachine.h"
#include "CharStateActor.h"
#include "CharNormalState.h"
#include "MainCharWeaponSlot.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharTakeDamageState.h"
#include "CharCombatComponent.h"
#include "CharBowComponent.h"
#include "CharTargetLockOnComponent.h"
#include "CharWidgetComponent.h"
#include "CharStatusComponent.h"
#include "LSH/DotDamageManager.h"

// Sets default values
AMainChar::AMainChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 300.f;
	springArm->bUsePawnControlRotation = true;
	check(springArm != nullptr);

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("camera"));
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	camera->bUsePawnControlRotation = false;
	check(camera != nullptr);

	instantDeathCameraPos = CreateDefaultSubobject<USceneComponent>(TEXT("instantDeathCameraPos"));
	instantDeathCameraPos->SetupAttachment(GetRootComponent());
	check(instantDeathCameraPos != nullptr);

	executionCameraPos = CreateDefaultSubobject<USceneComponent>(TEXT("executionCameraPos"));
	executionCameraPos->SetupAttachment(GetRootComponent());
	check(executionCameraPos != nullptr);

	stealthCameraPos = CreateDefaultSubobject<USceneComponent>(TEXT("stealthCameraPos"));
	stealthCameraPos->SetupAttachment(GetRootComponent());
	check(stealthCameraPos != nullptr);
		
	charStatusComponent = CreateDefaultSubobject<UCharStatusComponent>(TEXT("charStatusComponent"));
	check(charStatusComponent != nullptr);

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AMainChar::BeginPlay()
{
	Super::BeginPlay();

	charStateMachine->charStateActor = GetWorld()->SpawnActor<ACharStateActor>(charStateMachine->charStateActorBP);
	check(charStateMachine->charStateActor != nullptr);

	dotDamageManager = GetWorld()->SpawnActor<ADotDamageManager>(dotDamageManagerBP);
	check(dotDamageManager != nullptr);

	animInstance = Cast<UMainCharAnimInstance>(GetMesh()->GetAnimInstance());
	check(animInstance != nullptr);

	charStateMachine->SetCurState(charStateMachine->charStateActor->charNormalState);
}

void AMainChar::SetMainCharWeaponSlot(UMainCharWeaponSlot* _weaponSlot)
{
	mainCharWeaponSlot = _weaponSlot;
	check(mainCharWeaponSlot != nullptr);
}

void AMainChar::SetCharCombatComponent(UCharCombatComponent* _comp)
{
	charCombatComponent = _comp;
	check(charCombatComponent != nullptr);
}

void AMainChar::SetCharBowComponent(UCharBowComponent* _comp)
{
	charBowComponent = _comp;
	check(charBowComponent != nullptr);
}

void AMainChar::SetCharWidgetComponent(UCharWidgetComponent* _comp)
{
	charWidgetComponent = _comp;
	check(charWidgetComponent != nullptr);
}

void AMainChar::SetTargetLockOnComponent(UCharTargetLockOnComponent* _comp)
{
	charTargetLockOnComponent = _comp;
	check(charTargetLockOnComponent != nullptr);
}

void AMainChar::SetStateMachine(UCharStateMachine* _comp)
{
	charStateMachine = _comp;
	check(charStateMachine != nullptr);
}



