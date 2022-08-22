#pragma once

#include "CoreMinimal.h"
//подключились что бы мы могли быть как спектр
#include "GameFramework/Character.h"
#include "ShootBaseCharacter.generated.h"

//Подключили классы компонентов
class UCameraComponent;														//Это Форвард Декларейшен
class USpringArmComponent;
class UShootHealtsComponent;
class UTextRenderComponent;
class UShootWeaponComponent;												//только логический


// class AShootBaseWeapon;	//оружие класс перенесли в компонент оружия

UCLASS()
class GAMESPROJECT_API AShootBaseCharacter : public ACharacter
{
	GENERATED_BODY()
		 
public:

	AShootBaseCharacter (const FObjectInitializer& ObjInit);		//const FObjectInitializer& objInit - сделали что бы явно указать класс UCharacterMovementComponent

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components") //настройки для вращения камеры вокруг объекта
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components") //настройки для камеры создали
		UCameraComponent* CameraComponent;									//создали указатель на камеру



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")	//ДЛЯ хп
		UShootHealtsComponent* HealtsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UTextRenderComponent* HealtsTextComponent;





	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UShootWeaponComponent* WeaponComponent;



	//позволяет проигрывать анимацию не заходя в граф анимации
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation") //анимационный ассет
		UAnimMontage* DeathAnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		float LifeSpan = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);		//хранит вектор, предельную скорость при которой наносится урон определенный

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
		FVector2D LandedDamage = FVector2D(10.0f, 100.0f);		//при 900 - 10, при 1200 - 100


	/*	перенесли в оружие
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
		TSubclassOf<AShootBaseWeapon> WeaponClass;							//выбор класса оружия
	*/
	

	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
		bool isRuning() const;

	UFUNCTION(BlueprintCallable, Category = "Movement")			//угол 
		float GetMovementDirection() const;

private:

	bool RunKeyActiv = false;
	bool RunCharacter = false;

	//Движения персонажа
	void MoveForward(float Amount);
	void RightForward(float Amount);
	void Run();
	void StopRun();

	/*
	void LookUp(float Amount);			//сделали их напрямую 
	void TurnAround(float Amount);
	*/

	void OnDeath();
	//функция делегата срабатывающая при измении хп
	void OnHealtsChanged(float Healts, float HealthDelta);

	UFUNCTION()
	void OnGroundLanded(const FHitResult& Hit); //FHitResult - это информация о двух соприкосающих объектов


	
};
