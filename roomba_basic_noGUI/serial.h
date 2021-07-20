//�V���A���ʐM�\����class serial
//serial.cpp,h
//modified 060530
//windows/linux�N���X�v���b�g�t�H�[��
//---------------------------------------------------------------------------
#ifndef serialH
#define serialH
//---------------------------------------------------------------------------

class serial
{
public:
    char flag_opened;//com�|�[�g���J����Ă��邩�ǂ���
    char comport[16];//com�|�[�g��
    int baudrate;//�{�[���[�g�������ɏo��

    bool init(char *comport_in,int baudrate);
    bool close(void);
    void purge(void);//WinAPI��PurgeComm�����s����
    int receive(char *buf_ptr,int size);//�󂯎��o�b�t�@�̏ꏊ�ƃT�C�Y
    int send(char *buf_ptr,int size);//����o�b�t�@�̏ꏊ�ƃT�C�Y
    bool receive2(char *buf_ptr,int size);//ACK���󂯎��֐�
    unsigned char receive3(char *buf_ptr,int size);//�u���b�N�Ȃ��Ŏ�M�D�o�C�g����Ԃ�

};
//---------------------------------------------------------------------------
#endif