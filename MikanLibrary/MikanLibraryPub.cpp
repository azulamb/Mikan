#include "MikanLibrary.h"
//非公開クラス
extern CLASSDEF_SYSTEM              *_MikanSystem;
extern CLASSDEF_WINDOW              *_MikanWindow;
extern CLASSDEF_DIRECTX_DRAW        *_MikanDraw;
extern CLASSDEF_MIKANMOVIE          *_MikanMovie;
extern CLASSDEF_DIRECTX_INPUT       *_MikanInput;
#ifndef MIKAN_NO_SOUND
extern CLASSDEF_DIRECTX_SOUND       *_MikanSound;
#endif
extern CLASSDEF_NETWORK             *_MikanNet;
extern class FileIOManagement       *_MikanFile;

#define MDRAW_X(x) ( MikanDrawPub::base_x + x )
#define MDRAW_Y(y) ( MikanDrawPub::base_y + y )

int MikanDrawPub::base_x = 0;
int MikanDrawPub::base_y = 0;

/*!
@class   MikanSystemPub
@brief   MikanSystemクラス。ゲームに必要な処理を行う。
@author  Hiroki
@version 1.0
*/

/*!
@brief MikanSystemのコンストラクタ
@par   関数説明
何もしない。
*/
MikanSystemPub::MikanSystemPub( void ){}
/*!
@brief MikanSystemのデストラクタ
@par   関数説明
何もしない。
*/
MikanSystemPub::~MikanSystemPub( void ){}
/*!
@brief FPSを設定する。
@par   関数説明
FPSを設定する。
@param fps 設定するFPS。1秒間に何回実行するか。省略した場合60.0。
@return 設定した値。
*/
double MikanSystemPub::SetFPS( double fps ){
	return _MikanSystem->SetFPS( fps );
}
/*!
@brief FPSを取得する。
@par   関数説明
今のFPSを計測して返す。
@return 現在のFPS。計測した値が出る。ずっと低い値が出た場合は処理落ちを起こしている。
*/
double MikanSystemPub::GetFPS( void ){
	return _MikanSystem->GetFPS() + 0.5;
}
/*!
@brief 次のフレームまで待つ
@par   関数説明
実行すると前の時間などと比較し、どれだけ休めばよいかを計算し、その時間の分だけSleepする。
これで待つことによって、FPS制御を行なっている。
@return Sleepした時間(ミリ秒)。
*/
int MikanSystemPub::WaitNextFrame( int draw ){
	return _MikanSystem->WaitNextFrame( draw );
}
//待ち関数の設定
int MikanSystemPub::SetWaitFunction( int type ){
	return _MikanSystem->SetWaitFunction( type );
}
/*!
@brief デバイスロストを起こしているかどうか調べる
@par   関数説明
今現在デバイスロストを起こしているかどうかを調べる。
ただし、実行中にMainLoop内のプログラムは実行されないため、別手段を用いなければ意味が無い。
@return 0=正常。
@return その他=デバイスロスト中
*/
int MikanSystemPub::IsDeviceLost( void ){
	return _MikanSystem->IsDeviceLost();
}

/*!
@brief 垂直同期待ちの設定
@par   関数説明
垂直同期を待つかどうかの設定。SystemInitより前に実行しないと失敗する。
@param vs 0以外で垂直同期待ちし、0で待たない。-1は特殊な返り値なので使わないことをすすめる。
@return -1 失敗。
@return その他 設定した値が返ってくる。
*/
int MikanSystemPub::SetVsyncWait( int vs )
{
	if ( _MikanSystem->GetMikanPosition() <= MIKAN_POSITION_SYSTEMINIT )
	{
		return _MikanSystem->SetVsyncWait( vs );
	}
	return -1;
}
int MikanSystemPub::SetFullScreen( int fullscreen_on ){
	return _MikanSystem->SetFullScreen( fullscreen_on );
}
/*!
@brief ウィンドウ非アクティブ時の動作設定
@par   関数説明
ウィンドウが非アクティブになったときの挙動について設定する。
@param flag 0で非アクティブ時も動作を続け、それ以外の値で非アクティブ時に動作を止める。
@return 0 非アクティブ時にゲームを動作をさせる。
@return 1 非アクティブ時にゲームの動作を止める。
*/
int MikanSystemPub::SetInactiveWindow( int flag ){
	return _MikanSystem->SetInactiveWindow( flag );
}

//スレッドを立てて関数を実行する
int MikanSystemPub::RunThread( unsigned int tnum, int( *thread_func )( void * vparam ), void *param ){
	return _MikanSystem->RunThread( tnum, thread_func, param );
}

//スレッドを立てて関数を実行する
int MikanSystemPub::RunThread( unsigned int tnum, int( *thread_func )( void ) ){
	return _MikanSystem->RunThread( tnum, thread_func );
}

//立てたスレッドを破棄する
int MikanSystemPub::StopThread( unsigned int tnum ){
	return _MikanSystem->StopThread( tnum );
}
void * MikanSystemPub::GetThreadHandle( unsigned int tnum ){
	return _MikanSystem->GetThreadHandle( tnum );
}
int MikanSystemPub::Join( unsigned int tnum, unsigned long sleep ){
	return _MikanSystem->Join( tnum, sleep );
}
int MikanSystemPub::LockMainLoop( void ){
	return _MikanSystem->LockMainLoop();
}
int MikanSystemPub::UnlockMainLoop( void ){
	return _MikanSystem->UnlockMainLoop();
}
int MikanSystemPub::WaitClearScreen( int value ){
	return _MikanSystem->WaitClearScreen( value );
}
int MikanSystemPub::CreateLock( unsigned int lnum ){
	return _MikanSystem->CreateLock( lnum );
}
unsigned long MikanSystemPub::Lock( unsigned int lnum, unsigned long timeout ){
	return _MikanSystem->Lock( lnum, timeout );
}
int MikanSystemPub::Unlock( unsigned int lnum ){
	return _MikanSystem->Unlock( lnum );
}

/*!
@class   MikanWindowPub
@brief   MikanWindowクラス。ウィンドウ操作などを行う。
@author  Hiroki
@version 1.0
*/

/*!
@brief MikanWindowのコンストラクタ
@par   関数説明
何もしない。
*/
MikanWindowPub::MikanWindowPub(){}
/*!
@brief MikanWindowのデストラクタ
@par   関数説明
何もしない。
*/
MikanWindowPub::~MikanWindowPub(){}
//BOOL MikanWindowPub::SetWindow( int type ){ return _MikanWindow->SetWindow( type ); }
/*!
@brief ウィンドウの名前の取得
@par   関数説明
ウィンドウの名前を返す。
@return 現在のウィンドウ名(char*型)。
*/
char * MikanWindowPub::GetWindowName( void ){
	return _MikanWindow->GetWindowName();
}
/*!
@brief ウィンドウの名前の設定
@par   関数説明
ウィンドウの名前を設定する。反設定映はSetWindow()関数を使う。
@param name TCHAR型文字配列のウィンドウクラス名
@return 0=成功。
@return 1=与えた文字列へのポインタがNULL。
*/
int MikanWindowPub::SetWindowName( char * name )
{
	int ret = _MikanWindow->SetWindowName( name );
	//ウィンドウ設定の有効化
	_MikanWindow->SetWindow();
	return ret;
}
//ウィンドウのアイコン設定
int MikanWindowPub::SetWindowIcon( int iconnum ){
	return _MikanWindow->SetWindowIcon( iconnum, iconnum );
}
int MikanWindowPub::SetWindowIcon( int smalliconnum, int bigiconnum ){
	return _MikanWindow->SetWindowIcon( smalliconnum, bigiconnum );
}
int MikanWindowPub::SetWindowIcon( char *iconname ){
	return _MikanWindow->SetWindowIcon( iconname, iconname );
}
int MikanWindowPub::SetWindowIcon( char *smalliconname, char *bigiconname ){
	return _MikanWindow->SetWindowIcon( smalliconname, bigiconname );
}
/*!
@brief ウィンドウの横幅を取得
@par   関数説明
ウィンドウの横幅を取得する。
@param int type 初期値=0。0で設定値を返し、1で現在値を調べて返す。
@return ウィンドウの横幅。
*/
unsigned int MikanWindowPub::GetWindowWidth( int type ){
	return _MikanWindow->GetWindowWidth( type );
}
/*!
@brief ウィンドウの高さを取得
@par   関数説明
ウィンドウの高さを取得する。
@param int type 初期値=0。0で設定値を返し、1で現在値を調べて返す。
@return ウィンドウの高さ
*/
unsigned int MikanWindowPub::GetWindowHeight( int type ){
	return _MikanWindow->GetWindowHeight( type );
}
/*!
@brief ウィンドウのサイズ設定
@par   関数説明
ウィンドウのサイズを設定する。内部ではSetScreenSizeが使われている。
@param unsigned int width 横幅を与える。
@param unsigned int height 高さを与える。
@return 0=設定値が両方引数と異なる
@return 1=横幅は設定値が異なるが高さの設定値は引数通り
@return 2=横幅は設定値が引数通りだが高さの設定値は異なる
@return 3=設定値が両方引数と同じ
*/
int MikanWindowPub::SetWindowSize( unsigned int width, unsigned int height )
{
	//  int ret = _MikanWindow->SetWindowSize( width, height );
	//スクリーンのサイズからウィンドウサイズを決定する。
	int ret = _MikanWindow->SetScreenSize( width, height );
	//ウィンドウの設定の有効化。
	_MikanWindow->SetWindow();
	//UserInit以降で実行された場合はデバイスロスト復旧処理を強制的に行う。
	//これによりDirectXのデバイスが作り直される。
	if ( _MikanSystem->GetMikanPosition() > MIKAN_POSITION_LIBLARYINIT )
	{
		//デバイスロスト復旧処理を強制的に行う。
		_MikanDraw->RecoverFromDeviceLost( 1 );
	}
	return ret;
}

/*!
@brief 現在のウィンドウのX座標を取得
@par   関数説明
ウィンドウのX座標を取得する。
@param int type 初期値=0。0で設定値を返し、1で現在値を調べて返す。
@return 左からの位置。
*/
int MikanWindowPub::GetPositionX( int type ){
	return _MikanWindow->GetPositionX( type );
}
/*!
@brief 現在のウィンドウのY座標を取得
@par   関数説明
ウィンドウのY座標を取得する。
@param int type 初期値=0。0で設定値を返し、1で現在値を調べて返す。
@return 上からの位置。
*/
int MikanWindowPub::GetPositionY( int type ){
	return _MikanWindow->GetPositionY( type );
}
/*!
@brief 設定ウィンドウの座標設定
@par   関数説明
ウィンドウの座標を設定する。設定反映はSetWindow()関数を使う。
@param unsigned int x X座標を与える。
@param unsigned int y Y座標を与える。
@return 0を返す。
*/
int MikanWindowPub::SetPositionXY( int x, int y )
{
	int ret = _MikanWindow->SetPositionXY( x, y );
	//ウィンドウの設定の有効化。
	_MikanWindow->SetWindow();
	return ret;
}

// !フルスクリーンにする(SystemInitでの実行を推奨)。
int MikanWindowPub::SetFullScreen( void )
{
	return _MikanWindow->SetWindow( WT_FULLSCREEN );
}

int MikanWindowPub::GetDesktopWidth( void ){
	return Window::GetDesktopWidth();
}
int MikanWindowPub::GetDesktopHeight( void ){
	return Window::GetDesktopHeight();
}
int MikanWindowPub::GetDesktopSize( int *width, int *height ){
	return Window::GetDesktopSize( width, height );
}
/*!
@brief ウィンドウハンドルを返す
@par   関数説明
ウィンドウハンドルを取得する。
@return 現在のウィンドウハンドル。
*/
HWND MikanWindowPub::GetWindowHandle( void ){
	return _MikanWindow->GetWindowHandle();
}

// !ドラッグ&ドロップされた時に受け取る関数を設定する。
/*!
  \param func int Function( int files )という関数の名前を与える。引数は受け取ったファイル数。
  */
int MikanWindowPub::SetReceiveDragFileFunction( int( *func )( int files ) ){
	return _MikanWindow->SetReceiveDragFileFunction( func );
}

// !ドラッグ&ドロップされたファイル名を取得
/*!
  \param num ドラッグ&ドロップされたファイル番号
  \retval 文字列のポインタ。早くコピーすること。
  */
char * MikanWindowPub::GetReceiveDragFilePath( int num ){
	return _MikanWindow->GetReceiveDragFilePath( num );
}

/*!
@class   MikanDrawPub
@brief   MikanDrawクラス。DirectXを用いた描画を行う。
@author  Hiroki
@version 1.0
*/

/*!
@brief MikanDrawのコンストラクタ
@par   関数説明
色と基準座標を初期化しているだけ。
*/
MikanDrawPub::MikanDrawPub()
{
	//色の定義
	//何か賢い方法ないかね？
	ColorList.Black = COLOR_BLACK;
	ColorList.Aliceblue = COLOR_ALICEBLUE;
	ColorList.Darkcyan = COLOR_DARKCYAN;
	ColorList.Lightyellow = COLOR_LIGHTYELLOW;
	ColorList.Coral = COLOR_CORAL;
	ColorList.Dimgray = COLOR_DIMGRAY;
	ColorList.Lavender = COLOR_LAVENDER;
	ColorList.Teal = COLOR_TEAL;
	ColorList.Lightgoldenrodyellow = COLOR_LIGHTGOLDENRODYELLOW;
	ColorList.Tomato = COLOR_TOMATO;
	ColorList.Gray = COLOR_GRAY;
	ColorList.Lightsteelblue = COLOR_LIGHTSTEELBLUE;
	ColorList.Darkslategray = COLOR_DARKSLATEGRAY;
	ColorList.Lemonchiffon = COLOR_LEMONCHIFFON;
	ColorList.Orangered = COLOR_ORANGERED;
	ColorList.Darkgray = COLOR_DARKGRAY;
	ColorList.Lightslategray = COLOR_LIGHTSLATEGRAY;
	ColorList.Darkgreen = COLOR_DARKGREEN;
	ColorList.Wheat = COLOR_WHEAT;
	ColorList.Red = COLOR_RED;
	ColorList.Silver = COLOR_SILVER;
	ColorList.Slategray = COLOR_SLATEGRAY;
	ColorList.Green = COLOR_GREEN;
	ColorList.Burlywood = COLOR_BURLYWOOD;
	ColorList.Crimson = COLOR_CRIMSON;
	ColorList.Lightgrey = COLOR_LIGHTGREY;
	ColorList.Steelblue = COLOR_STEELBLUE;
	ColorList.Forestgreen = COLOR_FORESTGREEN;
	ColorList.Tan = COLOR_TAN;
	ColorList.Mediumvioletred = COLOR_MEDIUMVIOLETRED;
	ColorList.Gainsboro = COLOR_GAINSBORO;
	ColorList.Royalblue = COLOR_ROYALBLUE;
	ColorList.Seagreen = COLOR_SEAGREEN;
	ColorList.Khaki = COLOR_KHAKI;
	ColorList.Deeppink = COLOR_DEEPPINK;
	ColorList.Whitesmoke = COLOR_WHITESMOKE;
	ColorList.Midnightblue = COLOR_MIDNIGHTBLUE;
	ColorList.Mediumseagreen = COLOR_MEDIUMSEAGREEN;
	ColorList.Yellow = COLOR_YELLOW;
	ColorList.Hotpink = COLOR_HOTPINK;
	ColorList.White = COLOR_WHITE;
	ColorList.Navy = COLOR_NAVY;
	ColorList.Mediumaquamarine = COLOR_MEDIUMAQUAMARINE;
	ColorList.Gold = COLOR_GOLD;
	ColorList.Palevioletred = COLOR_PALEVIOLETRED;
	ColorList.Snow = COLOR_SNOW;
	ColorList.Darkblue = COLOR_DARKBLUE;
	ColorList.Darkseagreen = COLOR_DARKSEAGREEN;
	ColorList.Orange = COLOR_ORANGE;
	ColorList.Pink = COLOR_PINK;
	ColorList.Ghostwhite = COLOR_GHOSTWHITE;
	ColorList.Mediumblue = COLOR_MEDIUMBLUE;
	ColorList.Aquamarine = COLOR_AQUAMARINE;
	ColorList.Sandybrown = COLOR_SANDYBROWN;
	ColorList.Lightpink = COLOR_LIGHTPINK;
	ColorList.Floralwhite = COLOR_FLORALWHITE;
	ColorList.Blue = COLOR_BLUE;
	ColorList.Palegreen = COLOR_PALEGREEN;
	ColorList.Darkorange = COLOR_DARKORANGE;
	ColorList.Thistle = COLOR_THISTLE;
	ColorList.Linen = COLOR_LINEN;
	ColorList.Dodgerblue = COLOR_DODGERBLUE;
	ColorList.Lightgreen = COLOR_LIGHTGREEN;
	ColorList.Goldenrod = COLOR_GOLDENROD;
	ColorList.Magenta = COLOR_MAGENTA;
	ColorList.Antiquewhite = COLOR_ANTIQUEWHITE;
	ColorList.Cornflowerblue = COLOR_CORNFLOWERBLUE;
	ColorList.Springgreen = COLOR_SPRINGGREEN;
	ColorList.Peru = COLOR_PERU;
	ColorList.Fuchsia = COLOR_FUCHSIA;
	ColorList.Papayawhip = COLOR_PAPAYAWHIP;
	ColorList.Deepskyblue = COLOR_DEEPSKYBLUE;
	ColorList.Mediumspringgreen = COLOR_MEDIUMSPRINGGREEN;
	ColorList.Darkgoldenrod = COLOR_DARKGOLDENROD;
	ColorList.Violet = COLOR_VIOLET;
	ColorList.Blanchedalmond = COLOR_BLANCHEDALMOND;
	ColorList.Lightskyblue = COLOR_LIGHTSKYBLUE;
	ColorList.Lawngreen = COLOR_LAWNGREEN;
	ColorList.Chocolate = COLOR_CHOCOLATE;
	ColorList.Plum = COLOR_PLUM;
	ColorList.Bisque = COLOR_BISQUE;
	ColorList.Skyblue = COLOR_SKYBLUE;
	ColorList.Chartreuse = COLOR_CHARTREUSE;
	ColorList.Sienna = COLOR_SIENNA;
	ColorList.Orchid = COLOR_ORCHID;
	ColorList.Moccasin = COLOR_MOCCASIN;
	ColorList.Lightblue = COLOR_LIGHTBLUE;
	ColorList.Greenyellow = COLOR_GREENYELLOW;
	ColorList.Saddlebrown = COLOR_SADDLEBROWN;
	ColorList.Mediumorchid = COLOR_MEDIUMORCHID;
	ColorList.Navajowhite = COLOR_NAVAJOWHITE;
	ColorList.Powderblue = COLOR_POWDERBLUE;
	ColorList.Lime = COLOR_LIME;
	ColorList.Maroon = COLOR_MAROON;
	ColorList.Darkorchid = COLOR_DARKORCHID;
	ColorList.Peachpuff = COLOR_PEACHPUFF;
	ColorList.Paleturquoise = COLOR_PALETURQUOISE;
	ColorList.Limegreen = COLOR_LIMEGREEN;
	ColorList.Darkred = COLOR_DARKRED;
	ColorList.Darkviolet = COLOR_DARKVIOLET;
	ColorList.Mistyrose = COLOR_MISTYROSE;
	ColorList.Lightcyan = COLOR_LIGHTCYAN;
	ColorList.Yellowgreen = COLOR_YELLOWGREEN;
	ColorList.Brown = COLOR_BROWN;
	ColorList.Darkmagenta = COLOR_DARKMAGENTA;
	ColorList.Lavenderblush = COLOR_LAVENDERBLUSH;
	ColorList.Cyan = COLOR_CYAN;
	ColorList.Darkolivegreen = COLOR_DARKOLIVEGREEN;
	ColorList.Firebrick = COLOR_FIREBRICK;
	ColorList.Purple = COLOR_PURPLE;
	ColorList.Seashell = COLOR_SEASHELL;
	ColorList.Aqua = COLOR_AQUA;
	ColorList.Olivedrab = COLOR_OLIVEDRAB;
	ColorList.Indianred = COLOR_INDIANRED;
	ColorList.Indigo = COLOR_INDIGO;
	ColorList.Oldlace = COLOR_OLDLACE;
	ColorList.Turquoise = COLOR_TURQUOISE;
	ColorList.Olive = COLOR_OLIVE;
	ColorList.Rosybrown = COLOR_ROSYBROWN;
	ColorList.Darkslateblue = COLOR_DARKSLATEBLUE;
	ColorList.Ivory = COLOR_IVORY;
	ColorList.Mediumturquoise = COLOR_MEDIUMTURQUOISE;
	ColorList.Darkkhaki = COLOR_DARKKHAKI;
	ColorList.Darksalmon = COLOR_DARKSALMON;
	ColorList.Blueviolet = COLOR_BLUEVIOLET;
	ColorList.Honeydew = COLOR_HONEYDEW;
	ColorList.Darkturquoise = COLOR_DARKTURQUOISE;
	ColorList.Palegoldenrod = COLOR_PALEGOLDENROD;
	ColorList.Lightcoral = COLOR_LIGHTCORAL;
	ColorList.Mediumpurple = COLOR_MEDIUMPURPLE;
	ColorList.Mintcream = COLOR_MINTCREAM;
	ColorList.Lightseagreen = COLOR_LIGHTSEAGREEN;
	ColorList.Cornsilk = COLOR_CORNSILK;
	ColorList.Salmon = COLOR_SALMON;
	ColorList.Slateblue = COLOR_SLATEBLUE;
	ColorList.Azure = COLOR_AZURE;
	ColorList.Cadetblue = COLOR_CADETBLUE;
	ColorList.Beige = COLOR_BEIGE;
	ColorList.Lightsalmon = COLOR_LIGHTSALMON;
	ColorList.Mediumslateblue = COLOR_MEDIUMSLATEBLUE;
}
/*!
@brief MikanDrawのデストラクタ
@par   関数説明
何もしていない。
*/
MikanDrawPub::~MikanDrawPub(){}

//テクスチャの最大数の設定
unsigned int MikanDrawPub::SetTextureMax( unsigned int tmax )
{
	if ( _MikanSystem->GetMikanPosition() == MIKAN_POSITION_SYSTEMINIT )
	{
		return _MikanDraw->SetTextureMax( tmax );
	}
	return 0;
}

// !描画サイズの設定
/*!
  \param color 画面を塗り潰す色(デフォルト0xFF000000[黒])。
  */
int MikanDrawPub::SetScreenSize( int width, int height ){
	return _MikanDraw->SetScreenSize( width, height );
}

int MikanDrawPub::GetScreenWidth( int now ){
	return _MikanDraw->GetScreenWidth( now );
}

int MikanDrawPub::GetScreenHeight( int now ){
	return _MikanDraw->GetScreenHeight( now );
}

/*!
@brief 画面をきれいにする。
@par   関数説明
画面を単一色で塗りつぶす。
@param color 塗りつぶす色。省略で0xFF000000(黒)。
@return 0を返す。
*/
int MikanDrawPub::ClearScreen( unsigned long color ){
	return _MikanDraw->ClearScreen( color );
}

//描画基準x座標をセット
int MikanDrawPub::SetBaseX( int x )
{
	int bx = base_x;
	base_x = x;
	return bx;
}

//描画基準y座標をセット
int MikanDrawPub::SetBaseY( int y )
{
	int by = base_y;
	base_y = y;
	return by;
}

/*!
@brief テクスチャの合成方法の指定
@par   関数説明
テクスチャの合成方法を指定する。
@param kind 以下の値のどれかを指定する。
DRAW_TRC=透過色を有効にする。
DRAW_ADD=加算合成。
DRAW_SUB=減算合成。
DRAW_MUL=乗算合成。
DRAW_NON=何もしない。透過色が無効。
@return 0を返す。
*/
int MikanDrawPub::SetRender( int kind ){
	return _MikanDraw->SetRender( kind );
}
/*!
@brief 画面をレンダーターゲットにする
@par   関数説明
描画対象を画面にする。
@return 0を返す。
*/
int MikanDrawPub::SetRenderTarget( void ){
	return _MikanDraw->SetRenderTarget();
}
/*!
@brief テクスチャをレンダーターゲットにする
@par   関数説明
描画対象をテクスチャにする。
@param txnum 描画対象にするテクスチャ番号。
@return 0を返す。
*/
int MikanDrawPub::SetRenderTarget( unsigned int txnum ){
	return _MikanDraw->SetRenderTarget( txnum );
}
/*!
@brief 画像を読み込んでテクスチャを作る
@par   関数説明
画像ファイルを指定してテクスチャを作る。
@param txnum テクスチャ番号。
@param filename ファイル名。
@param tcolor 透過色。TRC_NONEで何もしない。TRC_ZEROで左上の色を透過色に指定する。省略した場合TRC_NONE(何もしない)。
@return 0=テクスチャ生成スレッドの生成。
@return 1=テクスチャ番号が不正。
@return 2=ファイル名がない。
@return 3=ファイル情報が読み取れなかった。
@return 4=領域確保に失敗。
*/
int MikanDrawPub::CreateTexture( unsigned int txnum, const char *filename, unsigned long tcolor, int thread ){
	return _MikanDraw->CreateTexture( txnum, filename, tcolor, thread );
}
int MikanDrawPub::CreateTexture( unsigned int txnum, HMODULE module, const char *resource, unsigned long tcolor, int thread )
{
	if ( module == NULL )
	{
		module = _MikanSystem->GetInstance();
	}
	return _MikanDraw->CreateTexture( txnum, module, resource, tcolor, thread );
}

/*!
@brief 空のテクスチャを作る
@par   関数説明
空のテクスチャを作る。
@param txnum テクスチャ番号。
@param width テクスチャの横幅。
@param height テクスチャの高さ。
@param thread スレッド生成フラグ。0でテクスチャ生成時にスレッドを作らない。省略すると1で、スレッド生成時にスレッドを作る。
@return 0=テクスチャ生成スレッドの生成。
@return 1=テクスチャ番号が不正。
@return 2=どちらかの大きさが0。
@return 3=領域確保に失敗。
*/
int MikanDrawPub::CreateTexture( unsigned int txnum, unsigned int width, unsigned int height, int thread ){
	return _MikanDraw->CreateTexture( txnum, width, height, thread );
}
/*!
@brief テクスチャの解放
@par   関数説明
テクスチャを解放する。
@param txnum テクスチャ番号。
@return 0を返す。
*/
int MikanDrawPub::ReleaseTexture( unsigned int txnum ){
	return _MikanDraw->ReleaseTexture( txnum );
}

/*!
@brief 点の描画
@par   関数説明
点を描画する。
@param sx 描画x座標。
@param sy 描画y座標。
@param color 色。省略すると白になる。
@return 0を返す。
*/
int MikanDrawPub::DrawPixel( int x, int y, unsigned long color ){
	return _MikanDraw->DrawPixel( MDRAW_X( x ), MDRAW_Y( y ), color );
}

/*!
@brief 線の描画
@par   関数説明
線を描画する。
@param sx 描画開始x座標。
@param sy 描画開始y座標。
@param ex 描画終了x座標。
@param ey 描画終了y座標。
@param color 線の色。省略すると白い線になる。
@param width 線の太さ。諸略すると1.0。
@param antialias アンチエイリアスをかけるならば1。省略すると0でアンチエイリアスはかからない。
@return 0を返す。
*/
int MikanDrawPub::DrawLine( int sx, int sy, int ex, int ey, unsigned long color, double width, int antialias ){
	return _MikanDraw->DrawLine( MDRAW_X( sx ), MDRAW_Y( sy ), MDRAW_X( ex ), MDRAW_Y( ey ), color, width, antialias );
}
int MikanDrawPub::DrawBoxLine( int dx, int dy, int w, int h, unsigned long color, double width, int antialias ){
	return _MikanDraw->DrawBoxLine( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color, width, antialias );
}
int MikanDrawPub::DrawBoxLineC( int dx, int dy, int w, int h, unsigned long color, double width, int antialias ){
	return _MikanDraw->DrawBoxLineC( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color, width, antialias );
}

/*!
@brief 長方形の描画
@par   関数説明
長方形を描画する。
@param dx 描画x座標。
@param dy 描画y座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param color 長方形の色。
@return 0を返す。
*/
int MikanDrawPub::DrawBox( int dx, int dy, int w, int h, unsigned long color ){
	return _MikanDraw->DrawBox( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color );
}
/*!
@brief 長方形の描画
@par   関数説明
長方形を描画する。各頂点に色を指定することでグラデーションをかける事ができる。
@param dx 描画x座標。
@param dy 描画y座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param color1 左上の色。
@param color2 右上の色。
@param color3 左下の色。
@param color4 右下の色。
@return 0を返す。
*/
int MikanDrawPub::DrawBox( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 ){
	return _MikanDraw->DrawBox( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color1, color2, color3, color4 );
}

int MikanDrawPub::DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color ){
	return _MikanDraw->DrawBox( MDRAW_X( x0 ), MDRAW_Y( y0 ), MDRAW_X( x1 ), MDRAW_Y( y1 ), MDRAW_X( x2 ), MDRAW_Y( y2 ), MDRAW_X( x3 ), MDRAW_Y( y3 ), color );
}
int MikanDrawPub::DrawBox( int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 ){
	return _MikanDraw->DrawBox( MDRAW_X( x0 ), MDRAW_Y( y0 ), MDRAW_X( x1 ), MDRAW_Y( y1 ), MDRAW_X( x2 ), MDRAW_Y( y2 ), MDRAW_X( x3 ), MDRAW_Y( y3 ), color1, color2, color3, color4 );
}


/*!
@brief 長方形の描画
@par   関数説明
長方形を中央を基準にして描画する。
@param dx 描画x座標。
@param dy 描画y座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param color 長方形の色。
@return 0を返す。
*/
int MikanDrawPub::DrawBoxC( int dx, int dy, int w, int h, unsigned long color ){
	return _MikanDraw->DrawBoxC( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color );
}
/*!
@brief 長方形の描画
@par   関数説明
長方形を中央を基準にして描画する。各頂点に色を指定することでグラデーションをかける事ができる。
@param dx 描画x座標。
@param dy 描画y座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param color1 左上の色。
@param color2 右上の色。
@param color3 左下の色。
@param color4 右下の色。
@return 0を返す。
*/
int MikanDrawPub::DrawBoxC( int dx, int dy, int w, int h, unsigned long color1, unsigned long color2, unsigned long color3, unsigned long color4 ){
	return _MikanDraw->DrawBoxC( MDRAW_X( dx ), MDRAW_Y( dy ), w, h, color1, color2, color3, color4 );
}
/*!
@brief テクスチャの描画
@par   関数説明
テクスチャを描画する。始点は左上。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/

int MikanDrawPub::DrawCircleLineC( int x, int y, int r, unsigned long color, double width, int antialias ){
	return _MikanDraw->DrawCircleLineC( MDRAW_X( x ), MDRAW_Y( y ), r, color, width, antialias );
}

int MikanDrawPub::DrawCircleLine( int x, int y, int w, int h, unsigned long color, double width, int antialias ){
	return _MikanDraw->DrawCircleLine( MDRAW_X( x ), MDRAW_Y( y ), w, h, color, width, antialias );
}

int MikanDrawPub::DrawCircleC( int x, int y, int r, unsigned long color ){
	return _MikanDraw->DrawCircleC( MDRAW_X( x ), MDRAW_Y( y ), r, color );
}

int MikanDrawPub::DrawCircle( int x, int y, int w, int h, unsigned long color ){
	return _MikanDraw->DrawCircle( MDRAW_X( x ), MDRAW_Y( y ), w, h, color );
}

int MikanDrawPub::DrawTexture( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag ){
	return _MikanDraw->DrawTexture( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, drawflag );
}
/*!
@brief テクスチャの描画
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag ){
	return _MikanDraw->DrawTextureC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, drawflag );
}
/*!
@brief テクスチャの拡大縮小描画
@par   関数説明
テクスチャを描画する。始点は左上。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param scale 拡大縮小率。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag ){
	return _MikanDraw->DrawTextureScaling( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, drawflag );
}
/*!
@brief テクスチャの拡大縮小描画
@par   関数説明
テクスチャを描画する。始点は左上。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param dw 描画横幅。
@param dh 描画高さ。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureScaling( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag ){
	return _MikanDraw->DrawTextureScaling( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, drawflag );
}
/*!
@brief テクスチャの拡大縮小描画
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param scale 拡大縮小率。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag ){
	return _MikanDraw->DrawTextureScalingC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, drawflag );
}
/*!
@brief テクスチャの拡大縮小描画
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param dw 描画横幅。
@param dh 描画高さ。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureScalingC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag ){
	return _MikanDraw->DrawTextureScalingC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, drawflag );
}
/*!
@brief テクスチャを回転描画する(ラジアン)
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param rad ラジアン。時計回り。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureRotationC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag ){
	return _MikanDraw->DrawTextureRotationC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, rad, drawflag );
}
/*!
@brief テクスチャを回転描画する(角度)
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param angle 角度。時計回り。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureRotationAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag ){
	return _MikanDraw->DrawTextureRotationAngleC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, angle, drawflag );
}
/*!
@brief テクスチャを回転拡大描画する
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param scale 拡大縮小率。
@param rad ラジアン。時計回り。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag ){
	return _MikanDraw->DrawTextureRotateScaleC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, rad, drawflag );
}
/*!
@brief テクスチャを回転拡大描画する
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param scale 拡大縮小率。
@param angle ラジアン。時計回り。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag ){
	return _MikanDraw->DrawTextureRotateScaleAngleC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, angle, drawflag );
}
/*!
@brief テクスチャを回転拡大描画する
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param dw 描画横幅。
@param dh 描画高さ。
@param rad ラジアン。時計回り。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureRotateScaleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, double rad, int drawflag ){
	return _MikanDraw->DrawTextureRotateScaleC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, rad, drawflag );
}
/*!
@brief テクスチャを回転拡大描画する
@par   関数説明
テクスチャを描画する。始点は中央。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param dw 描画横幅。
@param dh 描画高さ。
@param angle ラジアン。時計回り。
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureRotateScaleAngleC( unsigned int txnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, double angle, int drawflag ){
	return _MikanDraw->DrawTextureRotateScaleAngleC( txnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, angle, drawflag );
}
/*!
@brief テクスチャの4点を自由に決めて描画する
@par   関数説明
テクスチャを描画する。頂点指定は読み込んだテクスチャの左上、右上、左下、右下の(Zを描くような)順番で指定する。
@param txnum テクスチャ番号。
@param dx 描画x座標。
@param dy 描画y座標。
@param rx 読み込みx座標。
@param ry 読み込みy座標。
@param w 読み込み幅。
@param h 読み込み高さ。
@param x0 左上のx座標
@param y0 左上のy座標
@param x1 右上のx座標
@param y1 右上のy座標
@param x2 左下のx座標
@param y2 左下のy座標
@param x3 右下のx座標
@param y3 右下のy座標
@return 0=成功。
@return 1=テクスチャ番号が不正。
@return 2=テクスチャがない。
@return 3=テクスチャセットに失敗。
@return 4=テクスチャ設定に失敗。
@return 5=頂点ストリームの設定に失敗。
@return 6=レンダリングに失敗。
*/
int MikanDrawPub::DrawTextureVertex( unsigned int txnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag ){
	return _MikanDraw->DrawTextureVertex( txnum, rx, ry, w, h, MDRAW_X( x0 ), MDRAW_Y( y0 ), MDRAW_X( x1 ), MDRAW_Y( y1 ), MDRAW_X( x2 ), MDRAW_Y( y2 ), MDRAW_X( x3 ), MDRAW_Y( y3 ), drawflag );
}

int MikanDrawPub::CreatePolygon( unsigned int pnum, unsigned int max, D3DPRIMITIVETYPE filltype ){
	return _MikanDraw->CreatePolygon( pnum, max, filltype );
}

int MikanDrawPub::CreateRegularPolygon( unsigned int pnum, double cx, double cy, unsigned int n, double r, double rad, unsigned long color ){
	return _MikanDraw->CreateRegularPolygon( pnum, MDRAW_X( cx ), MDRAW_Y( cy ), n, r, rad, color );
}

int MikanDrawPub::SetTextureOnPolygon( unsigned int pnum, unsigned int tnum ){
	return _MikanDraw->SetTextureOnPolygon( pnum, tnum );
}

int MikanDrawPub::SetTextureOnPolygon( unsigned int pnum ){
	return _MikanDraw->SetTextureOnPolygon( pnum );
}

int MikanDrawPub::SetPolygonUV( unsigned int pnum, unsigned int vnum, int u, int v ){
	return _MikanDraw->SetPolygonUV( pnum, vnum, u, v );
}

int MikanDrawPub::SetPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color ){
	return _MikanDraw->SetPolygonVertex( pnum, vnum, MDRAW_X( x ), MDRAW_Y( y ), color );
}

int MikanDrawPub::EditPolygonVertex( unsigned int pnum, unsigned int vnum, double x, double y, unsigned long color, int u, int v, int edit ){
	return _MikanDraw->EditPolygonVertex( pnum, vnum, MDRAW_X( x ), MDRAW_Y( y ), color, (double)u / (double)_MikanDraw->GetTextureWidth( _MikanDraw->GetTextureNumOnPolygon( pnum ) ), (double)v / (double)_MikanDraw->GetTextureHeight( _MikanDraw->GetTextureNumOnPolygon( pnum ) ), edit );
}

int MikanDrawPub::TransformPolygonMoving( unsigned int pnum, double mx, double my ){
	return _MikanDraw->TransformPolygonMoving( pnum, mx, my );
}

int MikanDrawPub::TransformPolygonScaling( unsigned int pnum, double scale, unsigned int basenum ){
	return _MikanDraw->TransformPolygonScaling( pnum, scale, basenum );
}

int MikanDrawPub::DrawPolygon( unsigned int pnum, int cx, int cy, unsigned int begin, unsigned int end ){
	return _MikanDraw->DrawPolygon( pnum, MDRAW_X( cx ), MDRAW_Y( cy ), begin, end );
}

int MikanDrawPub::DrawPolygon( unsigned int pnum, int cx, int cy ){
	return _MikanDraw->DrawPolygon( pnum, MDRAW_X( cx ), MDRAW_Y( cy ) );
}

int MikanDrawPub::DrawPolygon( unsigned int pnum ){
	return _MikanDraw->DrawPolygon( pnum );
}

int MikanDrawPub::DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale, unsigned int start, unsigned int end ){
	return _MikanDraw->DrawPolygonScaling( pnum, MDRAW_X( cx ), MDRAW_Y( cy ), scale, start, end );
}

int MikanDrawPub::DrawPolygonScaling( unsigned int pnum, int cx, int cy, double scale ){
	return _MikanDraw->DrawPolygonScaling( pnum, MDRAW_X( cx ), MDRAW_Y( cy ), scale );
}

int MikanDrawPub::DrawRegularPolygon( double cx, double cy, unsigned int n, double r, double rad, unsigned long color ){
	return _MikanDraw->DrawRegularPolygon( MDRAW_X( cx ), MDRAW_Y( cy ), n, r, rad, color );
}

/*!
@brief 色の作成
@par   関数説明
Mikan(DirectX)で使われる色を作る。
@param alpha アルファ値。
@param red 赤。
@param green 緑。
@param blue 青。
@return 色。0xAARRGGBB。
*/
unsigned long MikanDrawPub::Color( unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue ){
	return _MikanDraw->Color( alpha, red, green, blue );
}
/*!
@brief カラーの設定
@par   関数説明
テクスチャを染める色を設定する。4色同時設定。
@param txnum テクスチャ番号。
@param color 色。
@return 0=成功。
@return 1=テクスチャ番号が不正。
*/
int MikanDrawPub::SetColor( unsigned int txnum, unsigned long color ){
	return _MikanDraw->SetColor( txnum, color );
}

int MikanDrawPub::SetAlpha( unsigned int txnum, unsigned char alpha ){
	return _MikanDraw->SetAlpha( txnum, alpha );
}


/*!
@brief 頂点カラーの設定
@par   関数説明
テクスチャを染める色を設定する。1色だけの設定なので、4回使えば4頂点に別々の色を設定する事ができる。
@param txnum テクスチャ番号。
@param color 色。
@return 0=成功。
@return 1=テクスチャ番号が不正。
*/
int MikanDrawPub::SetColorVertex( unsigned int txnum, unsigned long color ){
	return _MikanDraw->SetColorVertex( txnum, color );
}
int MikanDrawPub::OpenTexture( unsigned int txnum, int x, int y, int w, int h ){
	return _MikanDraw->OpenTexture( txnum, x, y, w, h );
}
int MikanDrawPub::CloseTexture( unsigned int txnum ){
	return _MikanDraw->CloseTexture( txnum );
}

/*!
@brief テクスチャの指定座標の色を取得する。
@par   関数説明
テクスチャの指定座標の色を取得する。
@param txnum テクスチャ番号。
@param x x座標。
@param y y座標。
@return 0=成功。
@return 1=テクスチャ番号が不正。
*/
unsigned long MikanDrawPub::GetColorTexturePixel( unsigned int txnum, int x, int y ){
	return _MikanDraw->GetColorTexturePixel( txnum, x, y );
}
/*!
@brief テクスチャの色を別の色に置換する
@par   関数説明
テクスチャの範囲内の色を置換する。
@param txnum テクスチャ番号。
@param scolor 検索する色。
@param rcolor 置換する色。
@param x 検索開始x座標。初期値は0。
@param y 検索開始y座標。初期値は0。
@param w 横幅。初期値は0で、テクスチャの横幅になる。
@param h 高さ。初期値は0で、テクスチャの高さになる。
@return 置換した回数を返す。一度も置換しなければ0。
*/
unsigned int MikanDrawPub::TextureColorReplacement( unsigned int txnum, unsigned long scolor, unsigned long rcolor, int x, int y, int w, int h ){
	return _MikanDraw->TextureColorReplacement( txnum, scolor, rcolor, x, y, w, h );
}
/*!
@brief テクスチャの色を検索する
@par   関数説明
テクスチャの範囲内の色を検索する。
@param txnum テクスチャ番号。
@param scolor 検索する色。
@param x 検索開始x座標。初期値は0。
@param y 検索開始y座標。初期値は0。
@param w 横幅。初期値は0で、テクスチャの横幅になる。
@param h 高さ。初期値は0で、テクスチャの高さになる。
@return 0 見つからなかった。
@return 初めてヒットするまでの検索済みピクセル数。左上から右下に向かう順番。
*/
unsigned int MikanDrawPub::TextureColorSearch( unsigned int txnum, unsigned long scolor, int x, int y, int w, int h ){
	return _MikanDraw->TextureColorSearch( txnum, scolor, x, y, w, h );
}
//テクスチャの範囲内の色を検索し数を数える。
unsigned int MikanDrawPub::TextureColorCount( unsigned int txnum, unsigned long scolor, int x, int y, int w, int h ){
	return _MikanDraw->TextureColorCount( txnum, scolor, x, y, w, h );
}
/*!
@brief テクスチャの横幅取得
@par   関数説明
テクスチャの横幅を取得。
@param txnum テクスチャ番号。
@return 0=失敗。
@return その他=テクスチャの横幅。
*/
unsigned int MikanDrawPub::GetTextureWidth( unsigned int txnum ){
	return _MikanDraw->GetTextureWidth( txnum );
}
/*!
@brief テクスチャの高さ取得
@par   関数説明
テクスチャの高さを取得。
@param txnum テクスチャ番号。
@return 0=失敗。
@return その他=テクスチャの高さ。
*/
unsigned int MikanDrawPub::GetTextureHeight( unsigned int txnum ){
	return _MikanDraw->GetTextureHeight( txnum );
}
/*!
@brief スクリーンショットを撮る
@par   関数説明
現在のスクリーンショット
@param filename ファイル名。
@param imagetype 画像のタイプ。
@return 常に0を返す。
*/
int MikanDrawPub::ScreenShot( const char *filename, unsigned int imagetype ){
	return _MikanDraw->ScreenShot( filename, imagetype );
}

// フォントを読み込む
int MikanDrawPub::LoadFontFile( const char *fontfile ){
	return _MikanDraw->LoadFontFile( fontfile );
}

int MikanDrawPub::LoadFontFile( HMODULE module, const char *resource )
{
	if ( module == NULL )
	{
		module = _MikanSystem->GetInstance();
	}
	return _MikanDraw->LoadFontFile( module, resource );
}

int MikanDrawPub::CreateFont( unsigned int fontnum, const char *fontname, int size, unsigned long color, int weight, int italic ){
	return _MikanDraw->CreateFont( fontnum, fontname, size, color, weight, italic );
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
int MikanDrawPub::CreateFont( unsigned int fontnum, const char *fontname, int size, unsigned long color ){
	return _MikanDraw->CreateFont( fontnum, fontname, size, color );
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
int MikanDrawPub::CreateFont( unsigned int fontnum, int size, unsigned long color ){
	return _MikanDraw->CreateFont( fontnum, size, color );
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
//int MikanDrawPub::Print( unsigned int fontnum, int x, int y, char *str ){ return _MikanDraw->Print( fontnum, x, y, str ); }
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
int MikanDrawPub::Printf( unsigned int fontnum, int x, int y, const char *format, ... )
{
	char str[ MAX_BUFFER_SIZE ];
	vsnprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, format, (char*)( &format + 1 ) );
	return _MikanDraw->Print( fontnum, MDRAW_X( x ), MDRAW_Y( y ), str );
}
/*!
@brief 文字列を整形描画する。
@par   関数説明
文字列を整形描画する。詳しくはC言語等のprintfなどを参照。
@param fontnum フォント番号。
@param x 描画するx座標。
@param y 描画するy座標。
@param color 文字色。
@param format 整形描画する文字列。
@param その他いくつでも 整形文字列で成形して描画する変数たち。
@return フォント番号。不正な場合は0が使われる。
*/
int MikanDrawPub::Printf( unsigned int fontnum, int x, int y, unsigned long color, const char *format, ... )
{
	char str[ MAX_BUFFER_SIZE ];
	vsnprintf_s( str, MAX_BUFFER_SIZE, MAX_BUFFER_SIZE, format, (char*)( &format + 1 ) );
	return _MikanDraw->Print( fontnum, MDRAW_X( x ), MDRAW_Y( y ), color, str );
}


struct IDirect3DTexture9 * MikanMovieManagementPub::GetTexture( unsigned int movnum ){
	return _MikanMovie->GetTexture( movnum );
}

int MikanMovieManagementPub::Lock( unsigned int movnum ){
	return _MikanMovie->Lock( movnum );
}

int MikanMovieManagementPub::Unlock( unsigned int movnum ){
	return _MikanMovie->Unlock( movnum );
}

int MikanMovieManagementPub::Load( unsigned int movnum, char *moviefile ){
	return _MikanMovie->Load( movnum, moviefile );
}

int MikanMovieManagementPub::Play( unsigned int movnum, int loopplay ){
	return _MikanMovie->Play( movnum, loopplay );
}

int MikanMovieManagementPub::Stop( unsigned int movnum ){
	return _MikanMovie->Stop( movnum );
}

int MikanMovieManagementPub::Pause( unsigned int movnum ){
	return _MikanMovie->Pause( movnum );
}

int MikanMovieManagementPub::IsPlay( unsigned int movnum ){
	return _MikanMovie->IsPlay( movnum );
}

long MikanMovieManagementPub::Size( unsigned int movnum ){
	return _MikanMovie->Size( movnum );
}

long MikanMovieManagementPub::Tell( unsigned int movnum ){
	return _MikanMovie->Tell( movnum );
}

int MikanMovieManagementPub::Seek( unsigned int movnum, long seek ){
	return _MikanMovie->Seek( movnum, seek );
}

double MikanMovieManagementPub::GetRate( unsigned int movnum ){
	return _MikanMovie->GetRate( movnum );
}

int MikanMovieManagementPub::SetRate( unsigned int movnum, double rate ){
	return _MikanMovie->SetRate( movnum, rate );
}

int MikanMovieManagementPub::GetWidth( unsigned int movnum ){
	return _MikanMovie->GetWidth( movnum );
}

int MikanMovieManagementPub::GetHeight( unsigned int movnum ){
	return _MikanMovie->GetHeight( movnum );
}

int MikanMovieManagementPub::SetColor( unsigned int movnum ){
	return _MikanMovie->SetColor( movnum );
}

int MikanMovieManagementPub::SetColor( unsigned int movnum, unsigned long color ){
	return _MikanMovie->SetColor( movnum, color );
}

int MikanMovieManagementPub::SetColor( unsigned int movnum, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3 ){
	return _MikanMovie->SetColor( movnum, color0, color1, color2, color3 );
}

int MikanMovieManagementPub::SetAlpha( unsigned int movnum, unsigned char alpha ){
	return _MikanMovie->SetAlpha( movnum, alpha );
}

int MikanMovieManagementPub::DrawMovie( unsigned int movnum, int dx, int dy, int drawflag ){
	return _MikanMovie->DrawMovie( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), drawflag );
}

int MikanMovieManagementPub::DrawMovie( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag ){
	return _MikanMovie->DrawMovie( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, drawflag );
}

int MikanMovieManagementPub::DrawMovieC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int drawflag ){
	return _MikanMovie->DrawMovieC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, drawflag );
}

int MikanMovieManagementPub::DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag ){
	return _MikanMovie->DrawMovieScaling( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, drawflag );
}

int MikanMovieManagementPub::DrawMovieScaling( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag ){
	return _MikanMovie->DrawMovieScaling( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, drawflag );
}

int MikanMovieManagementPub::DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, int drawflag ){
	return _MikanMovie->DrawMovieScalingC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, drawflag );
}

int MikanMovieManagementPub::DrawMovieScalingC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int dw, int dh, int drawflag ){
	return _MikanMovie->DrawMovieScalingC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, dw, dh, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotationC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double rad, int drawflag ){
	return _MikanMovie->DrawMovieRotationC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, rad, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotationAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double angle, int drawflag ){
	return _MikanMovie->DrawMovieRotationAngleC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, angle, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double rad, int drawflag ){
	return _MikanMovie->DrawMovieRotateScaleC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, rad, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, double scale, double angle, int drawflag ){
	return _MikanMovie->DrawMovieRotateScaleAngleC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, scale, angle, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotateScaleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double rad, int drawflag ){
	return _MikanMovie->DrawMovieRotateScaleC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, sw, sh, rad, drawflag );
}

int MikanMovieManagementPub::DrawMovieRotateScaleAngleC( unsigned int movnum, int dx, int dy, int rx, int ry, int w, int h, int sw, int sh, double angle, int drawflag ){
	return _MikanMovie->DrawMovieRotateScaleAngleC( movnum, MDRAW_X( dx ), MDRAW_Y( dy ), rx, ry, w, h, sw, sh, angle, drawflag );
}

int MikanMovieManagementPub::DrawMovieVertex( unsigned int movnum, int rx, int ry, int w, int h, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, int drawflag ){
	return _MikanMovie->DrawMovieVertex( movnum, rx, ry, w, h, MDRAW_X( x0 ), MDRAW_Y( y0 ), MDRAW_X( x1 ), MDRAW_Y( y1 ), MDRAW_X( x2 ), MDRAW_Y( y2 ), MDRAW_X( x3 ), MDRAW_Y( y3 ), drawflag );
}








/*!
@class   MikanInputPub
@brief   Mikaninputクラス。DirectXを用いた入力を行う。
@author  Hiroki
@version 1.0
*/

/*!
@brief コンストラクタ
@par   関数説明
何もしていない。
*/
MikanInputPub::MikanInputPub( void ){}
/*!
@brief デストラクタ
@par   関数説明
何もしていない。
*/
MikanInputPub::~MikanInputPub( void ){}
/*!
@brief パッドの認識
@par   関数説明
パッドの再認識を行う。
@param resume 0で初めから認識をし直す。省略した場合は1と同じで追加認識を行う。
@return 0=成功。
@return 1=データフォーマットの設定に失敗。
@return 2=モード設定に失敗。
*/
int MikanInputPub::RecognitionGamepad( int resume ){
	return _MikanInput->RecognitionGamepad( resume );
}
/*!
@brief 特定のキー入力の状態を取得する。
@par   関数説明
特定のキーに対する入力がどうなっているかを調べて返す。
@param keynum 調べたいキーの値。詳しくはkeymap.hに記されたキーマップ参照。
@return -1=離れた。
@return 0=押していない。
@return 0以上=押している。
*/
int MikanInputPub::GetKeyNum( int keynum ){
	return _MikanInput->GetKeyNum( keynum );
}

int MikanInputPub::GetKeyNumRapid( int keynum, int threshold, unsigned int interval ){
	return _MikanInput->GetKeyNumRapid( keynum, threshold, interval );
}

int MikanInputPub::GetKeyWhichButton( void ){
	return _MikanInput->GetKeyWhichButton();
}

/*!
@brief 関数実行時に押されているキーの状態を調べる。
@par   関数説明
関数を実行したときのキーボードの状態を取得し、押されている場合はバッファに状態を保存する。
このままでは入力の更新は行われない。
@return 0=成功。
@return 1=キー状態の取得が出来なかったので、入力制御を再度開始した。
*/
//int MikanInputPub::UpdateKeyStatus( void ){ return _MikanInput->UpdateKeyStatus(); }
/*!
@brief 実行時までに押されたキーに対し、値を更新する。
@par   関数説明
UpdateKeyStatusによって取得されたバッファを元に、キー入力を更新する。
この関数はある区間内(フレームなど)で一度だけ実行するのが好ましい。
この関数は単体でUpdateKeyStatusを行うので、事前に使う必要はない。
@return 0=成功。
@return 1=UpdateKeyStateが失敗。
*/
int MikanInputPub::UpdateKeyInput( void ){
	return _MikanInput->UpdateKeyInput();
}
/*!
@brief 指定されたパッドの指定ボタンの入力状態を取得する。
@par   関数説明
認識されたあるパッドの指定ボタンの入力状態を調べて返す。
@param padnum パッド番号。
@param button 上下左右はPAD_UP,PAD_DOWN,PAD_LEFT,PAD_RIGHTとなり、その他ボタンは1番からPAD_A,PAD_Bと
名付けられ、PAD_Lまで存在する。
@return -1=離れた。
@return 0=押していない。
@return 0以上=押している。
*/
int MikanInputPub::GetPadNum( int padnum, int button ){
	return _MikanInput->GetPadNum( padnum, button );
}
/*!
@brief 指定されたパッドのスティック入力を取得する。
@par   関数説明
認識されたあるパッドのアナログスティックの入力状態を調べて返す。
値は-1000から1000で方向はDirectXの座標系と同じで、PAD_UDは下方向が正、上方向が負。PAD_LRは右方向が正、左方向が負。
@param padnum パッド番号。
@param button アナログスティックの上下方向はPAD_UDで、左右方向はPAD_LRで取得できる。
@return 0=押していない。もしくは無効。
@return それ以外=スティックの入力。傾き具合。
*/
int MikanInputPub::GetPadStick( unsigned int padnum, int button ){
	return _MikanInput->GetPadStick( padnum, button );
}
/*!
@brief パッドの認識数
@par   関数説明
現在認識されているパッドの数を返す。
@return 現在に認識しているパッドの数。
*/
int MikanInputPub::GetPadMount( void ){
	return _MikanInput->GetPadMount();
}
int MikanInputPub::GetPadWhichButton( unsigned int padnum ){
	return _MikanInput->GetPadWhichButton( padnum );
}
/*!
@brief 現在指定パッドの状態を取得する。
@par   関数説明
現在どのパッドのどのボタンが押されているかを判断し、バッファにフラグを立てる。
@param padnum パッド番号
@return 0=成功。
@return 1=パッド番号が不正か、パッドが使われていない。
@return 2=パッドの入力準備が出来ていない。
@return 3=ステータスの取得に失敗。
*/
//int MikanInputPub::UpdatePadStatus( int padnum ){ return _MikanInput->UpdatePadStatus( padnum ); }
/*!
@brief 全パッドのキー入力の値を更新する。
@par   関数説明
全てのパッドの全てのボタンのキー入力の値を更新する。
@return 入力されたパッドの数。
*/
int MikanInputPub::UpdatePadInput( void ){
	return _MikanInput->UpdatePadInput();
}
/*!
@brief パッドのフォースフィードバックが使えるかどうか調べる
@par   関数説明
指定したパッドがフォースフィードバック(振動)に対応しているかどうかを調べる。
@return 0 パッドがフォースフィードバックに対応していないか、引数が不正。
@return 1 パッドがフォースフィードバックに対応している。
*/
int MikanInputPub::CanForceFeedback( unsigned int padnum ){
	return _MikanInput->CanForceFeedback( padnum );
}
/*!
@brief パッドのPOVモードを指定する
@par   関数説明
アナログ入力とデジタル入力の違いで顕著なのが十字キーとアナログスティックの動作の違いなので、それを指定する。
POVモードを真にすると十字キーとアナログスティックの入力を別とみなし、上下左右のPAD_UP,PAD_DOWN,PAD_LEFT,PAD_RIGHTは
十字キーのみで増えるようになる一方、(左)アナログスティックはPAD_LS_UP,PAD_LS_DOWN,PAD_LS_LEFT,PAD_LS_RIGHTで値を取得
するようになる。
POVモードを偽にすると、十字キーとアナログスティックの入力をPAD_UP,PAD_DOWN,PAD_LEFT,PAD_RIGHTに反映する。
バッファ値を取得するGetPadStick()に関しては何の効果もなく、GetPadNum()の挙動を変えるものと考えて良い。
@param padnum パッド番号。
@param povmode POVモード。0で無効、それ以外で有効にする。デフォルトでは無効。
@return 0 パッドがフォースフィードバックに対応していないか、引数が不正。
@return 1 パッドがフォースフィードバックに対応している。
*/
int MikanInputPub::SetPOVMode( unsigned int padnum, int povmode ){
	return _MikanInput->SetPOVMode( padnum, povmode );
}
/*!
@brief マウスのX座標を取得する
@par   関数説明
今現在のマウスの座標を、描画範囲内を基準に返す。左上が0となり、右に行くほど値が大きくなる。
@return 座標を返す。描画領域を超えると負の数やウィンドウサイズを越える。
*/
int MikanInputPub::GetMousePosX( void ){
	return _MikanInput->GetMousePosX();
}
/*!
@brief マウスのY座標を取得する
@par   関数説明
今現在のマウスの座標を、描画範囲内を基準に返す。左上が0となり下に行くほど値が大きくなる。
@return 座標を返す。描画領域を超えると負の数やウィンドウサイズを越える。
*/
int MikanInputPub::GetMousePosY( void ){
	return _MikanInput->GetMousePosY();
}
/*!
@brief マウスのボタン入力を取得する
@par   関数説明
マウスのボタンがどれだけ押されているかを返す。
@param num 1=左クリック、2=右クリック,3=ホイールクリック, ...
@return -1 = ボタンが離れた瞬間。
@return 0 = 押されていないかマウスのボタン番号が不正。
@return 1以上 = ボタンが押されているフレーム数。
*/
int MikanInputPub::GetMouseNum( unsigned int num ){
	return _MikanInput->GetMouseNum( num );
}
/*!
@brief マウスカーソルの座標を設定する
@par   関数説明
マウスカーソルの場所を変更する。
@return 0を返す。
*/
int MikanInputPub::SetMousePos( int x, int y ){
	return _MikanInput->SetMousePos( x, y );
}
int MikanInputPub::GetMouseWheel( void ){
	return _MikanInput->GetMouseWheel();
}
/*!
@brief マウス入力の値を更新する
@par   関数説明
マウスの入力の値を更新する。
@return 0 = 成功。
@return 1 = UpdateMouseStatus()が失敗。
*/
int MikanInputPub::UpdateMouseInput( void ){
	return _MikanInput->UpdateMouseInput();
}

int MikanInputPub::SetPad2KeyInput( int enable ){
	return _MikanInput->SetPad2KeyInput( enable );
}

int MikanInputPub::SetPad2KeyCode( unsigned int padnum, int padbutton, int keynum ){
	return _MikanInput->SetPad2KeyCode( padnum, padbutton, keynum );
}

int MikanInputPub::UpdatePad2KeyInput( void ){
	return _MikanInput->UpdatePad2KeyInput();
}

#ifndef MIKAN_NO_SOUND
bool MikanSoundPub::Load( int sound_number, const char* file_path, bool streaming ){
	return _MikanSound->Load( sound_number, file_path, streaming );
}
#ifndef _VS2010
bool MikanSoundPub::Load( int sound_number, HMODULE module, const char* resource )
{
	if ( module == NULL )
	{
		module = _MikanSystem->GetInstance();
	}
	return _MikanSound->Load( sound_number, module, resource );
}
#endif

void MikanSoundPub::Unload( int sound_number ){
	return _MikanSound->Unload( sound_number );
}

void MikanSoundPub::UnloadAll( void ){
	return _MikanSound->UnloadAll();
}

bool MikanSoundPub::Play( int sound_number, bool loop ){
	return _MikanSound->Play( sound_number, loop );
}

#ifndef _VS2010
bool MikanSoundPub::IsPlaying( int sound_number ){
	return _MikanSound->IsPlaying( sound_number );
}
#endif

void MikanSoundPub::Pause( int sound_number ){
	return _MikanSound->Pause( sound_number );
}

void MikanSoundPub::PauseAll( void ){
	return _MikanSound->PauseAll();
}

void MikanSoundPub::Stop( int sound_number ){
	return _MikanSound->Stop( sound_number );
}

void MikanSoundPub::StopAll( void ){
	return _MikanSound->StopAll();
}

int MikanSoundPub::GetVolume( int sound_number ){
	return _MikanSound->GetVolume( sound_number );
}

void MikanSoundPub::SetVolume( int sound_number, int volume ){
	return _MikanSound->SetVolume( sound_number, volume );
}

int MikanSoundPub::GetPan( int sound_number ){
	return _MikanSound->GetPan( sound_number );
}

void MikanSoundPub::SetPan( int sound_number, int pan ){
	return _MikanSound->SetPan( sound_number, pan );
}

unsigned long MikanSoundPub::GetFrequency( int sound_number ){
	return _MikanSound->GetFrequency( sound_number );
}

void MikanSoundPub::SetFrequency( int sound_number, unsigned long frequency ){
	return _MikanSound->SetFrequency( sound_number, frequency );
}

unsigned long MikanSoundPub::Tell( int sound_number ){
	return _MikanSound->Tell( sound_number );
}

void MikanSoundPub::Seek( int sound_number, unsigned long time ){
	return _MikanSound->Seek( sound_number, time );
}

bool MikanSoundPub::GetLoop( int sound_number, unsigned long* loop_start, unsigned long* loop_end ){
	return _MikanSound->GetLoop( sound_number, loop_start, loop_end );
}

bool MikanSoundPub::SetLoop( int sound_number, unsigned long loop_start, unsigned long loop_end ){
	return _MikanSound->SetLoop( sound_number, loop_start, loop_end );
}

#endif

/*!
@class   MikanNetworkPub
@brief   MikanNetworkクラス。ネットワーク通信を行う。
@author  Hiroki
@version 1.0
*/

MikanNetworkPub::MikanNetworkPub( void ){}
MikanNetworkPub::~MikanNetworkPub( void ){}

// サーバーモードにする。一度だけ実行可能。
SOCKET MikanNetworkPub::CreateTCPServer( unsigned int nnum, unsigned short port, int connect ){
	return _MikanNet->CreateTCPServer( nnum, port, connect );
}
SOCKET MikanNetworkPub::CreateUDPServer( unsigned int nnum, unsigned short port, int connect ){
	return _MikanNet->CreateUDPServer( nnum, port, connect );
}
SOCKET MikanNetworkPub::CreateBloadcastServer( unsigned int nnum, unsigned short port, int connect ){
	return _MikanNet->CreateBloadcastServer( nnum, port, connect );
}
SOCKET MikanNetworkPub::CreateMulticastServer( unsigned int nnum, const char *address, unsigned short port, int connect ){
	return _MikanNet->CreateMulticastServer( nnum, address, port, connect );
}
SOCKET MikanNetworkPub::CreateMulticastServer( unsigned int nnum, unsigned long address, unsigned short port, int connect ){
	return _MikanNet->CreateMulticastServer( nnum, address, port, connect );
}

// クライアントモードにする。
SOCKET MikanNetworkPub::CreateTCPClient( unsigned int nnum, const char *address, unsigned short port ){
	return _MikanNet->CreateTCPClient( nnum, address, port );
}
SOCKET MikanNetworkPub::CreateUDPClient( unsigned int nnum, const char *address, unsigned short port ){
	return _MikanNet->CreateUDPClient( nnum, address, port );
}
SOCKET MikanNetworkPub::CreateBloadcastClient( unsigned int nnum, const char *address, unsigned short port ){
	return _MikanNet->CreateBloadcastClient( nnum, address, port );
}
SOCKET MikanNetworkPub::CreateMulticastClient( unsigned int nnum, const char *address, unsigned short port ){
	return _MikanNet->CreateMulticastClient( nnum, address, port );
}

int MikanNetworkPub::Close( unsigned int nnum ){
	return _MikanNet->Close( nnum );
}

int MikanNetworkPub::Send( unsigned int nnum, const char *str ){
	return _MikanNet->Send( nnum, str );
}
int MikanNetworkPub::Send( unsigned int nnum, const char *str, unsigned int size ){
	return _MikanNet->Send( nnum, str, size );
}

char * MikanNetworkPub::Receive( unsigned int nnum, int *getsize, struct sockaddr *from ){
	return _MikanNet->Receive( nnum, getsize, from );
}
int MikanNetworkPub::Receive( unsigned int nnum, char *buf, int size, struct sockaddr *from ){
	return _MikanNet->Receive( nnum, buf, size, from );
}

SOCKET MikanNetworkPub::Accept( unsigned int nnum, struct sockaddr_in *client ){
	return _MikanNet->Accept( nnum, client );
}

//int MikanNetworkPub::HttpGet( const char *address, const char *filepath, const char *proxy, const char *user, const char *password, int *getsize, int *loadsize ){ return MikanNetwork::HttpGet( address, filepath, proxy, user, password, getsize, loadsize ); }
int MikanNetworkPub::HttpGet( const char *address, const char *filepath, int *getsize, int *loadsize ){
	return MikanNetwork::HttpGet( address, filepath, NULL, NULL, NULL, getsize, loadsize );
}
int MikanNetworkPub::HttpPost( const char *address, const char *data, int datasize, const char *filepath, int *getsize, int *loadsize ){
	return MikanNetwork::HttpPost( address, data, datasize, filepath, NULL, NULL, NULL, getsize, loadsize );
}
class MikanIO * MikanNetworkPub::GetMikanIO( unsigned int ionum ){
	return _MikanNet->GetMikanIO( ionum );
}

// 開く。-1で失敗。
int FileIOManagementPub::Open( unsigned int ionum, const char *io, const char *mode ){
	return _MikanFile->Open( ionum, io, mode );
}
// 1行読み込む。
char * FileIOManagementPub::ReadLine( unsigned int ionum, unsigned int read_size ){
	return _MikanFile->ReadLine( ionum, read_size );
}
int FileIOManagementPub::ReadLine( unsigned int ionum, char *buf, unsigned int max_size, unsigned int *read_size ){
	return _MikanFile->ReadLine( ionum, buf, max_size, read_size );
}

int FileIOManagementPub::Write( unsigned int ionum, const char *write_data, int write_size ){
	return _MikanFile->Write( ionum, write_data, write_size );
}
int FileIOManagementPub::Printf( unsigned int ionum, const char *format, ... )
{
	int ret;
	char *str;
	//va_listを使う方法もあるけど、別にこれで問題ないよね。
	str = (char *)calloc( 1024, sizeof( char ) );
	vsnprintf_s( str, 1024, 1024, format, (char*)( &format + 1 ) );

	ret = Write( ionum, str );

	free( str );

	return ret;
}

// 読み込む。NULLで失敗。
void * FileIOManagementPub::Read( unsigned int ionum, unsigned int read_size, int *readed_size ){
	return _MikanFile->Read( ionum, read_size, readed_size );
}
int FileIOManagementPub::Read( unsigned int ionum, void *data, unsigned int read_size, int *readed_size ){
	return _MikanFile->Read( ionum, data, read_size, readed_size );
}
// シークする。-1で失敗。
int FileIOManagementPub::Seek( unsigned int ionum, int offset, int seekmode ){
	return _MikanFile->Seek( ionum, offset, seekmode );
}
// 書き込む。-1で失敗。書き込んだサイズを返す。
int FileIOManagementPub::Write( unsigned int ionum, const void *write_data, int write_size ){
	return _MikanFile->Write( ionum, (const char *)write_data, write_size );
}
// 今まで読んだバイト数を返す。-1で失敗。
int FileIOManagementPub::Tell( unsigned int ionum ){
	return _MikanFile->Tell( ionum );
}
// サイズを返す。-1で失敗。もしくはTell。
int FileIOManagementPub::Size( unsigned int ionum ){
	return _MikanFile->Size( ionum );
}
// 閉じる。-1で失敗。
int FileIOManagementPub::Close( unsigned int ionum ){
	return _MikanFile->Close( ionum );
}
// IOを破棄する。
int FileIOManagementPub::Destroy( unsigned int ionum ){
	return _MikanFile->Destroy( ionum );
}
int FileIOManagementPub::CloseAll( void ){
	return _MikanFile->CloseAll();
}

int FileIOManagementPub::Remove( const char *file ){
	return _MikanFile->Remove( file );
}

int FileIOManagementPub::Move( const char *oldfile, const char *newfile ){
	return _MikanFile->Move( oldfile, newfile );
}

// FileIOの取得。
class FileIO * FileIOManagementPub::GetIO( unsigned int ionum ){
	return _MikanFile->GetIO( ionum );
}
class MikanIO * FileIOManagementPub::GetMikanIO( unsigned int ionum ){
	return _MikanFile->GetMikanIO( ionum );
}
