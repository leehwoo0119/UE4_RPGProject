// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SecondBossArea.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AMonsterBase;

UCLASS()
class LHW_API ASecondBossArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecondBossArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Area", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* area;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Button", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* bottomMesh;

	bool bCharInArea;

public:
	void SetSafeAreaMaterial(bool _reset);
	void SetAreaMaterialOpacity(bool _reset);
	bool CheckDamage();

	FORCEINLINE UBoxComponent* GetArea()const { return area; }

	UFUNCTION()
	virtual void DamageCollisionOnOverlapBegin(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void DamageCollisionOnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
