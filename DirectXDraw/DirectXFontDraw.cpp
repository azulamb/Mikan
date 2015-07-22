#include "DirectXDraw.h"

/*!
@brief フォントの初期化
@par   関数説明
フォントの初期化を行う。0番のフォントをここで作る。
@return 0を返す。
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
	//RemoveFontResourceExを実行する必要あり。
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
		// フォント番号が不正
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
@brief フォントを作る。
@par   関数説明
フォントを作る。
@param fontnum フォント番号。
@param fontname フォント名。
@param size フォントサイズ。省略した場合10。
@param color フォントの色。省略した場合0xFFFFFFF(白)。
@return 0=成功。
@return 1=フォント番号が不正。
*/
int DirectXDraw9::CreateFont( unsigned int fontnum, const char *fontname, int size, unsigned long color )
{
	return CreateFont( fontnum, fontname, size, color, 0, 0 );
}

/*!
@brief フォントを作る。
@par   関数説明
フォントを作る。フォントはゴシック体。
@param fontnum フォント番号。
@param size フォントサイズ。省略した場合10。
@param color フォントの色。省略した場合0xFFFFFFF(白)。
@return 0=成功。
@return 1=フォント番号が不正。
*/
int DirectXDraw9::CreateFont( unsigned int fontnum, int size, unsigned long color )
{
	return CreateFont( fontnum, DEFAULT_FONT, size, color, 0, 0 );
}

/*!
@brief フォントを再設定する。
@par   関数説明
フォントを再設定する。すでにCreateFontでフォントを作っている必要がある。
@param fontnum フォント番号。
@return 0=成功。
@return 1=フォント番号が不正。
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
	//フォント番号が不正
	return 1;
}

int DirectXDraw9::CleanupFont( void )
{
	DX_FREE( fontdata );
	return 0;
}

/*!
@brief フォントを解放する。
@par   関数説明
生成したフォントを解放する。
@return 0を返す。
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
@brief フォントをデバイスロストから復帰させる。
@par   関数説明
デバイスロストでフォント設定が全て消えるので、それを再設定する。
@return 0を返す。
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
@brief 文字列を描画する。
@par   関数説明
ただの文字列を描画する。
@param fontnum フォント番号。
@param x 描画するx座標。
@param y 描画するy座標。
@param str 描画する文字列。
@return フォント番号。不正な場合は0が使われる。
*/
int DirectXDraw9::Print( unsigned int fontnum, int x, int y, const char *str )
{
	//未生成フォントは0にする。
	if ( fontnum >= fontmax || fontdata[ fontnum ].use == 0 )
	{
		fontnum = 0;
	}
	//文字表示領域の設定。
	//これウィンドウサイズとかにしておくと後々安全。
	fontdata[ fontnum ].rect.left = x;
	fontdata[ fontnum ].rect.top = y;
	fontdata[ fontnum ].rect.right = x + 3000;
	fontdata[ fontnum ].rect.bottom = y + 3000;

	fontdata[ fontnum ].D3DFont->DrawText( NULL, str, -1, &( fontdata[ fontnum ].rect ), DT_NOCLIP, fontdata[ fontnum ].color );

	return fontnum;
}

/*!
@brief 文字列を描画する。
@par   関数説明
ただの文字列を描画する。
@param fontnum フォント番号。
@param x 描画するx座標。
@param y 描画するy座標。
@param str 描画する文字列。
@return フォント番号。不正な場合は0が使われる。
*/
int DirectXDraw9::Print( unsigned int fontnum, int x, int y, unsigned long color, const char *str )
{
	//未生成フォントは0にする。
	if ( fontnum >= fontmax || fontdata[ fontnum ].use == 0 )
	{
		fontnum = 0;
	}
	//文字表示領域の設定。
	//これウィンドウサイズとかにしておくと後々安全。
	fontdata[ fontnum ].rect.left = x;
	fontdata[ fontnum ].rect.top = y;
	fontdata[ fontnum ].rect.right = x + 3000;
	fontdata[ fontnum ].rect.bottom = y + 3000;

	fontdata[ fontnum ].D3DFont->DrawText( NULL, str, -1, &( fontdata[ fontnum ].rect ), DT_NOCLIP, color );

	return fontnum;
}

/*!
@brief 文字列を整形描画する。
@par   関数説明
文字列を整形描画する。詳しくはC言語等のprintfなどを参照。
@param fontnum フォント番号。
@param x 描画するx座標。
@param y 描画するy座標。
@param format 整形描画する文字列。
@param その他いくつでも 整形文字列で成形して描画する変数たち。
@return フォント番号。不正な場合は0が使われる。
*/
int DirectXDraw9::Printf( unsigned int fontnum, int x, int y, const char *format, ... )
{
	char str[ MAX_BUFFER_SIZE ];
	//va_listを使う方法もあるけど、別にこれで問題ないよね。
	vsnprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, format, (char*)( &format + 1 ) );
	return Print( fontnum, x, y, str );
}

/*!
@brief 文字列を整形描画する。
@par   関数説明
文字列を整形描画する。詳しくはC言語等のprintfなどを参照。
@param fontnum フォント番号。
@param x 描画するx座標。
@param y 描画するy座標。
@param color
@param format 整形描画する文字列。
@param その他いくつでも 整形文字列で成形して描画する変数たち。
@return フォント番号。不正な場合は0が使われる。
*/
int DirectXDraw9::Printf( unsigned int fontnum, int x, int y, unsigned long color, const char *format, ... )
{
	char str[ MAX_BUFFER_SIZE ];
	//va_listを使う方法もあるけど、別にこれで問題ないよね。
	vsnprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, format, (char*)( &format + 1 ) );
	return Print( fontnum, x, y, color, str );
}
