#pragma once



//��� ��� GetWeaponComponent() � GetHealthComponent() ����� ���������, ������ �������� � ���� ���������� ������� ����� ��������, �� ������ ��������� �������

//���������� ��������� �� ���������� ��� ���������� pawn, ���� �� �������.
class ShootUtils
{
public:
	template < typename T>
	static T* GetShootPlayerComponent(APawn* PlayerPawn) //���������� ��������� �� ��� �. ������� �������� ��������� �� Pawn, ��� �� �� �������� �� ���� ��� �� �������� Pawn
	{
		//��������� �� ������ ������
		//������ ��� ��� ��������� �� Pawn ������ � ��������� const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - ���������� ��������� �� pawn

		//�������� ���� �� �������
		if (!PlayerPawn) return nullptr;

		//�������� ��������� �� ��� ���������
		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());	//GetComponentByClass(������ subclass ���������� �������� ����� �����)
		//const auto Component = Player->GetComponentByClass(UShootHealtsComponent::StaticClass());	//GetComponentByClass(������ subclass ���������� �������� ����� �����)

		//�������� ��������� �� ��������� � ���������� ��������
		/*����� ����������
		const auto HealthComponent = Cast <T>(Component);	//UShootHealtsComponent - ��� � �������� �������. cast ��������� � �� ����
		return HealthComponent;*/

		return Cast <T>(Component);;

	}

};

