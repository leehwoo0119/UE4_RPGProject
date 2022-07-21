// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharComponent.generated.h"

class AMainChar;
class APlayerCameraManager;

UCLASS()
class LHW_API UCharComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharComponent();

protected:
	AMainChar* mainChar;
	APlayerCameraManager* camManager;

};
