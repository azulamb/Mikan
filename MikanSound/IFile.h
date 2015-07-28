#pragma once

namespace Thaleia
{

	///	ファイル読み込みのインターフェース.
	///
	///	暗号化ファイルの読み込みや, パッキングデータからのファイル抽出を行う場合は
	/// このインターフェースを継承し, read関数で生のデータを読み出せるようにすればよい.
	class IFile
	{
	public:
		virtual ~IFile() {}

	public:
		/// ファイル内容を読む.
		///
		/// buffer	読み取ったデータの書き込み先.
		/// length	最大の読み取りバイト数.
		///
		/// returns	実際に読み取ったバイト数. 必ずlength以下の値になる.
		virtual unsigned long read( void* buffer, unsigned long length ) = 0;

		/// ファイルサイズを得る.
		///
		/// returns	ファイルサイズ.
		virtual unsigned long size() const = 0;

		/// 現在の読み取り位置を得る.
		/// 
		/// returns	現在の読み取り位置
		virtual unsigned long tell() const = 0;

		/// 読み取り位置を変更する
		/// 
		/// offset		移動先.
		/// elatively	この引数がtureのとき, 現在の読み取り位置から相対移動する.
		/// 
		/// returns		読み取り位置の変更に成功すればtrue, さもなくばfalse.
		virtual bool seek( long long offset, bool relatively = false ) = 0;
	};

};