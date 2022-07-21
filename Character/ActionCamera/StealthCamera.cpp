// Fill out your copyright notice in the Description page of Project Settings.


#include "StealthCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AStealthCamera::AStealthCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 230.f;
	springArm->bUsePawnControlRotation = true;
	check(springArm != nullptr);

	stealthCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("stealthCamera"));
	stealthCamera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	stealthCamera->bUsePawnControlRotation = false;
	check(stealthCamera != nullptr);
}

