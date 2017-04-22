// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SpawnObject.generated.h"

UCLASS()
class DEMO2_API ASpawnObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnObject();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/** Return the mesh for the pickup */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return ObjectMesh; }

	/** Return whether or not the pickup is active */
	UFUNCTION(BlueprintPure, Category = "SpawnObject")
		bool IsActive();

	/** Allows other classes to safely change whether or not pickup is active*/
	UFUNCTION(BlueprintCallable, Category = "SpawnObject")
		void SetActive(bool NewPickupState);


protected:
	/**False when the object was hit, and true when it is not*/
	bool bIsActive;
private:
	/** Static mesh to represent the pickup in the level*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnObject", meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* ObjectMesh;
};
