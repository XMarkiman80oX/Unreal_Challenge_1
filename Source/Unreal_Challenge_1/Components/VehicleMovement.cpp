
// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleMovement.h"

// Sets default values for this component's properties
UVehicleMovement::UVehicleMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UVehicleMovement::BeginPlay()
{
	Super::BeginPlay();

	if (this->ArrivalThreshold == 0.0f) this->ArrivalThreshold = 1.0f;
	this->PreviousPosition = this->GetOwner()->GetActorLocation();

	this->Factories.Empty();
	TArray<AActor*> FoundFactories;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGenericFactory::StaticClass(), FoundFactories);

	for (AActor* FoundFactory : FoundFactories)
	{
		AGenericFactory* GenericFactory = Cast<AGenericFactory>(FoundFactory);
		if (GenericFactory)
		{
			Factories.Add(GenericFactory);
		}
	}

	TArray<AActor*> FoundCheckpoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckpoint::StaticClass(), FoundCheckpoints);
	for (AActor* FoundCheckpoint : FoundCheckpoints)
	{
		ACheckpoint* Checkpoint = Cast<ACheckpoint>(FoundCheckpoint);
		if (Checkpoint)
		{
			switch (Checkpoint->GetCheckpointType()) {
				case  ECheckpointType::LEFT_CHECKPOINT:
					this->Left_Checkpoint = FoundCheckpoint;
					break;
				case ECheckpointType::RIGHT_CHECKPOINT:
					this->Right_Checkpoint = FoundCheckpoint;
					break;
			}
		}
	}

	//this->GoToFactory(EFactoryType::LUMBERJACKS_HUT);

}

void UVehicleMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!this->Path_To_Be_Taken.IsEmpty() && this->CanMove)
	{
		FVector CurrentLocation = this->GetOwner()->GetActorLocation();
		FVector Direction = (this->Path_To_Be_Taken[0] - CurrentLocation).GetSafeNormal();
		FVector NewLocation = CurrentLocation + Direction * MovementSpeed * DeltaTime;

		this->GetOwner()->SetActorLocation(NewLocation);
		this->RotateVehicle(NewLocation, CurrentLocation);

		// Check if arrived at the target location
		if ((NewLocation - this->Path_To_Be_Taken[0]).SizeSquared() <= FMath::Square(ArrivalThreshold))
		{
			this->Path_To_Be_Taken.RemoveAt(0);
			//If not a checkpoint
			if(CurrentLocation.Y != 0) this->CanMove = false;
		}
	}

	this->Stay();
}

FVector UVehicleMovement::GetFactoryLocation(EFactoryType FactoryType)
{
	FVector FactoryLocation;

	for (AGenericFactory* Factory : this->Factories) 
	{
		if (Factory->GetFactoryType() == FactoryType && Factory != NULL) 
		{
			FactoryLocation = Factory->GetActorLocation();
			break;
		}
	}

	return FactoryLocation;
}

void UVehicleMovement::IncludeAdjacentCPLocations(FVector Adjacent_Factory_Location)
{
	FVector RelativeToFactory = FVector();
	FVector RelativeToVehicle = FVector();
	
	//If the factory destination is on the left
	if (Adjacent_Factory_Location.X > this->GetFactoryLocation(EFactoryType::SEWING_MACHINE_FACTORY).X && this->Left_Checkpoint) 
	{
		RelativeToFactory = this->Left_Checkpoint->GetActorLocation();
	}
	//If the factory destination is on the right
	else if (Adjacent_Factory_Location.X < this->GetFactoryLocation(EFactoryType::SEWING_MACHINE_FACTORY).X && this->Right_Checkpoint)
	{
		RelativeToFactory = this->Right_Checkpoint->GetActorLocation();
	}
	//If the player is on the left side
	if (this->GetOwner()->GetActorLocation().X > this->GetFactoryLocation(EFactoryType::SEWING_MACHINE_FACTORY).X && this->Left_Checkpoint)
	{
		RelativeToVehicle = this->Left_Checkpoint->GetActorLocation();
	}
	//If the player is on the right side
	else if (this->GetOwner()->GetActorLocation().X < this->GetFactoryLocation(EFactoryType::SEWING_MACHINE_FACTORY).X && this->Right_Checkpoint)
	{
		RelativeToVehicle = this->Right_Checkpoint->GetActorLocation();
	}

	//If the player is opposite to the side of the factory destination and if the player is within the lane of the sewing machine factory
	if (RelativeToFactory != RelativeToVehicle && this->GetOwner()->GetActorLocation().Y != this->GetFactoryLocation(EFactoryType::SEWING_MACHINE_FACTORY).Y) 
	{
		this->Path_To_Be_Taken.Add(RelativeToVehicle);
		this->Path_To_Be_Taken.Add(this->GetFactoryLocation(EFactoryType::SEWING_MACHINE_FACTORY));
		this->Path_To_Be_Taken.Add(RelativeToFactory);
	}
	//If the player is on the same side of the factory destination or if the player is within the lane of the sewing machine factory
	else if (RelativeToFactory == RelativeToVehicle || this->GetOwner()->GetActorLocation().Y == this->GetFactoryLocation(EFactoryType::SEWING_MACHINE_FACTORY).Y) 
	{
		this->Path_To_Be_Taken.Add(RelativeToFactory);
	}
}

void UVehicleMovement::Stay()
{
	this->Time_Elapsed_Since_Last_Tick = GetWorld()->GetTimeSeconds() - this->Start_Time;
	this->Loading_Time = this->RandomizeLoadingTime();
	this->Time_Elapsed_Float = this->Time_Elapsed_Since_Last_Tick / this->Loading_Time;
	if (this->Time_Elapsed_Float > 1.0f)
		this->Time_Elapsed_Float = 1.0f;

	if (Time_Elapsed_Since_Last_Tick >= this->Loading_Time)
	{
		UE_LOG(LogTemp, Display, TEXT("CAN MOVE"));

		this->CanMove = true;

		Start_Time = GetWorld()->GetTimeSeconds();
	}
}

float UVehicleMovement::RandomizeLoadingTime()
{
	return FMath::FRandRange(this->Loading_Time_LB, this->Loading_Time_UB);
}

void UVehicleMovement::RotateVehicle(FVector Destination, FVector CurrentLocation)
{
	float DesiredRotationDegrees = 0.0f;

	if (Destination.X > CurrentLocation.X)
	{
		DesiredRotationDegrees = 180.0f;
	}
	if (Destination.X < CurrentLocation.X)
	{
		DesiredRotationDegrees = 0.0f;
	}
	if (Destination.Y > CurrentLocation.Y)
	{
		DesiredRotationDegrees = -90.0f;
	}
	if (Destination.Y < CurrentLocation.Y)
	{
		DesiredRotationDegrees = 90.0f;
	}

	FRotator DesiredRotation = this->GetOwner()->GetActorRotation();
	DesiredRotation.Yaw = DesiredRotationDegrees;
	this->GetOwner()->SetActorRotation(DesiredRotation, ETeleportType::None);
}

/*
* Go To Checkpoint First, then Go To Factory
*/
void UVehicleMovement::GoToFactory(EFactoryType FactoryDestination)
{
	if (this->Path_To_Be_Taken.IsEmpty()) 
	{
		this->IncludeAdjacentCPLocations(this->GetFactoryLocation(FactoryDestination));
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("%d"), FactoryDestination));
		this->Path_To_Be_Taken.Add(this->GetFactoryLocation(FactoryDestination));
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Path is not empty");
		UE_LOG(LogTemp, Display, TEXT("Path is not empty"));
	}
}
