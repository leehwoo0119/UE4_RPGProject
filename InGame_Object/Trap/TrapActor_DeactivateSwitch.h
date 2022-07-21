// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrapActor.h"
#include "TrapActor_DeactivateSwitch.generated.h"

class UStaticMeshComponent;

UCLASS()
class LSH_API ATrapActor_DeactivateSwitch : public ATrapActor
{
	GENERATED_BODY()

public:
	ATrapActor_DeactivateSwitch();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* switchMesh;

public:
	UFUNCTION()
	virtual void TrapCollisionOnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
