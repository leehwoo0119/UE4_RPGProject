// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MonsterWeaponSlot.generated.h"

class AMonsterWeapon;
class AMonsterBase;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LSH_API UMonsterWeaponSlot : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMonsterWeaponSlot();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AMonsterWeapon* monsterFirstWeapon;
	AMonsterWeapon* monsterSecondWeapon;
	AMonsterBase* monsterBase;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FRotator spawnWeaponRot;

public:
	UPROPERTY(EditAnyWhere, Category = "Weapon")
	TSubclassOf<AMonsterWeapon> monsterFirstWeaponBP;	

	UPROPERTY(EditAnyWhere, Category = "Weapon")
	TSubclassOf<AMonsterWeapon> monsterSecondWeaponBP;

public:
	UFUNCTION(BlueprintCallable)
	void SetMonsterFirstWeaponSocket(FName _socketName);

	UFUNCTION(BlueprintCallable)
	void SetMonsterSecondWeaponSocket(FName _socketName);

	void SetMonsterBase(AMonsterBase* _monsterBase);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMonsterWeapon* GetMonsterFirstWeapon()const { return monsterFirstWeapon; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMonsterWeapon* GetMonsterSecondWeapon()const { return monsterSecondWeapon; }
};
