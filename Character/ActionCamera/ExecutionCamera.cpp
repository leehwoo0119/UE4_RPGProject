// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecutionCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AExecutionCamera::AExecutionCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 230.f;
	springArm->SocketOffset = FVector(0.0f, -100.0f, 0.0f);
	springArm->bUsePawnControlRotation = true;
	check(springArm != nullptr);

	executionCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("executionCamera"));
	executionCamera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	executionCamera->bUsePawnControlRotation = false;
	check(executionCamera != nullptr);
}
