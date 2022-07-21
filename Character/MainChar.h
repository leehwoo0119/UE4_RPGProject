// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainChar.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UCharStateMachine;
class UMainCharAnimInstance;
class UMainCharWeaponSlot;
class UCharCombatComponent;
class UCharBowComponent;
class UCharTargetLockOnComponent;
class UCharWidgetComponent;
class UCharStatusComponent;
class ADotDamageManager;

UCLASS()
class LHW_API AMainChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//Ä«¸Þ¶ó
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* springArm;

	UCharStateMachine* charStateMachine;

	UMainCharAnimInstance* animInstance;

	UMainCharWeaponSlot* mainCharWeaponSlot;

	UCharCombatComponent* charCombatComponent;

	UCharBowComponent* charBowComponent;

	UCharTargetLockOnComponent* charTargetLockOnComponent;

	UCharWidgetComponent* charWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UCharStatusComponent* charStatusComponent;

	ADotDamageManager* dotDamageManager;	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USceneComponent* instantDeathCameraPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USceneComponent* executionCameraPos;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USceneComponent* stealthCameraPos;

	UPROPERTY(EditAnyWhere, Category = "DotDamageManager")
	TSubclassOf<ADotDamageManager> dotDamageManagerBP;

public:
	FORCEINLINE USpringArmComponent* GetCharSpringArm()const { return springArm; }

	FORCEINLINE UCameraComponent* GetCharCamera()const { return camera; }

	FORCEINLINE USceneComponent* GetExecutionCameraPos()const { return executionCameraPos; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCharStateMachine* GetCharStateMachine()const { return charStateMachine; }

	FORCEINLINE UMainCharAnimInstance* GetAnimInstance()const { return animInstance; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UMainCharWeaponSlot* GetMainCharWeaponSlot()const { return mainCharWeaponSlot; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCharCombatComponent* GetCharCombatComponent()const { return charCombatComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCharBowComponent* GetCharBowComponent()const { return charBowComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCharTargetLockOnComponent* GetCharTargetLockOnComponent()const { return charTargetLockOnComponent; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCharWidgetComponent* GetCharWidgetComponent()const { return charWidgetComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCharStatusComponent* GetCharStatusComponent()const { return charStatusComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ADotDamageManager* GetDotDamageManager()const { return dotDamageManager; }

	UFUNCTION(BlueprintCallable)
	void SetMainCharWeaponSlot(UMainCharWeaponSlot* _weaponSlot);

	UFUNCTION(BlueprintCallable)
	void SetCharCombatComponent(UCharCombatComponent* _comp);

	UFUNCTION(BlueprintCallable)
	void SetCharBowComponent(UCharBowComponent* _comp);

	UFUNCTION(BlueprintCallable)
	void SetCharWidgetComponent(UCharWidgetComponent* _comp);

	UFUNCTION(BlueprintCallable)
	void SetTargetLockOnComponent(UCharTargetLockOnComponent* _comp);

	UFUNCTION(BlueprintCallable)
	void SetStateMachine(UCharStateMachine* _comp);
};
