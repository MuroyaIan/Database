
// �C���N���[�h��
#include <Sample_GameMode.h>

// �N���X����
void ASample_GameMode::StartPlay()
{
    Super::StartPlay();

    // �o�̓e�X�g
    check(GEngine != nullptr);

    // 5 �b�ԃf�o�b�O���b�Z�[�W��\�����܂��B
    // ����-1�� "Key "�l�ɂ���āA���b�Z�[�W�̍X�V�⃊�t���b�V�����ł��Ȃ��Ȃ�܂��B
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, L"Hello World, this is Sample_GameMode!");
}
