// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FirstBossChair.generated.h"

UCLASS()
class LHW_API AFirstBossChair : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFirstBossChair();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
