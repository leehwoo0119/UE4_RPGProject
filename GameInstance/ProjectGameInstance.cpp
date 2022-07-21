// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectGameInstance.h"
#include "Project_SLSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "MainChar.h"
#include "CharStatusComponent.h"
#include "LSH/MonsterBase.h"
#include "LSH/MonsterBaseStatus.h"
#include "LSH/MonsterCombatComponent.h"
#include "SaveCamp.h"
#include "Door.h"
#include "ItemActor.h"

UProjectGameInstance::UProjectGameInstance() :doorKeyNum(0), volumeSize(1.0f), backGroundVolumeSize(1.0f)
{
	slotName = TEXT("saveSlot");
	mapName = TEXT("Minimal_Default");
}

void UProjectGameInstance::AddDestroyedMonsterArr(AMonsterBase* _addMonster)
{
	int monsterID = _addMonster->GetMonsterStatus()->GetMonsterID();
	destroyedMonsterIDArr.AddUnique(monsterID);
}

void UProjectGameInstance::AddSaveCampArr(ASaveCamp* _addSaveCamp)
{
	int itemID = _addSaveCamp->GetItemID();
	campArr.AddUnique(itemID);
}
 
void UProjectGameInstance::AddSaveDoorArr(ADoor* _addDoor)
{
	int itemID = _addDoor->GetItemID();
	doorArr.AddUnique(itemID);
}

void UProjectGameInstance::AddDestroyItemActorArr(AItemActor* _addItemActor)
{
	int itemID = _addItemActor->GetItemID();
	destroyItemActorArr.AddUnique(itemID);
}

void UProjectGameInstance::PlusDoorKeyNum()
{
	doorKeyNum++;
}

void UProjectGameInstance::SaveData(UProject_SLSaveGame* _saveGameInstance, AMainChar* _mainChar)
{
	check(_mainChar->GetCharStatusComponent() != nullptr);
	_saveGameInstance->saveCharTransform = _mainChar->GetActorTransform();
	_saveGameInstance->saveCharCurHealthPoint = _mainChar->GetCharStatusComponent()->GetCurCharHealthPoint();
	_saveGameInstance->saveCharCalHealthPoint = _mainChar->GetCharStatusComponent()->GetCurCharHealthPoint();
	_saveGameInstance->saveCharCurHealthRecoveryPoint = _mainChar->GetCharStatusComponent()->GetCurCharRecoveryPoint();
	_saveGameInstance->saveCharCurUltimatePoint = _mainChar->GetCharStatusComponent()->GetCurCharUltimatePoint();

	_saveGameInstance->saveDoorKeyNum = doorKeyNum;

	_saveGameInstance->saveDestroyedMonsterIDArr.Empty();
	_saveGameInstance->saveDestroyedMonsterIDArr = destroyedMonsterIDArr;

	_saveGameInstance->saveDestroyItemActorArr.Empty();
	_saveGameInstance->saveDestroyItemActorArr = destroyItemActorArr;

	_saveGameInstance->saveCampArr.Empty();
	_saveGameInstance->saveCampArr = campArr;
	
	_saveGameInstance->saveDoorArr.Empty();
	_saveGameInstance->saveDoorArr = doorArr;

	_saveGameInstance->mapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
}

void UProjectGameInstance::SaveGame()
{
	UProject_SLSaveGame* saveGameInstance;
	if (UGameplayStatics::DoesSaveGameExist(slotName, 0))
	{
		saveGameInstance = Cast<UProject_SLSaveGame>(UGameplayStatics::LoadGameFromSlot(slotName, 0));
		check(saveGameInstance != nullptr);
	}
	else
	{
		saveGameInstance = Cast<UProject_SLSaveGame>(UGameplayStatics::CreateSaveGameObject(UProject_SLSaveGame::StaticClass()));
		check(saveGameInstance != nullptr);
	}

	AMainChar* mainChar = 
		Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);
	
	SaveData(saveGameInstance, mainChar);
	UGameplayStatics::SaveGameToSlot(saveGameInstance, slotName, 0);
}

void UProjectGameInstance::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(slotName, 0))
	{
		UProject_SLSaveGame* loadGameInstance = Cast<UProject_SLSaveGame>(UGameplayStatics::LoadGameFromSlot(slotName, 0));
		check(loadGameInstance != nullptr);
		
		LoadCharStatus(loadGameInstance);
		LoadGameDestroyMonster(loadGameInstance);
		LoadGameDestroyItemActor(loadGameInstance);
		LoadSaveCamp(loadGameInstance);
		LoadDoor(loadGameInstance);
		mapName = loadGameInstance->mapName;
	}	
}

void UProjectGameInstance::LoadCharStatus(UProject_SLSaveGame* _saveGameInstance)
{
	AMainChar* mainChar =
		Cast<AMainChar>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	check(mainChar != nullptr);

	mainChar->SetActorTransform(_saveGameInstance->saveCharTransform);
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharHealthPoint(_saveGameInstance->saveCharCurHealthPoint, true);
	mainChar->GetCharStatusComponent()->SetCalCharHealthPoint(_saveGameInstance->saveCharCurHealthPoint);
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharStaminaPoint(mainChar->GetCharStatusComponent()->GetMaxCharStaminaPoint(), true);
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharRecoveryPoint(_saveGameInstance->saveCharCurHealthRecoveryPoint, true);
	mainChar->GetCharStatusComponent()->PlusOrSetCurCharUltimatePoint(_saveGameInstance->saveCharCurUltimatePoint, true);
}

void UProjectGameInstance::LoadGameDestroyMonster(UProject_SLSaveGame* _saveGameInstance)
{
	TSubclassOf<AMonsterBase> classToFind;
	classToFind = AMonsterBase::StaticClass();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, FoundActors);
	for(auto checkDestroyedMon : FoundActors)
	{
		AMonsterBase* monsterBase = Cast<AMonsterBase>(checkDestroyedMon);
		check(monsterBase != nullptr);
		if (CheckDestroyedMonster(_saveGameInstance, monsterBase))
		{
			monsterBase->MON_COMBATCOMP->MonsterDeathDestroy();
			destroyedMonsterIDArr.AddUnique(monsterBase->MON_STATUS->GetMonsterID());
		}
	}
}

bool UProjectGameInstance::CheckDestroyedMonster(UProject_SLSaveGame* _saveGameInstance,AMonsterBase* _monsterBase)
{
	return (_saveGameInstance->saveDestroyedMonsterIDArr.Find(_monsterBase->MON_STATUS->GetMonsterID()) != -1);
}

void UProjectGameInstance::LoadGameDestroyItemActor(UProject_SLSaveGame* _saveGameInstance)
{
	TSubclassOf<AItemActor> classToFind;
	classToFind = AItemActor::StaticClass();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, FoundActors);

	for (auto checkDestroyedActor : FoundActors)
	{
		AItemActor* destroyActor = Cast<AItemActor>(checkDestroyedActor);
		check(destroyActor != nullptr);

		if (CheckDestroyedItem(_saveGameInstance, destroyActor))
		{
			destroyActor->DestroyItemActor();
			destroyItemActorArr.AddUnique(destroyActor->GetItemID());
		}
	}
}

bool UProjectGameInstance::CheckDestroyedItem(UProject_SLSaveGame* _saveGameInstance, AItemActor* _itemActor)
{
	return (_saveGameInstance->saveDestroyItemActorArr.Find(_itemActor->GetItemID()) != -1);
}

void UProjectGameInstance::LoadSaveCamp(UProject_SLSaveGame* _saveGameInstance)
{	
	TSubclassOf<ASaveCamp> classToFind;
	classToFind = ASaveCamp::StaticClass();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, FoundActors);

	for (auto checkActor : FoundActors)
	{
		ASaveCamp* saveCamp = Cast<ASaveCamp>(checkActor);
		check(saveCamp != nullptr);

		if (CheckSaveCamp(_saveGameInstance, saveCamp))
		{
			saveCamp->DrawFire();
			campArr.AddUnique(saveCamp->GetItemID());
		}
	}
}

bool UProjectGameInstance::CheckSaveCamp(UProject_SLSaveGame* _saveGameInstance, ASaveCamp* _saveCamp)
{
	return (_saveGameInstance->saveCampArr.Find(_saveCamp->GetItemID()) != -1);
}

void UProjectGameInstance::LoadDoor(UProject_SLSaveGame* _saveGameInstance)
{
	TSubclassOf<ADoor> classToFind;
	classToFind = ADoor::StaticClass();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), classToFind, FoundActors);

	for (auto checkActor : FoundActors)
	{
		ADoor* door = Cast<ADoor>(checkActor);
		check(door != nullptr);

		if (CheckDoor(_saveGameInstance, door))
		{
			door->OpenDoor();
			doorArr.AddUnique(door->GetItemID());
		}
	}
	doorKeyNum = _saveGameInstance->saveDoorKeyNum;
	//for (auto checkSaveDoor : _saveGameInstance->saveDoorArr)
	//{
	//	ADoor* saveDoor = Cast<ADoor>(checkSaveDoor);
	//	check(saveDoor != nullptr);
	//	saveDoor->OpenDoor();
	//	doorArr.AddUnique(saveDoor);
	//}
	//doorKeyNum = _saveGameInstance->saveDoorKeyNum;
}

bool UProjectGameInstance::CheckDoor(UProject_SLSaveGame* _saveGameInstance, ADoor* _door)
{
	return (_saveGameInstance->saveDoorArr.Find(_door->GetItemID()) != -1);
}


void UProjectGameInstance::DeleteSaveData()
{
	destroyedMonsterIDArr.Empty();
	campArr.Empty();
	doorArr.Empty();
	destroyItemActorArr.Empty();
	doorKeyNum = 0;
	if (UGameplayStatics::DoesSaveGameExist(slotName, 0))
	{		
		UGameplayStatics::DeleteGameInSlot(slotName, 0);
	}	
}

void UProjectGameInstance::GameStartGetMap()
{
	if (UGameplayStatics::DoesSaveGameExist(slotName, 0))
	{
		UProject_SLSaveGame* loadGameInstance = Cast<UProject_SLSaveGame>(UGameplayStatics::LoadGameFromSlot(slotName, 0));
		mapName = loadGameInstance->mapName;
	}
}

void UProjectGameInstance::SetVolumeSize(float _val)
{
	volumeSize = _val;
}

void UProjectGameInstance::SetBackGroundVolumeSize(float _val)
{
	backGroundVolumeSize = _val;
}
