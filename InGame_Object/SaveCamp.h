// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveCamp.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UProjectGameInstance;

UCLASS()
class LHW_API ASaveCamp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveCamp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveCamp", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* saveCampCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveCamp", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* saveCampMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveCamp", meta = (AllowPrivateAccess = "true"))
	TArray<UStaticMeshComponent*> saveCampMeshArr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveCamp", meta = (AllowPrivateAccess = "true"))
	bool bSearch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveCamp", meta = (AllowPrivateAccess = "true"))
	UProjectGameInstance* gameIS;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemID", meta = (AllowPrivateAccess = "true"))
	int itemID;

public:
	UFUNCTION(BlueprintCallable)
	void SetSaveCampCollision(bool _bCollision);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void DrawFire();

	FORCEINLINE int GetItemID()const { return itemID; }
};
