// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"
#include "MonsterBaseController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MonsterWeaponSlot.h"
#include "MonsterWeapon.h"
#include "MonsterBase_Normal.h"
#include "Kismet/KismetMathLibrary.h"
#include "MonsterStateMachine.h"
#include "EAIState.h"
#include "Components/WidgetComponent.h"
#include "TargetLockOnWidget.h"
#include "MonsterPointBar.h"
#include "MonsterBaseAnimInstance.h"
#include "MonsterTimerComponent.h"
#include "Components/SkeletalMeshComponent.h"

AMonsterBase::AMonsterBase() :bWanderMoveCancel(false)
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 250.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;

	targetLockOnWidget = CreateDefaultSubobject<UWidgetComponent>("targetLockOnWidget");
	targetLockOnWidget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	check(targetLockOnWidget != nullptr);

	monsterTimerComponent = CreateDefaultSubobject<UMonsterTimerComponent>("monsterTimerComponent");
	check(monsterTimerComponent != nullptr);

	headComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("headComponent"));
	headComponent->SetupAttachment(GetMesh(), FName("head"));
	check(headComponent != nullptr);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	animInstance = Cast<UMonsterBaseAnimInstance>(GetMesh()->GetAnimInstance());
	check(animInstance != nullptr);

	monsterWeaponSlot->SetMonsterBase(this);
	monsterWeaponSlot->SetMonsterFirstWeaponSocket(firstWeaponSocketName);
	if (monsterWeaponSlot->GetMonsterSecondWeapon())
		monsterWeaponSlot->SetMonsterSecondWeaponSocket(secondWeaponSocketName);

	monsterBaseController = Cast<AMonsterBaseController>(this->GetController());
	check(monsterBaseController != nullptr);
	
	startLocation = GetActorLocation();
	startRotation = GetActorRotation();
}

void AMonsterBase::SetMonsterWeaponSlot(UMonsterWeaponSlot* _weaponSlot)
{
	monsterWeaponSlot = _weaponSlot;
	check(monsterWeaponSlot != nullptr);
}

void AMonsterBase::SetMonsterCombatComponent(UMonsterCombatComponent* _component)
{
	monsterCombatComponent = _component;
	check(monsterCombatComponent != nullptr);
}

