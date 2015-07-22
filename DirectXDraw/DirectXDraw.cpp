/*!
@class   DirectXDraw DirectXDraw.h "DirectXDraw.h"
@brief   _MikanDraw�N���X�BDirectX��p�����`����s���B
@author  Hiroki
@version 1.0
*/
#include "DirectXDraw.h"

// �e�N�X�`���`��̏I��
int DirectXDraw9::EndDrawTexture( IDirect3DTexture9 *tex )
{
	hr = D3DDev->SetTexture( 0, tex );

#ifdef _DEBUG
	if ( FAILED( hr ) )
	{
		//�e�N�X�`���Z�b�g�Ɏ��s
		return 3;
	}
#endif

	//  D3DDev->SetTextureStageState( 0, D3DTSS_COLORARG2,D3DTA_DIFFUSE );
	/*  if( FAILED( hr ) )
	  {
	  //�e�N�X�`���ݒ�Ɏ��s
	  return 4;
	  }*/

	D3DDev->SetFVF( MIKAN_CUSTOMVERTEX_2D/*D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1*/ );

#ifdef _DEBUG
	if ( FAILED( hr ) )
	{
		//���_�X�g���[���̐ݒ�Ɏ��s
		return 5;
	}
#endif

	if ( FAILED( D3DDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, _cv, sizeof( CUSTOMVERTEX ) ) ) )
	{
		return -1;
	}

	return 0;
}

/*!
@brief �e�N�X�`���̕`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͍���B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int DirectXDraw9::DrawTexture( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTexture( (float)dx, (float)dy, (float)w, (float)h );

	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief �e�N�X�`���̕`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int DirectXDraw9::DrawTextureC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureC( (float)dx, (float)dy, (float)w, (float)h );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief �e�N�X�`���̊g��k���`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͍���B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param scale �g��k�����B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int DirectXDraw9::DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureScaling( (float)dx, (float)dy, (float)( scale * w ), (float)( scale * h ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief �e�N�X�`���̊g��k���`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͍���B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param dw �`�扡���B
@param dh �`�捂���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int DirectXDraw9::DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureScaling( (float)dx, (float)dy, (float)dw, (float)dh );
	return EndDrawTexture( dxtex[ txnum ].tex );
}


/*!
@brief �e�N�X�`���̊g��k���`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param scale �g��k�����B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int DirectXDraw9::DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureScalingC( (float)dx, (float)dy, (float)( w * scale ), (float)( h * scale ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief �e�N�X�`���̊g��k���`��
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param dw �`�扡���B
@param dh �`�捂���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int DirectXDraw9::DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureScalingC( (float)dx, (float)dy, (float)dw, (float)dh );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief �e�N�X�`������]�`�悷��(���W�A��)
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param rad ���W�A���B���v���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int DirectXDraw9::DrawTextureRotationC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotationC( (float)dx, (float)dy, (float)w, (float)h, rad );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

/*!
@brief �e�N�X�`������]�`�悷��(�p�x)
@par   �֐�����
�e�N�X�`����`�悷��B�n�_�͒����B
@param txnum �e�N�X�`���ԍ��B
@param dx �`��x���W�B
@param dy �`��y���W�B
@param rx �ǂݍ���x���W�B
@param ry �ǂݍ���y���W�B
@param w �ǂݍ��ݕ��B
@param h �ǂݍ��ݍ����B
@param angle �p�x�B���v���B
@return 0=�����B
@return 1=�e�N�X�`���ԍ����s���B
@return 2=�e�N�X�`�����Ȃ��B
@return 3=�e�N�X�`���Z�b�g�Ɏ��s�B
@return 4=�e�N�X�`���ݒ�Ɏ��s�B
@return 5=���_�X�g���[���̐ݒ�Ɏ��s�B
@return 6=�����_�����O�Ɏ��s�B
*/
int DirectXDraw9::DrawTextureRotationAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int flag )
{
	if ( txnum >= texmax )//if( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotationC( (float)dx, (float)dy, (float)w, (float)h, AngleToRadian( angle ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

// �e�N�X�`������]�g��`�悷��
int DirectXDraw9::DrawTextureRotateScale( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, double scale, double rad, int drawflag )
{
	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, drawflag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)( cx * scale ), (float)( cy * scale ), (float)dx, (float)dy, rad, (float)( w * scale ), (float)( h * scale ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

// �e�N�X�`������]�g��`�悷��
int DirectXDraw9::DrawTextureRotateScale( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int cx, int cy, int dw, int dh, double rad, int drawflag )
{
	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, drawflag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)cx, (float)cy, (float)dx, (float)dy, rad, (float)dw, (float)dh );
	return EndDrawTexture( dxtex[ txnum ].tex );
}


//�e�N�X�`������]�g��`�悷��
int DirectXDraw9::DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int flag )
{
	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)( w * scale * 0.5 ), (float)( h * scale * 0.5 ), (float)dx, (float)dy, rad, (float)( w * scale ), (float)( h * scale ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//�e�N�X�`������]�g��`�悷��
int DirectXDraw9::DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int flag )
{
	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)( w * scale * 0.5 ), (float)( h * scale * 0.5 ), (float)dx, (float)dy, AngleToRadian( angle ), (float)( w * scale ), (float)( h * scale ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//�e�N�X�`������]�g��`�悷��
int DirectXDraw9::DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int flag )
{
	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)( sw * 0.5 ), (float)( sh * 0.5 ), (float)( dx ), (float)( dy ), rad, (float)( sw ), (float)( sh ) );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//�e�N�X�`������]�g��`�悷��
int DirectXDraw9::DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int flag )
{
	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureRotateScale( (float)( sw * 0.5 ), (float)( sh * 0.5 ), (float)dx, (float)dy, AngleToRadian( angle ), (float)sw, (float)sh );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//�e�N�X�`����4�_�����R�Ɍ��߂ĕ`�悷��
int DirectXDraw9::DrawTextureVertex( unsigned int txnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int flag )
{
	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureVertex( (float)x0, (float)y0, (float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3 );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//�e�N�X�`����4�_�𑊑΍��W�Ŏ��R�Ɍ��߂ĕ`�悷��
int DirectXDraw9::DrawTextureRelativeVertex( unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3, int flag )
{
	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, flag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureVertex( (float)x0, (float)y0, (float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3 );
	return EndDrawTexture( dxtex[ txnum ].tex );
}

//�e�N�X�`����4�_�𑊑΍��W�Ŏ��R�Ɍ��߂ĕ`�悷��(�e�N�X�`���`�F�b�N�Ȃ�:�����p)
int DirectXDraw9::DrawTextureRelativeVertex( unsigned int txnum, int rx, int ry, int w, int h, double x0, double y0, double z0, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, int drawflag )
{
	// TODO: Z���W�B

	if ( txnum >= texmax )
	{
		//�e�N�X�`���ԍ����s��
		return 1;
	}

	_SetUV( (float)dxtex[ txnum ].width, (float)dxtex[ txnum ].height, rx, ry, w, h, drawflag );
	_SetColor( dxtex[ txnum ].vcolor );
	_DrawTextureVertex( (float)x0, (float)y0, (float)x1, (float)y1, (float)x2, (float)y2, (float)x3, (float)y3 );
	return EndDrawTexture( dxtex[ txnum ].tex );
}
