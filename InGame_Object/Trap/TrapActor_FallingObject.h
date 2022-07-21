// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrapActor.h"
#include "Components/TimeLineComponent.h"
#include "TrapActor_FallingObject.generated.h"

class UDestructibleComponent;

UCLASS()
class LSH_API ATrapActor_FallingObject : public ATrapActor
{
	GENERATED_BODY()

public:
	ATrapActor_FallingObject();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap", meta = (AllowPrivateAccess = "true"))
	UDestructibleComponent* destructibleMesh;

	FTimerHandle destroyHandle;
	int destroyTimer;

public:
	UFUNCTION()
	virtual void TrapCollisionOnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
