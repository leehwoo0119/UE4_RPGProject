// Fill out your copyright notice in the Description page of Project Settings.


#include "InstantDeathCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AInstantDeathCamera::AInstantDeathCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 250.f;
	springArm->SocketOffset.Y = 100.0f;
	springArm->bUsePawnControlRotation = true;
	check(springArm != nullptr);

	ultimateAttackCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ultimateAttackCamera"));
	ultimateAttackCamera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	ultimateAttackCamera->bUsePawnControlRotation = false;
}

