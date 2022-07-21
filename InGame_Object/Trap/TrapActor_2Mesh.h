// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrapActor.h"
#include "TrapActor_2Mesh.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class LSH_API ATrapActor_2Mesh : public ATrapActor
{
	GENERATED_BODY()

public:
	ATrapActor_2Mesh();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* firstMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* secondMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* damageCollision;

public:
	UFUNCTION()
	virtual void TrapCollisionOnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
