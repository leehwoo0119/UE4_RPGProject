// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialManager.generated.h"

class AMainChar;
class AMonsterBase;
class UMonster_Tutorial_CombatComponent;
class UStaticMeshComponent;
class UBoxComponent;
class UTutorialText;
class UTutorialMissionList;
class UTexture2D;
class ADoor;

UENUM(BlueprintType)
enum class ETutorialSection : uint8
{
	Default UMETA(DisplayName = "Default"),
	Section1 UMETA(DisplayName = "Section1"),
	Section2 UMETA(DisplayName = "Section2"),
	Section3 UMETA(DisplayName = "Section3"),
	Section4 UMETA(DisplayName = "Section4"),
	ClearAllSection UMETA(DisplayName = "ClearAllSection")
};

UENUM(BlueprintType)
enum class ETutorialMissionCheck : uint8
{
	MissionCheck1 UMETA(DisplayName = "MissionCheck1"),
	MissionCheck2 UMETA(DisplayName = "MissionCheck2"),
	MissionCheck3 UMETA(DisplayName = "MissionCheck3"),
	MissionCheck4 UMETA(DisplayName = "MissionCheck4"),
	Default UMETA(DisplayName = "Default")
};

UCLASS()
class LSH_API ATutorialManager : public AActor
{
	GENERATED_BODY()

public:
	ATutorialManager();

private:
	AMainChar* mainChar;

	AMonsterBase* monsterBase;

	UMonster_Tutorial_CombatComponent* tutorialMonsterComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* sceneRootComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* staticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* boxCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* section2_boxCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* section3_boxCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* section4_boxCollision;

	UTutorialText* tutorialText;
	UTutorialMissionList* tutorialMissionList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UTexture2D*> tutorialTextTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<UTexture2D*> tutorialMissionTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ADoor*> doorArray;

	int currentTextIndex;
	int currentMissionIndex;

	bool doOnce;
	bool sprintFirstClear;

	ETutorialSection tutorialSection;
	ETutorialMissionCheck tutorialMissionCheck;

	FTimerHandle missionTimerHandle;

protected:
	virtual void BeginPlay() override;

public:
	void PrintText();

	void PrintMissionList();

	void NextMissionCheck();

	void InitializationMissionList();

	void MissionSuccess(float _fTimerValue);

	void TutorialCheckTimer();

	void Section1_Mission();

	void Section2_Mission();

	void Section3_Mission();

	void Section4_Mission();

public:
	void SetMonsterBase(AMonsterBase* _monsterBase);

	UFUNCTION(BlueprintCallable)
	void SetTutorialText(UTutorialText* _tutorialText);

	UFUNCTION(BlueprintCallable)
	void SetTutorialMissionList(UTutorialMissionList* _tutorialMissionList);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ETutorialMissionCheck GetTutorialMissionCheck() { return tutorialMissionCheck; }

	UFUNCTION()
	virtual void BoxCollisionOnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void Section2_BoxCollisionOnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void Section3_BoxCollisionOnOverlapBegin(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void Section4_BoxCollisionOnOverlapBegin(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);
};
