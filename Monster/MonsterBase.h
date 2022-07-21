// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define MON_COMBATSTATE GetMonsterStateMachine()->GetCombatState()
#define MON_BEHAVIORSTATE GetMonsterStateMachine()->GetBehaviorState()
#define MON_BOSSPHASESTATE GetMonsterStateMachine()->GetBossPhaseState()
#define MON_STATEMACHINE GetMonsterStateMachine()
#define MON_ANIM GetAnimInstance()
#define MON_CTRL GetMonsterBaseController()
#define MON_STATUS GetMonsterStatus()
#define MON_COMBATCOMP GetMonsterCombatComponent()
#define MON_WIDGETCOMP GetMonsterWidgetComponent()
#define MON_TIMERCOMP GetMonsterTimerComponent()
#define MON_WEAPONSLOT GetMonsterWeaponSlot()

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CanTargetingActor.h"
#include "MonsterBase.generated.h"

class ECharState;
class UMonsterBaseAnimInstance;
class AMonsterBaseController;
class AMonsterBaseController;
class UMonsterWeaponSlot;
class UMonsterBaseStatus;
class UWidgetComponent;
class UMonsterCombatComponent;
class UMonsterWidgetComponent;
class UMonsterTimerComponent;
class UBoxComponent;
class USkeletalMeshComponent;

UCLASS()
class LSH_API AMonsterBase : public ACanTargetingActor
{
	GENERATED_BODY()

public:
	AMonsterBase();

protected:
	virtual void BeginPlay() override;

private:
	AMonsterBaseController* monsterBaseController;
	UMonsterWeaponSlot* monsterWeaponSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bWanderMoveCancel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* headComponent;

protected:
	UPROPERTY(EditAnyWhere, Category = "MonsterStatus", meta = (AllowPrivateAccess = "true"))
	UMonsterBaseStatus* monsterStatus;

	UMonsterBaseAnimInstance* animInstance;

	UMonsterStateMachine* monsterStateMachine;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "WidgetComponent", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* monsterPointBarWidget;

	UMonsterCombatComponent* monsterCombatComponent;

	FVector startLocation;

	FRotator startRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SocketName", meta = (AllowPrivateAccess = "true"))
	FName firstWeaponSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SocketName", meta = (AllowPrivateAccess = "true"))
	FName secondWeaponSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "StealthArea", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* stealthArea;

	UMonsterWidgetComponent* monsterWidgetComponent;

	UMonsterTimerComponent* monsterTimerComponent;

public:
	FORCEINLINE USkeletalMeshComponent* GetHeadComponent()const { return headComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UMonsterWeaponSlot* GetMonsterWeaponSlot()const { return monsterWeaponSlot; }

	FORCEINLINE AMonsterBaseController* GetMonsterBaseController()const { return monsterBaseController; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UMonsterStateMachine* GetMonsterStateMachine() const { return monsterStateMachine; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UMonsterBaseStatus* GetMonsterStatus()const { return monsterStatus; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UWidgetComponent* GetMonsterPointBarWidget()const { return monsterPointBarWidget; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UMonsterCombatComponent* GetMonsterCombatComponent() const { return monsterCombatComponent; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UMonsterWidgetComponent* GetMonsterWidgetComponent() const { return monsterWidgetComponent; }

	FORCEINLINE UMonsterTimerComponent* GetMonsterTimerComponent() const { return monsterTimerComponent; }

	FORCEINLINE UMonsterBaseAnimInstance* GetAnimInstance()const { return animInstance; }

	FORCEINLINE FVector GetStartLocation() const { return startLocation; }

	FORCEINLINE FRotator GetStartRotation() const { return startRotation; }

	UFUNCTION(BlueprintCallable)
	void SetMonsterWeaponSlot(UMonsterWeaponSlot* _weaponSlot);

	UFUNCTION(BlueprintCallable)
	void SetMonsterCombatComponent(UMonsterCombatComponent* _component);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UBoxComponent* GetStealthArea() const { return stealthArea; }

	FORCEINLINE bool GetWanderMoveCancel()const { return bWanderMoveCancel; }
};
