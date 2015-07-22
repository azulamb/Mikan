#include "DirectXDraw.h"

/*!
@brief �t�H���g�̏�����
@par   �֐�����
�t�H���g�̏��������s���B0�Ԃ̃t�H���g�������ō��B
@return 0��Ԃ��B
*/
int DirectXDraw9::FontInit( void )
{
	CreateFont( 0, DEFAULT_FONT, 10, 0xffffffff, 0, 0 );
	D3DDev->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_NONE );
	D3DDev->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_NONE );
	return 0;
}

int DirectXDraw9::LoadFontFile( const char *fontfile )
{
	AddFontResourceEx( fontfile, FR_PRIVATE, NULL );
	//RemoveFontResourceEx�����s����K�v����B
	return 0;
}

int DirectXDraw9::LoadFontFile( HMODULE module, const char *resource )
{
	void *font;
	HRSRC hres;
	unsigned long count;

	hres = FindResource( module, resource, "FONTFILE" );
	font = LockResource( LoadResource( NULL, hres ) );

	if ( AddFontMemResourceEx( font, SizeofResource( NULL, hres ), NULL, &count ) == NULL )
	{
		return 1;
	}

	return 0;
}

int DirectXDraw9::CreateFont( unsigned int fontnum, const char *fontname, int size, unsigned long color, int weight, int italic )
{
	int length;
	if ( fontnum >= fontmax )
	{
		// �t�H���g�ԍ����s��
		return 1;
	}

	fontdata[ fontnum ].height = size;
	fontdata[ fontnum ].width = 0;
	fontdata[ fontnum ].weight = weight;
	fontdata[ fontnum ].miplevel = 0;
	fontdata[ fontnum ].italic = italic ? true : false;
	fontdata[ fontnum ].charset = SHIFTJIS_CHARSET;
	fontdata[ fontnum ].outputprecision = OUT_DEFAULT_PRECIS;//0;//OUT_TT_PRECIS;
	fontdata[ fontnum ].quality = DEFAULT_QUALITY;//PROOF_QUALITY;//0;
	fontdata[ fontnum ].pitchandfamily = DEFAULT_PITCH | FF_DONTCARE;//FIXED_PITCH | FF_SCRIPT;//0;
	fontdata[ fontnum ].color = color;
	length = strlen( fontname );
	strcpy_s( fontdata[ fontnum ].fontname, length + 1, fontname );

	D3DXCreateFont( D3DDev,
		fontdata[ fontnum ].height,
		fontdata[ fontnum ].width,
		fontdata[ fontnum ].weight,
		fontdata[ fontnum ].miplevel,
		fontdata[ fontnum ].italic,
		fontdata[ fontnum ].charset,
		fontdata[ fontnum ].outputprecision,
		fontdata[ fontnum ].quality,
		fontdata[ fontnum ].pitchandfamily,
		fontdata[ fontnum ].fontname,
		&( fontdata[ fontnum ].D3DFont ) );

	fontdata[ fontnum ].use = 1;

	return 0;
}


/*!
@brief �t�H���g�����B
@par   �֐�����
�t�H���g�����B
@param fontnum �t�H���g�ԍ��B
@param fontname �t�H���g���B
@param size �t�H���g�T�C�Y�B�ȗ������ꍇ10�B
@param color �t�H���g�̐F�B�ȗ������ꍇ0xFFFFFFF(��)�B
@return 0=�����B
@return 1=�t�H���g�ԍ����s���B
*/
int DirectXDraw9::CreateFont( unsigned int fontnum, const char *fontname, int size, unsigned long color )
{
	return CreateFont( fontnum, fontname, size, color, 0, 0 );
}

/*!
@brief �t�H���g�����B
@par   �֐�����
�t�H���g�����B�t�H���g�̓S�V�b�N�́B
@param fontnum �t�H���g�ԍ��B
@param size �t�H���g�T�C�Y�B�ȗ������ꍇ10�B
@param color �t�H���g�̐F�B�ȗ������ꍇ0xFFFFFFF(��)�B
@return 0=�����B
@return 1=�t�H���g�ԍ����s���B
*/
int DirectXDraw9::CreateFont( unsigned int fontnum, int size, unsigned long color )
{
	return CreateFont( fontnum, DEFAULT_FONT, size, color, 0, 0 );
}

/*!
@brief �t�H���g���Đݒ肷��B
@par   �֐�����
�t�H���g���Đݒ肷��B���ł�CreateFont�Ńt�H���g������Ă���K�v������B
@param fontnum �t�H���g�ԍ��B
@return 0=�����B
@return 1=�t�H���g�ԍ����s���B
*/
int DirectXDraw9::ResetFont( unsigned int fontnum )
{
	//Recover Font
	if ( fontnum < fontmax && fontdata[ fontnum ].use )
	{
		D3DXCreateFont( D3DDev,
			fontdata[ fontnum ].height,
			fontdata[ fontnum ].width,
			fontdata[ fontnum ].weight,
			fontdata[ fontnum ].miplevel,
			fontdata[ fontnum ].italic,
			fontdata[ fontnum ].charset,
			fontdata[ fontnum ].outputprecision,
			fontdata[ fontnum ].quality,
			fontdata[ fontnum ].pitchandfamily,
			fontdata[ fontnum ].fontname,
			&( fontdata[ fontnum ].D3DFont ) );
		return 0;
	}
	//�t�H���g�ԍ����s��
	return 1;
}

int DirectXDraw9::CleanupFont( void )
{
	DX_FREE( fontdata );
	return 0;
}

/*!
@brief �t�H���g���������B
@par   �֐�����
���������t�H���g���������B
@return 0��Ԃ��B
*/
int DirectXDraw9::FontCleanUp( void )
{
	unsigned int f;

	for ( f = 0; f < fontmax; ++f )
	{
		DX_RELEASE( fontdata[ f ].D3DFont );
	}

	return 0;
}

/*!
@brief �t�H���g���f�o�C�X���X�g���畜�A������B
@par   �֐�����
�f�o�C�X���X�g�Ńt�H���g�ݒ肪�S�ď�����̂ŁA������Đݒ肷��B
@return 0��Ԃ��B
*/
int DirectXDraw9::FontRecover( void )
{
	unsigned int f;

	for ( f = 0; f < fontmax; ++f )
	{
		ResetFont( f );
	}

	return 0;
}

/*!
@brief �������`�悷��B
@par   �֐�����
�����̕������`�悷��B
@param fontnum �t�H���g�ԍ��B
@param x �`�悷��x���W�B
@param y �`�悷��y���W�B
@param str �`�悷�镶����B
@return �t�H���g�ԍ��B�s���ȏꍇ��0���g����B
*/
int DirectXDraw9::Print( unsigned int fontnum, int x, int y, const char *str )
{
	//�������t�H���g��0�ɂ���B
	if ( fontnum >= fontmax || fontdata[ fontnum ].use == 0 )
	{
		fontnum = 0;
	}
	//�����\���̈�̐ݒ�B
	//����E�B���h�E�T�C�Y�Ƃ��ɂ��Ă����ƌ�X���S�B
	fontdata[ fontnum ].rect.left = x;
	fontdata[ fontnum ].rect.top = y;
	fontdata[ fontnum ].rect.right = x + 3000;
	fontdata[ fontnum ].rect.bottom = y + 3000;

	fontdata[ fontnum ].D3DFont->DrawText( NULL, str, -1, &( fontdata[ fontnum ].rect ), DT_NOCLIP, fontdata[ fontnum ].color );

	return fontnum;
}

/*!
@brief �������`�悷��B
@par   �֐�����
�����̕������`�悷��B
@param fontnum �t�H���g�ԍ��B
@param x �`�悷��x���W�B
@param y �`�悷��y���W�B
@param str �`�悷�镶����B
@return �t�H���g�ԍ��B�s���ȏꍇ��0���g����B
*/
int DirectXDraw9::Print( unsigned int fontnum, int x, int y, unsigned long color, const char *str )
{
	//�������t�H���g��0�ɂ���B
	if ( fontnum >= fontmax || fontdata[ fontnum ].use == 0 )
	{
		fontnum = 0;
	}
	//�����\���̈�̐ݒ�B
	//����E�B���h�E�T�C�Y�Ƃ��ɂ��Ă����ƌ�X���S�B
	fontdata[ fontnum ].rect.left = x;
	fontdata[ fontnum ].rect.top = y;
	fontdata[ fontnum ].rect.right = x + 3000;
	fontdata[ fontnum ].rect.bottom = y + 3000;

	fontdata[ fontnum ].D3DFont->DrawText( NULL, str, -1, &( fontdata[ fontnum ].rect ), DT_NOCLIP, color );

	return fontnum;
}

/*!
@brief ������𐮌`�`�悷��B
@par   �֐�����
������𐮌`�`�悷��B�ڂ�����C���ꓙ��printf�Ȃǂ��Q�ƁB
@param fontnum �t�H���g�ԍ��B
@param x �`�悷��x���W�B
@param y �`�悷��y���W�B
@param format ���`�`�悷�镶����B
@param ���̑������ł� ���`������Ő��`���ĕ`�悷��ϐ������B
@return �t�H���g�ԍ��B�s���ȏꍇ��0���g����B
*/
int DirectXDraw9::Printf( unsigned int fontnum, int x, int y, const char *format, ... )
{
	char str[ MAX_BUFFER_SIZE ];
	//va_list���g�����@�����邯�ǁA�ʂɂ���Ŗ��Ȃ���ˁB
	vsnprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, format, (char*)( &format + 1 ) );
	return Print( fontnum, x, y, str );
}

/*!
@brief ������𐮌`�`�悷��B
@par   �֐�����
������𐮌`�`�悷��B�ڂ�����C���ꓙ��printf�Ȃǂ��Q�ƁB
@param fontnum �t�H���g�ԍ��B
@param x �`�悷��x���W�B
@param y �`�悷��y���W�B
@param color
@param format ���`�`�悷�镶����B
@param ���̑������ł� ���`������Ő��`���ĕ`�悷��ϐ������B
@return �t�H���g�ԍ��B�s���ȏꍇ��0���g����B
*/
int DirectXDraw9::Printf( unsigned int fontnum, int x, int y, unsigned long color, const char *format, ... )
{
	char str[ MAX_BUFFER_SIZE ];
	//va_list���g�����@�����邯�ǁA�ʂɂ���Ŗ��Ȃ���ˁB
	vsnprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, format, (char*)( &format + 1 ) );
	return Print( fontnum, x, y, color, str );
}
