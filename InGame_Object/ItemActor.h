// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

UCLASS()
class LHW_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemID", meta = (AllowPrivateAccess = "true"))
	int itemID;

public:
	void DestroyItemActor();
	FORCEINLINE int GetItemID()const { return itemID; }
};
