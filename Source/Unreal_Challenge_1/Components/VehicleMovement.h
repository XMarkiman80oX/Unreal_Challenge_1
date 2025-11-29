// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "../Entities/GenericFactory.h"
#include "../Entities/Checkpoint.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VehicleMovement.generated.h"

class AGenericFactory;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_CHALLENGE_1_API UVehicleMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVehicleMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	bool CanMove = true;
	FTimerHandle TimerHandle;
	FVector PreviousPosition = FVector();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Loading_Time = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Loading_Time_LB = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float Loading_Time_UB = 3.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) float Time_Elapsed_Since_Last_Tick = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) float Time_Elapsed_Float = 0.0f;
	float Start_Time = 0.0f;
	/*For Testing*/ UPROPERTY(EditAnywhere) TArray<FVector> Path_To_Be_Taken;
	int PathIndex = 0;

	//UPROPERTY(EditAnywhere) /*For Testing*/ EFactoryType DestinedFactory;
	UPROPERTY(EditAnywhere) float ArrivalThreshold = 10.0f;

	UPROPERTY(EditAnywhere) float MovementSpeed = 300.0f;

	UPROPERTY(VisibleAnywhere) TArray<AGenericFactory*> Factories;
	UPROPERTY(VisibleAnywhere) AActor* Left_Checkpoint = NULL;
	UPROPERTY(VisibleAnywhere) AActor* Right_Checkpoint = NULL;

	UPROPERTY(VisibleAnywhere) TArray<FVector> Factory_Locations;

	/*UPROPERTY(EditAnywhere) AGenericFactory* Test_Factory1 = NULL;
	UPROPERTY(EditAnywhere) AGenericFactory* Test_Factory2 = NULL;*/

protected:
	FVector GetFactoryLocation(EFactoryType FactoryType);
	void IncludeAdjacentCPLocations(FVector Adjacent_Factory_Location);
	//void CheckIsMoving();

	void Stay();
	float RandomizeLoadingTime();
	void RotateVehicle(FVector Destination, FVector CurrentLocation);

public:
	void GoToFactory(EFactoryType FactoryDestination);
	//bool GetIsMoving();
};

