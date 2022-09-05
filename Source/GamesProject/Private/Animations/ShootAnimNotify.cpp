// Shoot Game


#include "Animations/ShootAnimNotify.h"

//����������� �������
void UShootAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    //����� ��������
    OnNotified.Broadcast(MeshComp); //���������� ����� ������ � ���� ��� ���

    //����� �������
    Super::Notify(MeshComp, Animation);
}

