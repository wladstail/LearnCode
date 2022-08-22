// Shoot Game


#include "Animations/ShootAnimNotify.h"

//перегружаем функцию
void UShootAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    //вызов делегата
    OnNotified.Broadcast(MeshComp); //оповещение будет только у того чей меш

    //вызов функции
    Super::Notify(MeshComp, Animation);
}

