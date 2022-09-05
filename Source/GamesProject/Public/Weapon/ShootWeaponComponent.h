// Shoot Game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootCoreTypes.h"
#include "ShootWeaponComponent.generated.h"

class AShootBaseWeapon;	//������ �����

//USTRUCT(BlueprintType)
//struct FWeaponData
//{
//	GENERATED_USTRUCT_BODY()
//
//		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
//		TSubclassOf<AShootBaseWeapon> WeaponClass;
//
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
//		UAnimMontage* ReloadAnimMontage;
//};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMESPROJECT_API UShootWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShootWeaponComponent();


	void StartFire();	//��������
	void StopFire();

	//����� ������
	void NextWeapon();

	//�����������
	void Reload();

	//������� ������� ������� ���������� ������ ������ � ������� �������� ������. ��������� �������� �� ������ � �������� ��������� �������
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

	//��������� �������� ��������
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;


	//��������� ������� ��������� �� Puckups
	bool TryToAddAmmo(TSubclassOf<AShootBaseWeapon> WeaponType, int32 ClipsAmount);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
		//TSubclassOf<AShootBaseWeapon> WeaponClass;							//����� ������ ������
		/*TArray<TSubclassOf<AShootBaseWeapon>> WeaponClasses;*/				//����� ������ ������ ������
		TArray<FWeaponData> WeaponData;											//����� ������� ��������� ������ ������

	//����� ���� �������� ������ �� �����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapons")
		/*FName WeaponAttachPointName = "WeaponSocket";*/
		FName WeaponEquipSocketName = "WeaponSocket";

	//��� ������ � �������� ����������� �������������� ������
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		FName WeaponArmorySocketName = "ArmorySocket";

	//�������� ����� ������
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		UAnimMontage* EquipAnimMontage;

	//������� �������� ����������� ������, ������ ��������� �� �������� ��� ����� ������
	UPROPERTY()
		UAnimMontage* CurrentReloadAnimMontage = nullptr;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
private:

	//����� ��������� �� ������ ���������� ��� ������ ����� ������ �� ������� ������
	UPROPERTY()//����� ����� UOject, ����������� ������
		AShootBaseWeapon* CurrentWeapon = nullptr;	//���������� ���� �����, ������� ����� ������������ � Fire()


	//void SpawnWeapons();	//���� ������ � ������������� ������ � ���� ���������. ����� ��� � �������� ������������ ������ � �����

	//������ ������, ����� ��� �������� ���������� �� ������
	UPROPERTY()
		TArray<AShootBaseWeapon*> Weapons;	//��������� �� ������� ������

	//������ ������ �������� ������� ������ �� ������� ��������� CurrentWeapon
	int32 CurrentWeaponIndex = 0;

	//����� ������������� ��������, ���� ���� ������� ����� ������
	bool EquipAnimInProgress = false;

	//����� ������������� ��������, ���� ���� ������� ����������� ������
	bool ReloadAnimInProgress = false;

	

	//����� ������
	void SpawnWeapons();

	//������������ ������ ��� ������
	void AttachWeaponToSocket(AShootBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);

	//������������ ������ � ���� �������� ������
	void EquipWeapon(int32 WeaponIndex);	//�������� ��� ������ ������� ������ ������� ������ ����������

	//����������� �������� ����� ������
	void PlayAnimMontage(UAnimMontage* Animation);

	//��� ������� � AnimNotify ������� ������� 
	void InitAnimations();	//� ��� ������� ���� �������� ����������� AnimNotify � ������������� �� ��

	//��� ������� � AnimNotify ������� ������� 
	void OnEquipFinished(USkeletalMeshComponent* MeshComp);	//������ �� ������� Notify

	//������ ��� �����������
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);

	//���������� ������ ����� ����� ��������
	bool CanFire() const;

	//���������� ������ ����� ����� ������� ������
	bool CanEquip() const;


	bool CanReload() const;

	//������ �� ��� �������
	void OnClipEmpty(AShootBaseWeapon* AmmoWeaponEpty); //������ �������� ��������� �� ������ ������� ���������� ������������, ���� ����

	//��� ������ �����������, ���� ������������ ������� ��� ������� OnClipEmpty
	void ChangeClip();

	/*template <typename T>
	T* FindNotifyByClass(UAnimSequenceBase* Animation)
	{
		if (!Animation) return nullptr;

		const auto NotifyEvents = Animation->Notifies;
		for (auto NotifyEvent : NotifyEvents)
		{
			auto AnimNotify = Cast<T>(NotifyEvent.Notify);
			if (AnimNotify)
			{
				return AnimNotify;
			}
		}
		return nullptr;
	}*/
		

};