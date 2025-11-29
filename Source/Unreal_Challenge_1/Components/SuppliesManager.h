// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Entities/FactoryMaterial.h"
#include "VehicleMovement.h"
#include "Components/ActorComponent.h"
#include "SuppliesManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_CHALLENGE_1_API USuppliesManager : public UActorComponent
{
	GENERATED_BODY()

public:
	USuppliesManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(VisibleAnywhere) int InventoryCount;
	UPROPERTY(VisibleAnywhere) UVehicleMovement* VehicleMovementComponent;

	UPROPERTY(EditAnywhere) int Max_Inventory_Size = 4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) int LumberCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) int IronCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) int SteelBeamCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) int CoalCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) int SewingMachineCount = 0;

protected:
	void MakeDecision();
	EMaterialType FindLeastMaterial();

	//int getLeastMaterialCountIndex(TArray<int> InventoryCount);
	EMaterialType GetRandomMaterialType();
	void CountMaterials();

	UFUNCTION(BlueprintCallable)
	TMap<EMaterialType, int> SendRequiredMaterials(TArray<EMaterialType> RequiredMaterials);
public:
	UFUNCTION(BlueprintCallable)
	void CollectMaterials(TMap<EFactoryType, int> MaterialsFromFactory);
	
	/*UFUNCTION(BlueprintCallable)
	TArray<AFactoryMaterial*> GetInventory();*/

	UFUNCTION(BlueprintCallable)
	int GetInventorySize();

	UFUNCTION(BlueprintCallable)
	int GetAmountOf(EMaterialType ChosenMaterial);

	UFUNCTION(BlueprintCallable)
	int GetAvailableOf(EMaterialType Material);
};
