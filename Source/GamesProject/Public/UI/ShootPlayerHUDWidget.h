// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShootCoreTypes.h"		//добавили структуру из хедера
#include "ShootPlayerHUDWidget.generated.h"

/*Реализация в шаблонной функции ShootUtils
class UShootWeaponComponent;
class UShootHealtsComponent;*/

UCLASS()
class GAMESPROJECT_API UShootPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	//функция которая доступна в БП и выдает хп в процентах
	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetHealthPercent() const;

	//функция обертка которая возвращает иконки оружия и прицела текущего оружия. Структуру передаем по ссылке в качестве параметра функции
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

	//получение текущего арсенала
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	//возвращает true когда персонаж жив
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerAlive() const;
	
	//возвращает true когда мы в режиме наблюдателя
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerSpectating() const;

	//можем подписаться на делегат OnHealtsChanged и узнать когда здоровье изменяется
	virtual bool Initialize() override;

	//event событие для блупринта
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")	//BlueprintImplementableEvent - позволяет создавать event, событие в блупринте
	void OnTakeDamage();

private:

	//функция где будет бинд на делегат компонента здоровья
	void OnHeatlsChanged(float Health, float HealthDelta);

	/*Реализация в шаблонной функции ShootUtils
	
	////функция которая возвращает указатель на компонент оружия
	UShootWeaponComponent* GetWeaponComponent() const;

	////функция которая возвращает указатель на компонент здоровья, нужен для IsPlayerAlive()
	UShootHealtsComponent* GetHealthComponent() const;
	*/
};
