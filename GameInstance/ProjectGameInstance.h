// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectGameInstance.generated.h"

class AMonsterBase;
class AMainChar;
class UProject_SLSaveGame;
class ASaveCamp;
class ADoor;
class AItemActor;
class USoundBase;

UCLASS()
class LHW_API UProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UProjectGameInstance();
	
private:
	TArray<int> destroyedMonsterIDArr;
	TArray<int> campArr;
	TArray<int> doorArr;
	TArray<int> destroyItemActorArr;
	uint8 doorKeyNum;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString mapName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString slotName;

	float volumeSize;
	float backGroundVolumeSize;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE uint8 GetDoorKeyNum()const { return doorKeyNum; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE TArray<int> GetDestroyedMonsterIDArr()const { return destroyedMonsterIDArr; }

	void AddDestroyedMonsterArr(AMonsterBase* _addMonster);

	UFUNCTION(BlueprintCallable)
	void AddSaveCampArr(ASaveCamp* _addSaveCamp);

	UFUNCTION(BlueprintCallable)
	void AddSaveDoorArr(ADoor* _addDoor);

	UFUNCTION(BlueprintCallable)
	void AddDestroyItemActorArr(AItemActor* _addItemActor);

	UFUNCTION(BlueprintCallable)
	void PlusDoorKeyNum();

	void SaveData(UProject_SLSaveGame* _saveGameInstance, AMainChar* _mainChar);

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();

	void LoadCharStatus(UProject_SLSaveGame* _saveGameInstance);

	void LoadGameDestroyMonster(UProject_SLSaveGame* _saveGameInstance);

	bool CheckDestroyedMonster(UProject_SLSaveGame* _saveGameInstance, AMonsterBase* _monsterBase);

	bool CheckDestroyedItem(UProject_SLSaveGame* _saveGameInstance, AItemActor* _itemActor);

	bool CheckSaveCamp(UProject_SLSaveGame* _saveGameInstance, ASaveCamp* _saveCamp);

	bool CheckDoor(UProject_SLSaveGame* _saveGameInstance, ADoor* _door);

	void LoadGameDestroyItemActor(UProject_SLSaveGame* _saveGameInstance);

	void LoadSaveCamp(UProject_SLSaveGame* _saveGameInstance);

	void LoadDoor(UProject_SLSaveGame* _saveGameInstance);
	
	UFUNCTION(BlueprintCallable)
	void DeleteSaveData();

	UFUNCTION(BlueprintCallable)
	void GameStartGetMap();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlaySoundManager(USoundBase* _soundAsset, FVector _soundPos, float _volumeBal, float _pitchVal, float _startVal);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void PlayBackGroundSoundManager(USoundBase* _soundAsset, float _volumeBal, float _pitchVal);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StopBackGroundSound();

	UFUNCTION(BlueprintImplementableEvent)
	void DrawMenu();

	UFUNCTION(BlueprintCallable)
	void SetVolumeSize(float _val);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetVolumeSize()const { return volumeSize; }

	UFUNCTION(BlueprintCallable)
	void SetBackGroundVolumeSize(float _val);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetBackGroundVolumeSize()const { return  backGroundVolumeSize; }
};
