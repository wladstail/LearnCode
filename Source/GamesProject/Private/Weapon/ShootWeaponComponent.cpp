// Shoot Game


#include "Weapon/ShootWeaponComponent.h"
#include "Weapon/ShootBaseWeapon.h"
#include "GameFramework/Character.h"	//��������� �� ��������� 
#include "Animations/ShootEquipFinishedAnimNotify.h"	//��� �������� �����������
#include "Animations/ShootReloadFinishedAnimNotify.h"	//��� �������� �����������
#include "Animations/AnimUtils.h"						//��� �������� �����������	

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 WeaponNum = 2;

		UShootWeaponComponent::UShootWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	
}



	void UShootWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	/*InitAnimations();*/

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);

	//�������� ������ � ����
	/*SpawnWeapons();*/
	CurrentWeaponIndex = 0;

	//����� �������� �����������
	InitAnimations();

	//����� ������ �����
	SpawnWeapons();

	//������������ �������� ������ � ������� �������� ������ ����� 
	EquipWeapon(CurrentWeaponIndex);
}

	//�������� ������ � ����
	void UShootWeaponComponent::SpawnWeapons()	//����� ������ � ������������� ���� � ������ ������(����� ������ ��������� � ���� ����)
{
	//if (!GetWorld()) return;	//�������� ��� ��������� �� ��� �� �������


	ACharacter* Character = Cast<ACharacter>(GetOwner());	//���� �� ��������� ������, ��� �� ��������� �������� ����
	//if (!Character) return;//���� ������� ������� �� �������, ���� ��� ��������

	//�������� ��� ���� �������� � ���
	if (!Character || !GetWorld()) return;

	//������ ��������� ������� ������
	//for (auto WeaponClass : WeaponClasses)
	for (auto OneWeaponData : WeaponData)	//OneWeaponData - ���������� �����
	{
		//auto Weapon = GetWorld()->SpawnActor<AShootBaseWeapon>(WeaponClass);
		auto Weapon = GetWorld()->SpawnActor<AShootBaseWeapon>(OneWeaponData.WeaponClass);	//OneWeaponData.WeaponClass - ���� ���������
		if (!Weapon) continue;

		//����� ������ � ���� � ����� ������� ��� �� ���������
		//CurrentWeapon = GetWorld()->SpawnActor<AShootBaseWeapon>(WeaponClass); //����� ������ ������. SpawnActor - ���������� ��������� �� ��������� ������. ������� ����� ��������� ��������� � ���������� Weapon. 2 �������� ��������� ��������� ������
		//if (!CurrentWeapon) return; //�������� ��� ��������� �� �������

		//������������� �� ������� �����������
		Weapon->OnClipEmpty.AddUObject(this, &UShootWeaponComponent::OnClipEmpty);

		//����������� ��� ������� ������ ������� ���������
		Weapon->SetOwner(Character);

		//��������� � ������ ���������, ��� �������� ������ �������� ����� Add
		Weapons.Add(Weapon);

		//����������� ��� ������ � ArmorySocket, � ��� ���������� � Equip
		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);	//��������� �� ������, ��� ���������, �����
	}
}

	//������������ ������ ������ � ������
	void UShootWeaponComponent::AttachWeaponToSocket(AShootBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)//��������� �� ������, ��� ���������, �����
{
	//�������� ��� ��� ����
	if (!Weapon || !SceneComponent) return;

	//������� ��� ��������� � ������������ ��������� � ����
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);//1 ������������� ������������� ��������������� ���������� � ���� �� ������ ������� 2 ��������� ��� ���
	//CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);	//������������ ��� � ������ ���������.1 ��������� �� ��������� � �������� ����� �������������� 3 �������� ������ GetMesh - ���������� ��������� �� Skeleton mesh
	//CurrentWeapon->SetOwner(Character);//���������� ������ �������� ��� �� �����, ��� � �������� ���������� ��� ������ character �������� GetOwner()
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

	void UShootWeaponComponent::StartFire()	//��� ������ ���������� ������� Fire �� CurrentWeapon
	{
		//�������� ��� ����� ��������
		if (!CanFire()) return;	//���� ������� ������������

		CurrentWeapon->StartFire();
	}

	void UShootWeaponComponent::StopFire()	//��� ������ ���������� ������� Fire �� CurrentWeapon
	{
		if (!CurrentWeapon) return;	//���� ������� ������������

		CurrentWeapon->StopFire();
	}

	//����� ����������� ��������� ���������� ������� � ���� ����������� ���������
	void UShootWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
	{
		//��������� �������� ������ �� ����
		CurrentWeapon = nullptr;

		//�� ������� ������ ����������� ������ �� ���� � ���������� ���
		for (auto Weapon : Weapons)
		{
			Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform); //KeepWorldTransform - ����������� ���������
			Weapon->Destroy();
		}
		
		//������� ������ ������ 
		Weapons.Empty();

		//���������� ������ ��� EndPlay
		Super::EndPlay(EndPlayReason);
	}

	//������������ ������ � ���� �������� ������ � �������� ������ ����� ����� ������� ������, ������� ������� ����� ������ ��� ������ �� Begin Play
	void UShootWeaponComponent::EquipWeapon(int32 WeaponIndex)
	{
		//��������, ��� ������ ����������
		if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
		{
			UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
			return;
		}

		//��������� �� ���������
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		
		//�������� ��� �� ����
		if (!Character) return;

		//������� ������ ������� �� �����, ��� �� ��� ����� ��������� �� ������ �� ����� ���� ������ ���
		if (CurrentWeapon)
		{
			//������� ��� �������� ��� ����� ������
			CurrentWeapon->StopFire();

			//������������ ������ � �����
			AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
		}

		//����������� ��������� ������� ������� �������
		CurrentWeapon = Weapons[WeaponIndex];

		//������ �������� ����������� 
		// CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage; //�� ������� �������� ��������� �� ������� WeaponData � ������ �������� �������� �����������
		const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) {  //������ ��� ���� ��������� �� ������� WeaponData ������� ������������� �������� ������ ������ � ������ �������� �������� �����������. FindByPredicate - ���������� ������ ��� ����. ��������� ������� ����� ����������� � ������� �������� ������� � ���� ����� ��������� �������� ��������� ������� �� ������ ��������� �� ����
			return Data.WeaponClass == CurrentWeapon->GetClass();                                 //CurrentWeaponData - ��������� ���������� ��������� �� ��������� ��������� ( �������� ��� ����� ������� )
			});

		//�������� ��� �� ������� ���������
		CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

		//������ CurrentWeapon ��������� �� ����� ������� �� ����� ������������ � �������� ������� ������ � ������������ ��� � ���� ���������
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

		//����� �������� ������ �� ������ ����������, ��� ���� ��� �� ������� ����� ������
		EquipAnimInProgress = true;

		//����������� �������� ����� ������
		PlayAnimMontage(EquipAnimMontage);
	}

	//��� ����� ������, �������� ������ ������ �� 1 � ��� �� �� �������� ��������� �� ������ �������� ����� �� ������ ����� �������
	void UShootWeaponComponent::NextWeapon()
	{
		//�������� ��� EquipAnimInProgress �� false, �� ����� ������ ����
		if (!CanEquip()) return;
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();	//Weapons.Num() - ���������� ��������� �������

		//������������ ������ � ���� � ���������� �������� ������
		EquipWeapon(CurrentWeaponIndex);
	}

	//������������ �������� �����������
	void UShootWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
	{
		//�������� ��������� �� ��������� � � ���� ����������� ��������
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		if (!Character) return;

		//�������� �� ���� PlayAnimMontage ��� ����� � �������

		Character->PlayAnimMontage(Animation);
	}

	//����� ����� �������� �����������
	void UShootWeaponComponent::InitAnimations()
	{
		/*if (!EquipAnimMontage) return;*/
		/*auto EquipFinishedNotify = FindNotifyByClass<UShootEquipFinishedAnimNotify>(EquipAnimMontage);*/
		auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UShootEquipFinishedAnimNotify>(EquipAnimMontage); //�������� ��������� �� EquipFinishedNotify, �������� ��������� �� EquipAnimMontage�. UShootEquipFinishedAnimNotify ����� ������������� notify
		
		//�������� ���� ����� ������ notify, �� ����������� ��� ������
		if (EquipFinishedNotify)
		{
			//�������� ������ � �������� Notifies F12
			//���� �� ������� ������� ��� ������� OnNotified � ������ ����� AddUObject ���� ������� OnEquipFinished
			EquipFinishedNotify->OnNotified.AddUObject(this, &UShootWeaponComponent::OnEquipFinished);
		}
		else
		{
			//�� ��������� � �������� notify 
			UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
			checkNoEntry();
		}
		/*const auto NotifyEvents = EquipAnimMontage->Notifies;
		for (auto NotifyEvent : NotifyEvents)*/

		//������ ������ � ������� ���� �������� ����������� ������
		for (auto OneWeaponData : WeaponData)
		{
		/*	auto EquipFinishedNotify = Cast<UShootEquipFinishedAnimNotify>(NotifyEvent.Notify);	//����������� � ������ ���� � �������� ��������� �� EquipFinishedNotify
			if (EquipFinishedNotify) ���� �������, �� ���� Notifies ��� � �����
			{
				EquipFinishedNotify->OnNotified.AddUObject(this, &UShootWeaponComponent::OnEquipFinished);
				break;
			}*/
			/*auto ReloadFinishedNotify = FindNotifyByClass<UShootReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
			if (!ReloadFinishedNotify) continue;*/
			//������ �� ��� �������� ����������� Notify ����� ������
			auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UShootReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);	//����� ����� Notify ���� UShootReloadFinishedAnimNotify, �������� �� ������� ��������� OneWeaponData.ReloadAnimMontage
			
			//���� �� ����� notify ��� ������� ��������
			if (!ReloadFinishedNotify)
			{
				//�� ��������� � �������� notify 
				UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
				checkNoEntry();
			}

			//���� ����� ������ ������� ��� �����������
			ReloadFinishedNotify->OnNotified.AddUObject(this, &UShootWeaponComponent::OnReloadFinished);
		}
	}

	//���� �������� �����������
	void UShootWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
	{
		//�������� ��������� �� ���������
		ACharacter* Character = Cast<ACharacter>(GetOwner());

		//�������� ���� ��������� � �����, ��� ������ � �������, ���� ���� �� ����� �������
		/*if (!Character) return;*/
		if (!Character || MeshComp != Character->GetMesh()) return;

		/*if (MeshComp == Character->GetMesh())
		{
			UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
		}*/
		EquipAnimInProgress = false;	//����� ������������� �������� ���� ���� �� ������� ����� ������
	}

	//�������� ��� ����� ��������
	bool UShootWeaponComponent::CanFire() const
	{
		/*return CurrentWeapon && !EquipAnimInProgress;*/
		return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress; //������ �������� ���� ��� ������, �������� �������� ����������, �����������
	}

	//���������� ��������� ���������� EquipAnimInProgress, ��� ���� ���� ������� ����� ������
	bool UShootWeaponComponent::CanEquip() const
	{
		/*return !EquipAnimInProgress;*/
		return !EquipAnimInProgress && !ReloadAnimInProgress;
	}

	//����������� ���� ������������ ��������
	void UShootWeaponComponent::Reload()
	{
		//����� �� �����������
		ChangeClip();
		/*if (!CanReload()) return;
		ReloadAnimInProgress = true;
		PlayAnimMontage(CurrentReloadAnimMontage);*/
	}

	//������ ��� �����������
	void UShootWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
	{
		//�������� ��������� �� ���������
		ACharacter* Character = Cast<ACharacter>(GetOwner());

		//�������� ���� ��������� � �����, ��� ������ � �������, ���� ���� �� ����� �������
		/*if (!Character) return;*/
		if (!Character || MeshComp != Character->GetMesh()) return;

		//���������� ����������� �� �����������
		ReloadAnimInProgress = false;
	}

	//����� �� ������ ���� ������������
	bool UShootWeaponComponent::CanReload() const
	{
		/*return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;*/
		return CurrentWeapon             //
			&& !EquipAnimInProgress   //
			&& !ReloadAnimInProgress  //
			&& CurrentWeapon->CanReload();
	}

	//���������� �� ������, ��� ������ ����� � ���������� �����������
	void UShootWeaponComponent::OnClipEmpty(AShootBaseWeapon* AmmoWeaponEpty)
	{
		//�������� �� ������� ���������
		if (!AmmoWeaponEpty) return;

		//�������� ���� ��������� �� ������� ������ ���������  AmmoWeaponEpty, �� ��� �������� ����������� 
		if (CurrentWeapon == AmmoWeaponEpty)
		{
			ChangeClip();
		}
		//���� ������� ������ ������
		else
		{
			//�������� �� ����� ������� ������
			for (const auto Weapon : Weapons)
			{
				//���� ����� ������ �������
				if (Weapon == AmmoWeaponEpty)
				{
					//�������� ����������� ������� ������. �������� ������� ������������� �� �����, �� ������ ����������� ���������
					Weapon->ChangeClip();
				}
			}
		}
		//��������� ����, �.�. ������� ��� ChangeClip();
	}

	//����� �� ����������� � � ������
	void UShootWeaponComponent::ChangeClip()
	{
		//�������� ���� �� �� ����� ��������������, �� ������� �� �������
		if (!CanReload()) return;

		//���� ���� �������� ��� �����������
		CurrentWeapon->StopFire();

		//�������� ��� ����� �����������
		CurrentWeapon->ChangeClip();

		//���� ���� ��� �����������. ���������� ����������
		ReloadAnimInProgress = true;

		//������������ �������� �����������
		PlayAnimMontage(CurrentReloadAnimMontage);
	}

	//�������� ������ ��� �������� ������
	//������� ������� ������� ���������� ������ ������ � ������� �������� ������. ��������� �������� �� ������ � �������� ��������� �������
	bool UShootWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
	{
		if (CurrentWeapon)
		{
			UIData = CurrentWeapon->GetUIData();
			return true;
		}
		return false;

		/*������� �� ������
		//����� ������ � ���, ��� ���� ��������� �� ���� ������� ������
		if (CurrentWeapon)
		{
			//�������� ������� GetUIData
		}
		//����� ���������� ������ ���������
		*/
	}

	//��������� �������� ��������
	bool UShootWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
	{
		if (CurrentWeapon)
		{
			AmmoData = CurrentWeapon->GetAmmoData();
			return true;
		}
		return false;
	}

	//��������� ���������� �������� �� Puckups
	bool UShootWeaponComponent::TryToAddAmmo(TSubclassOf<AShootBaseWeapon> WeaponType, int32 ClipsAmount)
	{
		//�������� �� ������� ������ � ���������� ���������� �� ����� ��� ������
		for (const auto Weapon : Weapons)
		{
			//�������� �� ���� � �������� ����������� �� ������ � ����
			if (Weapon && Weapon->IsA(WeaponType))	//IsA(��� �����������) � ���������� ��� ���� ������� ������ ����������� ������� ���� - UObject �������
			{

				return Weapon->TryToAddAmmo(ClipsAmount); //TryToAddAmmo ��������� � ������ �������� ������
			}
		}

		return false;
	}
