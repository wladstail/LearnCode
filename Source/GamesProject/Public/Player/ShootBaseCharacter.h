#pragma once

#include "CoreMinimal.h"
//������������ ��� �� �� ����� ���� ��� ������
#include "GameFramework/Character.h"
#include "ShootBaseCharacter.generated.h"

//���������� ������ �����������
class UCameraComponent;														//��� ������� �����������
class USpringArmComponent;
class UShootHealtsComponent;
class UTextRenderComponent;
class UShootWeaponComponent;												//������ ����������


// class AShootBaseWeapon;	//������ ����� ��������� � ��������� ������

UCLASS()
class GAMESPROJECT_API AShootBaseCharacter : public ACharacter
{
	GENERATED_BODY()
		 
public:

	AShootBaseCharacter (const FObjectInitializer& ObjInit);		//const FObjectInitializer& objInit - ������� ��� �� ���� ������� ����� UCharacterMovementComponent

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components") //��������� ��� �������� ������ ������ �������
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components") //��������� ��� ������ �������
		UCameraComponent* CameraComponent;									//������� ��������� �� ������



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")	//��� ��
		UShootHealtsComponent* HealtsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UTextRenderComponent* HealtsTextComponent;





	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UShootWeaponComponent* WeaponComponent;



	//��������� ����������� �������� �� ������ � ���� ��������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation") //������������ �����
		UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float LifeSpan = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);		//������ ������, ���������� �������� ��� ������� ��������� ���� ������������

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		FVector2D LandedDamage = FVector2D(10.0f, 100.0f);		//��� 900 - 10, ��� 1200 - 100


	/*	��������� � ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
		TSubclassOf<AShootBaseWeapon> WeaponClass;							//����� ������ ������
	*/
	

	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool isRuning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")			//���� 
		float GetMovementDirection() const;

private:

	bool RunKeyActiv = false;
	bool RunCharacter = false;

	//�������� ���������
	void MoveForward(float Amount);
	void RightForward(float Amount);
	void Run();
	void StopRun();

	/*
	void LookUp(float Amount);			//������� �� �������� 
	void TurnAround(float Amount);
	*/

	void OnDeath();
	//������� �������� ������������� ��� ������� ��
	void OnHealtsChanged(float Healts, float HealthDelta);

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit); //FHitResult - ��� ���������� � ���� ������������� ��������


	
};
