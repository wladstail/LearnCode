// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShootCoreTypes.h"		//�������� ��������� �� ������
#include "ShootPlayerHUDWidget.generated.h"

/*���������� � ��������� ������� ShootUtils
class UShootWeaponComponent;
class UShootHealtsComponent;*/

UCLASS()
class GAMESPROJECT_API UShootPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:

	//������� ������� �������� � �� � ������ �� � ���������
	UFUNCTION(BlueprintCallable, Category = "UI")
		float GetHealthPercent() const;

	//������� ������� ������� ���������� ������ ������ � ������� �������� ������. ��������� �������� �� ������ � �������� ��������� �������
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

	//��������� �������� ��������
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	//���������� true ����� �������� ���
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerAlive() const;
	
	//���������� true ����� �� � ������ �����������
	UFUNCTION(BlueprintCallable, Category = "UI")
		bool IsPlayerSpectating() const;

	//����� ����������� �� ������� OnHealtsChanged � ������ ����� �������� ����������
	virtual bool Initialize() override;

	//event ������� ��� ���������
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")	//BlueprintImplementableEvent - ��������� ��������� event, ������� � ���������
	void OnTakeDamage();

private:

	//������� ��� ����� ���� �� ������� ���������� ��������
	void OnHeatlsChanged(float Health, float HealthDelta);

	/*���������� � ��������� ������� ShootUtils
	
	////������� ������� ���������� ��������� �� ��������� ������
	UShootWeaponComponent* GetWeaponComponent() const;

	////������� ������� ���������� ��������� �� ��������� ��������, ����� ��� IsPlayerAlive()
	UShootHealtsComponent* GetHealthComponent() const;
	*/
};
