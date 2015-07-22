/*
  UV�W�J��`����W�̌v�Z���s���e�N�X�`���`��̊�{�N���X�B
  */

#ifndef CLASSDEF_DIRECTX_DRAW_BASE
#define CLASSDEF_DIRECTX_DRAW_BASE

#define USERHW

//�`��t���O
enum
{
	DRAW_LR = 0x1, //���E���]
	DRAW_UD = 0x2  //�㉺���]
};

/*!
@struct  CUSTOMVERTEX
@brief   ���_�̍\���́B
@author  Hiroki
@version 1.0
*/
struct CUSTOMVERTEX
{
	float         x, y, z; // ���_���W
#ifdef USERHW
	float         rhw;     // ���Z��
#endif
	unsigned long color;   // ���_�̐F
	float         u, v;    // �e�N�X�`�����W
};

class DirectXDrawBase
{
public:
	struct CUSTOMVERTEX _cv[ 4 ];
	// �e�N�X�`������o�^����B
	virtual int _SetUV( float width, float height, int rx, int ry, int w, int h, int drawflag );
	// ���_�F�B
	virtual int _SetColor( unsigned long *colors );
	virtual int _SetColor( unsigned long lt_color, unsigned long rt_color, unsigned long ld_color, unsigned long rd_color );

	// �e�N�X�`����`�悷��
	virtual int _DrawTexture( float dx, float dy, float w, float h );
	// �e�N�X�`����`�悷��
	virtual int _DrawTextureC( float dx, float dy, float w, float h );

	// �e�N�X�`�����g��k���`�悷��
	virtual int _DrawTextureScaling( float dx, float dy, float dw, float dh );
	// �e�N�X�`�����g��k���`�悷��
	virtual int _DrawTextureScalingC( float dx, float dy, float dw, float dh );

	// �e�N�X�`������]�`�悷��
	virtual int _DrawTextureRotationC( float dx, float dy, float dw, float dh, double rad );

	// �e�N�X�`������]�g��`�悷��
	virtual int _DrawTextureRotateScale( float cx, float cy, float dx, float dy, double rad, float dw, float dh );

	// �e�N�X�`����4�_�����R�Ɍ��߂ĕ`�悷��
	virtual int _DrawTextureVertex( float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3 );

};

#endif
