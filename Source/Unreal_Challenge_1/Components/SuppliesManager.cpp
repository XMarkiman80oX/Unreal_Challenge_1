// Fill out your copyright notice in the Description page of Project Settings.


#include "SuppliesManager.h"

// Sets default values for this component's properties
USuppliesManager::USuppliesManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USuppliesManager::BeginPlay()
{
	Super::BeginPlay();

	this->VehicleMovementComponent = Cast<UVehicleMovement>(this->GetOwner()->GetComponentByClass(UVehicleMovement::StaticClass()));

	if (!this->VehicleMovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("UVehicleMovement component not found!"));
	}

}


// Called every frame
void USuppliesManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	this->CountMaterials();
	this->MakeDecision();
}

void USuppliesManager::MakeDecision()
{
	EMaterialType LeastMaterial = this->FindLeastMaterial();
	//UE_LOG(LogTemp, Display, TEXT("LEAST MATERIAL: %d"), LeastMaterial);

	switch (LeastMaterial) 
	{
		case EMaterialType::SEWING_MACHINE:
			this->VehicleMovementComponent->GoToFactory(EFactoryType::SEWING_MACHINE_FACTORY);
			break;
		case EMaterialType::COAL:
			this->VehicleMovementComponent->GoToFactory(EFactoryType::COAL_MINE);
			break;
		case EMaterialType::STEEL_BEAM:
			this->VehicleMovementComponent->GoToFactory(EFactoryType::FURNACE);
			break;
		case EMaterialType::LUMBER:
			this->VehicleMovementComponent->GoToFactory(EFactoryType::LUMBERJACKS_HUT);
			break;
		case EMaterialType::IRON:
			this->VehicleMovementComponent->GoToFactory(EFactoryType::IRON_MINE);
			break;
	}
}
EMaterialType USuppliesManager::FindLeastMaterial()
{
	TMap<EMaterialType, int> MaterialCounts;
	MaterialCounts.Add(EMaterialType::LUMBER, this->LumberCount);
	MaterialCounts.Add(EMaterialType::IRON, this->IronCount);
	MaterialCounts.Add(EMaterialType::COAL, this->CoalCount);
	MaterialCounts.Add(EMaterialType::SEWING_MACHINE, this->SewingMachineCount);
	MaterialCounts.Add(EMaterialType::STEEL_BEAM, this->SteelBeamCount);

	// Find the minimum count among all material types
	int MinCount = TNumericLimits<int>::Max();
	for (const auto& Pair : MaterialCounts)
	{
		MinCount = FMath::Min(MinCount, Pair.Value);
	}

	// Find material types with the minimum count
	TArray<EMaterialType> LeastMaterials;
	for (const auto& Pair : MaterialCounts)
	{
		if (Pair.Value == MinCount)
		{
			LeastMaterials.Add(Pair.Key);
		}
	}

	// If there are multiple material types with the same minimum count,
	// select one randomly
	if (LeastMaterials.Num() > 1)
	{
		int RandomIndex = FMath::RandRange(0, LeastMaterials.Num() - 1);
		return LeastMaterials[RandomIndex];
	}
	else if (LeastMaterials.Num() == 1)
	{
		return LeastMaterials[0];
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("NO MATERIAL COUNTS ARE FOUND"));
	return EMaterialType::SEWING_MACHINE;
}

EMaterialType USuppliesManager::GetRandomMaterialType()
{
	UEnum* EMaterialTypePtr = StaticEnum<EMaterialType>();
	int MaterialTypeCount = EMaterialTypePtr->NumEnums();

	int RandomizedIndex = FMath::RandRange(0, MaterialTypeCount - 1);

	EMaterialType RandomizedMaterialType = (EMaterialType)EMaterialTypePtr->GetValueByIndex(RandomizedIndex);

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Randomized Material Type: %d"), RandomizedMaterialType));

	return RandomizedMaterialType;
}

void USuppliesManager::CountMaterials()
{
	this->InventoryCount = this->CoalCount + this->SteelBeamCount + this->LumberCount + this->IronCount;
}

TMap<EMaterialType, int> USuppliesManager::SendRequiredMaterials(TArray<EMaterialType> RequiredMaterials)
{
	TMap<EMaterialType, int> MaterialsToSend;

	for (EMaterialType RequiredMaterial : RequiredMaterials)
	{
		switch (RequiredMaterial)
		{
			case EMaterialType::COAL:
				MaterialsToSend.Add(RequiredMaterial, this->CoalCount);
				if (this->CoalCount > 0)
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "MATERIALS SENT TO PRE REQ FACTORY");
				this->CoalCount = 0;
				break;
			case EMaterialType::STEEL_BEAM:
				MaterialsToSend.Add(RequiredMaterial, this->SteelBeamCount);
				if (this->SteelBeamCount > 0)
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "MATERIALS SENT TO PRE REQ FACTORY");
				this->SteelBeamCount = 0;
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "MATERIALS SENT TO PRE REQ FACTORY");
				break;
			case EMaterialType::LUMBER:
				MaterialsToSend.Add(RequiredMaterial, this->LumberCount);
				if(this->LumberCount > 0)
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "MATERIALS SENT TO PRE REQ FACTORY");
				this->LumberCount = 0;
				break;
			case EMaterialType::IRON:
				MaterialsToSend.Add(RequiredMaterial, this->IronCount);
				if(this->IronCount > 0)
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "MATERIALS SENT TO PRE REQ FACTORY");
				this->IronCount = 0;
				break;
			default:
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Material Of This Type Exists Or You Passed a Sewing Machine");
				break;
		}
	}
	return MaterialsToSend;
}

void USuppliesManager::CollectMaterials(TMap<EFactoryType, int> MaterialsFromFactory)
{
	EFactoryType FactoryTypeSender; // Initialize CoalCount to a default value

	// Check if the map is not empty
	if (!MaterialsFromFactory.IsEmpty())
	{
		// Get an iterator pointing to the first element
		auto It = MaterialsFromFactory.CreateIterator();

		// Access the value of the first element
		FactoryTypeSender = It.Key();
		switch (FactoryTypeSender)
		{
			case EFactoryType::SEWING_MACHINE_FACTORY:
				this->SewingMachineCount = It.Value();
				break;
			case EFactoryType::COAL_MINE:
				this->CoalCount = It.Value();
				break;
			case EFactoryType::FURNACE:
				this->SteelBeamCount = It.Value();
				break;
			case EFactoryType::LUMBERJACKS_HUT:
				this->LumberCount = It.Value();
				break;
			case EFactoryType::IRON_MINE:
				this->IronCount = It.Value();
				break;
			default:
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Material Of This Type Exists");
				break;
		}

	}
}
int USuppliesManager::GetInventorySize()
{
	return this->InventoryCount;
}

int USuppliesManager::GetAmountOf(EMaterialType ChosenMaterial)
{
	switch (ChosenMaterial) 
	{
		case EMaterialType::SEWING_MACHINE:
			return this->SewingMachineCount;
			break;
		case EMaterialType::COAL:
			return this->CoalCount;
			break;
		case EMaterialType::STEEL_BEAM:
			return this->SteelBeamCount;
			break;
		case EMaterialType::LUMBER:
			return this->LumberCount;
			break;
		case EMaterialType::IRON:
			return this->IronCount;
			break;
		default:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Material Of This Type Exists");
			return 0;
			break;
	}
}

int USuppliesManager::GetAvailableOf(EMaterialType Material)
{
	switch (Material)
	{
		case EMaterialType::SEWING_MACHINE:
			return this->InventoryCount - this->SewingMachineCount;
			break;
		case EMaterialType::COAL:
			return this->InventoryCount - this->CoalCount;
			break;
		case EMaterialType::STEEL_BEAM:
			return this->InventoryCount - this->SteelBeamCount;
			break;
		case EMaterialType::LUMBER:
			return this->InventoryCount - this->LumberCount;
			break;
		case EMaterialType::IRON:
			return this->InventoryCount - this->IronCount;
			break;
		default:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Material Of This Type Exists");
			return 0;
			break;
	}
}
