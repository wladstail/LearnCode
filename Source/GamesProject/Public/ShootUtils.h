#pragma once



//так как GetWeaponComponent() и GetHealthComponent() почти совпадают, только различия в типе компонента который хотим получить, то делаем шаблонную функцию

//возвращает указатель на конкретный тип компонента pawn, если он имеется.
class ShootUtils
{
public:
	template < typename T>
	static T* GetShootPlayerComponent(APawn* PlayerPawn) //возвращает указатель на тип Т. Сделали параметр указатель на Pawn, что бы не зависела от того как мы получаем Pawn
	{
		//указатель на нашего игрока
		//убрали так как указатель на Pawn теперь в параметре const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - возвращает указатель на pawn

		//проверка если он нулевой
		if (!PlayerPawn) return nullptr;

		//получаем указатель на наш компонент
		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());	//GetComponentByClass(внутри subclass КОМПОНЕНТа которого хотим найти)
		//const auto Component = Player->GetComponentByClass(UShootHealtsComponent::StaticClass());	//GetComponentByClass(внутри subclass КОМПОНЕНТа которого хотим найти)

		//приводим указатель на компонент к компоненту здоровья
		/*можно переписать
		const auto HealthComponent = Cast <T>(Component);	//UShootHealtsComponent - тип к которому привели. cast проверяет и на нуль
		return HealthComponent;*/

		return Cast <T>(Component);;

	}

};

