// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MainCharWeaponSlot.generated.h"

class AMainCharKatana;
class AMainCharBow;
class AMainCharArrow;
class AMainChar;
class UStaticMeshComponent;
class UStaticMeshComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LHW_API UMainCharWeaponSlot : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMainCharWeaponSlot();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
private:
	AMainCharKatana* mainCharKatana;
	AMainCharBow* mainCharBow;
	AMainCharArrow* mainCharArrow;
	AMainCharArrow* mainCharArrowToHand;
	AMainChar* mainChar;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* katanaSocket;

public:
	UPROPERTY(EditAnyWhere, Category = "Weapon")
	TSubclassOf<AMainCharKatana> mainCharKatanaBP;

	UPROPERTY(EditAnyWhere, Category = "Weapon")
	TSubclassOf<AMainCharBow> mainCharBowBP;

	UPROPERTY(EditAnyWhere, Category = "Weapon")
	TSubclassOf<AMainCharArrow> mainCharArrowBP;

public:
	UFUNCTION(BlueprintCallable)
	void SetKatanaSocket(FName _socketName);

	UFUNCTION(BlueprintCallable)
	void SetBowSocket(FName _socketName);

	UFUNCTION(BlueprintCallable)
	void AttachArrowToHand();

	UFUNCTION(BlueprintCallable)
	void DettachArrowToHand();

	void SpawnArrow(FTransform _transform);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMainCharKatana* GetMainCharKatana()const { return mainCharKatana; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMainCharBow* GetMainCharBow()const { return mainCharBow; }
	FORCEINLINE AMainCharArrow* GetMainCharArrow()const { return mainCharArrow; }
};
