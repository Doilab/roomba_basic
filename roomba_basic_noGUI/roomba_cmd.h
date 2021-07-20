//Roomba�Ƃ̒ʐM�R�}���h��`
//�����Ȃ��ɂ��邽�߂ɂ�U������

#define RB_OFF
#define RB_PASSIVE
#define RB_SAFE 131U
#define RB_FULL 132U

#define RB_START 128U
#define RB_RESET 7U
#define RB_STOP 173U

#define RB_POWER 133U //�p���[�_�E��
#define RB_DRIVE 137U //���x�C���񔼌a�w��쓮�D4�o�C�g�f�[�^�K�v
#define RB_DRIVE_PWM 146U //4�o�C�g�f�[�^�K�v
#define RB_MOTORS 138U //�u���V�E�z���Ȃǂ̕����w��D1�o�C�g�f�[�^�K�v
#define RB_PWM_MOTORS 144U //�u���V�E�z���Ȃǂ̋����w��D1�o�C�g�f�[�^�K�v

#define RB_SENSORS 142U //�Z���T�f�[�^�v���D1�o�C�g�f�[�^�K�v

#define RB_LIGHT_BUMPER 45U //���o���p�[�������������ǂ���
#define RB_LIGHT_BUMPER_L_SIG 46U //���o���p�[��
#define RB_LIGHT_BUMPER_FL_SIG 47U //���o���p�[���O
#define RB_LIGHT_BUMPER_CL_SIG 48U //���o���p�[����
#define RB_LIGHT_BUMPER_CR_SIG 49U //���o���p�[�E��
#define RB_LIGHT_BUMPER_FR_SIG 50U //���o���p�[�E�O
#define RB_LIGHT_BUMPER_R_SIG 51U //���o���p�[�E

#define RB_LEFT_ENC 43U //���G���R�[�_�J�E���g
#define RB_RIGHT_ENC 44U //�E�G���R�[�_�J�E���g

#define RB_SONG 140U //�����f�B�L���D
#define RB_PLAY 141U //�����f�B�Đ��D1�o�C�g�f�[�^�K�v

#define RB_OI_MODE 35U //�����o�̃��[�h��Ԃ�

#define RB_SEEK_DOCK 143U //�h�b�N��T��

#define RB_LEDS 139U //LED����

//---------------------------
//�R�}���h�Z�b�g�֐�
//�z��ɃR�}���h���Z�b�g����
//---------------------------

//--------------------------
int joint_high_low_byte(int hbyte, int lbyte)
{
	//��ʃo�C�g�Ɖ��ʃo�C�g�̌���
	int hb,lb,val;
	hb=(0x00ff&hbyte);
	val=(hbyte<<8);
	val|=(0x00ff&lbyte);
    //printf("H[0x%x]:L[0x%x]  -- val[%d]\n", hbyte, lbyte, val);
	return val;
}
//---------------------------------------
int set_songA_command(char *buf_out, int song)
{
    //�����f�B�[�o�b�t�@�ɃZ�b�g
    //�Ԃ�l�o�C�g��
	int byte;

	buf_out[0]=RB_SONG;
	buf_out[1]=(unsigned char)song;//1�o�C�g���Z�b�g�Dusigned char�Ɍ^�Z�b�g
	int length=11;//�Ȃ̒���
	buf_out[2]=length;//�Ȃ̒���

	buf_out[3]=90;//����
	buf_out[4]=16;//���̒���
	buf_out[5]=86;//����
	buf_out[6]=16;//���̒���
	buf_out[7]=81;//����
	buf_out[8]=16;//���̒���
	buf_out[9]=86;//����
	buf_out[10]=16;//���̒���

	buf_out[11]=88;//����
	buf_out[12]=16;//���̒���
	buf_out[13]=93;//����
	buf_out[14]=48;//���̒���

	buf_out[15]=88;//����
	buf_out[16]=16;//���̒���
	buf_out[17]=90;//����
	buf_out[18]=16;//���̒���
	buf_out[19]=88;//����
	buf_out[20]=16;//���̒���
	buf_out[21]=81;//����
	buf_out[22]=16;//���̒���
	buf_out[23]=86;//����
	buf_out[24]=48;//���̒���

	byte=3+length*2;
	//rb_s1.send(buf_out,byte);//�R�}���h���M

	return byte;

}

//--------------------------------------------------
int set_songB_command(char *buf_out, int song)
{
    //�����f�B�[�o�b�t�@�ɃZ�b�g
    //�Ԃ�l�o�C�g��
	int byte;

	buf_out[0]=RB_SONG;
	buf_out[1]=(unsigned char)song;//1�o�C�g���Z�b�g�Dusigned char�Ɍ^�Z�b�g
	int length=17;//�Ȃ̒���
	buf_out[2]=length;//�Ȃ̒���
	int L=24;//1���̒���


	buf_out[3]=64;//����
	buf_out[4]=L*3;//���̒���
	buf_out[5]=67;//����
	buf_out[6]=L*2;//���̒���
	buf_out[7]=69;//����
	buf_out[8]=L*1;//���̒���
	buf_out[9]=69;//����
	buf_out[10]=L*6;//���̒���

	buf_out[11]=65;//����
	buf_out[12]=L*3;//���̒���
	buf_out[13]=69;//����
	buf_out[14]=L*2;//���̒���
	buf_out[15]=71;//����
	buf_out[16]=L*1;//���̒���
	buf_out[17]=71;//����
	buf_out[18]=L*6;//���̒���

	buf_out[19]=74;//����
	buf_out[20]=L*3;//���̒���
	buf_out[21]=72;//����
	buf_out[22]=L*2;//���̒���
	buf_out[23]=64;//����
	buf_out[24]=L*1;//���̒���

	buf_out[25]=67;//����
	buf_out[26]=L*3;//���̒���
	buf_out[27]=65;//����
	buf_out[28]=L*2;//���̒���
	buf_out[29]=64;//����
	buf_out[30]=L*1;//���̒���

	buf_out[31]=64;//����
	buf_out[32]=L*3;//���̒���
	buf_out[33]=62;//����
	buf_out[34]=L*3;//���̒���
	buf_out[35]=60;//����
	buf_out[36]=L*6;//���̒���

	byte=3+length*2;
	//rb_s1.send(buf_out,byte);//�R�}���h���M

	return byte;

}

//--------------------------------------------------
int set_christmas_song_command(char *buf_out, int song)
{
    //�����f�B�[�o�b�t�@�ɃZ�b�g
    //�Ԃ�l�o�C�g��
	int byte;

	buf_out[0]=RB_SONG;
	buf_out[1]=(unsigned char)song;//1�o�C�g���Z�b�g�Dusigned char�Ɍ^�Z�b�g
	int length=14;//�Ȃ̒���
	buf_out[2]=length;//�Ȃ̒���

	int L=2;//1���̒���
	buf_out[3]=86;//����
	buf_out[4]=32*L;//���̒���
	buf_out[5]=86;//����
	buf_out[6]=16*L;//���̒���
	buf_out[7]=89;//����
	buf_out[8]=24*L;//���̒���
	buf_out[9]=86;//����
	buf_out[10]=8*L;//���̒���
	buf_out[11]=83;//����
	buf_out[12]=16*L;//���̒���

	buf_out[13]=84;//����
	buf_out[14]=48*L;//���̒���
	buf_out[15]=88;//����
	buf_out[16]=48*L;//���̒���

	buf_out[17]=84;//����
	buf_out[18]=16*L;//���̒���
	buf_out[19]=79;//����
	buf_out[20]=16*L;//���̒���
	buf_out[21]=76;//����
	buf_out[22]=16*L;//���̒���
	buf_out[23]=79;//����
	buf_out[24]=24*L;//���̒���
	buf_out[25]=77;//����
	buf_out[26]=8*L;//���̒���
	buf_out[27]=74;//����
	buf_out[28]=16*L;//���̒���

	buf_out[29]=72;//����
	buf_out[30]=96*L;//���̒���


	byte=3+length*2;

	return byte;

}

//--------------------------------------------------
char set_drive_command(char *buf_out, int Lpwm, int Rpwm)
{
	int LHbyte=Lpwm&0xff00;
	LHbyte=(LHbyte>>8);
	int LLbyte=Lpwm&0x00ff;
	int RHbyte=Rpwm&0xff00;
	RHbyte=(RHbyte>>8);
	int RLbyte=Rpwm&0x00ff;

	buf_out[0]=(unsigned char)LHbyte;
	buf_out[1]=(unsigned char)LLbyte;
	buf_out[2]=(unsigned char)RHbyte;
	buf_out[3]=(unsigned char)RLbyte;

	return 1;
}
//--------------------------------------------------
